cmake_minimum_required(VERSION 2.8)

set(SRCS
  src/UriCommon.c
  src/UriCommon.h
  src/UriCompare.c
  src/UriEscape.c
  src/UriFile.c
  src/UriIp4.c
  src/UriIp4Base.c
  src/UriIp4Base.h
  src/UriNormalize.c
  src/UriNormalizeBase.c
  src/UriNormalizeBase.h
  src/UriParse.c
  src/UriParseBase.c
  src/UriParseBase.h
  src/UriQuery.c
  src/UriRecompose.c
  src/UriResolve.c
  src/UriShorten.c)

set(INCS
  include/uriparser/Uri.h
  include/uriparser/UriBase.h
  include/uriparser/UriDefsAnsi.h
  include/uriparser/UriDefsConfig.h
  include/uriparser/UriDefsUnicode.h
  include/uriparser/UriIp4.h)

include_directories(include)
add_library(uriparser ${SRCS})

install(FILES ${INCS}
   DESTINATION include/uriparser/)

