add_library(kml_contrib unzip.c iomem_simple.c)
add_dependencies(kml_contrib ${ZLIB_LIBRARY} ${MINIZIP_LIBRARY})
target_link_libraries(kml_contrib ${ZLIB_LIBRARY} ${MINIZIP_LIBRARY})
