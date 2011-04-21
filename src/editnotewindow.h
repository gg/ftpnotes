// Defines a simple note editor with the ability to edit a note's text and
// color, and save or delete a note.

#pragma once
#ifndef FTPNOTES_EDITNOTEWINDOW_H__
#define FTPNOTES_EDITNOTEWINDOW_H__

#include <QtCore/QScopedPointer>
#include <QtGui/QDialog>
#include <QtGui/QPushButton>

#include "note.h"

#include "ui_editnote.h"

namespace ftpnotes {

struct Note;

class EditNoteWindow : public QDialog {
 Q_OBJECT
 public:
  EditNoteWindow(const Note& note, QWidget* parent = 0);

  ~EditNoteWindow() {}

 signals:
  void SaveNote(const Note& note);
  void DeleteNote(const Note& note);

 private:
  void Initialize();
  void InitializeColorPicker();
  void InitializeButtonBox();

  QScopedPointer<Ui::EditNoteWindow> window_;
  Note note_;
  QPushButton* save_button_;
  QPushButton* delete_button_;

 private slots:
  void OnNoteColorIdChanged(int color_id);
  void OnNoteTextChanged();
  void UpdateWindowIcon();
  void OnSaveClicked();
  void OnDeleteClicked();
};

}  // namespace ftpnotes

#endif  // FTPNOTES_EDITNOTEWINDOW_H__
