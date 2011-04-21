#include "mainwindow.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QRect>
#include <QtCore/QSettings>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariant>
#include <QtGlobal>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

#include "editnotewindow.h"
#include "notesmodel.h"
#include "notesmodelfactory.h"
#include "notesxmlvalidator.h"

namespace ftpnotes {

namespace {

QString GetDefaultNotesDir() {
#ifdef Q_OS_MAC
  return QString("%1/%2")
      .arg(QDir::home())
      .arg("Library/Application Support/FullTiltPoker");
#elif defined(Q_OS_WIN32)
  QSettings settings(
      "HKEY_CURRENT_USER\\Software\\Classes\\FullTiltPokerApp\\defaulticon",
      QSettings::NativeFormat);
  QVariant defaulticon = settings.value("Default");

  if (!defaulticon.isValid()) return QString();

  // The defaulticon registry entry is something like:
  // "C:\Program Files\Full Tilt Poker\FullTiltPoker.exe",0
  QString tmp = defaulticon.toString();
  tmp.chop(2);
  tmp.remove('"').replace("\\", "\\\\");

  QFileInfo fileinfo(tmp);
  return fileinfo.absolutePath();
#else
  return QString();
#endif
}

}  // unnamed namespace

MainWindow::MainWindow() : window_(new Ui::MainWindow),
                           window_title_(tr("FTP Notes")),
                           model_(),
                           proxy_model_(new QSortFilterProxyModel()) {
  Initialize();
}

void MainWindow::Initialize() {
  window_->setupUi(this);

  setWindowTitle(window_title_);

  proxy_model_->setDynamicSortFilter(true);
  proxy_model_->setSortCaseSensitivity(Qt::CaseInsensitive);

  window_->tableView->setModel(proxy_model_.data());

  connect(window_->addPlayerButton, SIGNAL(clicked()), this,
          SLOT(OnAddPlayerButtonClicked()));

  connect(window_->playerFilter, SIGNAL(textChanged(QString)), this,
          SLOT(FilterPlayers()));

  connect(window_->actionOpen, SIGNAL(triggered()), this,
          SLOT(OnOpenNotesFileTriggered()));

  connect(window_->actionQuit, SIGNAL(triggered()), this,
          SLOT(close()));

  connect(window_->tableView, SIGNAL(activated(QModelIndex)),
          this, SLOT(OpenNotesEditor(QModelIndex)));
}

void MainWindow::changeEvent(QEvent* e) {
  QMainWindow::changeEvent(e);

  switch (e->type()) {
   case QEvent::LanguageChange:
     window_->retranslateUi(this);
     break;
   default:
     break;
  }
}

void MainWindow::OnOpenNotesFileTriggered() {
  QString default_notes_dir(GetDefaultNotesDir());
  QString notes_filename = QFileDialog::getOpenFileName(
      this, tr("Open Notes XML File"), default_notes_dir,
      tr("XML Files (*.xml)"));

  if (notes_filename.isNull() || notes_filename == notes_filename_) return;

  if (!NotesXMLValidator::Validate(notes_filename)) {
    QMessageBox::warning(this, tr("Error"), tr("Invalid notes file!"));
    return;
  }

  LoadNotesFile(notes_filename);
}

void MainWindow::LoadNotesFile(const QString& notes_filename) {
  QSharedPointer<NotesModel> new_model;
  try {
    new_model = NotesModelFactory::Create(notes_filename);
  }
  catch (NotesModelCreationError& /*e*/) {
    QMessageBox::warning(this, tr("Error"), tr("Unable to load notes file!"));
    return;
  }

  window_->centralWidget->setEnabled(true);

  emit CloseOpenNoteEditors();

  QFileInfo notes_file_info(notes_filename);
  notes_filename_ = notes_filename;

  setWindowTitle(QString("%1 - %2")
                 .arg(notes_file_info.fileName())
                 .arg(window_title_));

  proxy_model_->setSourceModel(new_model.data());
  model_ = new_model;

  window_->tableView->sortByColumn(0, Qt::AscendingOrder);
}

void MainWindow::FilterPlayers() {
  QRegExp regexp(window_->playerFilter->text(), Qt::CaseInsensitive,
                 QRegExp::FixedString);
  proxy_model_->setFilterRegExp(regexp);
}

void MainWindow::OnAddPlayerButtonClicked() {
  QString filtered_player = window_->playerFilter->text();
  if (filtered_player.isEmpty())
    DisplayAddPlayerDialog();
  else if (model_->HasNoteForPlayer(filtered_player))
    HighlightPlayer(filtered_player);
  else
    OpenNotesEditor(filtered_player);
}

void MainWindow::OpenNotesEditor(const QModelIndex& index) {
  QString player = index.data().toString();
  Note note(model_->GetNote(player));
  OpenNotesEditor(note);
}

void MainWindow::OpenNotesEditor(const Note& note) {
  EditNoteWindow* editor = new EditNoteWindow(note, this);

  connect(editor, SIGNAL(SaveNote(const Note&)), this,
          SLOT(SaveNote(const Note&)));

  connect(editor, SIGNAL(DeleteNote(const Note&)), this,
          SLOT(DeleteNote(const Note&)));

  connect(this, SIGNAL(CloseOpenNoteEditors()), editor, SLOT(close()));

  editor->show();
}

inline void MainWindow::OpenNotesEditor(const QString& player) {
  OpenNotesEditor(Note(player));
}

inline void MainWindow::DisplayAddPlayerDialog() {
  bool success;
  QString player = QInputDialog::getText(this, tr("Add Player"),
                                         tr("Player Name:"),
                                         QLineEdit::Normal, "",
                                         &success);
  if (success && !player.isEmpty()) {
    if (model_->HasNoteForPlayer(player)) {
      HighlightPlayer(player);
    }
    else {
      OpenNotesEditor(player);
    }
  }
}

void MainWindow::HighlightPlayer(const QString& player) {
  if (!player.contains(window_->playerFilter->text(), Qt::CaseInsensitive))
    window_->playerFilter->clear();

  QList<QStandardItem*> items = model_->findItems(
      player, Qt::MatchExactly|Qt::MatchFixedString, 0);

  if (items.count() == 1) {
    QModelIndex index = proxy_model_->mapFromSource(items.first()->index());
    window_->tableView->selectRow(index.row());
    window_->tableView->scrollTo(index);
    window_->tableView->setFocus();
  }
}

void MainWindow::SaveNote(const Note& note) {
  model_->SaveNote(note);
  model_->ExportToFile(notes_filename_);

  window_->playerFilter->clear();
  window_->tableView->sortByColumn(0, Qt::AscendingOrder);

  HighlightPlayer(note.player);
}

void MainWindow::DeleteNote(const Note& note) {
  model_->DeleteNote(note);
  model_->ExportToFile(notes_filename_);
}

}  // namespace ftpnotes
