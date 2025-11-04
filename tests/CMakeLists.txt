include(CTest)
list(APPEND testrules
  str
  store
  status
  request
  packetbuilder
  response
  headerset
  chunked
)


list(TRANSFORM testrules PREPEND test_)
set(profilerules ${testrules})
list(TRANSFORM profilerules APPEND -profile)
add_custom_target(profile_tests DEPENDS ${profilerules})

add_library(fixtures OBJECT fixtures.c fixtures.h)
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/chttp
)

foreach (t IN LISTS testrules) 
    # Test help
  add_executable(${t} ${t}.c 
    $<TARGET_OBJECTS:fixtures>
  )
  target_include_directories(${t} PUBLIC 
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}"
  )
  target_link_libraries(${t} chttp)
  add_test(NAME ${t} COMMAND ${t})

  add_custom_target(${t}-exec 
    COMMAND ctest --verbose -R "^${t}$"
    DEPENDS ${t} ${CMAKE_PROJECT_NAME}
  )
  add_custom_target(${t}-debug 
    COMMAND gdb --command=${PROJECT_SOURCE_DIR}/tests/${t}.gdb ./${t}
    DEPENDS ${t} ${CMAKE_PROJECT_NAME}
  )
  add_custom_target(${t}-profile
    COMMAND "valgrind" ${VALGRIND_FLAGS} ${PROJECT_BINARY_DIR}/tests/${t}
    DEPENDS ${t} ${CMAKE_PROJECT_NAME}
  )
endforeach()
