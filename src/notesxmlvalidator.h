// Defines a validator that validates a FullTiltPoker XML notes file.

#pragma once
#ifndef FTPNOTES_NOTESXMLVALIDATOR_H__
#define FTPNOTES_NOTESXMLVALIDATOR_H__

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QString>
#include <QtXmlPatterns/QXmlSchema>
#include <QtXmlPatterns/QXmlSchemaValidator>

namespace ftpnotes {

class NotesXMLValidator {
 public:
  static bool Validate(const QString& xml_filepath);

 private:
  static bool Validate(QFile& xml_file,
                       const QString& schema_filepath);
};

inline bool NotesXMLValidator::Validate(const QString& xml_filepath) {
  QDir schemas_dir(":/schemas");

  QFile xml_file(xml_filepath);
  if (!xml_file.open(QIODevice::ReadOnly)) {
    return false;
  }

  foreach (const QString& schema_filename, schemas_dir.entryList()) {
    QString schema_filepath = schemas_dir.absoluteFilePath(schema_filename);
    if (Validate(xml_file, schema_filepath))
      return true;
  }

  return false;
}

inline bool NotesXMLValidator::Validate(QFile& xml_file,
                                        const QString& schema_filepath) {
  QFile schema_file(schema_filepath);
  if (!schema_file.open(QIODevice::ReadOnly)) {
    return false;
  }

  QXmlSchema schema;
  schema.load(&schema_file);

  if (!schema.isValid()) return false;

  QXmlSchemaValidator validator(schema);

  return validator.validate(
      &xml_file, QUrl::fromLocalFile(xml_file.fileName()));
}

}  // namespace ftpnotes

#endif  // FTPNOTES_NOTESXMLVALIDATOR_H__
