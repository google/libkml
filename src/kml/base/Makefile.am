AM_CPPFLAGS = -I$(top_srcdir)/src \
              -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

lib_LTLIBRARIES = libkmlbase.la
libkmlbase_la_SOURCES = \
	file.cc \
	file_posix.cc \
	referent.cc \
	string_util.cc \
	time_util.cc

# These header files will be installed in $(includedir)/kml/base
libkmlbaseincludedir = $(includedir)/kml/base
libkmlbaseinclude_HEADERS = \
	file.h \
	referent.h \
	tempfile.h \
	string_util.h \
	time_util.h \
	unit_test.h \
	util.h

EXTRA_DIST = \
	file_win32.cc

if HAVE_CPPUNIT 
DATA_DIR = $(top_srcdir)/testdata
TESTS = file_test referent_test tempfile_test string_util_test time_util_test
check_PROGRAMS = $(TESTS)

file_test_SOURCES = file_test.cc
file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
file_test_LDFLAGS = $(CPPUNIT_LIBS)
file_test_LDADD = libkmlbase.la

referent_test_SOURCES = referent_test.cc
referent_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
referent_test_LDFLAGS = $(CPPUNIT_LIBS)
referent_test_LDADD= libkmlbase.la

tempfile_test_SOURCES = tempfile_test.cc
tempfile_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
tempfile_test_LDFLAGS = $(CPPUNIT_LIBS)
tempfile_test_LDADD = libkmlbase.la \
        $(top_builddir)/src/kml/dom/libkmldom.la

string_util_test_SOURCES = string_util_test.cc
string_util_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
string_util_test_LDFLAGS = $(CPPUNIT_LIBS)
string_util_test_LDADD= libkmlbase.la

time_util_test_SOURCES = time_util_test.cc
time_util_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
time_util_test_LDFLAGS = $(CPPUNIT_LIBS)
time_util_test_LDADD= libkmlbase.la

CLEANFILES = check_PROGRAMS
endif
