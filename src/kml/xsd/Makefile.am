AM_CPPFLAGS = -I$(top_srcdir)/src \
              -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

lib_LTLIBRARIES = libkmlxsd.la
libkmlxsd_la_SOURCES = \
	xsd_element.cc \
	xsd_file.cc \
	xsd_handler.cc \
	xsd_primitive_type.cc \
	xsd_util.cc \
	xst_parser.cc

# These header files will be installed in $(includedir)/kml/xsd
libkmlxsdincludedir = $(includedir)/kml/xsd
libkmlxsdinclude_HEADERS = \
	xsd_complex_type.h \
	xsd_element.h \
	xsd_file.h \
	xsd_handler.h \
	xsd_primitive_type.h \
	xsd_simple_type.h \
	xsd_schema.h \
	xsd_type.h \
	xsd_util.h \
	xst_parser.h

if HAVE_CPPUNIT 
DATA_DIR = $(top_srcdir)/testdata
TESTS = \
	xsd_complex_type_test \
	xsd_file_test \
	xsd_element_test \
	xsd_handler_test \
	xsd_primitive_type_test \
	xsd_simple_type_test \
	xsd_schema_test \
	xsd_util_test \
	xst_parser_test

check_PROGRAMS = $(TESTS)

TODO: move Attributes and expat to kmlbase and remove libkmldom here.

xsd_complex_type_test_SOURCES = xsd_complex_type_test.cc
xsd_complex_type_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
xsd_complex_type_test_LDFLAGS = $(CPPUNIT_LIBS)
xsd_complex_type_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsd_file_test_SOURCES = xsd_file_test.cc
xsd_file_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
xsd_file_test_LDFLAGS = $(CPPUNIT_LIBS)
# TODO: move expat to kmlbase to avoid dependency here on kmldom
xsd_file_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsd_element_test_SOURCES = xsd_element_test.cc
xsd_element_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
xsd_element_test_LDFLAGS = $(CPPUNIT_LIBS)
xsd_element_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsd_handler_test_SOURCES = xsd_handler_test.cc
xsd_handler_test_CXXFLAGS = -DDATADIR=\"$(DATA_DIR)\" $(CPPUNIT_CFLAGS)
xsd_handler_test_LDFLAGS = $(CPPUNIT_LIBS)
# TODO: move expat to kmlbase to avoid dependency here on kmldom
xsd_handler_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsd_primitive_type_test_SOURCES = xsd_primitive_type_test.cc
xsd_primitive_type_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
xsd_primitive_type_test_LDFLAGS = $(CPPUNIT_LIBS)
xsd_primitive_type_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsd_schema_test_SOURCES = xsd_schema_test.cc
xsd_schema_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
xsd_schema_test_LDFLAGS = $(CPPUNIT_LIBS)
xsd_schema_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsd_simple_type_test_SOURCES = xsd_simple_type_test.cc
xsd_simple_type_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
xsd_simple_type_test_LDFLAGS = $(CPPUNIT_LIBS)
xsd_simple_type_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsd_util_test_SOURCES = xsd_util_test.cc
xsd_util_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
xsd_util_test_LDFLAGS = $(CPPUNIT_LIBS)
xsd_util_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xst_parser_test_SOURCES = xst_parser_test.cc
xst_parser_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
xst_parser_test_LDFLAGS = $(CPPUNIT_LIBS)
xst_parser_test_LDADD = libkmlxsd.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

CLEANFILES = check_PROGRAMS
endif
