
add_library(curlfetch curlfetch.cc)
target_link_libraries(curlfetch curl) #use CURL_LIBRARY


build_example(csvurl2gmap kmlconvenience curlfetch)
build_example(getgmapkml kmlconvenience curlfetch)
build_example(getgsheetcsv kmlconvenience curlfetch)
build_example(getgsheetkml kmlconvenience curlfetch)
build_example(gsheet2gmap kmlconvenience curlfetch)
build_example(kmlwalk kmlconvenience curlfetch)
build_example(kmzfetch kmlconvenience curlfetch)
build_example(listfeed kmlconvenience curlfetch)
build_example(listgdocs kmlconvenience curlfetch)
build_example(listgmaps kmlconvenience curlfetch)
build_example(listgphotos kmlconvenience curlfetch)
build_example(resolvestyle kmlconvenience curlfetch)
build_example(searchgmap kmlconvenience curlfetch)
build_example(uploadgmap kmlconvenience curlfetch)
build_example(uploadgmapcsv kmlconvenience curlfetch)
build_example(uploadgmapkml kmlconvenience curlfetch)
build_example (uploadgsheet kmlconvenience curlfetch)
build_example(xsdprint kmlconvenience kmlxsd curlfetch)

if(INSTALL_EXAMPLES)
file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
install(FILES ${eg_files}  DESTINATION ${KML_EXAMPLES_DIR} COMPONENT Development)
install(FILES README curlfetch.h prompt.h
  DESTINATION ${KML_EXAMPLES_DIR}
  COMPONENT Development)
endif()
