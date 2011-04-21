// Defines the main application window.

#pragma once
#ifndef FTPNOTES_MAINWINDOW_H__
#define FTPNOTES_MAINWINDOW_H__

#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedPointer>
#include <QtCore/QScopedPointer>
#include <QtGui/QMainWindow>
#include <QtGui/QSortFilterProxyModel>

#include "note.h"
#include "notesmodel.h"

#include "ui_mainwindow.h"

namespace ftpnotes {

struct Note;
class NotesModel;

class MainWindow : public QMainWindow {
 Q_OBJECT
 public:
  MainWindow();
  ~MainWindow() {}

 signals:
  // When loading a new notes file, we destroy open editors.
  void CloseOpenNoteEditors();

 protected:
  void changeEvent(QEvent* e);

 private:
  void Initialize();

  void OpenNotesEditor(const QString& player);
  void OpenNotesEditor(const Note& note);
  void DisplayAddPlayerDialog();

  // Highlights a player in the player list.
  void HighlightPlayer(const QString& player);

  void LoadNotesFile(const QString& filename);

  QScopedPointer<Ui::MainWindow> window_;
  QString window_title_;

  QSharedPointer<NotesModel> model_;
  QScopedPointer<QSortFilterProxyModel> proxy_model_;

  QString notes_filename_;

 private slots:
  void OnOpenNotesFileTriggered();
  void FilterPlayers();
  void OnAddPlayerButtonClicked();
  void OpenNotesEditor(const QModelIndex& index);
  void SaveNote(const Note& note);
  void DeleteNote(const Note& note);
};

}  // namespace ftpnotes

#endif  // FTPNOTES_MAINWINDOW_H__
