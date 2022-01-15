TEMPLATE = lib
CONFIG += plugin static
QT +=widgets

DESTDIR = ../../plugins
TARGET = $$qtLibraryTarget(basictools)
INCLUDEPATH += $$PWD/../../app

HEADERS += \
    basictools.h

SOURCES += \
    basictools.cpp


