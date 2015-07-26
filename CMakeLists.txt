cmake_minimum_required(VERSION 2.8)
project(LibKML)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "3")
set(VERSION_PATCH "0")
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

option(BUILD_TESTING  "Build testing." OFF)
option(BUILD_EXAMPLES  "Build examples." OFF)
option(INSTALL_EXAMPLES   "Install examples sources and executables" OFF)
option(BUILD_SHARED_LIBS  "Build shared libs." ON)

option(WITH_SWIG   "Build all swig bindings"   ON)
option(WITH_PYTHON "Build python bindings" ON)
option(WITH_JAVA   "Build java bindings"   ON)

set(LIBKML_DATA_DIR  ${CMAKE_SOURCE_DIR}/testdata CACHE "Directory containing test data" PATH)

#AM_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -pedantic -fno-rtti
#AM_TEST_CXXFLAGS = -Wall -Wextra -Wno-unused-parameter -Werror -fno-rtti -DGTEST_HAS_RTTI=0
if(CMAKE_COMPILER_IS_GNUCXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -pedantic -fno-rtti")
set(TEST_FLAGS "-Wall -Wextra -Wno-unused-parameter -fno-rtti -DGTEST_HAS_RTTI=0")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT DEFINED BIN_INSTALL_DIR)
  set(BIN_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/bin")
endif(NOT DEFINED BIN_INSTALL_DIR)
if(NOT DEFINED LIB_INSTALL_DIR)
  set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif(NOT DEFINED LIB_INSTALL_DIR)
if(NOT DEFINED DATA_INSTALL_DIR)
  set(DATA_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share")
endif(NOT DEFINED DATA_INSTALL_DIR)
if(NOT DEFINED INCLUDE_INSTALL_DIR)
  set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include/kml")
endif(NOT DEFINED INCLUDE_INSTALL_DIR)
if(NOT DEFINED MAN_INSTALL_DIR)
  set(MAN_INSTALL_DIR "${DATA_INSTALL_DIR}/man")
endif(NOT DEFINED MAN_INSTALL_DIR)
if(NOT DEFINED RESOURCE_INSTALL_DIR)
  set(RESOURCE_INSTALL_DIR "${DATA_INSTALL_DIR}/libkml${VERSION_MAJOR}/resource/")
endif(NOT DEFINED RESOURCE_INSTALL_DIR)
if(NOT DEFINED LOCALE_INSTALL_DIR)
  set(LOCALE_INSTALL_DIR "${DATA_INSTALL_DIR}/locale/")
endif(NOT DEFINED LOCALE_INSTALL_DIR)
if(NOT DEFINED PYTHON_INSTALL_DIR)
  set(PYTHON_INSTALL_DIR "${LIB_INSTALL_DIR}/python")
endif(NOT DEFINED PYTHON_INSTALL_DIR)
if(NOT DEFINED JAVA_INSTALL_DIR)
  set(JAVA_INSTALL_DIR "${DATA_INSTALL_DIR}/java")
endif(NOT DEFINED JAVA_INSTALL_DIR)

# Path to additional CMake modules
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
    ${CMAKE_MODULE_PATH})

include(LibKMLHelper)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

include(ExternalProject)

if(NOT INSTALL_DIR)
  set(INSTALL_DIR "${CMAKE_INSTALL_PREFIX}" CACHE "install directory " PATH)
endif()

set(KMLBASE_LINK_LIBS "" CACHE INTERNAL "KMLBASE_LINK_LIBS")
set(KMLBASE_DEPENDS "" CACHE INTERNAL "KMLBASE_DEPENDS")
set(MINIZIP_DEPENDS "" CACHE INTERNAL "MINIZIP_DEPENDS")

find_package(EXPAT)
if(EXPAT_FOUND)
  include_directories(${EXPAT_INCLUDE_DIR})
else()
  include(External_expat)
  list(APPEND KMLBASE_DEPENDS EXPAT)
endif()

find_package(ZLIB 1.2.8)
if(ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIR})
else()
  include(External_zlib)
  list(APPEND KMLBASE_DEPENDS ZLIB)
  list(APPEND MINIZIP_DEPENDS ZLIB)
endif()

find_package(MiniZip)
if(MINIZIP_FOUND)
  include_directories(${MINIZIP_INCLUDE_DIR})
else()
  include(External_minizip)
  list(APPEND KMLBASE_DEPENDS MINIZIP)
endif()

find_package(UriParser)
if(URIPARSER_FOUND)
  include_directories(${URIPARSER_INCLUDE_DIR})
else()
  include(External_uriparser)
  list(APPEND KMLBASE_DEPENDS URIPARSER)
endif()

find_package(Boost)
if(Boost_FOUND)
  message(STATUS "Found Boost: ${Boost_VERSION}")
else()
  include(External_boost)
  list(APPEND KMLBASE_DEPENDS BOOST)
  message(STATUS "Found Boost includes: ${Boost_INCLUDE_DIRS}")
endif()

include_directories(${Boost_INCLUDE_DIRS})

list(APPEND KMLBASE_LINK_LIBS ${EXPAT_LIBRARY})
list(APPEND KMLBASE_LINK_LIBS ${ZLIB_LIBRARY})
list(APPEND KMLBASE_LINK_LIBS ${MINIZIP_LIBRARY})
list(APPEND KMLBASE_LINK_LIBS ${URIPARSER_LIBRARY})

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)

if(BUILD_TESTING)
  enable_testing()
  find_package(GoogleTest REQUIRED)
  include_directories(${GTEST_INCLUDE_DIR})
  include_directories(${CMAKE_SOURCE_DIR}/tests)
  add_definitions("-DDATADIR=\"${LIBKML_DATA_DIR}\"")
  add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
