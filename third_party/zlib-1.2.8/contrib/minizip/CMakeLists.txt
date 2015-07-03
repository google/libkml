
# libminizip_la_LDFLAGS = $(AM_LDFLAGS) -version-info 1:0:0 -lz

# minizip_includedir = $(includedir)/minizip


set(SRCS
  ioapi.c
  mztools.c
  unzip.c
  zip.c )

set(INCS
  crypt.h
  ioapi.h
  mztools.h
  unzip.h
  zip.h )

if(WIN32)
  list(APPEND SRCS iowin32.c)
  list(APPEND INCS iowin32.h)
endif()

add_library(minizip ${SRCS})

install(FILES ${INCS}
   DESTINATION include/minizip)

