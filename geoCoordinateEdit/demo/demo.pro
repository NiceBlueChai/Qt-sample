include($$PWD/../plugin/geocoordinateEdit.pri)

QT  += widgets

TEMPLATE = app

DESTDIR = $$PWD/../bin

FORMS += \
    widget.ui

HEADERS += \
    widget.h

SOURCES += \
    main.cpp \
    widget.cpp

