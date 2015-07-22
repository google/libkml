build_example(
  NAME csvregionator
  DEPENDS kmlbase kmldom kmlengine kmlconvenience kmlregionator)

build_example(
  NAME kmlregionator
  DEPENDS kmlbase kmldom kmlengine kmlconvenience kmlregionator)

if(INSTALL_EXAMPLES)
  file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
  install(
    FILES ${eg_files}
    DESTINATION examples/regionator)
endif()
