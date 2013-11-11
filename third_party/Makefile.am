# As of automake 1.14 it's not possible to use variable references
# in AM_CPPFLAGS -- the combination of -I$(top_srcdir) and $(foo) generates
# directories with the literal unexpanded $(foo) name. It looks like a real
# automake bug (see http://debbugs.gnu.org/cgi/bugreport.cgi?bug=13928) but
# for now we work around it by explicit values.
#uriparser = uriparser-0.7.5
#googletest = gtest-1.7.0

AM_CPPFLAGS = -I$(top_srcdir)/third_party/zlib-1.2.3/contrib \
	      -I$(top_srcdir)/third_party/uriparser-0.7.5/include \
	      -I$(top_srcdir)/third_party/gtest-1.7.0 \
	      -I$(top_srcdir)/third_party/gtest-1.7.0/include

lib_LTLIBRARIES = libminizip.la liburiparser.la
noinst_LTLIBRARIES = libgtest.la libgtest_main.la

libminizip_la_SOURCES = \
	zlib-1.2.3/contrib/minizip/unzip.c \
	zlib-1.2.3/contrib/minizip/ioapi.c \
	zlib-1.2.3/contrib/minizip/iomem_simple.c \
	zlib-1.2.3/contrib/minizip/zip.c
libminizip_la_LDFLAGS = -lz

liburiparser_la_SOURCES = \
	uriparser-0.7.5/lib/UriCommon.c \
	uriparser-0.7.5/lib/UriCommon.h \
	uriparser-0.7.5/lib/UriCompare.c \
	uriparser-0.7.5/lib/UriEscape.c \
	uriparser-0.7.5/lib/UriFile.c \
	uriparser-0.7.5/lib/UriIp4.c \
	uriparser-0.7.5/lib/UriIp4Base.c \
	uriparser-0.7.5/lib/UriIp4Base.h \
	uriparser-0.7.5/lib/UriNormalize.c \
	uriparser-0.7.5/lib/UriNormalizeBase.c \
	uriparser-0.7.5/lib/UriNormalizeBase.h \
	uriparser-0.7.5/lib/UriParse.c \
	uriparser-0.7.5/lib/UriParseBase.c \
	uriparser-0.7.5/lib/UriParseBase.h \
	uriparser-0.7.5/lib/UriQuery.c \
	uriparser-0.7.5/lib/UriRecompose.c \
	uriparser-0.7.5/lib/UriResolve.c \
	uriparser-0.7.5/lib/UriShorten.c
liburiparser_la_LDFLAGS = -version-info 1:5:0

boost_root = boost_1_34_1
boost = $(boost_root)/boost
boostinc = $(includedir)/kml/third_party/$(boost)
libboostincludedir = $(boostinc)
libboostinclude_HEADERS = \
	${boost}/assert.hpp \
	${boost}/checked_delete.hpp \
	${boost}/config.hpp \
	${boost}/intrusive_ptr.hpp \
	${boost}/scoped_ptr.hpp

libboostdetailincludedir = $(boostinc)/detail
libboostdetailinclude_HEADERS = \
	$(boost)/detail/workaround.hpp

libboostconfigincludedir = $(boostinc)/config
libboostconfiginclude_HEADERS = \
	$(boost)/config/posix_features.hpp \
	$(boost)/config/select_compiler_config.hpp \
	$(boost)/config/select_stdlib_config.hpp \
	$(boost)/config/select_platform_config.hpp \
	$(boost)/config/suffix.hpp \
	$(boost)/config/user.hpp

libboostconfigcompilerincludedir = $(boostinc)/config/compiler
libboostconfigcompilerinclude_HEADERS = \
	$(boost)/config/compiler/gcc.hpp
	
libboostconfigno_tr1includedir = $(boostinc)/config/no_tr1
libboostconfigno_tr1include_HEADERS = \
	$(boost)/config/no_tr1/utility.hpp
	
libboostconfigstdlibincludedir = $(boostinc)/config/stdlib
libboostconfigstdlibinclude_HEADERS = \
	$(boost)/config/stdlib/libstdcpp3.hpp

libboostconfigplatformincludedir = $(boostinc)/config/platform
libboostconfigplatforminclude_HEADERS = \
	$(boost)/config/platform/linux.hpp \
	$(boost)/config/platform/macos.hpp

libgtest_la_SOURCES = \
	gtest-1.7.0/src/gtest-all.cc

libgtest_main_la_SOURCES = gtest-1.7.0/src/gtest_main.cc
libgtest_main_la_LIBADD = libgtest.la

# TODO: add the new gtest libs for windows.
#	gtest-1.7.0.win32/debug/gtest.lib \
#	gtest-1.7.0.win32/release/gtest.lib

EXTRA_DIST = \
	expat.win32/expat.h \
	expat.win32/expat_external.h \
	expat.win32/libexpat.dll \
	expat.win32/libexpat.lib \
	expat.win32/libexpatMT.lib \
	expat.win32/libexpatw.dll \
	expat.win32/libexpatw.lib \
	expat.win32/libexpatwMT.lib \
	$(boost_root)/LICENSE_1_0.txt \
	$(boost)/README \
	$(boost)/config/abi_prefix.hpp \
	$(boost)/config/abi_suffix.hpp \
	$(boost)/config/auto_link.hpp \
	$(boost)/config/abi/borland_prefix.hpp \
	$(boost)/config/abi/borland_suffix.hpp \
	$(boost)/config/abi/msvc_prefix.hpp \
	$(boost)/config/abi/msvc_suffix.hpp \
	$(boost)/config/compiler/borland.hpp \
	$(boost)/config/compiler/comeau.hpp \
	$(boost)/config/compiler/common_edg.hpp \
	$(boost)/config/compiler/compaq_cxx.hpp \
	$(boost)/config/compiler/digitalmars.hpp \
	$(boost)/config/compiler/gcc_xml.hpp \
	$(boost)/config/compiler/greenhills.hpp \
	$(boost)/config/compiler/hp_acc.hpp \
	$(boost)/config/compiler/intel.hpp \
	$(boost)/config/compiler/kai.hpp \
	$(boost)/config/compiler/metrowerks.hpp \
	$(boost)/config/compiler/mpw.hpp \
	$(boost)/config/compiler/sgi_mipspro.hpp \
	$(boost)/config/compiler/sunpro_cc.hpp \
	$(boost)/config/compiler/vacpp.hpp \
	$(boost)/config/compiler/visualc.hpp \
	$(boost)/config/no_tr1/complex.hpp \
	$(boost)/config/no_tr1/functional.hpp \
	$(boost)/config/no_tr1/memory.hpp \
	$(boost)/config/platform/aix.hpp \
	$(boost)/config/platform/amigaos.hpp \
	$(boost)/config/platform/beos.hpp \
	$(boost)/config/platform/bsd.hpp \
	$(boost)/config/platform/cygwin.hpp \
	$(boost)/config/platform/hpux.hpp \
	$(boost)/config/platform/irix.hpp \
	$(boost)/config/platform/qnxnto.hpp \
	$(boost)/config/platform/solaris.hpp \
	$(boost)/config/platform/win32.hpp \
	$(boost)/config/requires_threads.hpp \
	$(boost)/config/stdlib/dinkumware.hpp \
	$(boost)/config/stdlib/libcomo.hpp \
	$(boost)/config/stdlib/modena.hpp \
	$(boost)/config/stdlib/msl.hpp \
	$(boost)/config/stdlib/roguewave.hpp \
	$(boost)/config/stdlib/sgi.hpp \
	$(boost)/config/stdlib/stlport.hpp \
	$(boost)/config/stdlib/vacpp.hpp \
	gtest-1.7.0/include/gtest/gtest-death-test.h \
	gtest-1.7.0/include/gtest/gtest-message.h \
	gtest-1.7.0/include/gtest/gtest-param-test.h \
	gtest-1.7.0/include/gtest/gtest-param-test.h.pump \
	gtest-1.7.0/include/gtest/gtest-printers.h \
	gtest-1.7.0/include/gtest/gtest-spi.h \
	gtest-1.7.0/include/gtest/gtest-test-part.h \
	gtest-1.7.0/include/gtest/gtest-typed-test.h \
	gtest-1.7.0/include/gtest/gtest.h \
	gtest-1.7.0/include/gtest/gtest_pred_impl.h \
	gtest-1.7.0/include/gtest/gtest_prod.h \
	gtest-1.7.0/include/gtest/internal/gtest-death-test-internal.h \
	gtest-1.7.0/include/gtest/internal/gtest-filepath.h \
	gtest-1.7.0/include/gtest/internal/gtest-internal.h \
	gtest-1.7.0/include/gtest/internal/gtest-linked_ptr.h \
	gtest-1.7.0/include/gtest/internal/gtest-param-util-generated.h \
	gtest-1.7.0/include/gtest/internal/gtest-param-util-generated.h.pump \
	gtest-1.7.0/include/gtest/internal/gtest-param-util.h \
	gtest-1.7.0/include/gtest/internal/gtest-port.h \
	gtest-1.7.0/include/gtest/internal/gtest-string.h \
	gtest-1.7.0/include/gtest/internal/gtest-tuple.h \
	gtest-1.7.0/include/gtest/internal/gtest-tuple.h.pump \
	gtest-1.7.0/include/gtest/internal/gtest-type-util.h \
	gtest-1.7.0/include/gtest/internal/gtest-type-util.h.pump \
	gtest-1.7.0/src/gtest-death-test.cc \
	gtest-1.7.0/src/gtest-filepath.cc \
	gtest-1.7.0/src/gtest-internal-inl.h \
	gtest-1.7.0/src/gtest-port.cc \
	gtest-1.7.0/src/gtest-printers.cc \
	gtest-1.7.0/src/gtest-test-part.cc \
	gtest-1.7.0/src/gtest-typed-test.cc \
	gtest-1.7.0/src/gtest.cc \
	uriparser-0.7.5/COPYING \
	uriparser-0.7.5.win32/debug/uriparser.lib \
	uriparser-0.7.5.win32/release/uriparser.lib \
	uriparser-0.7.5/include/uriparser/Uri.h \
	uriparser-0.7.5/include/uriparser/UriBase.h \
	uriparser-0.7.5/include/uriparser/UriDefsAnsi.h \
	uriparser-0.7.5/include/uriparser/UriDefsConfig.h \
	uriparser-0.7.5/include/uriparser/UriDefsUnicode.h \
	uriparser-0.7.5/include/uriparser/UriIp4.h \
	zlib-1.2.3/zconf.h \
	zlib-1.2.3/zlib.h \
	zlib-1.2.3/contrib/minizip/crypt.h \
	zlib-1.2.3/contrib/minizip/ioapi.h \
	zlib-1.2.3/contrib/minizip/iowin32.c \
	zlib-1.2.3/contrib/minizip/iowin32.h \
	zlib-1.2.3/contrib/minizip/miniunz.c \
	zlib-1.2.3/contrib/minizip/minizip.c \
	zlib-1.2.3/contrib/minizip/mztools.c \
	zlib-1.2.3/contrib/minizip/mztools.h \
	zlib-1.2.3/contrib/minizip/unzip.h \
	zlib-1.2.3/contrib/minizip/zip.h \
	zlib-1.2.3.win32/bin/minizip.dll \
	zlib-1.2.3.win32/bin/zlib1.dll \
	zlib-1.2.3.win32/lib/minizip.lib \
	zlib-1.2.3.win32/lib/zdll.lib \
	zlib-1.2.3.win32/lib/zlib.lib

