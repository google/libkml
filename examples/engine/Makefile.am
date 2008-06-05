AM_CPPFLAGS = -I$(top_srcdir)/src \
		-I$(top_srcdir)/third_party/boost_1_34_1

noinst_PROGRAMS = clone kmlfile printstyle

clone_SOURCES = clone.cc
clone_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la

kmlfile_SOURCES = kmlfile.cc
kmlfile_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la

printstyle_SOURCES = printstyle.cc
printstyle_LDADD = \
	$(top_builddir)/src/kml/util/libkmlutil.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la
