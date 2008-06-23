AM_CPPFLAGS = -I$(top_srcdir)/third_party/zlib-1.2.3/contrib

lib_LTLIBRARIES = libminizip.la
libminizip_la_SOURCES = \
	zlib-1.2.3/contrib/minizip/unzip.c \
	zlib-1.2.3/contrib/minizip/ioapi.c \
	zlib-1.2.3/contrib/minizip/zip.c
libminizip_la_LDFLAGS = -lz

boost = boost_1_34_1/boost
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

EXTRA_DIST = \
	zlib-1.2.3/contrib/minizip/crypt.h \
	zlib-1.2.3/contrib/minizip/ioapi.h \
	zlib-1.2.3/contrib/minizip/iowin32.c \
	zlib-1.2.3/contrib/minizip/iowin32.h \
	zlib-1.2.3/contrib/minizip/miniunz.c \
	zlib-1.2.3/contrib/minizip/minizip.c \
	zlib-1.2.3/contrib/minizip/mztools.c \
	zlib-1.2.3/contrib/minizip/mztools.h \
	zlib-1.2.3/contrib/minizip/unzip.h \
	zlib-1.2.3/contrib/minizip/zip.h

