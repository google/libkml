set(TESTS xsd_complex_type
xsd_file
xsd_element
xsd_handler
xsd_primitive_type
xsd_simple_type
xsd_schema
xsd_util
xst_parser)

foreach(TEST ${TESTS})
  build_test(xsd ${TEST} kmlxsd)
endforeach()