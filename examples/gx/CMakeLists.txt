
build_example(
  NAME gpxfly
  DEPENDS kmlconvenience)

build_example(
  NAME featuretour
  DEPENDS kmlconvenience)


if(INSTALL_EXAMPLES)
file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
install(
  FILES ${eg_files}
  DESTINATION examples/gx)
endif()