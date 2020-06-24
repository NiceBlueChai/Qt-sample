QT += widgets bluetooth

TEMPLATE = app

TARGET = bluetoothLE

HEADERS += \
    $$PWD/ble.h

SOURCES += \
    $$PWD/ble.cpp \
    $$PWD/main.cpp

FORMS = $$PWD/ble.ui
