add_library(kml_contrib unzip.c iomem_simple.c)
  
target_link_libraries(kml_contrib ${ZLIB_LIBRARY} ${MINIZIP_LIBRARY})

install(TARGETS kml_contrib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)