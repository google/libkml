if(INSTALL_EXAMPLES)
  file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.java")
  install(
    FILES ${eg_files}
    DESTINATION examples/java) ##compile java
  install(
    FILES run.sh
    DESTINATION examples/java) ##
endif()
