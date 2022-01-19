
add_executable(tst-chars-utils WIN32 tst-chars-utils.cpp)
add_test(NAME tst-chars-utils COMMAND tst-chars-utils)

target_link_libraries(tst-chars-utils
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    utils
    )
add_dependencies(tst-chars-utils utils)
target_include_directories(tst-chars-utils
    PRIVATE
    ${CMAKE_SOURCE_DIR}/utils
)

add_executable(tst-gui-utils WIN32 tst-gui-utils.cpp)
add_test(NAME tst-gui-utils COMMAND tst-gui-utils)
target_link_libraries(tst-gui-utils
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    utils
    )
add_dependencies(tst-gui-utils utils)
target_include_directories(tst-gui-utils
    PRIVATE
    ${CMAKE_SOURCE_DIR}/utils
)
