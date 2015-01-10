macro(build_target _target _srcs _dep_libs)
add_library(${_target} ${_srcs})
foreach(dep_lib ${_dep_libs})
add_dependencies(${_target} ${dep_lib})
endforeach()
set(_link_libs ${ARGN})
target_link_libraries(${_target} ${_dep_libs} ${_link_libs})
if(VERSION_STRING)
  set_target_properties(${_target} PROPERTIES
    VERSION   "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
    SOVERSION "${VERSION_MAJOR}.${VERSION_MINOR}")
endif()

endmacro()

macro(install_target _target)
  install(TARGETS ${_target}
    RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
    LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION lib COMPONENT Development)

endmacro()

function(build_test _prefix _target _libs)
 set(_src ${_target}_test.cc)
set(_extra_libs ${ARGN})
 add_executable(test_${_target} ${_src})
 add_dependencies(test_${_target} ${_libs} ${_extra_libs})
 target_link_libraries(test_${_target} ${_libs} ${_extra_libs} ${GTEST_LIBRARY})
 add_test(${prefix}_${_target} ${_target})
endfunction()

function(build_example _target _libs)
 set(_src ${_target}.cc)
set(_extra_libs ${ARGN})
 add_executable(example_${_target} ${_src})
 add_dependencies(example_${_target} ${_libs} ${_extra_libs})
 target_link_libraries(example_${_target} ${_libs} ${_extra_libs})
endfunction()