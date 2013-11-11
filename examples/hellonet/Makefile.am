if HAVE_CURL
AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/examples/hellonet \
	      -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -pedantic -Wno-long-long -fno-rtti
endif

noinst_PROGRAMS = csvurl2gmap getgmapkml getgsheetcsv getgsheetkml \
		gsheet2gmap kmlwalk kmzfetch listfeed listgdocs listgmaps \
		listgphotos resolvestyle searchgmap uploadgmap uploadgmapcsv \
		uploadgmapkml uploadgsheet xsdprint

noinst_LTLIBRARIES = libcurlfetch.la

libcurlfetch_la_SOURCES = curlfetch.cc
libcurlfetch_la_LIBADD = -lcurl

csvurl2gmap_SOURCES = csvurl2gmap.cc
csvurl2gmap_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

getgmapkml_SOURCES = getgmapkml.cc
getgmapkml_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

getgsheetcsv_SOURCES = getgsheetcsv.cc
getgsheetcsv_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

getgsheetkml_SOURCES = getgsheetkml.cc
getgsheetkml_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

gsheet2gmap_SOURCES = gsheet2gmap.cc
gsheet2gmap_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kmlwalk_SOURCES = kmlwalk.cc
kmlwalk_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kmzfetch_SOURCES = kmzfetch.cc
kmzfetch_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

listfeed_SOURCES = listfeed.cc
listfeed_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

listgdocs_SOURCES = listgdocs.cc
listgdocs_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

listgmaps_SOURCES = listgmaps.cc
listgmaps_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

listgphotos_SOURCES = listgphotos.cc
listgphotos_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

resolvestyle_SOURCES = resolvestyle.cc
resolvestyle_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

searchgmap_SOURCES = searchgmap.cc
searchgmap_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

uploadgmap_SOURCES = uploadgmap.cc
uploadgmap_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

uploadgmapcsv_SOURCES = uploadgmapcsv.cc
uploadgmapcsv_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

uploadgmapkml_SOURCES = uploadgmapkml.cc
uploadgmapkml_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

uploadgsheet_SOURCES = uploadgsheet.cc
uploadgsheet_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

xsdprint_SOURCES = xsdprint.cc
xsdprint_LDADD = \
        libcurlfetch.la \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/xsd/libkmlxsd.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

EXTRA_DIST = README curlfetch.h prompt.h
endif
