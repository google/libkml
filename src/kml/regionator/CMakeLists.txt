file(GLOB SRCS "*.cc" )
build_target(kmlregionator "${SRCS}" kmlbase;kmldom;kmlengine;kmlconvenience)

file(GLOB include_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES ${include_files}
  DESTINATION ${KML_REGIONATOR_INCLUDE_DIR}
  COMPONENT Development)

install_target(kmlregionator)