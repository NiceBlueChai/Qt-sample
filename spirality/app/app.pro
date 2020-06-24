QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
DESTDIR = ..

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    paintarea.cpp

HEADERS += \
    interface.h \
    mainwindow.h \
    paintarea.h

LIBS =-L../plugins

LIBS +=-lbasictools
