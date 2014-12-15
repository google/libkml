// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the unit tests for the location utility functions.

#include "kml/engine/location_util.h"
#include "kml/base/file.h"
#include "kml/dom.h"
#include "kml/engine/bbox.h"
#include "kml/engine/kml_file.h"
#include "gtest/gtest.h"

using kmlbase::File;
using kmldom::CoordinatesPtr;
using kmldom::KmlFactory;
using kmldom::LatLonBoxPtr;
using kmldom::LatLonAltBoxPtr;
using kmldom::LinearRingPtr;
using kmldom::LineStringPtr;
using kmldom::LocationPtr;
using kmldom::ModelPtr;
using kmldom::MultiGeometryPtr;
using kmldom::PhotoOverlayPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::PolygonPtr;

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlengine {

// Avoid linking in kmlconvenience...
static PointPtr CreatePointCoordinates(double lat, double lon) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  PointPtr point = kml_factory->CreatePoint();
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();
  coordinates->add_latlng(lat, lon);
  point->set_coordinates(coordinates);
  return point;
}

// This tests the GetCenter() function.
TEST(LocationUtilTest, TestGetCenter) {
  KmlFactory* factory = KmlFactory::GetFactory();
  // NULL output pointer(s) should not crash.
  LatLonBoxPtr llb = factory->CreateLatLonBox();
  GetCenter(llb, NULL, NULL);
  double lat, lon;
  GetCenter(llb, &lat, NULL);
  // Missing lon pointer still saves a result for lat.
  ASSERT_EQ(0.0, lat);
  GetCenter(llb, NULL, &lon);
  // Missing lat pointer still saves a result for lon.
  ASSERT_EQ(0.0, lat);
  // A default LatLonBox is well defined thus so is its center.
  GetCenter(llb, &lat, &lon);
  ASSERT_EQ(0.0, lat);
  ASSERT_EQ(0.0, lon);
  // A default LatLonAltBox is well defined thus so is its center.
  LatLonAltBoxPtr llab = factory->CreateLatLonAltBox();
  GetCenter(llab, &lat, &lon);
  ASSERT_EQ(0.0, lat);
  ASSERT_EQ(0.0, lon);
}

TEST(LocationUtilTest, TestGetFeatureLatLon) {
  KmlFactory* factory = KmlFactory::GetFactory();
  const double kLat(-22.22);
  const double kLon(42.123);
  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_geometry(CreatePointCoordinates(kLat, kLon));
  double lat, lon;
  ASSERT_TRUE(GetFeatureLatLon(placemark, &lat, &lon));
  ASSERT_EQ(kLat, lat);
  ASSERT_EQ(kLon, lon);
}

TEST(LocationUtilTest, TestGetModelLatLon) {
  KmlFactory* factory = KmlFactory::GetFactory();
  const double kLat(-22.22);
  const double kLon(42.123);
  ModelPtr model = factory->CreateModel();
  LocationPtr location = factory->CreateLocation();
  location->set_latitude(kLat);
  location->set_longitude(kLon);
  model->set_location(location);
  double lat, lon;
  ASSERT_TRUE(GetModelLatLon(model, &lat, &lon));
  ASSERT_EQ(kLat, lat);
  ASSERT_EQ(kLon, lon);
}

TEST(LocationUtilTest, TestGetPlacemarkLatLon) {
  KmlFactory* factory = KmlFactory::GetFactory();
  const double kLat(-22.22);
  const double kLon(42.123);
  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_geometry(CreatePointCoordinates(kLat, kLon));
  double lat, lon;
  ASSERT_TRUE(GetPlacemarkLatLon(placemark, &lat, &lon));
  ASSERT_EQ(kLat, lat);
  ASSERT_EQ(kLon, lon);
}

TEST(LocationUtilTest, TestGetPointLatLon) {
  const double kLat(-22.22);
  const double kLon(42.123);
  double lat, lon;
  ASSERT_TRUE(GetPointLatLon(CreatePointCoordinates(kLat, kLon), &lat, &lon));
  ASSERT_EQ(kLat, lat);
  ASSERT_EQ(kLon, lon);
}

// This internal utility function parses the testcase file to a KmlFile.
static KmlFilePtr ParseFromDataDirFile(const string& subdir,
                                       const string& filename) {
  string kml_data;
  const string kml_file =
    File::JoinPaths(File::JoinPaths(string(DATADIR), subdir), filename);
  return File::ReadFileToString(kml_file, &kml_data) ?
      KmlFile::CreateFromParse(kml_data, NULL) : NULL;
}

// This is a table of test cases.
static const struct {
  const char* subdir;  // Subdirectory of testdata.
  const char* kml_filename;  // Path relative to subdir.
  const char* feature_id;  // id= of Feature within file.
  bool has_bounds;  // Expected return value of GetFeatureBounds.
  double north;  // Expected values of Bbox fields iff has_bounds == true.
  double south;
  double east;
  double west;
  bool has_loc;  // Expected return value of GetFeatureLatLon.
  double lat;  // Expected values of GetFeatureLatLon iff has_loc == true.
  double lon;
} kTestCases[] = {
  {  // A 2d Placemark.
    "kml", "kmlsamples.kml", "simple-placemark",
    true, 37.4222899014025, 37.4222899014025, -122.082203542568,
    -122.082203542568,
    true, 37.4222899014025, -122.082203542568 },
  {  // A 3d Placemark.
    "kml", "kmlsamples.kml", "floating-placemark",
    true, 37.4220033612141, 37.4220033612141, -122.084075, -122.084075,
    true, 37.4220033612141, -122.084075 },
  {  // A Placemark with no Geometry.
    "kml", "kmlsamples.kml", "descriptive-html-placemark",
    false, 0, 0, 0, 0,
    false, 0, 0 },
  {  // A 2d LineString Placemark.
    "kml", "kmlsamples.kml", "tessellated-linestring-placemark",
    true, 36.1067787047714, 36.0905099328766, -112.081423783034,
    -112.087026775269,
    true, 36.098644318824, -112.0842252791515 },
  {  // A 3d LineString Placemark (altitudeMode=absolute, and alt != 0).
    "kml", "kmlsamples.kml", "purple-line-placemark", true, 36.0944767260255,
    36.086463123013, -112.265654928602, -112.269526855561,
    true, 36.09046992451925, -112.2675908920815 },
  {  // A Polygon with only an outerBoundaryIs.
    "kml", "kmlsamples.kml", "b41", true, 37.4228181532365, 37.4220817196725,
    -122.08509907149, -122.086016227378,
    true, 37.4224499364545, -122.085557649434 },
  {  // A Polygon with holes.
    "kml", "kmlsamples.kml", "pentagon", true, 38.872910162817, 38.868757801256,
    -77.0531553685479, -77.0584405629039,
    true, 38.8708339820365, -77.0557979657259 },
  {  // A Folder with multiple Features, but none with any location info.
    "kml", "kmlsamples.kml", "screen-overlays-folder",
    false, 0, 0, 0, 0,
    false, 0, 0 },
  {  // Model
    "kml", "model-macky.kml", "model-macky",
    true, 40.009993372683, 40.009993372683, -105.272774533734,
    -105.272774533734,
    true, 40.009993372683, -105.272774533734
  },
  {  // PhotoOverlay
    "kml", "photooverlay-zermatt.kml", "photooverlay-zermatt",
    true, 45.968226693, 45.968226693, 7.71792711000002, 7.71792711000002,
    true, 45.968226693, 7.71792711000002
  },
#if 0
  {  // TODO: GroundOverlay
    "kml", "kmlsamples.kml", "large-groundoverlay",
    true, 45.968226693, 45.968226693, 7.71792711000002, 7.71792711000002,
    true, 45.968226693, 7.71792711000002
  },
#endif
  {  // A Folder with multiple Point Placemarks.
    "kml", "kmlsamples.kml", "placemarks-folder",
    true, 37.4222899014025, 37.4215692786755, -122.082203542568,
    -122.085766700618,
    true, 37.421929590039, -122.083985121593
  },
  {  // A Folder with multiple LineString Placemarks.
    "kml", "kmlsamples.kml", "paths-folder",
    true, 36.1067787047714, 36.0795495214565, -112.080622229595,
    -112.269526855561,
    true, 36.09316411311395, -112.175074542578
  },
  {  // A Folder with multiple Polygon Placemarks.
    "kml", "kmlsamples.kml", "google-campus-folder",
    true, 37.4228181532365, 37.4212932884059, -122.082850226966,
    -122.086016227378,
    true, 37.4220557208212, -122.084433227172
  },
  {  // A Document with multiple Folders each with multiple Features.
    "kml", "kmlsamples.kml", "root-document",
    true, 38.872910162817, 36.0795495214565, -77.0531553685479,
    -122.086016227378,
    true, 37.476229842136746, -99.569585797962958
  }
};

TEST(LocationUtilTest, RunTestCases) {
  size_t size = sizeof(kTestCases)/sizeof(kTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    KmlFilePtr kml_file =
        ParseFromDataDirFile(kTestCases[i].subdir, kTestCases[i].kml_filename);
    // Assert basic sanity of KmlFile.
    ASSERT_TRUE(kml_file) << kTestCases[i].kml_filename;
    ASSERT_TRUE(kml_file->get_root());
    kmldom::FeaturePtr feature = kmldom::AsFeature(
        kml_file->GetObjectById(kTestCases[i].feature_id));
    // Asserts both that this id is found and is a Feature.
    ASSERT_TRUE(feature);
    Bbox bbox;
    ASSERT_EQ(kTestCases[i].has_bounds, GetFeatureBounds(feature, &bbox))
        << kTestCases[i].kml_filename << " " << kTestCases[i].feature_id;
    // GetFeatureBounds returns the same no matter the state of the bbox arg.
    ASSERT_EQ(kTestCases[i].has_bounds, GetFeatureBounds(feature, NULL));
    if (kTestCases[i].has_bounds) {
      // If has_bounds then the test case n,s,e,w are valid to test.
      ASSERT_EQ(kTestCases[i].north, bbox.get_north());
      ASSERT_EQ(kTestCases[i].south, bbox.get_south());
      ASSERT_EQ(kTestCases[i].east, bbox.get_east());
      ASSERT_EQ(kTestCases[i].west, bbox.get_west());
    }
    double lat, lon;
    ASSERT_EQ(kTestCases[i].has_loc, GetFeatureLatLon(feature, &lat, &lon));
    // GetFeatureBounds returns same no matter the state of the lat/lon args.
    ASSERT_EQ(kTestCases[i].has_loc, GetFeatureLatLon(feature, &lat, NULL));
    ASSERT_EQ(kTestCases[i].has_loc, GetFeatureLatLon(feature, NULL, &lon));
    ASSERT_EQ(kTestCases[i].has_loc, GetFeatureLatLon(feature, NULL, NULL));
    if (kTestCases[i].has_loc) {
      // If has_loc then the test case lat,lon are valid to test.
      ASSERT_DOUBLE_EQ(kTestCases[i].lat, lat);
      ASSERT_DOUBLE_EQ(kTestCases[i].lon, lon);
    }
  }
}

// Test GetGeometryBounds on null/empty args.
TEST(LocationUtilTest, TestGetGeometryBoundsNullEmpty) {
  ASSERT_FALSE(GetGeometryBounds(NULL, NULL));
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  Bbox bbox;
  PointPtr point = kml_factory->CreatePoint();
  ASSERT_FALSE(GetGeometryBounds(point, NULL));
  ASSERT_FALSE(GetGeometryBounds(point, &bbox));
  LineStringPtr linestring = kml_factory->CreateLineString();
  ASSERT_FALSE(GetGeometryBounds(linestring, NULL));
  ASSERT_FALSE(GetGeometryBounds(linestring, &bbox));
  LinearRingPtr linearring = kml_factory->CreateLinearRing();
  ASSERT_FALSE(GetGeometryBounds(linearring, NULL));
  ASSERT_FALSE(GetGeometryBounds(linearring, &bbox));
  PolygonPtr poly = kml_factory->CreatePolygon();
  ASSERT_FALSE(GetGeometryBounds(poly, &bbox));  // Issue 148
  poly->set_outerboundaryis(kml_factory->CreateOuterBoundaryIs());
  ASSERT_FALSE(GetGeometryBounds(poly, &bbox));
  ModelPtr model = kml_factory->CreateModel();
  ASSERT_FALSE(GetGeometryBounds(model, NULL));
  ASSERT_FALSE(GetGeometryBounds(model, &bbox));
  MultiGeometryPtr multigeometry = kml_factory->CreateMultiGeometry();
  ASSERT_FALSE(GetGeometryBounds(multigeometry, NULL));
  ASSERT_FALSE(GetGeometryBounds(multigeometry, &bbox));
}

}  // end namespace kmlengine
