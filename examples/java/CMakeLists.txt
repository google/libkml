if(INSTALL_EXAMPLES)
  file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.java")
  install( FILES ${eg_files}  DESTINATION ${KML_EXAMPLES_DIR} COMPONENT Development) ##compile java
  install( FILES run.sh DESTINATION ${KML_EXAMPLES_DIR} COMPONENT Development) ##
endif()
