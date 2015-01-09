add_subdirectory(contrib)

if(HAVE_SYSTEM_URIPARSER)
  include_directories(${URIPARSER_INCLUDE_DIR})
endif()

if(HAVE_SYSTEM_EXPAT)
  include_directories(${EXPAT_INCLUDE_DIR})
endif()

file(GLOB SRCS "*.cc" )
build_target(kmlbase "${SRCS}" kml_contrib ${EXPAT_LIBRARY} ${URIPARSER_LIBRARY})

file(GLOB include_files "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(
  FILES ${include_files}
  DESTINATION ${KML_BASE_INCLUDE_DIR}
  COMPONENT Development)

install_target(kmlbase)