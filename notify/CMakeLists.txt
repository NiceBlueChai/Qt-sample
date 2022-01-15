cmake_minimum_required(VERSION 3.8)

project(notify)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type" FORCE)
endif()

find_package(Qt5 CONFIG COMPONENTS Widgets Svg REQUIRED)


add_executable(notify
    formnotify.ui
    formnotify.cpp
    formnotify.h
    main.cpp
    notifymanager.cpp
    notifymanager.h
    notify_resource.qrc
    )

target_link_libraries(notify Qt5::Widgets Qt5::Svg)
