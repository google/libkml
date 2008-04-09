INCLUDES = -I$(top_srcdir)/src 
lib_LTLIBRARIES = libkmldom.la
libkmldom_la_SOURCES = \
	abstractlatlonbox.cc \
	abstractview.cc \
	balloonstyle.cc \
	colorstyle.cc \
	container.cc \
	document.cc \
	element.cc \
	extendeddata.cc \
	feature.cc \
	folder.cc \
	geometry.cc \
	hotspot.cc \
	iconstyle.cc \
	kml_cast.cc \
	kml_factory.cc \
	kml.cc \
	link.cc \
	labelstyle.cc \
	linestyle.cc \
	liststyle.cc \
	model.cc \
	networklink.cc \
	networklinkcontrol.cc \
	object.cc \
	overlay.cc \
	placemark.cc \
	polystyle.cc \
	region.cc \
	schema.cc \
	snippet.cc \
	style.cc \
	stylemap.cc \
	styleselector.cc \
	substyle.cc \
	timeprimitive.cc \
	vec2.cc \
	xsd.cc \
	kml_handler.cc \
	parser.cc \
	serializer.cc

libkmldomincludedir = $(includedir)/kml/dom
libkmldominclude_HEADERS = \
	abstractlatlonbox.h \
	abstractview.h \
	balloonstyle.h \
	colorstyle.h \
	container.h \
	document.h \
	element.h \
	extendeddata.h \
	feature.h \
	folder.h \
	geometry.h \
	hotspot.h \
	iconstyle.h \
	kml.h \
	kml22.h \
	kml_cast.h \
	kml_factory.h \
	kml_funcs.h \
	kmldom.h \
	labelstyle.h \
	linestyle.h \
	link.h \
	liststyle.h \
	model.h \
	networklink.h \
	networklinkcontrol.h \
	object.h \
	overlay.h \
	placemark.h \
	polystyle.h \
	region.h \
	schema.h \
	snippet.h \
	style.h \
	stylemap.h \
	styleselector.h \
	substyle.h \
	timeprimitive.h \
	vec2.h

if HAVE_CPPUNIT 
TESTS = abstractlatlonbox_test \
	abstractview_test \
	balloonstyle_test \
	colorstyle_test \
	container_test \
	document_test \
	element_test \
	extendeddata_test \
	feature_test \
	folder_test \
	geometry_test \
	hotspot_test \
	iconstyle_test \
	kml_cast_test \
	kml_test \
	kml_factory_test \
	labelstyle_test \
	linestyle_test \
	link_test \
	liststyle_test \
	model_test \
	networklink_test \
	networklinkcontrol_test \
	object_test \
	overlay_test \
	placemark_test \
	polystyle_test \
	region_test \
	schema_test \
	snippet_test \
	style_test \
	stylemap_test \
	styleselector_test \
	substyle_test \
	timeprimitive_test \
	vec2_test \
	unknown_test \
	attributes_test \
	parser_test \
	serializer_test \
	xsd_test

check_PROGRAMS = $(TESTS)

# Unit tests for KMLDOM classes.
abstractlatlonbox_test_SOURCES = abstractlatlonbox_test.cc
abstractlatlonbox_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
abstractlatlonbox_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
abstractlatlonbox_test_LDADD= libkmldom.la

abstractview_test_SOURCES = abstractview_test.cc
abstractview_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
abstractview_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
abstractview_test_LDADD= libkmldom.la

balloonstyle_test_SOURCES = balloonstyle_test.cc
balloonstyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
balloonstyle_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
balloonstyle_test_LDADD= libkmldom.la

colorstyle_test_SOURCES = colorstyle_test.cc
colorstyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
colorstyle_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
colorstyle_test_LDADD= libkmldom.la

container_test_SOURCES = container_test.cc
container_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
container_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
container_test_LDADD= libkmldom.la

document_test_SOURCES = document_test.cc
document_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
document_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
document_test_LDADD= libkmldom.la

element_test_SOURCES = element_test.cc
element_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
element_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
element_test_LDADD= libkmldom.la

extendeddata_test_SOURCES = extendeddata_test.cc
extendeddata_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
extendeddata_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
extendeddata_test_LDADD= libkmldom.la

feature_test_SOURCES = feature_test.cc
feature_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
feature_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
feature_test_LDADD= libkmldom.la

folder_test_SOURCES = folder_test.cc
folder_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
folder_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
folder_test_LDADD= libkmldom.la

geometry_test_SOURCES = geometry_test.cc
geometry_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
geometry_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
geometry_test_LDADD= libkmldom.la

hotspot_test_SOURCES = hotspot_test.cc
hotspot_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
hotspot_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
hotspot_test_LDADD= libkmldom.la

iconstyle_test_SOURCES = iconstyle_test.cc
iconstyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
iconstyle_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
iconstyle_test_LDADD= libkmldom.la

kml_cast_test_SOURCES = kml_cast_test.cc
kml_cast_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
kml_cast_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
kml_cast_test_LDADD= libkmldom.la

kml_test_SOURCES = kml_test.cc
kml_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
kml_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
kml_test_LDADD= libkmldom.la

kml_factory_test_SOURCES = kml_factory_test.cc
kml_factory_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
kml_factory_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
kml_factory_test_LDADD= libkmldom.la

labelstyle_test_SOURCES = labelstyle_test.cc
labelstyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
labelstyle_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
labelstyle_test_LDADD= libkmldom.la

linestyle_test_SOURCES = linestyle_test.cc
linestyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
linestyle_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
linestyle_test_LDADD= libkmldom.la

link_test_SOURCES = link_test.cc
link_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
link_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
link_test_LDADD= libkmldom.la

liststyle_test_SOURCES = liststyle_test.cc
liststyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
liststyle_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
liststyle_test_LDADD= libkmldom.la

model_test_SOURCES = model_test.cc
model_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
model_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
model_test_LDADD= libkmldom.la

networklink_test_SOURCES = networklink_test.cc
networklink_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
networklink_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
networklink_test_LDADD= libkmldom.la

networklinkcontrol_test_SOURCES = networklinkcontrol_test.cc
networklinkcontrol_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
networklinkcontrol_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
networklinkcontrol_test_LDADD= libkmldom.la

object_test_SOURCES = object_test.cc
object_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
object_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
object_test_LDADD= libkmldom.la

overlay_test_SOURCES = overlay_test.cc
overlay_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
overlay_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
overlay_test_LDADD= libkmldom.la

placemark_test_SOURCES = placemark_test.cc
placemark_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
placemark_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
placemark_test_LDADD= libkmldom.la

polystyle_test_SOURCES = polystyle_test.cc
polystyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
polystyle_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
polystyle_test_LDADD= libkmldom.la

region_test_SOURCES = region_test.cc
region_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
region_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
region_test_LDADD= libkmldom.la

schema_test_SOURCES = schema_test.cc
schema_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
schema_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
schema_test_LDADD= libkmldom.la

snippet_test_SOURCES = snippet_test.cc
snippet_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
snippet_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
snippet_test_LDADD= libkmldom.la

style_test_SOURCES = style_test.cc
style_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
style_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
style_test_LDADD= libkmldom.la

stylemap_test_SOURCES = stylemap_test.cc
stylemap_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
stylemap_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
stylemap_test_LDADD= libkmldom.la

styleselector_test_SOURCES = styleselector_test.cc
styleselector_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
styleselector_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
styleselector_test_LDADD= libkmldom.la

substyle_test_SOURCES = substyle_test.cc
substyle_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
substyle_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
substyle_test_LDADD= libkmldom.la

timeprimitive_test_SOURCES = timeprimitive_test.cc
timeprimitive_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
timeprimitive_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
timeprimitive_test_LDADD= libkmldom.la

vec2_test_SOURCES = vec2_test.cc
vec2_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
vec2_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
vec2_test_LDADD= libkmldom.la

unknown_test_SOURCES = unknown_test.cc
unknown_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
unknown_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
unknown_test_LDADD= libkmldom.la

# Unit tests for internal classes.
attributes_test_SOURCES = attributes_test.cc
attributes_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
attributes_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
attributes_test_LDADD= libkmldom.la

parser_test_SOURCES = parser_test.cc
parser_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
parser_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
parser_test_LDADD= libkmldom.la

serializer_test_SOURCES = serializer_test.cc
serializer_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
serializer_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
serializer_test_LDADD= libkmldom.la

xsd_test_SOURCES = xsd_test.cc
xsd_test_CXXFLAGS = $(CPPUNIT_CFLAGS)
xsd_test_LDFLAGS = $(CPPUNIT_LIBS) -lexpat
xsd_test_LDADD= libkmldom.la

CLEANFILES = check_PROGRAMS
endif
