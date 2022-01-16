cmake_minimum_required(VERSION 3.8)

project(VLCPlayer)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build Type" FORCE)
endif()


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


add_executable(VLCPlayer
    vlcplayer.ui
    vlcplayer.cpp
    vlcplayer.h
    main.cpp
)

if(UNIX)
    include(FindPkgConfig)
    FIND_PACKAGE(PkgConfig REQUIRED)
    PKG_CHECK_MODULES(VLC REQUIRED libvlc>=3.0.1)

    target_link_libraries(VLCPlayer Qt${QT_VERSION_MAJOR}::Widgets ${VLC_LIBRARIES} utils)
endif(UNIX)


if(MSVC)
    option(AUTO_DEPLOY "auto deploy use windeployqt.exe" ON)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/sdk/include)
    target_link_directories(VLCPlayer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/sdk/lib)
    target_link_libraries(VLCPlayer Qt${QT_VERSION_MAJOR}::Widgets libvlccore libvlc utils)

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
        set(VCVAR64_BAT_PATH CACHE PATH "vcvars64.bat path" "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/")
        # C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat
        find_program(BAT_vcvars_platform NAMES vcvars64.bat
            PATHS ${VCVAR64_BAT_PATH})
        if(NOT BAT_vcvars_platform)
            message(FATAL_ERROR "cannot find vcvars64.bat")
        endif()
        find_program(TOOL_WINDEPLOYQT NAMES windeployqt PATHS "${CMAKE_PREFIX_PATH}/tools/qt5")
        if(NOT TOOL_WINDEPLOYQT)
            message(FATAL_ERROR "cannot find windeployqt.exe")
        endif()
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

target_include_directories(VLCPlayer PRIVATE ${CMAKE_SOURCE_DIR}/utils)
