AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

noinst_PROGRAMS = gpxtracktokml

gpxtracktokml_SOURCES = gpxtracktokml.cc
gpxtracktokml_LDADD = \
	$(top_builddir)/src/kml/convenience/libkmlconvenience.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/base/libkmlbase.la

