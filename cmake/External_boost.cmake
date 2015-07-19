ExternalProject_Add(BOOST
  PREFIX BOOST
  URL http://sourceforge.net/projects/boost/files/boost-binaries/1.50.0/boost_1_50_headers.zip/download
  URL_MD5 1605dc6085cb2dc778ef5ab6c0e59083
  SOURCE_DIR ${INSTALL_DIR}/BOOST/include/
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND "" )


set(Boost_INCLUDE_DIRS ${INSTALL_DIR}/BOOST/include)