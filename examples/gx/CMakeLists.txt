build_example(gpxfly kmlconvenience)
build_example(featuretour kmlconvenience)

if(INSTALL_EXAMPLES)
file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
install( FILES ${eg_files}  DESTINATION ${KML_EXAMPLES_DIR} COMPONENT Development)
endif()