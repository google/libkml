file(GLOB SRCS "*.cc" )
build_target(kmldom "${SRCS}" kmlbase)
install_target(kmldom)
