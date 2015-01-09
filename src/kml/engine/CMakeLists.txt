file(GLOB SRCS "*.cc" )
build_target(kmlengine "${SRCS}" kmlbase;kmldom)

file(GLOB include_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES ${include_files}
  DESTINATION ${KML_ENGINE_INCLUDE_DIR}
  COMPONENT Development)

install_target(kmlengine)