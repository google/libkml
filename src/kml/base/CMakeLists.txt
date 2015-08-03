
if(HAVE_SYSTEM_EXPAT)
  include_directories(${EXPAT_INCLUDE_DIR})
endif()

list(APPEND KMLBASE_LINK_LIBS ${EXPAT_LIBRARY})

file(GLOB SRCS "*.cc"
  contrib/minizip/unzip.c
  contrib/minizip/iomem_simple.c)

if(WIN32)
  list(REMOVE_ITEM SRCS "${CMAKE_CURRENT_SOURCE_DIR}/file_posix.cc")
else()
  list(REMOVE_ITEM SRCS "${CMAKE_CURRENT_SOURCE_DIR}/file_win32.cc")
endif()

#supress W-long-long
if(NOT WIN32)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/zip_file.cc COMPILE_FLAGS "-Wno-long-long")
endif()

if(WIN32)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/file_win32.cc COMPILE_FLAGS "/DUNICODE /D_UNICODE")
endif()

file(GLOB INCS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

build_target(
  NAME kmlbase
  SRCS ${SRCS}
  INCS ${INCS}
  DEPENDS ${KMLBASE_DEPENDS}
  LINKS ${KMLBASE_LINK_LIBS} )
