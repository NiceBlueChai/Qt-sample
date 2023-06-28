cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(utils VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)

set(PROJECT_SOURCES
    chars-utils.cpp
    chars-utils.h
    gui-utils.cpp
    gui-utils.h
    paint-utils.cpp
    paint-utils.h
)

add_library(utils ${PROJECT_SOURCES})
target_link_libraries(utils PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
