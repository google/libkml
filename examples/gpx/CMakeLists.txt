# AM_CPPFLAGS = -I$(top_srcdir)/src \
# 	      -I$(top_srcdir)/third_party/boost_1_34_1
# if GCC
# AM_CXXFLAGS = -Wall -Werror -pedantic -Wno-long-long -fno-rtti
# endif
build_example(gpxtracktokml kmlconvenience)