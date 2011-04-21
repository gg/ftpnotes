#include "editnotewindow.h"

#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtGui/QPushButton>

namespace ftpnotes {

EditNoteWindow::EditNoteWindow(const Note& note, QWidget* parent)
    : QDialog(parent), window_(new Ui::EditNoteWindow), note_(note),
      save_button_(new QPushButton(tr("&Save"), this)),
      delete_button_(new QPushButton(tr("&Delete"), this)) {
  Initialize();
}

void EditNoteWindow::Initialize() {
  window_->setupUi(this);

  InitializeColorPicker();
  InitializeButtonBox();

  window_->textEdit->setText(note_.text);

  setWindowTitle(note_.player);

  UpdateWindowIcon();

  connect(window_->colorPicker, SIGNAL(currentIndexChanged(int)), this,
          SLOT(OnNoteColorIdChanged(int)));
  connect(window_->textEdit, SIGNAL(textChanged()), this,
          SLOT(OnNoteTextChanged()));
  connect(save_button_, SIGNAL(clicked()), this, SLOT(OnSaveClicked()));
  connect(delete_button_, SIGNAL(clicked()), this, SLOT(OnDeleteClicked()));

  window_->textEdit->setFocus();
}

void EditNoteWindow::InitializeColorPicker() {
  window_->colorPicker->setDuplicatesEnabled(true);
  window_->colorPicker->setIconSize(QSize(window_->colorPicker->width(),10));

  NoteColors note_colors;
  for (int i = 0; i < note_colors.count(); i++) {
    QPixmap pixmap(window_->colorPicker->width(), 10);
    pixmap.fill(note_colors.at(i));
    QIcon icon(pixmap);
    window_->colorPicker->insertItem(i, icon, "");
  }

  window_->colorPicker->setCurrentIndex(note_.color_id);
}

void EditNoteWindow::InitializeButtonBox() {
  window_->buttonBox->addButton(save_button_, QDialogButtonBox::ActionRole);
  window_->buttonBox->addButton(delete_button_, QDialogButtonBox::ActionRole);
}

void EditNoteWindow::OnNoteColorIdChanged(int color_id) {
  note_.color_id = static_cast<NoteColorId>(color_id);
  UpdateWindowIcon();
}

void EditNoteWindow::UpdateWindowIcon() {
  setWindowIcon(note_.icon());
}

void EditNoteWindow::OnNoteTextChanged() {
  note_.text = window_->textEdit->toPlainText();
}

void EditNoteWindow::OnSaveClicked() {
  emit SaveNote(note_);
  close();
}

void EditNoteWindow::OnDeleteClicked() {
  QMessageBox delete_confirmation;
  delete_confirmation.setWindowTitle(tr("Confirm Delete"));
  delete_confirmation.setText(
      tr("Are you sure you want to delete this player note?"));
  delete_confirmation.setStandardButtons(QMessageBox::Cancel);
  delete_confirmation.addButton(tr("Delete"), QMessageBox::AcceptRole);

  int ret = delete_confirmation.exec();

  switch (ret) {
    case QMessageBox::Cancel:
      break;
    default: // Delete button was clicked
      emit DeleteNote(note_);
      close();
  }
}

}  // namespace ftpnotes
