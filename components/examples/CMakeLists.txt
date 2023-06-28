cmake_minimum_required(VERSION 3.12)

project(nbcui_examples VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type" FORCE)
endif()

if (MSVC)
    #    vs2017 or later
    if (${MSVC_VERSION} LESS  1910)
        message(FATAL_ERROR "Please use VS 15.0 (v141 toolset) or later")
        return()
    endif()
    add_compile_options(/utf-8 /EHsc)

endif()


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets  Core Svg REQUIRED)

add_definitions(-DNBC_NAMESPACE=nbc)
add_definitions(-DNBC_BUILD_LIB=1)

file(GLOB_RECURSE examples_sources ./*)


add_executable(nbcui_examples ${examples_sources})



target_link_libraries(nbcui_examples PUBLIC nbcui)


# nbc_ui.cmake.in

# install


