set(TESTS feature_list_region_handler
regionator
regionator_qid
regionator_util)

foreach(TEST ${TESTS})
  build_test(
    GROUP regionator
    NAME ${TEST}
    DEPENDS kmlregionator)
endforeach()
