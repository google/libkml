INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/third_party/zlib-1.2.3/contrib

lib_LTLIBRARIES = libkmlutil.la
libkmlutil_la_SOURCES = fileio.cc kmz.cc
libkmlutil_la_LIBADD = $(top_builddir)/third_party/libminizip.la

# These header files will be installed in $(includedir)/kml/util
libkmlutilincludedir = $(includedir)/kml/util
libkmlutilinclude_HEADERS = \
	fileio.h \
	kmz.h \
	unit_test.h \
	util.h

if HAVE_CPPUNIT 
DATA_DIR = $(top_srcdir)/testdata
TESTS = kmz_test
check_PROGRAMS = $(TESTS)
kmz_test_SOURCES = kmz_test.cc
kmz_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
kmz_test_LDFLAGS = $(CPPUNIT_LIBS) -lz
kmz_test_LDADD = libkmlutil.la
CLEANFILES = check_PROGRAMS
endif
