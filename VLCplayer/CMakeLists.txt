cmake_minimum_required(VERSION 3.8)

project(VLCPlayer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type" FORCE)
endif()

if(NOT MSVC)
    message(WARNING "Current vlc library in sdk Only supported MSVC_64 compiler(you can use another libvlc library)")
if(UNIX)
include(FindPkgConfig)
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(VLC REQUIRED libvlc>=3.0.1)
endif(Unix)
endif(NOT MSVC)

find_package(Qt5 CONFIG COMPONENTS Widgets REQUIRED)
option(AUTO_DEPLOY "auto deploy use windeployqt.exe" ON)
if(AUTO_DEPLOY)
    if(WIN32)
        # C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat
        find_program(BAT_vcvars_platform NAMES vcvars64.bat
            PATHS "C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/VC/Auxiliary/Build")
        if(NOT BAT_vcvars_platform)
            message(FATAL_ERROR "cannot find vcvars64.bat")
        endif()
        find_program(TOOL_WINDEPLOYQT NAMES windeployqt PATHS "${CMAKE_PREFIX_PATH}/tools/qt5")
        if(NOT TOOL_WINDEPLOYQT)
            message(FATAL_ERROR "cannot find windeployqt.exe")
        endif()


    endif()
endif(AUTO_DEPLOY)



add_executable(VLCPlayer
    vlcplayer.ui
    vlcplayer.cpp
    vlcplayer.h
    main.cpp
)

if(UNIX)
target_link_libraries(VLCPlayer Qt5::Widgets ${VLC_LIBRARIES})
endif(UNIX)


if(MSVC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sdk/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/sdk/lib)
target_link_libraries(VLCPlayer Qt5::Widgets libvlccore libvlc)

install(TARGETS VLCPlayer
        EXPORT MyLibTargets
        LIBRARY DESTINATION bin  # 动态库安装路径
        ARCHIVE DESTINATION lib  # 静态库安装路径
        RUNTIME DESTINATION bin  # 可执行文件安装路径
        PUBLIC_HEADER DESTINATION include  # 头文件安装路径
)

install(
    FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/bin/libvlc.dll
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/bin/libvlccore.dll
    DESTINATION
    bin)
install(
    DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/sdk/plugins
    DESTINATION
    bin)

if(AUTO_DEPLOY)
    # 如果环境变量中找不到msvc运行时库就只会复制qt的库
#    install(CODE "

#                execute_process(
#                        COMMAND ${TOOL_WINDEPLOYQT} \"\${CMAKE_INSTALL_PREFIX}/bin/VLCPlayer.exe\" -verbose=2
#                )
#    ")
# 执行安装后再生成这个，就会自动将依赖库复制过来
add_custom_target(deploy_target
    COMMAND cmd.exe /k \"\"${BAT_vcvars_platform}\"&&${TOOL_WINDEPLOYQT} \"${CMAKE_INSTALL_PREFIX}/bin/VLCPlayer.exe\" -verbose=2\")
endif()
endif(MSVC)
