add_library(kml_contrib unzip.c iomem_simple.c)
add_dependencies(kml_contrib ${ZLIB_LIBRARY} ${MINIZIP_LIBRARY})
target_link_libraries(kml_contrib ${ZLIB_LIBRARY} ${MINIZIP_LIBRARY})

install(TARGETS kml_contrib
    RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
    LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION lib COMPONENT Development)