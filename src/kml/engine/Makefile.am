AM_CPPFLAGS= -I$(top_srcdir)/src \
	     -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

libkmlengineincludedir = $(includedir)/kml/engine
libkmlengineinclude_HEADERS = \
	object_id_parser_observer.h

if HAVE_CPPUNIT 
TESTS = object_id_parser_observer_test

check_PROGRAMS = $(TESTS)

object_id_parser_observer_test_SOURCES = object_id_parser_observer_test.cc
object_id_parser_observer_test_CXXFLAGS = $(CPPUNIT_CFLAGS) $(AM_CXXFLAGS)
object_id_parser_observer_test_LDFLAGS = $(CPPUNIT_LIBS)
object_id_parser_observer_test_LDADD= $(top_builddir)/src/kml/dom/libkmldom.la

CLEANFILES = check_PROGRAMS
endif
