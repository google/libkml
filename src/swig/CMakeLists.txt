
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

if(WITH_PYTHON)
find_package(PythonLibs REQUIRED)
if(NOT PYTHONLIBS_FOUND)
  message(SEND_ERROR "Python Libraries not found.")
endif()

find_package ( PythonInterp REQUIRED )

include_directories(${PYTHON_INCLUDE_PATH})

execute_process(COMMAND
  python -c "from distutils.sysconfig import get_python_lib; print get_python_lib(True)"
  OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
  OUTPUT_STRIP_TRAILING_WHITESPACE )
endif(WITH_PYTHON)

# if(WITH_JAVA)

# endif(WITH_JAVA)

if(WITH_PYTHON)
  file(GLOB IFILES "kml*.i")
  foreach(INTERFACE_FILE ${IFILES})
    get_filename_component(MODULE_NAME ${INTERFACE_FILE} NAME_WE)
    set_source_files_properties(${INTERFACE_FILE} PROPERTIES CPLUSPLUS ON )
    set_source_files_properties(${INTERFACE_FILE} PROPERTIES CMAKE_SWIG_FLAGS "-includeall")
    set_source_files_properties( ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}PYTHON_wrap.cxx COMPILE_FLAGS "-Wno-long-long" )
    swig_add_module(${MODULE_NAME} python ${INTERFACE_FILE})
    swig_link_libraries(${MODULE_NAME} kmlbase kmlengine kmldom ${PYTHON_LIBRARIES})
    add_custom_command(
      TARGET _${MODULE_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E echo "Byte-compiling ${MODULE_NAME}.py"
      COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/cmake/pycompile.py ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.py
      DEPENDS kmlbase kmlengine kmldom)

    install(TARGETS
      _${MODULE_NAME}
      DESTINATION ${PYTHON_SITE_PACKAGES} )

    install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.py
      ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.pyc
      DESTINATION ${PYTHON_SITE_PACKAGES} )

  endforeach()

endif(WITH_PYTHON)
