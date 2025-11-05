cmake_minimum_required(VERSION 3.7)
project(chttp 
    VERSION 1.1.0
    LANGUAGES C
)


# verbosity
set(CMAKE_VERBOSE_MAKEFILE OFF)


# project configuration
include(config.cmake)


# public include directory
include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/include
)
configure_file(
  include/chttp/chttp.h.in 
  include/chttp/chttp.h 
  @ONLY
)


# cmake libs
include(cmake/build.cmake)
include(cmake/menu.cmake)
include(cmake/lint.cmake)
include(cmake/valgrind.cmake)
include(cmake/cpack.cmake)
include(cmake/uninstall.cmake)


# core tunneling and networking
add_subdirectory(chttp)


install(TARGETS chttp DESTINATION "lib")
install(FILES 
  ${PROJECT_BINARY_DIR}/include/chttp/chttp.h 
  ${PROJECT_SOURCE_DIR}/include/chttp/chunked.h
  ${PROJECT_SOURCE_DIR}/include/chttp/headerset.h
  ${PROJECT_SOURCE_DIR}/include/chttp/packet.h
  ${PROJECT_SOURCE_DIR}/include/chttp/request.h
  ${PROJECT_SOURCE_DIR}/include/chttp/response.h
  ${PROJECT_SOURCE_DIR}/include/chttp/status.h
  ${PROJECT_SOURCE_DIR}/include/chttp/types.h
  DESTINATION "include/chttp"
)


# test
enable_testing()
add_subdirectory(tests)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
