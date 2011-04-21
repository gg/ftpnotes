// Defines a Note.

#pragma once
#ifndef FTPNOTES_NOTE_H__
#define FTPNOTES_NOTE_H__

// Qt does not compile well at warning level 4, use warning level 3 instead.
#pragma warning(push, 3)
#include <QtCore/QList>
#include <QtGui/QColor>
#include <QtGui/QIcon>
#pragma warning(pop)

namespace ftpnotes {

enum NoteColorId {
  kColor_Purple = 0,
  kColor_Pink,
  kColor_Red,
  kColor_Orange,
  kColor_YellowOrange,
  kColor_Yellow,
  kColor_YellowGreen,
  kColor_Green,
  kColor_BlueGreen,
  kColor_LightBlue,
  kColor_Blue,
  kColor_DarkBlue,
  kColor_DarkPurple,
  kColor_Violet
};

struct NoteColors : QList<QColor> {
  NoteColors() {
    // Order matters: each item corresponds to a NoteColorId.
    append(QColor(217,0,255)); // [0] -> Purple
    append(QColor(255,83,229)); // [1] -> Pink
    append(QColor(222,23,2));   // [2] -> Red
    append(QColor(255,109,24)); // [3] -> Orange
    append(QColor(255,173,0));  // [4] -> YellowOrange
    append(QColor(255,240,39)); // [5] -> Yellow
    append(QColor(209,255,3));  // [6] -> YellowGreen
    append(QColor(99,255,21));  // [7] -> Green
    append(QColor(19,209,132)); // [8] -> BlueGreen
    append(QColor(2,246,255));  // [9] -> LightBlue
    append(QColor(5,154,250));  // [10] -> Blue
    append(QColor(12,64,215)); // [11] -> DarkBlue
    append(QColor(130,21,255)); // [12] -> DarkPurple
    append(QColor(184,0,241)); // [13] -> Violet
  }
};


struct Note {
  Note() {}
  Note(const QString& player, const QString& text = QString(),
       NoteColorId color_id = kColor_YellowGreen)
      : player(player), text(text), color_id(color_id) {}

  QColor color() const {
    static NoteColors colors;
    return colors.at(color_id);
  }

  QIcon icon() const {
    QString icon_filename = QString(":/icons/%1.png").arg(color_id);
    return QIcon(QPixmap(icon_filename));
  }

  QString player;
  QString text;
  NoteColorId color_id;
};

}  // namespace ftpnotes

#endif  // FTPNOTES_NOTE_H__
