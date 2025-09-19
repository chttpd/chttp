cmake_minimum_required(VERSION 3.7)
project(chttp 
    VERSION 1.0.0
    LANGUAGES C
)


# verbosity
set(CMAKE_VERBOSE_MAKEFILE OFF)


# cmake libs
include (cmake/build.cmake)
include (cmake/menu.cmake)
include (cmake/lint.cmake)
include (cmake/valgrind.cmake)
include (cmake/cpack.cmake)
include (cmake/install.cmake)


# core tunneling and networking
add_subdirectory(chttp)


# test
enable_testing()
add_subdirectory(tests)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
