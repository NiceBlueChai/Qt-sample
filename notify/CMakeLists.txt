cmake_minimum_required(VERSION 3.8)

project(notify)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type" FORCE)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Svg REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Svg REQUIRED)
if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 COMPONENTS Core5Compat REQUIRED)
endif()

add_executable(notify
    formnotify.ui
    formnotify.cpp
    formnotify.h
    main.cpp
    notifymanager.cpp
    notifymanager.h
    notify_resource.qrc
    )

target_include_directories(notify
    PRIVATE
    ${CMAKE_SOURCE_DIR}/utils)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(notify Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg Qt${QT_VERSION_MAJOR}::Core5Compat
        Qt${QT_VERSION_MAJOR}::Core5Compat
        utils)
else()
    target_link_libraries(notify Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Svg utils)
endif()
