file(GLOB SRCS "*.cc" )
file(GLOB INCS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

build_target(
  NAME kmlconvenience
  SRCS ${SRCS}
  INCS ${INCS}
  DEPENDS kmlengine;kmldom;kmlbase)
