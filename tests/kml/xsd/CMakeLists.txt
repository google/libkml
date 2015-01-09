set(TESTS xsd_complex_type_test
xsd_file_test
xsd_element_test
xsd_handler_test
xsd_primitive_type_test
xsd_simple_type_test
xsd_schema_test
xsd_util_test
xst_parser_test)

foreach(TEST ${TESTS})
  build_test(xsd ${TEST} kmlxsd)
endforeach()