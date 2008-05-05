AM_CPPFLAGS = -I$(top_srcdir)/src \
	      -I$(top_srcdir)/third_party/boost_1_34_1

COMMONLIBS = $(top_builddir)/src/kml/dom/libkmldom.la

pkglib_LTLIBRARIES=

if WITH_SWIG
if WITH_JAVA
pkglib_LTLIBRARIES += libkmldom_swig_java.la
endif
if WITH_PYTHON
pkglib_LTLIBRARIES += libkmldom_swig_python.la
endif
endif

libkmldom_swig_java_la_SOURCES = kmldom_java.cc
libkmldom_swig_java_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(JAVA_INCLUDE_DIR)
libkmldom_swig_java_la_LIBADD = $(COMMONLIBS)
kmldom_java.cc: kmldom.i
	$(SWIG) -c++ $(AM_CPPFLAGS) -java -o kmldom_java.cc \
	$(top_srcdir)/src/swig/kmldom.i

libkmldom_swig_python_la_SOURCES = kmldom_python.cc kmldom.py
libkmldom_swig_python_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(PYTHON_INCLUDE_DIR)
libkmldom_swig_python_la_LIBADD = $(COMMONLIBS)
libkmldom_swig_python_la_LDFLAGS = -dynamiclib -python -lexpat
kmldom_python.cc: kmldom.i
	$(SWIG) -c++ $(AM_CPPFLAGS) -python -o kmldom_python.cc \
	$(top_srcdir)/src/swig/kmldom.i

pythonext_DATA = _kmldom.so kmldom.py
_kmldom.so:
	cp $(top_builddir)/src/swig/.libs/libkmldom_swig_python.$(SHLIBSUFFIX) _kmldom.so
kmldom.py:
	cp $(top_srcdir)/src/swig/kmldom.py kmldom.py
pythonextdir = $(PYTHON_SITE_PACKAGES_DIR)

clean-local:
	-rm -f _kmldom.so

EXTRA_DIST = \
	kmldom.i \
	kmlelements.i \
	kmldom_test.py

# The Python library on both Linux and OS X has to be called _kmldom.so. As
# above, we accomplish this with a _DATA directive that copies the file. Since
# there is an inherit dependency, we have to disable parallel builds in this
# file, otherwise 'make -j3' or similar is likely to hit the race condition.
.NOTPARALLEL: 
