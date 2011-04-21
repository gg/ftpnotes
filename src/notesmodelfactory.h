// Defines a factory that creates NotesModel objects.

#pragma once
#ifndef FTPNOTES_NOTESMODELFACTORY_H__
#define FTPNOTES_NOTESMODELFACTORY_H__

#include <stdexcept>

#include <QtCore/QSharedPointer>

#include "notesmodel.h"

namespace ftpnotes {

class NotesModelCreationError : public std::runtime_error {
 public:
  NotesModelCreationError()
      : std::runtime_error("Unable to create notes model") {}
};

class NotesModelFactory {
 public:
  static QSharedPointer<NotesModel> Create(const QString& notes_filename);
};


inline QSharedPointer<NotesModel> NotesModelFactory::Create(
    const QString& notes_filename) {
  QFile notes_file(notes_filename);
  notes_file.open(QIODevice::ReadOnly);

  QDomDocument notes_xml;
  if (!notes_xml.setContent(&notes_file))
    throw NotesModelCreationError();

  QSharedPointer<NotesModel> notes_model(new NotesModel(notes_xml));
  return notes_model;
}

}  // namespace ftpnotes

#endif  // FTPNOTES_NOTESMODELFACTORY_H__
