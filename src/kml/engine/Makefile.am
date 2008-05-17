AM_CPPFLAGS= -I$(top_srcdir)/src \
	     -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

lib_LTLIBRARIES = libkmlengine.la
libkmlengine_la_SOURCES = \
	kml_file.cc

libkmlengineincludedir = $(includedir)/kml/engine
libkmlengineinclude_HEADERS = \
	kml_file.h \
	object_id_parser_observer.h \
	shared_style_parser_observer.h

if HAVE_CPPUNIT 
TESTS = kml_file_test \
	object_id_parser_observer_test \
	shared_style_parser_observer_test

check_PROGRAMS = $(TESTS)

# Unit tests for KML engine classes.
kml_file_test_SOURCES = kml_file_test.cc
kml_file_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
kml_file_test_LDFLAGS = $(CPPUNIT_LIBS)
kml_file_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la

object_id_parser_observer_test_SOURCES = object_id_parser_observer_test.cc
object_id_parser_observer_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
object_id_parser_observer_test_LDFLAGS = $(CPPUNIT_LIBS)
object_id_parser_observer_test_LDADD= libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la

shared_style_parser_observer_test_SOURCES = shared_style_parser_observer_test.cc
shared_style_parser_observer_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
shared_style_parser_observer_test_LDFLAGS = $(CPPUNIT_LIBS)
shared_style_parser_observer_test_LDADD= \
	$(top_builddir)/src/kml/dom/libkmldom.la

CLEANFILES = check_PROGRAMS
endif
