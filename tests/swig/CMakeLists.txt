if(WITH_PYTHON)
add_test(NAME test_python_kmlbase
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/kmlbase_test.py)

add_test(NAME test_python_kmldom
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/kmldom_test.py)

add_test(NAME test_python_kmlengine
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/kmlengine_test.py)
endif()

if(WITH_JAVA)
add_test( NAME test_java_kmlbase
  COMMAND
  ${Java_JAVA_EXECUTABLE}
  -cp "${CMAKE_JAVA_INCLUDE_PATH}${PATH_SEPARATOR}${CMAKE_BINARY_DIR}/src/swig/LibKML.jar;${CMAKE_CURRENT_BINARY_DIR}"
  KmlBaseTest )

endif()