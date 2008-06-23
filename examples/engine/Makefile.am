AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1 \
	      -I$(top_srcdir)/third_party/zlib-1.2.3/contrib

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

noinst_PROGRAMS = clone kmlfile kmzchecklinks printstyle

clone_SOURCES = clone.cc
clone_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la

kmlfile_SOURCES = kmlfile.cc
kmlfile_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la

kmzchecklinks_SOURCES = kmzchecklinks.cc
kmzchecklinks_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la

printstyle_SOURCES = printstyle.cc
printstyle_LDADD = \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la
