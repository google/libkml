ExternalProject_Add(URIPARSER
  PREFIX URIPARSER
  URL "http://sourceforge.net/projects/uriparser/files/Sources/0.8.2/uriparser-0.8.2.tar.bz2/download"
  URL_MD5 c5cf6b3941d887deb7defc2a86c40f1d
  BINARY_DIR ${CMAKE_BINARY_DIR}/URIPARSER/build
  DOWNLOAD_DIR ${DOWNLOAD_LOCATION}
  PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/cmake/UriParser_cmake_lists_txt ${CMAKE_BINARY_DIR}/URIPARSER/src/URIPARSER/CMakeLists.txt
  CMAKE_CACHE_ARGS
  -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR}
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS} )

include_project_vars(URIPARSER "liburiparser")
