include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_library(store OBJECT store.c store.h)
add_library(str OBJECT str.c str.h)
add_library(uri OBJECT uri.c uri.h)
add_library(request OBJECT request.c request.h)
add_library(chttp STATIC
  $<TARGET_OBJECTS:store>
  $<TARGET_OBJECTS:str>
  $<TARGET_OBJECTS:uri>
  $<TARGET_OBJECTS:request>
)
