cmake_minimum_required(VERSION 3.8)

project(Qt-Sample)

add_subdirectory(BLE)
add_subdirectory(notify)
add_subdirectory(utils)
add_subdirectory(geoCoordinateEdit)
add_subdirectory(spirality)
add_subdirectory(VLCplayer)

option(BUILD_TESTS "build tests" OFF)

if(BUILD_TESTS)
    add_subdirectory(tests)
    enable_testing()
endif(BUILD_TESTS)
