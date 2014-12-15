file(GLOB SRCS "*.cc" )
build_target(kmlxsd "${SRCS}" kmlbase)
install_target(kmlxsd)
