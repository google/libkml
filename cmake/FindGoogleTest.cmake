# Find the Google Test Framework
# Defines:

#  GTEST_INCLUDE_DIR - gtest include directory
#  GTEST_LIBRARY     - gtest library file
#  GTEST_FOUND       - TRUE if gtest is found

if (GTEST_INCLUDE_DIR)
 #check cache 
  set(GTEST_FIND_QUIETLY TRUE)
endif ()


if (NOT GTEST_INCLUDE_DIR)
    find_path(GTEST_INCLUDE_DIR NAMES gtest.h PATH_SUFFIXES gtest)
    set(GTEST_INCLUDE_DIR ${GTEST_INCLUDE_DIR}/gtest CACHE PATH "Google Test includes")
endif ()

find_library(GTESTMAIN_LIB NAMES gtest_main)

find_library(GTEST_LIB NAMES gtest)

if (GTEST_INCLUDE_DIR AND GTEST_LIB AND GTESTMAIN_LIB)
   set(GTEST_FOUND TRUE)
   set(GTEST_LIBRARY ${GTEST_LIB} ${GTESTMAIN_LIB})
endif ()

if (GTEST_FOUND)
   if (NOT GTEST_FIND_QUIETLY)
      message(STATUS "Found Google Test Framework: ${GTEST_LIBRARY}")
   endif ()
else ()
    if (GTEST_FIND_REQUIRED)
      message(FATAL_ERROR "Could NOT find Google Test Framework")
    else ()
      message(STATUS "Could NOT find Google Test Framework")
    endif ()
endif ()


