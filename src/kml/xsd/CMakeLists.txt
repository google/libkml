file(GLOB SRCS "*.cc" )
build_target(kmlxsd "${SRCS}" kmlbase)

file(GLOB include_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES ${include_files}
  DESTINATION ${KML_XSD_INCLUDE_DIR}
  COMPONENT Development)

install_target(kmlxsd)