add_subdirectory(contrib)

if(HAVE_SYSTEM_URIPARSER)
  include_directories(${URIPARSER_INCLUDE_DIR})
endif()

if(HAVE_SYSTEM_EXPAT)
  include_directories(${EXPAT_INCLUDE_DIR})
endif()

file(GLOB SRCS "*.cc" )
file(GLOB INCS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
build_target(
  NAME kmlbase
  SRCS ${SRCS}
  INCS ${INCS}  
  DEPENDS kml_contrib
  LINKS ${EXPAT_LIBRARY} ${URIPARSER_LIBRARY})

