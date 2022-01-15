cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(basictools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
#if (${QT_VERSION_MAJOR} EQUAL 6)
# 与Q_IMPORT_PLUGIN(BasicToolsPlugin)冲突了，会加载两遍插件，暂时全使用手动定义宏的方式进行插件编译
#qt_add_plugin(
#    ${PROJECT_NAME}
#    STATIC
#    CLASS_NAME BasicToolsPlugin
#    )
#else()
#    add_definitions(-DQT_STATICPLUGIN)
#    add_library(${PROJECT_NAME} STATIC "")
#endif()

add_definitions(-DQT_STATICPLUGIN)
add_library(${PROJECT_NAME} STATIC )

target_sources(${PROJECT_NAME}
    PRIVATE
    basictools.cpp
    basictools.h
    basictools.json
    )

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../app
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    )
