file(GLOB SRCS "*.cc" )
file(GLOB INCS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

build_target(
  NAME kmlregionator
  SRCS ${SRCS}
  INCS ${INCS}
  DEPENDS kmlbase;kmldom;kmlengine;kmlconvenience)
