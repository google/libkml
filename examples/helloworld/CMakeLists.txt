add_library(helloutil print.cc)
target_link_libraries(helloutil minizip) #use MINIZIP_LIBRARY

build_example(createkml kmldom kmlbase)
build_example(circlegen kmlconvenience kmlengine kmldom kmlbase)
build_example(checklinks kmlengine kmldom kmlbase)
build_example(countkml kmlengine kmldom kmlbase)
build_example(helloattrs kmldom kmlbase)
build_example(helloenum kmldom kmlbase)
build_example(hellofeatures kmldom kmlbase helloutil)
build_example(hellofolder kmldom kmlbase)
build_example(hellogeometry kmldom kmlbase)
build_example(hellohref kmldom kmlbase helloutil)
build_example(hellokmz kmldom kmlbase kmlengine)
build_example(helloregion kmldom kmlbase helloutil)
build_example(helloworld kmldom kmlbase)
build_example(parsekml kmldom kmlbase)
build_example(parsens kmldom kmlbase)
build_example(prettykml kmldom kmlbase kmlengine)
build_example(printgeometry kmldom kmlbase kmlengine)
build_example(sharedstyles kmldom kmlbase kmlengine kmlconvenience)
build_example(simplifylines kmldom kmlbase kmlengine kmlconvenience)
build_example(sortplacemarks kmldom kmlbase kmlengine kmlconvenience)


if(INSTALL_EXAMPLES)
file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
install(FILES ${eg_files}  DESTINATION ${KML_EXAMPLES_DIR} COMPONENT Development)
install(FILES print.h
  DESTINATION ${KML_EXAMPLES_DIR}
  COMPONENT Development)
endif()
