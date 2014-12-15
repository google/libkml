# Find the UriParser library
# Defines:

#  URIPARSER_INCLUDE_DIR - UriParser include directory
#  URIPARSER_LIBRARY     - UriParser library file
#  URIPARSER_FOUND       - TRUE if UriParser is found

if (URIPARSER_INCLUDE_DIR)
 #check cache 
  set(URIPARSER_FIND_QUIETLY TRUE)
endif ()


if (NOT URIPARSER_INCLUDE_DIR)
    find_path(URIPARSER_INCLUDE_DIR NAMES Uri.h UriBase.h PATH_SUFFIXES uriparser)
    set(URIPARSER_INCLUDE_DIR ${URIPARSER_INCLUDE_DIR}/uriparser CACHE PATH "UriParser includes")
endif ()

find_library(URIPARSER_LIBRARY NAMES uriparser)

if (URIPARSER_INCLUDE_DIR AND URIPARSER_LIBRARY)
   set(URIPARSER_FOUND TRUE)
endif ()

if (URIPARSER_FOUND)
   if (NOT URIPARSER_FIND_QUIETLY)
      message(STATUS "Found UriParser: ${URIPARSER_LIBRARY}")
   endif ()
else ()
    if (URIPARSER_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find UriParser library")
    else ()
      message(STATUS "Could not find UriParser library")
    endif ()
endif ()


