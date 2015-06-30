build_example(
  NAME balloonwalker
  DEPENDS kmlengine)

build_example(
  NAME clone
  DEPENDS kmlengine)

build_example(
  NAME csv2kml
  DEPENDS kmlconvenience)

build_example(
  NAME csvinfo
  DEPENDS kmlconvenience)

build_example(
  NAME import
  DEPENDS kmlengine)

build_example(
  NAME inlinestyles
  DEPENDS kmlengine)

build_example(
  NAME kmlfile
  DEPENDS kmlengine)

build_example(
  NAME kml2kmz
  DEPENDS kmlengine)

build_example(
  NAME kmzchecklinks
  DEPENDS kmlengine)

build_example(
  NAME oldschema
  DEPENDS kmlengine)

build_example(
  NAME parsebig
  DEPENDS kmlengine)

build_example(
  NAME printstyle
  DEPENDS kmlengine)

build_example(
  NAME change
  DEPENDS kmlconvenience)

build_example(
  NAME splitstyles
  DEPENDS kmlconvenience)

build_example(
  NAME streamkml
  DEPENDS kmlconvenience)

if(INSTALL_EXAMPLES)
  file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
  install(FILES ${eg_files}  DESTINATION examples/engine)
endif(INSTALL_EXAMPLES)
