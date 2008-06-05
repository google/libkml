AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1 \
	      -I$(top_srcdir)/third_party/zlib-1.2.3/contrib

if GCC
AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -fno-rtti
endif

noinst_PROGRAMS = kmlwalk

kmlwalk_SOURCES = kmlwalk.cc curlfetch.cc
kmlwalk_LDADD = \
	$(top_builddir)/src/kml/dom/libkmldom.la \
	$(top_builddir)/src/kml/engine/libkmlengine.la \
	/usr/local/lib/libcurl.la
