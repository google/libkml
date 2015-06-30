# AM_CPPFLAGS = -I$(top_srcdir)/src \
# 	      -I$(top_srcdir)/third_party/boost_1_34_1
# if GCC
# AM_CXXFLAGS = -Wall -Werror -pedantic -Wno-long-long -fno-rtti
# endif
build_example(
  NAME gpxtracktokml
  DEPENDS kmlconvenience)

if(INSTALL_EXAMPLES)
  file(GLOB eg_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")
  install(FILES ${eg_files}  DESTINATION examples/gpx)
endif(INSTALL_EXAMPLES)
