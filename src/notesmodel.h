// Defines a NotesModel that is a wrapper over a FullTiltPoker notes XML file.

#ifndef FTPNOTES_NOTESMODEL_H__
#define FTPNOTES_NOTESMODEL_H__

#include <QtCore/QFile>
#include <QtCore/QHash>
#include <QtCore/QTextStream>
#include <QtGui/QColor>
#include <QtGui/QStandardItemModel>
#include <QtXml/QDomDocument>

#include "note.h"

namespace ftpnotes {

namespace detail {
  static const QString kNewlineSeparator = "&#xa;";
  static const QString kNoteTagName = "NOTE";
  static const QString kPlayerAttribute = "PlayerId";
  static const QString kColorAttribute = "ColourIx";
  static const QString kNoteTextAttribute = "Text";
}  // namespace detail

class NotesModel : public QStandardItemModel {
 public:
  NotesModel(QDomDocument notes_xml) : notes_xml_(notes_xml) {
    Initialize();
  }

  Note GetNote(const QString& player) const;
  bool HasNoteForPlayer(const QString& player) const;
  void SaveNote(const Note& note);

  void DeleteNote(const Note& note) { DeleteNote(note.player); }
  void DeleteNote(const QString& player);

  void ExportToFile(const QString& filename);

 private:
  void Initialize();
  void InitializeNotes();
  void InitializeModelData();

  void UpdateNote(const Note& note);
  void AddNote(const Note& note);
  void AddNoteItem(const Note& note);


  NoteColorId GetNoteColorId(const QString& player) const;
  QString GetNoteText(const QString& player) const;

  QDomDocument notes_xml_;
  // Maps player to a note entry dom element
  QHash<QString, QDomElement> notes_;
  // Maps player to a note item
  QHash<QString, QStandardItem*> note_items_;
};


inline Note NotesModel::GetNote(const QString& player) const {
  if (HasNoteForPlayer(player))
    return Note(player, GetNoteText(player), GetNoteColorId(player));
  else
    return Note(player);
}

inline bool NotesModel::HasNoteForPlayer(const QString& player) const {
  return notes_.contains(player);
}

inline void NotesModel::SaveNote(const Note& note) {
  if (HasNoteForPlayer(note.player))
    UpdateNote(note);
  else
    AddNote(note);
}

inline void NotesModel::UpdateNote(const Note& note) {
  notes_[note.player].setAttribute(detail::kColorAttribute, note.color_id);
  notes_[note.player].setAttribute(detail::kNoteTextAttribute, note.text);
  note_items_[note.player]->setIcon(note.icon());
  note_items_[note.player]->setToolTip(note.text);
}

inline void NotesModel::AddNote(const Note& note) {
  QDomElement note_element = notes_xml_.createElement(detail::kNoteTagName);
  note_element.setAttribute(detail::kPlayerAttribute, note.player);
  note_element.setAttribute(detail::kColorAttribute, note.color_id);
  note_element.setAttribute(detail::kNoteTextAttribute, note.text);

  QDomElement root = notes_xml_.documentElement();
  QDomNode notes_node = root.firstChild();
  notes_node.appendChild(note_element);
  notes_.insert(note.player, note_element);

  AddNoteItem(note);
}

inline void NotesModel::DeleteNote(const QString& player) {
  if (!HasNoteForPlayer(player)) return;

  QDomElement root = notes_xml_.documentElement();
  QDomNode notes_node = root.firstChild();

  removeRow(note_items_[player]->row());

  notes_node.removeChild(notes_[player]);
  notes_.remove(player);
  note_items_.remove(player);
}

inline void NotesModel::ExportToFile(const QString& filename) {
  QFile output_file(filename);
  output_file.open(QIODevice::WriteOnly);
  QTextStream out(&output_file);
  notes_xml_.save(out, 0);
  output_file.close();
}

inline void NotesModel::Initialize() {
  InitializeNotes();
  InitializeModelData();
}

inline void NotesModel::InitializeNotes() {
  QDomElement root = notes_xml_.documentElement();
  QDomNode notes_node = root.firstChild();

  QDomElement note_element = notes_node.firstChildElement(detail::kNoteTagName);

  while (!note_element.isNull()) {
    QString player = note_element.attribute(detail::kPlayerAttribute);
    notes_.insert(player, note_element);
    note_element = note_element.nextSiblingElement(detail::kNoteTagName);
  }
}

inline void NotesModel::InitializeModelData() {
  setColumnCount(1);
  setHeaderData(0, Qt::Horizontal, tr("Player"));
  foreach (const QString& player, notes_.keys()) {
    Note note(GetNote(player));
    AddNoteItem(note);
  }
}

inline void NotesModel::AddNoteItem(const Note& note) {
  QStandardItem* item = new QStandardItem(note.player);
  item->setIcon(note.icon());
  item->setToolTip(note.text);

  appendRow(item);
  note_items_.insert(note.player, item);
}

inline NoteColorId NotesModel::GetNoteColorId(const QString& player) const {
  return static_cast<NoteColorId>(
      notes_[player].attribute(detail::kColorAttribute).toInt());
}

inline QString NotesModel::GetNoteText(const QString& player) const {
  return notes_[player].attribute(detail::kNoteTextAttribute);
}

}  // namespace ftpnotes

#endif // FTPNOTES_NOTESMODEL_H__
