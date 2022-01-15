cmake_minimum_required(VERSION 3.8)

project(BLE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type" FORCE)
endif()
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Bluetooth REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Bluetooth REQUIRED)

if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 COMPONENTS Core5Compat REQUIRED)
endif()

#if (Qt5Widgets_FOUND)
#    if (Qt5Widgets_VERSION VERSION_LESS 5.12.0)
#        message(FATAL_ERROR "Minimum supported Qt5 version is 5.12.0!")
#    endif()
#else()
#    message(SEND_ERROR "The Qt5Widgets library could not be found!")
#endif(Qt5Widgets_FOUND)
add_executable(ble ble.cpp ble.h ble.ui main.cpp)

if(QT_VERSION_MAJOR EQUAL 6)
    target_link_libraries(ble PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Bluetooth Qt${QT_VERSION_MAJOR}::Core5Compat)
else()
    target_link_libraries(ble PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Bluetooth)
endif()
