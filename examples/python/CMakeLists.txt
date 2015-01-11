if(INSTALL_EXAMPLES)
  file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.py")
  install( FILES ${eg_files}  DESTINATION ${KML_EXAMPLES_DIR} COMPONENT Development) ##compile python
endif()
