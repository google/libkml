set(TESTS bbox
clone
entity_mapper
feature_balloon
feature_visitor
feature_view
find
find_xml_namespaces
get_link_parents
get_links
href
id_mapper
kmz_cache
kml_cache
kml_file
kml_stream
kml_uri
kmz_file
link_util
location_util
merge
object_id_parser_observer
old_schema_parser_observer
parse_old_schema
schema_parser_observer
shared_style_parser_observer
style_inliner
style_merger
style_resolver
style_splitter
update_processor
update)

foreach(TEST ${TESTS})
  build_test(
    GROUP engine
    NAME ${TEST}
    DEPENDS kmlengine)  
endforeach()
