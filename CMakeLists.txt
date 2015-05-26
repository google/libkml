# Sources of non-templated classes.
cmake_minimum_required(VERSION 2.8)

PROJECT(libkml)

SET(libkml_VERSION_MAJOR "0")
SET(libkml_VERSION_MINOR "4")
SET(libkml_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the library.
SET(libkml_VERSION_STRING "${libkml_VERSION_MAJOR}.${libkml_VERSION_MINOR}.${libkml_VERSION_PATCH}")

# Allow sources in subdirectories to see the include files.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party)

#
IF(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" CACHE INTERNAL "Required on unix systems")
  SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fPIC" CACHE INTERNAL "Required on unix systems")
ENDIF(UNIX)

# Currently there are some issues with building KML as shared library.
SET(BUILD_SHARED_LIBS OFF CACHE BOOL "Build kML as static library." FORCE)

# Boost is required.
OPTION(LIBKML_USE_EXTERNAL_BOOST "Use external Boost" OFF)
IF(NOT LIBKML_USE_EXTERNAL_BOOST)
  set(Boost_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1)
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1)
ELSE(NOT LIBKML_USE_EXTERNAL_BOOST)
  set(BOOST_ROOT "" CACHE PATH "The root path to Boost")
  set(Boost_USE_MULTITHREAD ON)
  if(WIN32)
    set(Boost_USE_STATIC_LIBS ON)
  endif(WIN32)
  add_definitions(-DBOOST_ALL_NO_LIB)
  find_package(Boost REQUIRED)
  set(CMAKE_THREAD_PREFER_PTHREAD 1)
  find_package(Threads)
  set(Boost_LIBRARIES ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
  FIND_PACKAGE(Boost REQUIRED)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF(NOT LIBKML_USE_EXTERNAL_BOOST)

# Expat is required.
SET(LIBKML_USE_EXTERNAL_EXPAT OFF CACHE BOOL "Use external expat")

# 1. the find way:
if(LIBKML_USE_EXTERNAL_EXPAT)
  FIND_PACKAGE(EXPAT)
else(LIBKML_USE_EXTERNAL_EXPAT)
  # 2. the build your own way:
  IF(NOT BUILD_SHARED_LIBS)
    ADD_DEFINITIONS(-DXML_STATIC)
  ENDIF()
  IF(UNIX)
    ADD_DEFINITIONS(-DHAVE_MEMMOVE)
  ENDIF(UNIX)
  ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/third_party/expat.src)
  SET(EXPAT_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/expat.src/lib")
  SET(EXPAT_LIBRARIES expat) # CMake target name in the build-your-own case
endif(LIBKML_USE_EXTERNAL_EXPAT)

# Both ways:
INCLUDE_DIRECTORIES(${EXPAT_INCLUDE_DIRS})

# uriparser is required.
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.5/include)

SET(libkmlbase_la_SOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/kml/base)
INCLUDE_DIRECTORIES(${libkmlbase_la_SOURCES_DIR})
SET(libkmlbase_la_SOURCES
  ${libkmlbase_la_SOURCES_DIR}/attributes.cc
  ${libkmlbase_la_SOURCES_DIR}/csv_splitter.cc
  ${libkmlbase_la_SOURCES_DIR}/date_time.cc
  ${libkmlbase_la_SOURCES_DIR}/expat_parser.cc
  ${libkmlbase_la_SOURCES_DIR}/expat_handler_ns.cc
  ${libkmlbase_la_SOURCES_DIR}/file.cc
  ${libkmlbase_la_SOURCES_DIR}/math_util.cc
  ${libkmlbase_la_SOURCES_DIR}/mimetypes.cc
  ${libkmlbase_la_SOURCES_DIR}/referent.cc
  ${libkmlbase_la_SOURCES_DIR}/string_util.cc
  ${libkmlbase_la_SOURCES_DIR}/time_util.cc
  ${libkmlbase_la_SOURCES_DIR}/uri_parser.cc
  ${libkmlbase_la_SOURCES_DIR}/version.cc
  ${libkmlbase_la_SOURCES_DIR}/xml_namespaces.cc
  ${libkmlbase_la_SOURCES_DIR}/zip_file.cc
)

IF(UNIX)
	SET(libkmlbase_la_SOURCES ${libkmlbase_la_SOURCES} ${libkmlbase_la_SOURCES_DIR}/file_posix.cc )
ELSE(UNIX)
	SET(libkmlbase_la_SOURCES ${libkmlbase_la_SOURCES}
			${libkmlbase_la_SOURCES_DIR}/file_win32.cc
			${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h )
ENDIF(UNIX)

SET(libkmlconvenience_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/convenience")
INCLUDE_DIRECTORIES(${libkmlconvenience_la_SOURCES_DIR})
SET(libkmlconvenience_la_SOURCES
	${libkmlconvenience_la_SOURCES_DIR}/atom_util.cc
	${libkmlconvenience_la_SOURCES_DIR}/csv_file.cc
	${libkmlconvenience_la_SOURCES_DIR}/csv_parser.cc
	${libkmlconvenience_la_SOURCES_DIR}/convenience.cc
	${libkmlconvenience_la_SOURCES_DIR}/feature_list.cc
	${libkmlconvenience_la_SOURCES_DIR}/google_doc_list.cc
	${libkmlconvenience_la_SOURCES_DIR}/google_maps_data.cc
	${libkmlconvenience_la_SOURCES_DIR}/google_picasa_web.cc
	${libkmlconvenience_la_SOURCES_DIR}/google_spreadsheets.cc
	${libkmlconvenience_la_SOURCES_DIR}/http_client.cc
	${libkmlconvenience_la_SOURCES_DIR}/kmz_check_links.cc
)

SET(libkmldom_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/dom")
INCLUDE_DIRECTORIES(${libkmldom_la_SOURCES_DIR})
SET(libkmldom_la_SOURCES
	${libkmldom_la_SOURCES_DIR}/abstractlatlonbox.cc
	${libkmldom_la_SOURCES_DIR}/abstractview.cc
	${libkmldom_la_SOURCES_DIR}/atom.cc
	${libkmldom_la_SOURCES_DIR}/balloonstyle.cc
	${libkmldom_la_SOURCES_DIR}/colorstyle.cc
	${libkmldom_la_SOURCES_DIR}/container.cc
	${libkmldom_la_SOURCES_DIR}/document.cc
	${libkmldom_la_SOURCES_DIR}/element.cc
	${libkmldom_la_SOURCES_DIR}/extendeddata.cc
	${libkmldom_la_SOURCES_DIR}/feature.cc
	${libkmldom_la_SOURCES_DIR}/folder.cc
	${libkmldom_la_SOURCES_DIR}/geometry.cc
	${libkmldom_la_SOURCES_DIR}/gx_timeprimitive.cc
	${libkmldom_la_SOURCES_DIR}/gx_tour.cc
	${libkmldom_la_SOURCES_DIR}/hotspot.cc
	${libkmldom_la_SOURCES_DIR}/iconstyle.cc
	${libkmldom_la_SOURCES_DIR}/kml_cast.cc
	${libkmldom_la_SOURCES_DIR}/kml_factory.cc
	${libkmldom_la_SOURCES_DIR}/kml.cc
	${libkmldom_la_SOURCES_DIR}/kml_handler.cc
	${libkmldom_la_SOURCES_DIR}/kml_handler_ns.cc
	${libkmldom_la_SOURCES_DIR}/link.cc
	${libkmldom_la_SOURCES_DIR}/labelstyle.cc
	${libkmldom_la_SOURCES_DIR}/linestyle.cc
	${libkmldom_la_SOURCES_DIR}/liststyle.cc
	${libkmldom_la_SOURCES_DIR}/model.cc
	${libkmldom_la_SOURCES_DIR}/networklink.cc
	${libkmldom_la_SOURCES_DIR}/networklinkcontrol.cc
	${libkmldom_la_SOURCES_DIR}/object.cc
	${libkmldom_la_SOURCES_DIR}/overlay.cc
	${libkmldom_la_SOURCES_DIR}/parser.cc
	${libkmldom_la_SOURCES_DIR}/placemark.cc
	${libkmldom_la_SOURCES_DIR}/polystyle.cc
	${libkmldom_la_SOURCES_DIR}/region.cc
	${libkmldom_la_SOURCES_DIR}/schema.cc
	${libkmldom_la_SOURCES_DIR}/serializer.cc
	${libkmldom_la_SOURCES_DIR}/snippet.cc
	${libkmldom_la_SOURCES_DIR}/style.cc
	${libkmldom_la_SOURCES_DIR}/stylemap.cc
	${libkmldom_la_SOURCES_DIR}/styleselector.cc
	${libkmldom_la_SOURCES_DIR}/substyle.cc
	${libkmldom_la_SOURCES_DIR}/timeprimitive.cc
	${libkmldom_la_SOURCES_DIR}/vec2.cc
	${libkmldom_la_SOURCES_DIR}/visitor.cc
	${libkmldom_la_SOURCES_DIR}/visitor_driver.cc
	${libkmldom_la_SOURCES_DIR}/xal.cc
	${libkmldom_la_SOURCES_DIR}/xml_serializer.cc
	${libkmldom_la_SOURCES_DIR}/xsd.cc
)

SET(libkmlengine_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/engine")
INCLUDE_DIRECTORIES(${libkmlengine_la_SOURCES_DIR})
SET(libkmlengine_la_SOURCES
	${libkmlengine_la_SOURCES_DIR}/clone.cc
	${libkmlengine_la_SOURCES_DIR}/entity_mapper.cc
	${libkmlengine_la_SOURCES_DIR}/feature_balloon.cc
	${libkmlengine_la_SOURCES_DIR}/feature_view.cc
	${libkmlengine_la_SOURCES_DIR}/feature_visitor.cc
	${libkmlengine_la_SOURCES_DIR}/find.cc
	${libkmlengine_la_SOURCES_DIR}/find_xml_namespaces.cc
	${libkmlengine_la_SOURCES_DIR}/get_links.cc
	${libkmlengine_la_SOURCES_DIR}/get_link_parents.cc
	${libkmlengine_la_SOURCES_DIR}/href.cc
	${libkmlengine_la_SOURCES_DIR}/id_mapper.cc
	${libkmlengine_la_SOURCES_DIR}/kml_cache.cc
	${libkmlengine_la_SOURCES_DIR}/kml_file.cc
	${libkmlengine_la_SOURCES_DIR}/kml_stream.cc
	${libkmlengine_la_SOURCES_DIR}/kml_uri.cc
	${libkmlengine_la_SOURCES_DIR}/kmz_cache.cc
	${libkmlengine_la_SOURCES_DIR}/kmz_file.cc
	${libkmlengine_la_SOURCES_DIR}/link_util.cc
	${libkmlengine_la_SOURCES_DIR}/location_util.cc
	${libkmlengine_la_SOURCES_DIR}/merge.cc
	${libkmlengine_la_SOURCES_DIR}/parse_old_schema.cc
	${libkmlengine_la_SOURCES_DIR}/style_inliner.cc
	${libkmlengine_la_SOURCES_DIR}/style_merger.cc
	${libkmlengine_la_SOURCES_DIR}/style_resolver.cc
	${libkmlengine_la_SOURCES_DIR}/style_splitter.cc
	${libkmlengine_la_SOURCES_DIR}/update.cc
	${libkmlengine_la_SOURCES_DIR}/update_processor.cc
)

SET(libkmlregionator_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/regionator")
INCLUDE_DIRECTORIES(${libkmlregionator_la_SOURCES_DIR})
SET(libkmlregionator_la_SOURCES
	${libkmlregionator_la_SOURCES_DIR}/feature_list_region_handler.cc
	${libkmlregionator_la_SOURCES_DIR}/regionator.cc
	${libkmlregionator_la_SOURCES_DIR}/regionator_util.cc
)

IF(BUILD_SHARED_LIBS)
  ADD_DEFINITIONS(-DZLIB_DLL)
  ADD_DEFINITIONS(-DZLIB_INTERNAL)
ENDIF()
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib")
SET(libzlib_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib/minizip")
INCLUDE_DIRECTORIES(${libzlib_la_SOURCES_DIR})
SET(libzlib_la_SOURCES
	${libzlib_la_SOURCES_DIR}/unzip.c
	${libzlib_la_SOURCES_DIR}/ioapi.c
	${libzlib_la_SOURCES_DIR}/zip.c
	${libzlib_la_SOURCES_DIR}/iomem_simple.c
)

SET(libzlib_src_la_SOURCES "")

SET(liburiparser_la_SOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.5/lib")
INCLUDE_DIRECTORIES(${liburiparser_la_SOURCES_DIR})
SET( liburiparser_la_SOURCES
	${liburiparser_la_SOURCES_DIR}/UriCommon.c
	${liburiparser_la_SOURCES_DIR}/UriCommon.h
	${liburiparser_la_SOURCES_DIR}/UriCompare.c
	${liburiparser_la_SOURCES_DIR}/UriEscape.c
	${liburiparser_la_SOURCES_DIR}/UriFile.c
	${liburiparser_la_SOURCES_DIR}/UriIp4.c
	${liburiparser_la_SOURCES_DIR}/UriIp4Base.c
	${liburiparser_la_SOURCES_DIR}/UriIp4Base.h
	${liburiparser_la_SOURCES_DIR}/UriNormalize.c
	${liburiparser_la_SOURCES_DIR}/UriNormalizeBase.c
	${liburiparser_la_SOURCES_DIR}/UriNormalizeBase.h
	${liburiparser_la_SOURCES_DIR}/UriParse.c
	${liburiparser_la_SOURCES_DIR}/UriParseBase.c
	${liburiparser_la_SOURCES_DIR}/UriParseBase.h
	${liburiparser_la_SOURCES_DIR}/UriQuery.c
	${liburiparser_la_SOURCES_DIR}/UriRecompose.c
	${liburiparser_la_SOURCES_DIR}/UriResolve.c
	${liburiparser_la_SOURCES_DIR}/UriShorten.c
)

SET(libkml_SOURCES
        ${libkmlbase_la_SOURCES}
        ${libkmlconvenience_la_SOURCES}
        ${libkmldom_la_SOURCES}
        ${libkmlengine_la_SOURCES}
        ${libkmlregionator_la_SOURCES}
        ${libzlib_la_SOURCES}
        ${liburiparser_la_SOURCES}
        ${libzlib_src_la_SOURCES}
        )
# Adjust the compiler flags to avoid problems with ossim code.
IF(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  FOREACH(f ${libkml_SOURCES} )
#        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -Wall -Werror -ansi -pedantic -fno-rtti )
        SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS -w )
  ENDFOREACH(f)
ELSEIF(MSVC) 
	FOREACH(f ${libkml_SOURCES} )
		SET_SOURCE_FILES_PROPERTIES( ${f} PROPERTIES COMPILE_FLAGS /W0 )
	ENDFOREACH(f)      
ENDIF(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
IF(WIN32)
        IF(NOT CYGWIN)
                ADD_DEFINITIONS(-DWIN32 -DUNICODE -D_UNICODE)
        ENDIF(NOT CYGWIN)
ENDIF(WIN32)

ADD_LIBRARY(kml ${libkml_SOURCES} )
LIST(APPEND KML_TARGETS kml)

IF(NOT LIBKML_USE_EXTERNAL_EXPAT)
  TARGET_LINK_LIBRARIES(kml expat)
  LIST(APPEND KML_TARGETS expat)
ELSE (NOT LIBKML_USE_EXTERNAL_EXPAT)
  TARGET_LINK_LIBRARIES(kml ${EXPAT_LIBRARIES})
ENDIF(NOT LIBKML_USE_EXTERNAL_EXPAT)

IF(LIBKML_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(kml PROPERTIES ${LIBKML_LIBRARY_PROPERTIES})
ENDIF(LIBKML_LIBRARY_PROPERTIES)

IF(NOT LIBKML_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS kml
    RUNTIME DESTINATION bin COMPONENT RuntimeLibraries
    LIBRARY DESTINATION lib COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION lib COMPONENT Development)
ENDIF(NOT LIBKML_INSTALL_NO_LIBRARIES)

FILE(GLOB __files0 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/*.h")
INSTALL(FILES ${__files0} DESTINATION include/kml COMPONENT Development)

FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/dom/*.h")
INSTALL(FILES ${__files1} DESTINATION include/kml/dom COMPONENT Development)

FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/base/*.h")
INSTALL(FILES ${__files2} DESTINATION include/kml/base COMPONENT Development)

FILE(GLOB __files3 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/convenience/*.h")
INSTALL(FILES ${__files3} DESTINATION include/kml/convenience COMPONENT Development)

FILE(GLOB __files4 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/engine/*.h")
INSTALL(FILES ${__files4}
    DESTINATION include/kml/engine COMPONENT Development)

FILE(GLOB __files5 "${CMAKE_CURRENT_SOURCE_DIR}/src/kml/regionator/*.h")
INSTALL(FILES ${__files5} DESTINATION include/kml/regionator COMPONENT Development)

FILE(GLOB __files6 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/uriparser-0.7.5/include/uriparser/*.h")
INSTALL(FILES ${__files6} DESTINATION include/kml/third_party/uriparser-0.7.5/include/uriparser
        COMPONENT Development)

IF(NOT LIBKML_USE_EXTERNAL_BOOST)
FILE(GLOB __files70 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/*.hpp")
INSTALL(FILES ${__files70}
    DESTINATION include/kml/third_party/boost_1_34_1/boost
    COMPONENT Development)
FILE(GLOB __files7 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/detail/*.hpp")
INSTALL(FILES ${__files7}
    DESTINATION include/kml/third_party/boost_1_34_1/boost/detail
    COMPONENT Development)
FILE(GLOB __files8 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/*.hpp")
INSTALL(FILES ${__files8}
    DESTINATION include/kml/third_party/boost_1_34_1/boost/config
    COMPONENT Development)
FILE(GLOB __files9 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/no_tr1/*.hpp")
INSTALL(FILES ${__files9}
    DESTINATION include/kml/third_party/boost_1_34_1/boost/config/no_tr1
    COMPONENT Development)
FILE(GLOB __files10 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/stdlib/*.hpp")
INSTALL(FILES ${__files10}
    DESTINATION include/kml/third_party/boost_1_34_1/boost/config/stdlib
   COMPONENT Development)

FILE(GLOB __files11 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/platform/*.hpp")
INSTALL(FILES ${__files11}
    DESTINATION include/kml/third_party/boost_1_34_1/boost/config/platform
    COMPONENT Development)
ENDIF(NOT LIBKML_USE_EXTERNAL_BOOST)

FILE(GLOB __files12 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/*.h")
INSTALL(FILES ${__files12}
    DESTINATION include/kml/third_party/zlib-1.2.3
    COMPONENT Development)
FILE(GLOB __files13 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib/*.h")
INSTALL(FILES ${__files13}
    DESTINATION include/kml/third_party/zlib-1.2.3/contrib
    COMPONENT Development)
FILE(GLOB __files14 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/zlib-1.2.3/contrib/minizip/*.h")
INSTALL(FILES ${__files14}
    DESTINATION include/kml/third_party/zlib-1.2.3/contrib/minizip
    COMPONENT Development)

IF(NOT LIBKML_USE_EXTERNAL_BOOST)
FILE(GLOB __files15 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/compiler/*.hpp")
INSTALL(FILES ${__files15}
    DESTINATION include/kml/third_party/boost_1_34_1/boost/config/compiler
    COMPONENT Development)
FILE(GLOB __files16 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost_1_34_1/boost/config/abi/*.hpp")
INSTALL(FILES ${__files16}
    DESTINATION include/kml/third_party/boost_1_34_1/boost/config/abi
    COMPONENT Development)
ENDIF(NOT LIBKML_USE_EXTERNAL_BOOST)

IF(UNIX)
ELSE(UNIX)
  FILE(GLOB __files17 "${CMAKE_CURRENT_SOURCE_DIR}/third_party/expat.src/lib/*.h")
  INSTALL(FILES ${__files17}
      DESTINATION include/kml/third_party/expat.src
      COMPONENT Development)
ENDIF(UNIX)

# Setup the export configuration.
export(TARGETS ${KML_TARGETS} FILE ${libkml_BINARY_DIR}/KMLTargets.cmake)
set(libkml_EXPORT_FILE "${libkml_BINARY_DIR}/KMLConfig.cmake" )
configure_file("${libkml_SOURCE_DIR}/KMLConfig.cmake.in"
               "${libkml_EXPORT_FILE}"
               @ONLY)

INSTALL(FILES ${libkml_BINARY_DIR}/KMLConfig.cmake DESTINATION lib/cmake)
INSTALL(FILES ${libkml_BINARY_DIR}/KMLTargets.cmake DESTINATION lib/cmake)
