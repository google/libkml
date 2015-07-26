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
    find_path(URIPARSER_INCLUDE_DIR NAMES uriparser/Uri.h uriparser/UriBase.h )
endif ()

find_library(URIPARSER_LIBRARY NAMES uriparser)

include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( URIPARSER DEFAULT_MSG URIPARSER_LIBRARY URIPARSER_INCLUDE_DIR )

mark_as_advanced( URIPARSER_INCLUDE_DIR URIPARSER_LIBRARY )

if(URIPARSER_FOUND)
  set(URIPARSER_INCLUDE_DIRS ${URIPARSER_INCLUDE_DIR})
  set(URIPARSER_LIBRARIES    ${URIPARSER_LIBRARY})
else()
  set(URIPARSER_INCLUDE_DIRS)
  set(URIPARSER_LIBRARIES)
endif()
