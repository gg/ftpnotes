TEMPLATE = app
TARGET = ftpnotes
DEPENDPATH += .
DESTDIR = bin
INCLUDEPATH += .
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build

HEADERS += src/editnotewindow.h \
           src/notesmodel.h \
           src/notesmodelfactory.h \
           src/notesxmlvalidator.h \
           src/mainwindow.h
FORMS += src/editnote.ui src/mainwindow.ui
SOURCES += src/main.cc src/mainwindow.cc src/editnotewindow.cc
RESOURCES += src/resources.qrc

CONFIG += release
QT += core gui xml xmlpatterns
