add_library(kml_contrib unzip.c iomem_simple.c)

if(NOT HAVE_SYSTEM_ZLIB)

  message(STATUS "zlib_inc3x_check=${ZLIB_INCLUDE_DIR}")
  set( ZLIB_INCLUDE_DIR ${CMAKE_BINARY_DIR}/third_party/zlib-1.2.8)
  
include_directories(${CMAKE_BINARY_DIR}/third_party/zlib-1.2.8)
endif()

  
message(STATUS "zlib_inc3x=${ZLIB_INCLUDE_DIR}")

target_link_libraries(kml_contrib ${ZLIB_LIBRARY} ${MINIZIP_LIBRARY})

install(TARGETS kml_contrib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)