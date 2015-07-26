find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

if(WITH_PYTHON)
  find_package(PythonLibs REQUIRED)
  if(NOT PYTHONLIBS_FOUND)
    message(FATAL_ERROR "Python Libraries not found.")
  endif()

  find_package(PythonInterp REQUIRED)

  include_directories(${PYTHON_INCLUDE_PATH})

  # execute_process(COMMAND
  #   python -c "from distutils.sysconfig import get_python_lib; print get_python_lib(True)"
  #   OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
  #   OUTPUT_STRIP_TRAILING_WHITESPACE )

  file(GLOB IFILES "kml*.i")
  foreach(INTERFACE_FILE ${IFILES})
    get_filename_component(MODULE_NAME ${INTERFACE_FILE} NAME_WE)
    set_source_files_properties(${INTERFACE_FILE} PROPERTIES CPLUSPLUS ON )
    set_source_files_properties(${INTERFACE_FILE} PROPERTIES CMAKE_SWIG_FLAGS "-includeall -DSWIGPYTHON")
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
      DESTINATION ${PYTHON_INSTALL_DIR})

    install(FILES
      ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.py
      ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}.pyc
      DESTINATION ${PYTHON_INSTALL_DIR})

  endforeach()

endif(WITH_PYTHON)

if(WITH_JAVA)
  find_package(Java REQUIRED)
  find_package(JNI REQUIRED)
  mark_as_advanced(JAVA_HOME)

  include_directories(${JAVA_INCLUDE_PATH}
    ${JNI_INCLUDE_DIRS})
  include_directories ( ${JAVA_INCLUDE_PATH} ${JNI_INCLUDE_DIRS} )

  # Make sure the nested directory structure exists
  set(JAVA_SOURCE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/com/googlecode/libkml)
  set(JAVA_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build)
  if(NOT EXISTS "${JAVA_SOURCE_DIRECTORY}")
    file(MAKE_DIRECTORY ${JAVA_SOURCE_DIRECTORY})
  endif()
  if(NOT EXISTS "${JAVA_BINARY_DIRECTORY}")
    file(MAKE_DIRECTORY ${JAVA_BINARY_DIRECTORY})
  endif()
  #  java_package_dir = com/googlecode/libkml
  file(GLOB IFILES "kml*.i")
  set(LIBKML_JAVA_PACKAGE "com.googlecode.libkml")
  set(CMAKE_SWIG_OUTDIR ${JAVA_SOURCE_DIRECTORY})
  set(CMAKE_SWIG_FLAGS -package "${LIBKML_JAVA_PACKAGE}" ${CMAKE_SWIG_GLOBAL_FLAGS})

  foreach(INTERFACE_FILE ${IFILES})
    get_filename_component(MODULE_NAME ${INTERFACE_FILE} NAME_WE)
    set_source_files_properties(${INTERFACE_FILE} PROPERTIES CPLUSPLUS ON)
    set_source_files_properties(${INTERFACE_FILE} PROPERTIES CMAKE_SWIG_FLAGS "-includeall  -DSWIGJAVA")
    set_source_files_properties( ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_NAME}JAVA_wrap.cxx COMPILE_FLAGS "-w")
    swig_add_module(${MODULE_NAME}_swig_java java ${INTERFACE_FILE})
    swig_link_libraries(${MODULE_NAME}_swig_java kmlbase kmlengine kmldom)
    install(TARGETS ${MODULE_NAME}_swig_java DESTINATION ${LIB_INSTALL_DIR})
  endforeach()

  add_custom_target(LibKML_jar ALL DEPENDS LibKML.jar)

  add_custom_command(
    OUTPUT LibKML.jar
    COMMENT "Creating jar file..."
    COMMAND ${Java_JAVAC_EXECUTABLE} -d ${JAVA_BINARY_DIRECTORY} ${JAVA_SOURCE_DIRECTORY}/*.java
    COMMAND ${Java_JAR_EXECUTABLE} cf ${CMAKE_CURRENT_BINARY_DIR}/LibKML.jar -C ${JAVA_BINARY_DIRECTORY} com
    DEPENDS LibKML_jar kmlbase_java kmldom_java kmlengine_java kmlengine kmlbase kmldom)

   # Get the location of the extension directory
   string(REGEX REPLACE "include"  "jre/lib/ext" JAVA_EXTENSION_DIR ${JAVA_INCLUDE_PATH})

   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibKML.jar
     DESTINATION ${JAVA_INSTALL_DIR})

endif(WITH_JAVA)
