
if(HAVE_SYSTEM_EXPAT)
  include_directories(${EXPAT_INCLUDE_DIR})
endif()

list(APPEND KMLBASE_LINK_LIBS ${EXPAT_LIBRARY})

file(GLOB SRCS "*.cc"
  contrib/minizip/unzip.c
  contrib/minizip/iomem_simple.c)

list(REMOVE_ITEM SRCS "${CMAKE_CURRENT_SOURCE_DIR}/file_posix.cc")

file(GLOB INCS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

build_target(
  NAME kmlbase
  SRCS ${SRCS}
  INCS ${INCS}
  DEPENDS ${KMLBASE_DEPENDS}
  LINKS ${KMLBASE_LINK_LIBS} )
