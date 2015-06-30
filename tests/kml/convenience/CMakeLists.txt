set(TESTS csv_file
csv_parser
element_counter
feature_list
google_doc_list
google_maps_data
google_picasa_web
google_spreadsheets
gpx_trk_pt_handler
http_client
kml_feature_list_saver
kmz_check_links)

foreach(TEST ${TESTS})
  build_test(
    GROUP regionator
    NAME ${TEST}
    DEPENDS kmlbase;kmlengine;kmldom;kmlconvenience)
endforeach()
