file(GLOB SRCS "*.cc" )
build_target(kmlconvenience "${SRCS}" kmlengine;kmldom;kmlbase)
install_target(kmlconvenience)
