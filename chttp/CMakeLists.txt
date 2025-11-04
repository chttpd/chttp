include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_library(common OBJECT common.c common.h)
add_library(store OBJECT store.c store.h)
add_library(str OBJECT str.c str.h)
add_library(chunked OBJECT chunked.c chunked.h)
add_library(uri OBJECT uri.c uri.h)
add_library(headerset OBJECT headerset.c headerset.h)
add_library(status OBJECT status.c status.h)
add_library(request OBJECT request.c request.h)
add_library(packetbuilder OBJECT packetbuilder.c packetbuilder.h)
add_library(response OBJECT response.c response.h)
add_library(chttp STATIC
  $<TARGET_OBJECTS:common>
  $<TARGET_OBJECTS:store>
  $<TARGET_OBJECTS:str>
  $<TARGET_OBJECTS:chunked>
  $<TARGET_OBJECTS:status>
  $<TARGET_OBJECTS:uri>
  $<TARGET_OBJECTS:headerset>
  $<TARGET_OBJECTS:request>
  $<TARGET_OBJECTS:packetbuilder>
  $<TARGET_OBJECTS:response>
)
