file(GLOB SRCS "*.cc" )
build_target(kmlengine "${SRCS}" kmlbase;kmldom)
install_target(kmlengine)
