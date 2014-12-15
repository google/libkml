set(TESTS
attributes_test
color32_test
csv_splitter_test
date_time_test
expat_handler_ns_test
expat_parser_test
file_test
math_util_test
referent_test
string_util_test
tempfile_test
time_util_test
util_test
vec3_test
version_test
xml_element_test
xml_file_test
xml_namespaces_test
xmlns_test)

foreach(TEST ${TESTS})
  build_test(${TEST} kmlbase)
endforeach()

build_test(net_cache_test kmlbase ${URIPARSER_LIBRARY})
build_test(uri_parser_test kmlbase ${URIPARSER_LIBRARY})
build_test(zip_file_test kmlbase ${MINIZIP_LIBRARY})
