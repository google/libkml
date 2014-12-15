macro(build_target _target _srcs _dep_libs)
add_library(${_target} ${_srcs})
add_dependencies(${_target} ${_dep_libs})
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

function(build_test _target _libs)
 set(_srcs ${_target}.cc)
set(_extra_libs ${ARGN})
 add_executable(${_target} ${_srcs})
 add_dependencies(${_target} ${_libs} ${_extra_libs})
 target_link_libraries(${_target} ${_libs} ${_extra_libs} ${GTEST_LIBRARY})
endfunction()