include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_library(store OBJECT store.c store.h)
add_library(str OBJECT str.c str.h)
add_library(buffprinter OBJECT buffprinter.c buffprinter.h)
add_library(uri OBJECT uri.c uri.h)
add_library(status OBJECT status.c status.h)
add_library(request OBJECT request.c request.h)
add_library(response OBJECT response.c response.h)
add_library(chttp STATIC
  $<TARGET_OBJECTS:store>
  $<TARGET_OBJECTS:str>
  $<TARGET_OBJECTS:buffprinter>
  $<TARGET_OBJECTS:status>
  $<TARGET_OBJECTS:uri>
  $<TARGET_OBJECTS:request>
  $<TARGET_OBJECTS:response>
)
