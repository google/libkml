INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/examples/helloworld

noinst_PROGRAMS = prettykml
prettykml_SOURCES = prettykml.cc
prettykml_LDADD = \
	$(top_builddir)/third_party/libminizip.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/util/libkmlutil.la
prettykml_LDFLAGS = -lexpat -lz

