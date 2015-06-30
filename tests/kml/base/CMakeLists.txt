set(TESTS
attributes
color32
csv_splitter
date_time
expat_handler_ns
file
math_util
referent
string_util
tempfile
time_util
util
vec3
version
xml_element
xml_file
xml_namespaces
xmlns)

foreach(TEST ${TESTS})
  build_test(
    GROUP base
    NAME ${TEST}
    DEPENDS kmlbase)
endforeach()

build_test(
  GROUP base
  NAME net_cache
  LINKS ${URIPARSER_LIBRARY}
  DEPENDS kmlbase)

build_test(
  GROUP base
  NAME uri_parser
  LINKS ${URIPARSER_LIBRARY}
  DEPENDS kmlbase)

build_test(
  GROUP base
  NAME zip_file
  LINKS ${MINIZIP_LIBRARY}
  DEPENDS kmlbase)

build_test(
  GROUP base
  NAME expat_parser
  DEPENDS kmlbase;kmldom)