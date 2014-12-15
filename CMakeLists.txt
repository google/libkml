cmake_minimum_required(VERSION 2.8)
project(LibKML)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "3")
set(VERSION_PATCH "0")
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

option(BUILD_TESTING  "Build testing." TRUE)
option(BUILD_EXAMPLES  "Build examples." TRUE)
option(BUILD_SHARED_LIBS  "Build shared libs." TRUE)

option(BUILD_SWIG        "Build swig bindings"   FALSE)
option(BUILD_SWIG_PYTHON "Build python bindings" FALSE)
option(BUILD_SWIG_JAVA   "Build java bindings"   FALSE)

set(LIBKML_DATA_DIR  ${CMAKE_SOURCE_DIR}/testdata CACHE "Directory containing test data" PATH)

#AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -pedantic -fno-rtti
#AM_TEST_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -fno-rtti -DGTEST_HAS_RTTI=0
if(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -Wno-unused-parameter -Werror -pedantic -fno-rtti")
set(TEST_FLAGS "-Wall -Wextra -Wno-unused-parameter -Werror -fno-rtti -DGTEST_HAS_RTTI=0")
endif()


set(HAVE_SYSTEM_ZLIB CACHE INTERNAL FALSE)
set(HAVE_SYSTEM_MINIZIP CACHE INTERNAL FALSE)
set(HAVE_SYSTEM_EXPAT CACHE INTERNAL FALSE)
set(HAVE_SYSTEM_BOOST CACHE INTERNAL FALSE)
set(HAVE_SYSTEM_URIPARSER CACHE INTERNAL FALSE)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH})

include(ImportZLIB)
include(ImportMiniZip)
include(ImportEXPAT)
include(ImportBoost)
include(ImportUriParser)

include(LibKMLHelper)

if(HAVE_SYSTEM_BOOST)
  message(STATUS "Found Boost: ${Boost_VERSION}")
  include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories(${CMAKE_SOURCE_DIR}/src/)
add_subdirectory(src)

if(BUILD_TESTING)
  include(ImportGoogleTest)
  include_directories(${GTEST_INCLUDE_DIR})
  include_directories(${CMAKE_SOURCE_DIR}/tests)
  add_definitions("-DDATADIR=\"${LIBKML_DATA_DIR}\"")
  add_subdirectory(tests)
endif()