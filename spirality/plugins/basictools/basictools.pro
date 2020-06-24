TEMPLATE = lib
CONFIG += plugin static
QT +=widgets

DESTDIR = ../../plugins
TARGET = $$qtLibraryTarget(basictools)
INCLUDEPATH += ../../app

HEADERS += \
    basictools.h

SOURCES += \
    basictools.cpp


