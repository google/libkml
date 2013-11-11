AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/examples/xsd \
	      -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -pedantic -Wno-long-long -fno-rtti
endif

noinst_PROGRAMS = xsdchildren xsdcoverage xsdelements xsdenums xsdfind xsdtypes

xsdchildren_SOURCES = xsdchildren.cc
xsdchildren_LDADD = \
	$(top_builddir)/src/kml/xsd/libkmlxsd.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsdcoverage_SOURCES = xsdcoverage.cc
xsdcoverage_LDADD = \
	$(top_builddir)/src/kml/xsd/libkmlxsd.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsdelements_SOURCES = xsdelements.cc
xsdelements_LDADD = \
	$(top_builddir)/src/kml/xsd/libkmlxsd.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsdenums_SOURCES = xsdenums.cc
xsdenums_LDADD = \
	$(top_builddir)/src/kml/xsd/libkmlxsd.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsdfind_SOURCES = xsdfind.cc
xsdfind_LDADD = \
	$(top_builddir)/src/kml/xsd/libkmlxsd.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsdtypes_SOURCES = xsdtypes.cc
xsdtypes_LDADD = \
	$(top_builddir)/src/kml/xsd/libkmlxsd.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

EXTRA_DIST = \
	kml-aliases.txt
