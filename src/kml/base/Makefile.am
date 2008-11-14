AM_CPPFLAGS = -I$(top_srcdir)/src \
              -I$(top_srcdir)/third_party/boost_1_34_1 \
              -I$(top_srcdir)/third_party/uriparser-0.7.1/include \
              -I$(top_srcdir)/third_party/googletest-r108/include

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
AM_TEST_CXXFLAGS = -Wall -Werror -ansi -fno-rtti -DGTEST_HAS_RTTI=0
endif

libkmlbase_la_LDFLAGS = -lexpat

lib_LTLIBRARIES = libkmlbase.la
libkmlbase_la_SOURCES = \
	attributes.cc \
	color32.h \
	date_time.cc \
	expat_handler_ns.cc \
	expat_parser.cc \
	file.cc \
	file_posix.cc \
	referent.cc \
	string_util.cc \
	time_util.cc

# These header files will be installed in $(includedir)/kml/base
libkmlbaseincludedir = $(includedir)/kml/base
libkmlbaseinclude_HEADERS = \
	attributes.h \
	date_time.h \
	color32.h \
	expat_handler.h \
	expat_handler_ns.h \
	expat_parser.h \
	file.h \
	memory_file.h \
	net_cache.h \
	referent.h \
	string_string_map.h \
	string_util.h \
	tempfile.h \
	time_util.h \
	unit_test.h \
	uri_parser.h \
	util.h \
	vec3.h \
	xmlns.h

EXTRA_DIST = \
	file_win32.cc \
	net_cache_test_util.h

DATA_DIR = $(top_srcdir)/testdata
TESTS = \
	attributes_test \
	color32_test \
	date_time_test \
	expat_handler_ns_test \
	file_test \
	net_cache_test \
	referent_test \
	string_string_map_test \
	string_util_test \
	tempfile_test \
	time_util_test \
	util_test \
	uri_parser_test \
	vec3_test \
	xmlns_test

check_PROGRAMS = $(TESTS)

attributes_test_SOURCES = attributes_test.cc
attributes_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
attributes_test_LDADD = libkmlbase.la \
			$(top_builddir)/third_party/libgtest.la

color32_test_SOURCES = color32_test.cc
color32_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
color32_test_LDADD = libkmlbase.la \
		     $(top_builddir)/third_party/libgtest.la

date_time_test_SOURCES = date_time_test.cc
date_time_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
date_time_test_LDADD = libkmlbase.la \
		       $(top_builddir)/third_party/libgtest.la

expat_handler_ns_test_SOURCES = expat_handler_ns_test.cc
expat_handler_ns_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
expat_handler_ns_test_LDADD = libkmlbase.la \
			      $(top_builddir)/third_party/libgtest.la

file_test_SOURCES = file_test.cc
file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
file_test_LDADD = libkmlbase.la \
		  $(top_builddir)/third_party/libgtest.la

net_cache_test_SOURCES = net_cache_test.cc
net_cache_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
net_cache_test_LDADD = libkmlbase.la \
        $(top_builddir)/third_party/liburiparser.la \
	$(top_builddir)/third_party/libgtest.la

referent_test_SOURCES = referent_test.cc
referent_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
referent_test_LDADD= libkmlbase.la \
		     $(top_builddir)/third_party/libgtest.la

tempfile_test_SOURCES = tempfile_test.cc
tempfile_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
tempfile_test_LDADD = libkmlbase.la \
		      $(top_builddir)/third_party/libgtest.la

string_string_map_test_SOURCES = string_string_map_test.cc
string_string_map_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
string_string_map_test_LDADD= libkmlbase.la \
			$(top_builddir)/third_party/libgtest.la

string_util_test_SOURCES = string_util_test.cc
string_util_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
string_util_test_LDADD= libkmlbase.la \
			$(top_builddir)/third_party/libgtest.la

time_util_test_SOURCES = time_util_test.cc
time_util_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
time_util_test_LDADD= libkmlbase.la \
		      $(top_builddir)/third_party/libgtest.la

util_test_SOURCES = util_test.cc
util_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
util_test_LDADD= libkmlbase.la \
		 $(top_builddir)/third_party/libgtest.la

uri_parser_test_SOURCES = uri_parser_test.cc
uri_parser_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(AM_TEST_CXXFLAGS)
uri_parser_test_LDADD= libkmlbase.la \
        $(top_builddir)/third_party/liburiparser.la \
	$(top_builddir)/third_party/libgtest.la

vec3_test_SOURCES = vec3_test.cc
vec3_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
vec3_test_LDADD= libkmlbase.la \
		 $(top_builddir)/third_party/libgtest.la

xmlns_test_SOURCES = xmlns_test.cc
xmlns_test_CXXFLAGS = $(AM_TEST_CXXFLAGS)
xmlns_test_LDADD= libkmlbase.la \
		  $(top_builddir)/third_party/libgtest.la

CLEANFILES = check_PROGRAMS

