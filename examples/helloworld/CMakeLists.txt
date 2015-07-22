add_library(helloutil print.cc)
target_link_libraries(helloutil ${MINIZIP_LIBRARY})

build_example(
  NAME createkml
  DEPENDS kmldom kmlbase)

build_example(
  NAME circlegen
  DEPENDS kmlconvenience kmlengine kmldom kmlbase)

build_example(
  NAME checklinks
  DEPENDS kmlengine kmldom kmlbase)

build_example(
  NAME countkml
  DEPENDS kmlengine kmldom kmlbase)

build_example(
  NAME helloattrs
  DEPENDS kmldom kmlbase)

build_example(
  NAME helloenum
  DEPENDS kmldom kmlbase)

build_example(
  NAME hellofeatures
  DEPENDS kmldom kmlbase helloutil)

build_example(
  NAME hellofolder
  DEPENDS kmldom kmlbase)

build_example(
  NAME hellogeometry
  DEPENDS kmldom kmlbase)

build_example(
  NAME hellohref
  DEPENDS kmldom kmlbase helloutil)

build_example(
  NAME hellokmz
  DEPENDS kmldom kmlbase kmlengine)

build_example(
  NAME helloregion
  DEPENDS kmldom kmlbase helloutil)

build_example(
  NAME helloworld
  DEPENDS kmldom kmlbase)

build_example(
  NAME parsekml
  DEPENDS kmldom kmlbase)

build_example(
  NAME parsens
  DEPENDS kmldom kmlbase)

build_example(
  NAME prettykml
  DEPENDS kmldom kmlbase kmlengine)

build_example(
  NAME printgeometry
  DEPENDS kmldom kmlbase kmlengine)

build_example(
  NAME sharedstyles
  DEPENDS kmldom kmlbase kmlengine kmlconvenience)

build_example(
  NAME simplifylines
  DEPENDS kmldom kmlbase kmlengine kmlconvenience)

build_example(
  NAME sortplacemarks
  DEPENDS kmldom kmlbase kmlengine kmlconvenience)


if(INSTALL_EXAMPLES)
file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
install(FILES ${eg_files}  DESTINATION ${KML_EXAMPLES_DIR} COMPONENT Development)
install(FILES print.h
  DESTINATION ${KML_EXAMPLES_DIR}
  COMPONENT Development)
endif()
