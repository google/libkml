file(GLOB SRCS "*.cc" )
build_target(kmlregionator "${SRCS}" kmlbase;kmldom;kmlengine;kmlconvenience)
install_target(kmlregionator)
