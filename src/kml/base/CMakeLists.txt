add_subdirectory(contrib)

if(HAVE_SYSTEM_EXPAT)
  include_directories(${EXPAT_INCLUDE_DIR})
endif()

message(STATUS "zlib_inc3=${ZLIB_INCLUDE_DIR}")


file(GLOB SRCS "*.cc" )
file(GLOB INCS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
build_target(
  NAME kmlbase
  SRCS ${SRCS}
  INCS ${INCS}
  DEPENDS ${KMLBASE_DEPENDS}
  LINKS ${EXPAT_LIBRARY} )
