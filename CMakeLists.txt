cmake_minimum_required(VERSION 2.8.5)
project(LibKML)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "3")
set(VERSION_PATCH "0")
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

option(BUILD_TESTING  "Build testing." OFF)
option(BUILD_EXAMPLES  "Build examples." OFF)
option(INSTALL_EXAMPLES   "Install examples sources and executables" OFF)
option(BUILD_SHARED_LIBS  "Build shared libs." ON)

option(BUILD_SWIG        "Build swig bindings"   OFF)
option(BUILD_SWIG_PYTHON "Build python bindings" OFF)
option(BUILD_SWIG_JAVA   "Build java bindings"   OFF)

set(LIBKML_DATA_DIR  ${CMAKE_SOURCE_DIR}/testdata CACHE "Directory containing test data" PATH)

#AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -pedantic -fno-rtti
#AM_TEST_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -fno-rtti -DGTEST_HAS_RTTI=0
if(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Wextra -Wno-unused-parameter -pedantic -fno-rtti")
set(TEST_FLAGS "-Wall -Wextra -Wno-unused-parameter -fno-rtti -DGTEST_HAS_RTTI=0")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(HAVE_SYSTEM_ZLIB CACHE INTERNAL FALSE)
set(HAVE_SYSTEM_MINIZIP CACHE INTERNAL FALSE)
set(HAVE_SYSTEM_EXPAT CACHE INTERNAL FALSE)
set(HAVE_SYSTEM_BOOST CACHE INTERNAL FALSE)
set(HAVE_SYSTEM_URIPARSER CACHE INTERNAL FALSE)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH})

include(LibKMLHelper)

set(KMLBASE_DEPENDS)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()  

include(ImportEXPAT)
include(ImportBoost)
include(ExternalProject)

include(ImportUriParser)
if(NOT HAVE_SYSTEM_URIPARSER)
  set(URIPARSER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.8.2/include)
  include_directories(${URIPARSER_INCLUDE_DIR})
  set(URIPARSER_LIBRARY uriparser)
  add_subdirectory(third_party/uriparser-0.8.2)
  list(APPEND KMLBASE_DEPENDS uriparser )
endif()

if(NOT INSTALL_DIR)
  set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/../install")
endif()

set(KMLBASE_LINK_LIBS)
set(KMLBASE_DEPENDS)

include(ImportZLIB)
if(NOT HAVE_SYSTEM_ZLIB)
  include(External_zlib)
  list(APPEND KMLBASE_DEPENDS ZLIB)
  list(APPEND KMLBASE_LINK_LIBS ${ZLIB_LIBRARY})
endif()

include(ImportMinizip)
if(NOT HAVE_SYSTEM_MINIZIP)
  include(External_minizip)
  list(APPEND KMLBASE_DEPENDS MINIZIP)
  list(APPEND KMLBASE_LINK_LIBS ${MINIZIP_LIBRARY})  
endif()

if(HAVE_SYSTEM_BOOST)
  message(STATUS "Found Boost: ${Boost_VERSION}")
  include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)

if(BUILD_TESTING)
  enable_testing()
  include(ImportGoogleTest)
  include_directories(${GTEST_INCLUDE_DIR})
  include_directories(${CMAKE_SOURCE_DIR}/tests)
  add_definitions("-DDATADIR=\"${LIBKML_DATA_DIR}\"")
  add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
