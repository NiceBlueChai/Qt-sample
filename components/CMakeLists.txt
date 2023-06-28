cmake_minimum_required(VERSION 3.12)

project(nbcui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")

option(NBC_BUILD_SHARED "Bulid shared library" ON)
option(BUILD_EXAMPLES "Build examples" ON)

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

file(GLOB_RECURSE source_files CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE header_files CONFIGURE_DEPENDS include/*.h  src/*.h)

set(source_files ${source_files} resource/nbc.qrc)

set(nbcui_lib_target "nbcui")

if (NBC_BUILD_SHARED)
    add_library(${nbcui_lib_target} SHARED "")
else()
    add_library(${nbcui_lib_target} STATIC "")
endif()

target_sources(${nbcui_lib_target}
    PRIVATE ${source_files}  ${header_files}
)

set(nbcui_public_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/navigation/navigation-widget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/navigation/navigation-panel.h
)

set(nbcui_private_headers
    ${CMAKE_CURRENT_SOURCE_DIR}/src/navigation/navigation-widget_p.h
)

set_target_properties(${nbcui_lib_target} PROPERTIES
    DEBUG_POSTFIX "_d"
    PUBLIC_HEADER "${nbcui_public_headers}"
    PRIVATE_HEADER "${nbcui_private_headers}"
)

target_include_directories(${nbcui_lib_target} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(${nbcui_lib_target} PUBLIC
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Svg
)


if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()


if(WIN32 AND NOT CYGWIN)
    set(DEF_INSTALL_CMAKEDIR lib/CMake)
else()
    set(DEF_INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME})
endif()

set(INSTALL_CMAKEDIR ${DEF_INSTALL_CMAKEDIR} CACHE PATH "Installation directory for CMake files")

# Installation
install(
  TARGETS
    ${nbcui_lib_target}
  EXPORT
    NBCUiTargets
  PUBLIC_HEADER
    DESTINATION include/nbcui
    COMPONENT dev
  PRIVATE_HEADER
    DESTINATION include/nbcui/private
    COMPONENT dev
)

install (
  EXPORT
    NBCUiTargets
  NAMESPACE
    "NBC::"
  DESTINATION
    ${INSTALL_CMAKEDIR}
  COMPONENT
    dev
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/NBCUiConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/NBCUiConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/NBCUiConfig.cmake
  INSTALL_DESTINATION ${INSTALL_CMAKEDIR}
)

install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/NBCUiConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/NBCUiConfigVersion.cmake
  DESTINATION
    ${INSTALL_CMAKEDIR}
)
install(
  FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/global/nbc.h
  DESTINATION
    include/nbcui/global
  COMPONENT
    dev
)

