if(WITH_PYTHON)
add_test( NAME test_python_kmlbase
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/kmlbase_test.py)
endif()