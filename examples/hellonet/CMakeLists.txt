
add_library(curlfetch curlfetch.cc)
target_link_libraries(curlfetch ${CURL_LIBRARIES})

build_example(
  NAME csvurl2gmap
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME getgmapkml
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME getgsheetcsv
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME getgsheetkml
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME gsheet2gmap
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME kmlwalk
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME kmzfetch
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME listfeed
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME listgdocs
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME listgmaps
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME listgphotos
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME resolvestyle
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME searchgmap
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME uploadgmap
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME uploadgmapcsv
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME uploadgmapkml
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME uploadgsheet
  DEPENDS kmlconvenience curlfetch)

build_example(
  NAME xsdprint
  DEPENDS kmlconvenience kmlxsd curlfetch)

if(INSTALL_EXAMPLES)
file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
install(
  FILES ${eg_files}
  DESTINATION examples/hellonet)

install(
  FILES README curlfetch.h prompt.h
  DESTINATION examples/hellonet)
endif()
