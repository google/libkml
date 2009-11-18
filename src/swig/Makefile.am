SUBDIRS = java python

AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1 \
	      -I$(top_srcdir)/third_party/uriparser-0.7.5/include

COMMONLIBS = $(top_builddir)/src/kml/engine/libkmlengine.la \
             $(top_builddir)/src/kml/dom/libkmldom.la \
	     $(top_builddir)/src/kml/base/libkmlbase.la

pkglib_LTLIBRARIES=

if WITH_SWIG
if WITH_JAVA
pkglib_LTLIBRARIES += libkmlbase_swig_java.la
pkglib_LTLIBRARIES += libkmldom_swig_java.la
pkglib_LTLIBRARIES += libkmlengine_swig_java.la
endif
if WITH_PYTHON
pkglib_LTLIBRARIES += libkmlbase_swig_python.la
pkglib_LTLIBRARIES += libkmldom_swig_python.la
pkglib_LTLIBRARIES += libkmlengine_swig_python.la
endif
endif

java_package_dir = com/googlecode/libkml

libkmlbase_swig_java_la_SOURCES = kmlbase_java.cc
libkmlbase_swig_java_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(JAVA_INCLUDE_DIR) -I$(JAVA_INCLUDE_DIR)/$(PLATFORM)
libkmlbase_swig_java_la_LIBADD = $(COMMONLIBS)
kmlbase_java.cc: kmlbase.i
	mkdir -p $(java_package_dir)
	$(SWIG) -c++ $(AM_CPPFLAGS) -java -o kmlbase_java.cc \
	-package com.googlecode.libkml \
	-outdir $(java_package_dir) $(top_srcdir)/src/swig/kmlbase.i

libkmlbase_swig_python_la_SOURCES = kmlbase_python.cc kmlbase.py
libkmlbase_swig_python_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(PYTHON_INCLUDE_DIR)
libkmlbase_swig_python_la_LIBADD = $(COMMONLIBS)
libkmlbase_swig_python_la_LDFLAGS = -dynamiclib -python -lexpat
kmlbase_python.cc: kmlbase.i
	$(SWIG) -c++ $(AM_CPPFLAGS) -python -o kmlbase_python.cc \
	$(top_srcdir)/src/swig/kmlbase.i

libkmldom_swig_java_la_SOURCES = kmldom_java.cc
libkmldom_swig_java_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(JAVA_INCLUDE_DIR) -I$(JAVA_INCLUDE_DIR)/$(PLATFORM)
libkmldom_swig_java_la_LIBADD = $(COMMONLIBS)
kmldom_java.cc: kmldom.i
	mkdir -p $(java_package_dir)
	$(SWIG) -c++ $(AM_CPPFLAGS) -java -o kmldom_java.cc \
	-package com.googlecode.libkml \
	-outdir $(java_package_dir) $(top_srcdir)/src/swig/kmldom.i

libkmldom_swig_python_la_SOURCES = kmldom_python.cc kmldom.py
libkmldom_swig_python_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(PYTHON_INCLUDE_DIR)
libkmldom_swig_python_la_LIBADD = $(COMMONLIBS)
libkmldom_swig_python_la_LDFLAGS = -dynamiclib -python -lexpat
kmldom_python.cc: kmldom.i
	$(SWIG) -c++ $(AM_CPPFLAGS) -python -o kmldom_python.cc \
	$(top_srcdir)/src/swig/kmldom.i

libkmlengine_swig_java_la_SOURCES = kmlengine_java.cc
libkmlengine_swig_java_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(JAVA_INCLUDE_DIR) -I$(JAVA_INCLUDE_DIR)/$(PLATFORM)
libkmlengine_swig_java_la_LIBADD = $(COMMONLIBS)
kmlengine_java.cc: kmlengine.i
	mkdir -p $(java_package_dir)
	$(SWIG) -c++ $(AM_CPPFLAGS) -java -o kmlengine_java.cc \
	-package com.googlecode.libkml \
	-outdir $(java_package_dir) $(top_srcdir)/src/swig/kmlengine.i

libkmlengine_swig_python_la_SOURCES = kmlengine_python.cc kmlengine.py
libkmlengine_swig_python_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(PYTHON_INCLUDE_DIR)
libkmlengine_swig_python_la_LIBADD = $(COMMONLIBS)
libkmlengine_swig_python_la_LDFLAGS = -dynamiclib -python -lexpat
kmlengine_python.cc: kmlengine.i
	$(SWIG) -c++ $(AM_CPPFLAGS) -python -o kmlengine_python.cc \
	$(top_srcdir)/src/swig/kmlengine.i

if WITH_SWIG
if WITH_PYTHON
pythonext_DATA = _kmlbase.so kmlbase.py _kmldom.so kmldom.py _kmlengine.so kmlengine.py
_kmlbase.so:
	cp $(top_builddir)/src/swig/.libs/libkmlbase_swig_python.$(SHLIBSUFFIX) _kmlbase.so
kmlbase.py:
	cp $(top_srcdir)/src/swig/kmlbase.py kmlbase.py
_kmldom.so:
	cp $(top_builddir)/src/swig/.libs/libkmldom_swig_python.$(SHLIBSUFFIX) _kmldom.so
kmldom.py:
	cp $(top_srcdir)/src/swig/kmldom.py kmldom.py
_kmlengine.so:
	cp $(top_builddir)/src/swig/.libs/libkmlengine_swig_python.$(SHLIBSUFFIX) _kmlengine.so
kmlengine.py:
	cp $(top_srcdir)/src/swig/kmlengine.py kmlengine.py

pythonextdir = $(PYTHON_SITE_PACKAGES_DIR)
endif
if WITH_JAVA
javadir = $(datadir)/java
java_DATA = LibKML.jar
# TODO: shared libs need to be bundled in jar?
LibKML.jar: kmlbase_java.cc kmldom_java.cc kmlengine_java.cc
	$(JAVAC) $(java_package_dir)/*.java
	$(JAR) cf LibKML.jar $(java_package_dir)/*.class

# This copies the Java archive file into the tarball when 'make dist' is called:
dist-hook: LibKML.jar
	cp -p -v $(top_builddir)/src/swig/LibKML.jar $(distdir)/

endif
clean-local:
	-rm -f kmlbase_java.cc kmldom_java.cc kmlengine_java.cc \
	  LibKML.jar $(java_package_dir)/* \
	  kmlbase_python.cc kmldom_python.cc kmlengine_python.cc \
	  kmlbase.py kmldom.py kmlengine.py \
	  _kmlbase.so _kmldom.so _kmlengine.so
endif

EXTRA_DIST = \
	libkml_intrusive_ptr.i \
	kmlbase.i \
	kmlbase_test.py \
	kmldom.i \
	kmldom_test.py \
	kmlelements.i \
	kmlengine.i \
	kmlengine_test.py

# The Python library on both Linux and OS X has to be called _kmldom.so. As
# above, we accomplish this with a _DATA directive that copies the file. Since
# there is an inherit dependency, we have to disable parallel builds in this
# file, otherwise 'make -j3' or similar is likely to hit the race condition.
.NOTPARALLEL:
