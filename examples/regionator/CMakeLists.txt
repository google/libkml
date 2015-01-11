

build_example(csvregionator kmlbase kmldom kmlengine kmlconvenience kmlregionator)
build_example(kmlregionator kmlbase kmldom kmlengine kmlconvenience kmlregionator)

if(INSTALL_EXAMPLES)
file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
install(FILES ${eg_files}  DESTINATION ${KML_EXAMPLES_DIR} COMPONENT Development)
endif()
