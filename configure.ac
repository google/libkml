dnl Process this file with autoconf to produce a configure script.
AC_INIT(libkml, 0.2, opensource@google.com)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE

dnl Where can we find cppunit headers and libs?
AM_PATH_CPPUNIT(1.12.1)
dnl If we have cppunit installed, we set a define here. The unit tests 
dnl elsewhere will build conditionally against this flag.
AM_CONDITIONAL(HAVE_CPPUNIT, test "$CPPUNIT_LIBS")

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL

dnl automake will add -DHAVE_CONFIG_H to compiler flags
AM_CONFIG_HEADER([config.h]) 

AC_CONFIG_FILES([
	Makefile
	examples/Makefile
	examples/helloworld/Makefile
	src/Makefile
	src/kml/Makefile
	src/kml/dom/Makefile
	src/kml/util/Makefile
	third_party/Makefile
])

AC_OUTPUT

