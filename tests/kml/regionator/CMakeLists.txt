set(TESTS feature_list_region_handler_test
regionator_test
regionator_qid_test
regionator_util_test)

foreach(TEST ${TESTS})
  build_test(regionator ${TEST} kmlregionator)
endforeach()
