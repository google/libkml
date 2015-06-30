if(INSTALL_EXAMPLES)
  file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.py")
  install(
    FILES ${eg_files}
    DESTINATION examples/python) ##compile python
endif()
