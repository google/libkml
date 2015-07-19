 ExternalProject_Add(URIPARSER
   PREFIX URIPARSER
   URL "http://sourceforge.net/projects/uriparser/files/Sources/0.8.2/uriparser-0.8.2.tar.bz2/download"
   URL_MD5 c5cf6b3941d887deb7defc2a86c40f1d
   BINARY_DIR ${CMAKE_BINARY_DIR}/URIPARSER/build
   DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
   PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${PATCH_DIR}/URIPARSER/CMakeLists.txt ${CMAKE_BINARY_DIR}/URIPARSER/src/URIPARSER/
   CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/URIPARSER/src/URIPARSER/configure --disable-test --disable-doc --prefix=${INSTALL_DIR}/URIPARSER)

 include_project_vars(URIPARSER "liburiparser.so")


