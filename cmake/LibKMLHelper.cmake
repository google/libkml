macro(build_target)
  cmake_parse_arguments(LIB  "" "NAME" "SRCS;INCS;LINKS;DEPENDS" ${ARGN} )
  add_library(${LIB_NAME} ${LIB_SRCS})
  foreach(LIB_DEPEND ${LIB_DEPENDS})
    add_dependencies(${LIB_NAME} ${LIB_DEPEND})
  endforeach()
  target_link_libraries(${LIB_NAME} ${LIB_DEPENDS} ${LIB_LINKS})
  if(VERSION_STRING)
    set_target_properties(${LIB_NAME} PROPERTIES
      VERSION   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
      SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
  endif()
  set(kmlbase__INCLUDE_DIR "dd/ddfg")
  string(LENGTH ${LIB_NAME} ${LIB_NAME}_LEN)
  MATH(EXPR ${LIB_NAME}_END "${${LIB_NAME}_LEN} - 3")
  string(SUBSTRING ${LIB_NAME} 3 ${${LIB_NAME}_END} ${LIB_NAME}_INCLUDE_DIR)
  install(
    FILES ${LIB_INCS}
    DESTINATION include/${${LIB_NAME}_INCLUDE_DIR})

  install_target(${LIB_NAME})
  
endmacro(build_target)

macro(install_target _target)
  install(TARGETS ${_target}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
endmacro(install_target)

function(build_test)
  cmake_parse_arguments(TEST  "" "GROUP;NAME" "LINKS;DEPENDS" ${ARGN} )
  add_executable(test_${TEST_NAME} ${TEST_NAME}_test.cc)
  add_dependencies(test_${TEST_NAME} ${TEST_DEPENDS})
  target_link_libraries(test_${TEST_NAME} ${TEST_DEPENDS} ${TEST_LINKS} ${GTEST_LIBRARY})
  add_test(${TEST_GROUP}_${TEST_NAME} ${CMAKE_BINARY_DIR}/bin/test_${TEST_NAME})
endfunction(build_test)

function(build_example)
  cmake_parse_arguments(EXAMPLE  "" "NAME" "LINKS;DEPENDS" ${ARGN} )
  add_executable(example_${EXAMPLE_NAME} ${EXAMPLE_NAME}.cc)
  add_dependencies(example_${EXAMPLE_NAME} ${EXAMPLE_DEPENDS})
  target_link_libraries(example_${EXAMPLE_NAME} ${EXAMPLE_LINKS} ${EXAMPLE_DEPENDS})
endfunction(build_example)
