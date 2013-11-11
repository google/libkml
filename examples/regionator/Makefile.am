AM_CPPFLAGS = -I$(top_srcdir)/src \
              -I$(top_srcdir)/third_party/boost_1_34_1 \
              -I$(top_srcdir)/third_party/zlib-1.2.3/contrib

if GCC
AM_CXXFLAGS = -Wall -Werror -pedantic -Wno-long-long -fno-rtti
endif

noinst_PROGRAMS = csvregionator kmlregionator

csvregionator_SOURCES = csvregionator.cc
csvregionator_LDADD = \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/regionator/libkmlregionator.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la

kmlregionator_SOURCES = kmlregionator.cc
kmlregionator_LDADD = \
	$(top_builddir)/src/kml/base/libkmlbase.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/regionator/libkmlregionator.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la

