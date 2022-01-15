QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nbcnotify
TEMPLATE = app
DESTDIR = $$PWD/../bin
CONFIG += warn_off c++11

FORMS += \
    formnotify.ui

HEADERS += \
    formnotify.h \
    notifymanager.h

SOURCES += \
    formnotify.cpp \
    main.cpp \
    notifymanager.cpp
