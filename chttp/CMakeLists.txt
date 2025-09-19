set(CONFIG_CHTTP_REQUEST_BUFFSIZE 8192)
set(CONFIG_CHTTP_REQUEST_HEADERSMAX 32)
configure_file(config.h.in config.h @ONLY)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)


# configure_file(
#   ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in 
#   ${CMAKE_CURRENT_BINARY_DIR}/config.h 
#   @ONLY
# )
configure_file(
  config.h.in 
  config.h 
  @ONLY
)


add_library(linearbuffer OBJECT linearbuffer.c linearbuffer.h)
add_library(chttp STATIC
  $<TARGET_OBJECTS:linearbuffer>
)
