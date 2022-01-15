cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(geoCoordinateEdit VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Designer REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Designer REQUIRED)

add_library(geoCoordinateEdit "")
target_sources(geoCoordinateEdit
    PRIVATE
    geocoordinateedit.cpp
    geocoordinateeditplugin.cpp
    geocoordinateedit.h
    geocoordinateeditplugin.h
    icons.qrc
    )
target_link_libraries(geoCoordinateEdit PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Designer)
