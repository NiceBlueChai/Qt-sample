cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(geoCoordinateEdit-demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} "")

target_sources(${PROJECT_NAME}
    PRIVATE
    main.cpp
    widget.cpp
    widget.h
    widget.ui)
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../plugin
    ${CMAKE_SOURCE_DIR}/utils
    )
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    geoCoordinateEdit
    Qt${QT_VERSION_MAJOR}::Widgets
    utils
    )

