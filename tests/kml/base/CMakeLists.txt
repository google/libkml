set(TESTS
attributes
color32
csv_splitter
date_time
expat_handler_ns
expat_parser
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
  build_test(base ${TEST} kmlbase)
endforeach()

build_test(base net_cache kmlbase ${URIPARSER_LIBRARY})
build_test(base uri_parser kmlbase ${URIPARSER_LIBRARY})
build_test(base zip_file kmlbase ${MINIZIP_LIBRARY})
