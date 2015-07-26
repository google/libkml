# Find the MiniZip library
# Defines:

#  MINIZIP_INCLUDE_DIR - MiniZip includes
#  MINIZIP_LIBRARY     - MiniZip libraries
#  MINIZIP_FOUND       - TRUE if minizip is found


if(MINIZIP_INCLUDE_DIR)
  #check cache
  set(MINIZIP_FIND_QUIETLY TRUE)
endif()

if(NOT MINIZIP_INCLUDE_DIR)
  find_path(MINIZIP_INCLUDE_DIR NAMES minizip/unzip.h minizip/zip.h)
endif()

find_library(MINIZIP_LIBRARY NAMES minizip)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MINIZIP DEFAULT_MSG MINIZIP_LIBRARY MINIZIP_INCLUDE_DIR )

mark_as_advanced(MINIZIP_INCLUDE_DIR MINIZIP_LIBRARY)

if(MINIZIP_FOUND)
  set(MINIZIP_INCLUDE_DIRS ${MINIZIP_INCLUDE_DIR})
  set(MINIZIP_LIBRARIES    ${MINIZIP_LIBRARY})
else()
  set(MINIZIP_INCLUDE_DIRS)
  set(MINIZIP_LIBRARIES)
endif()
