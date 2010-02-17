AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

noinst_PROGRAMS = \
	balloonwalker change clone csv2kml csvinfo import inlinestyles kmlfile \
	kml2kmz kmzchecklinks oldschema parsebig printstyle splitstyles \
	streamkml

balloonwalker_SOURCES = balloonwalker.cc
balloonwalker_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

clone_SOURCES = clone.cc
clone_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

csv2kml_SOURCES = csv2kml.cc
csv2kml_LDADD = \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

csvinfo_SOURCES = csvinfo.cc
csvinfo_LDADD = \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

import_SOURCES = import.cc
import_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

inlinestyles_SOURCES = inlinestyles.cc
inlinestyles_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kmlfile_SOURCES = kmlfile.cc
kmlfile_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kml2kmz_SOURCES = kml2kmz.cc
kml2kmz_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

kmzchecklinks_SOURCES = kmzchecklinks.cc
kmzchecklinks_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

oldschema_SOURCES = oldschema.cc
oldschema_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

parsebig_SOURCES = parsebig.cc
parsebig_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

printstyle_SOURCES = printstyle.cc
printstyle_LDADD = \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

change_SOURCES = change.cc
change_LDADD = \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

splitstyles_SOURCES = splitstyles.cc
splitstyles_LDADD = \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

streamkml_SOURCES = streamkml.cc
streamkml_LDADD = \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la
