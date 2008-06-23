AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/zlib-1.2.3/contrib

lib_LTLIBRARIES = libkmlutil.la
libkmlutil_la_SOURCES = file.cc file_posix.cc

# These header files will be installed in $(includedir)/kml/util
libkmlutilincludedir = $(includedir)/kml/util
libkmlutilinclude_HEADERS = \
	file.h \
	tempfile.h \
	unit_test.h \
	util.h

EXTRA_DIST = \
	file_win32.cc

if HAVE_CPPUNIT 
DATA_DIR = $(top_srcdir)/testdata
TESTS = file_test tempfile_test
check_PROGRAMS = $(TESTS)

file_test_SOURCES = file_test.cc
file_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
file_test_LDFLAGS = $(CPPUNIT_LIBS)
file_test_LDADD = libkmlutil.la

tempfile_test_SOURCES = tempfile_test.cc
tempfile_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
tempfile_test_LDFLAGS = $(CPPUNIT_LIBS)
tempfile_test_LDADD = libkmlutil.la

CLEANFILES = check_PROGRAMS
endif
