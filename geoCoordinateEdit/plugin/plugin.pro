CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(geoCoordinateEditplugin)
TEMPLATE    = lib
DESTDIR = $$PWD/../bin
HEADERS     = geocoordinateeditplugin.h
SOURCES     = geocoordinateeditplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += designer
} else {
    CONFIG += designer
}

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(geocoordinateedit.pri)

QMAKE_POST_LINK = copy $$system_path($${DESTDIR}/$${TARGET}.dll) $$system_path($$[QT_INSTALL_PLUGINS]/designer)
message($$QMAKE_POST_LINK)
