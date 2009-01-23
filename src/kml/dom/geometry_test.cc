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

// This file contains the unit tests for Coordinates, Geometry, Point,
// LineString, LinearRing, Polygon, InnerBoundaryIs, OuterBoundaryIs, and
// MultiGeometry.

#include "kml/dom/geometry.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/kml_funcs.h"
#include "gtest/gtest.h"

using kmlbase::Vec3;

namespace kmldom {

// Test Coordinates.
class CoordinatesTest : public testing::Test {
 protected:
  virtual void SetUp() {
    coordinates_ = KmlFactory::GetFactory()->CreateCoordinates();
  }

  CoordinatesPtr coordinates_;
};

TEST_F(CoordinatesTest, TestType) {
  ASSERT_EQ(Type_coordinates, coordinates_->Type());
  ASSERT_TRUE(coordinates_->IsA(Type_coordinates));
}

// Verify proper defaults:
TEST_F(CoordinatesTest, TestDefaults) {
  ASSERT_EQ(static_cast<size_t>(0), coordinates_->get_coordinates_array_size());
}

// Verify the add_latlng() setter.
TEST_F(CoordinatesTest, TestAddLatLng) {
  const double kLat(-22.22);
  const double kLon(44.44);
  coordinates_->add_latlng(kLat, kLon);
  ASSERT_EQ(static_cast<size_t>(1),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3 = coordinates_->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(kLat, vec3.get_latitude());
  ASSERT_DOUBLE_EQ(kLon, vec3.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec3.get_altitude());
}

// Verify the add_latlngalt() setter.
TEST_F(CoordinatesTest, TestAddLatLngAlt) {
  const double kLat(-22.22);
  const double kLon(44.44);
  const double kAlt(10001.2002);
  coordinates_->add_latlngalt(kLat, kLon, kAlt);
  ASSERT_EQ(static_cast<size_t>(1),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3 = coordinates_->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(kLat, vec3.get_latitude());
  ASSERT_DOUBLE_EQ(kLon, vec3.get_longitude());
  ASSERT_DOUBLE_EQ(kAlt, vec3.get_altitude());
}

// Verify the add_vec3() setter.
TEST_F(CoordinatesTest, TestAddVec3) {
  const double kLat(-22.22);
  const double kLon(44.44);
  const double kAlt(10001.2002);
  Vec3 v(kLon, kLat, kAlt);
  coordinates_->add_vec3(v);
  ASSERT_EQ(static_cast<size_t>(1),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3_0 = coordinates_->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(kLat, vec3_0.get_latitude());
  ASSERT_DOUBLE_EQ(kLon, vec3_0.get_longitude());
  ASSERT_DOUBLE_EQ(kAlt, vec3_0.get_altitude());

  coordinates_->add_vec3(Vec3());
  ASSERT_EQ(static_cast<size_t>(2),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3_1 = coordinates_->get_coordinates_array_at(1);
  ASSERT_DOUBLE_EQ(0.0, vec3_1.get_latitude());
  ASSERT_DOUBLE_EQ(0.0, vec3_1.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec3_1.get_altitude());

  coordinates_->add_vec3(Vec3(kLon, kLat));
  ASSERT_EQ(static_cast<size_t>(3),
            coordinates_->get_coordinates_array_size());
  Vec3 vec3_2 = coordinates_->get_coordinates_array_at(2);
  ASSERT_DOUBLE_EQ(kLat, vec3_2.get_latitude());
  ASSERT_DOUBLE_EQ(kLon, vec3_2.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec3_2.get_altitude());
}

// Verify a bunch of points in a <coordinates> element.
TEST_F(CoordinatesTest, TestAddLatLngAltMany) {
  const size_t kNumPoints(1001);
  size_t i;
  for (i = 0; i < kNumPoints; ++i) {
    coordinates_->add_latlngalt(static_cast<double>(i % 90),
                                static_cast<double>(i % 180),
                                static_cast<double>(i));
  }
  ASSERT_EQ(static_cast<size_t>(kNumPoints),
            coordinates_->get_coordinates_array_size());
  for (i = 0; i < kNumPoints; ++i) {
    Vec3 vec3 = coordinates_->get_coordinates_array_at(i);
    ASSERT_DOUBLE_EQ(static_cast<double>(i % 90), vec3.get_latitude());
    ASSERT_DOUBLE_EQ(static_cast<double>(i % 180), vec3.get_longitude());
    ASSERT_DOUBLE_EQ(static_cast<double>(i), vec3.get_altitude());
  }
}

TEST_F(CoordinatesTest, TestParseVec3) {
  const char* basic_3d_point = "1.123,-2.789,3000.5919";
  char *endp;
  Vec3 vec;
  ASSERT_TRUE(Coordinates::ParseVec3(basic_3d_point, &endp, &vec));
  ASSERT_DOUBLE_EQ(-2.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(1.123, vec.get_longitude());
  ASSERT_DOUBLE_EQ(3000.5919, vec.get_altitude());
  ASSERT_EQ(basic_3d_point + strlen(basic_3d_point), endp);

  const char* basic_3d_line = "-122.123,38.789,1050.0987 "
                              "-122.123,39.789,1050.098";
  ASSERT_TRUE(Coordinates::ParseVec3(basic_3d_line, &endp, &vec));
  ASSERT_DOUBLE_EQ(38.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(-122.123, vec.get_longitude());
  ASSERT_DOUBLE_EQ(1050.0987, vec.get_altitude());
  ASSERT_TRUE(Coordinates::ParseVec3(endp, &endp, &vec));
  ASSERT_DOUBLE_EQ(39.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(-122.123, vec.get_longitude());
  ASSERT_DOUBLE_EQ(1050.098, vec.get_altitude());

  const char* basic_2d_point = "10.10,-20.20";
  ASSERT_TRUE(Coordinates::ParseVec3(basic_2d_point, &endp, &vec));
  ASSERT_DOUBLE_EQ(-20.20, vec.get_latitude());
  ASSERT_DOUBLE_EQ(10.10, vec.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* point2d_with_1space = "15.10, -24.20";
  ASSERT_TRUE(Coordinates::ParseVec3(point2d_with_1space, &endp, &vec));
  ASSERT_DOUBLE_EQ(-24.20, vec.get_latitude());
  ASSERT_DOUBLE_EQ(15.10, vec.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* point2d_with_2spaces = "15.11 , -24.25";
  ASSERT_TRUE(Coordinates::ParseVec3(point2d_with_2spaces, &endp, &vec));
  ASSERT_DOUBLE_EQ(-24.25, vec.get_latitude());
  ASSERT_DOUBLE_EQ(15.11, vec.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* basic_2d_line = "122.123,-38.789 "
                              "122.123,-39.789";
  ASSERT_TRUE(Coordinates::ParseVec3(basic_2d_line, &endp, &vec));
  ASSERT_DOUBLE_EQ(-38.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(122.123, vec.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());
  ASSERT_TRUE(Coordinates::ParseVec3(endp, &endp, &vec));
  ASSERT_DOUBLE_EQ(-39.789, vec.get_latitude());
  ASSERT_DOUBLE_EQ(122.123, vec.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  // How our own serializer emits <coordinates>
  const char* line_with_newlines = "\n"
                                   "  -160.073803556017,22.0041773078075\n"
                                   "  -160.121962433575,21.9639787234984\n"
                                   "  -160.22633646805,21.8915919620539\n"
                                   "  ";
  ASSERT_TRUE(Coordinates::ParseVec3(line_with_newlines, &endp, &vec));

  const char* exponential_2d_pt = "1E-02, 2E-02";  // 0.01, 0.02
  ASSERT_TRUE(Coordinates::ParseVec3(exponential_2d_pt, &endp, &vec));
  ASSERT_DOUBLE_EQ(0.02, vec.get_latitude());
  ASSERT_DOUBLE_EQ(0.01, vec.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  // Ensure junk data is handled gracefully.
  const char* junk_coords1 = "this will not parse correctly";
  ASSERT_FALSE(Coordinates::ParseVec3(junk_coords1, &endp, &vec));

  const char* junk_coords2 = "0,foo";  // Will parse successfully.
  ASSERT_TRUE(Coordinates::ParseVec3(junk_coords2, &endp, &vec));
  ASSERT_DOUBLE_EQ(0.0, vec.get_latitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* junk_coords3 = "bar,0";  // Will parse successfully.
  ASSERT_TRUE(Coordinates::ParseVec3(junk_coords3, &endp, &vec));
  ASSERT_DOUBLE_EQ(0.0, vec.get_latitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_longitude());
  ASSERT_DOUBLE_EQ(0.0, vec.get_altitude());

  const char* junk_coords4 = "\n";  // Will fail parsing.
  ASSERT_FALSE(Coordinates::ParseVec3(junk_coords4, &endp, &vec));
}

TEST_F(CoordinatesTest, TestParsePoint) {
  // Parser gathers character data and sets element
  coordinates_->Parse("1.1,-2.2,3.3");
  ASSERT_EQ(static_cast<size_t>(1), coordinates_->get_coordinates_array_size());
  Vec3 vec = coordinates_->get_coordinates_array_at(0);
  ASSERT_DOUBLE_EQ(-2.2, vec.get_latitude());
  ASSERT_DOUBLE_EQ(1.1, vec.get_longitude());
  ASSERT_DOUBLE_EQ(3.3, vec.get_altitude());
}

TEST_F(CoordinatesTest, TestParseLine) {
  // Parser gathers character data and sets element
  coordinates_->Parse("1.1,-2.2,3.3 -5.1,32.9872,10000.3");
  ASSERT_EQ(static_cast<size_t>(2), coordinates_->get_coordinates_array_size());
  ASSERT_DOUBLE_EQ(-2.2,
                   coordinates_->get_coordinates_array_at(0).get_latitude());
  ASSERT_DOUBLE_EQ(1.1,
                   coordinates_->get_coordinates_array_at(0).get_longitude());
  ASSERT_DOUBLE_EQ(3.3,
                   coordinates_->get_coordinates_array_at(0).get_altitude());
}

TEST_F(CoordinatesTest, TestParseBadSeparator) {
  // Ensure graceful handling of bad data.
  coordinates_->Parse("130.999*66.56083,75");
  ASSERT_EQ(static_cast<size_t>(1), coordinates_->get_coordinates_array_size());
  ASSERT_DOUBLE_EQ(75.0,
                   coordinates_->get_coordinates_array_at(0).get_latitude());
  ASSERT_DOUBLE_EQ(130.999,
                   coordinates_->get_coordinates_array_at(0).get_longitude());
  ASSERT_DOUBLE_EQ(0.0,
                   coordinates_->get_coordinates_array_at(0).get_altitude());
}

// Test Point.
class PointTest : public testing::Test {
 protected:
  virtual void SetUp() {
    point_ = KmlFactory::GetFactory()->CreatePoint();
  }

  PointPtr point_;
};

TEST_F(PointTest, TestType) {
  ASSERT_EQ(Type_Point, point_->Type());
  ASSERT_TRUE(point_->IsA(Type_Point));
  ASSERT_TRUE(point_->IsA(Type_Geometry));
  ASSERT_TRUE(point_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(PointTest, TestDefaults) {
  ASSERT_FALSE(point_->has_id());
  ASSERT_FALSE(point_->has_targetid());
  ASSERT_FALSE(point_->has_extrude());
  ASSERT_FALSE(point_->get_extrude());
  ASSERT_FALSE(point_->has_altitudemode());
  ASSERT_TRUE(ALTITUDEMODE_CLAMPTOGROUND == point_->get_altitudemode());
  ASSERT_FALSE(point_->has_coordinates());
  ASSERT_TRUE(NULL == point_->get_coordinates());
}

// Verify setting default makes has_xxx() true:
TEST_F(PointTest, TestSetToDefaultValues) {
  point_->set_extrude(point_->get_extrude());
  point_->set_altitudemode(point_->get_altitudemode());
  point_->set_coordinates(NULL);  // should not crash
  ASSERT_TRUE(point_->has_extrude());
  ASSERT_TRUE(point_->has_altitudemode());
  ASSERT_FALSE(point_->has_coordinates());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(PointTest, TestSetGetHasClear) {
  // Non-default values:
  bool extrude = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();
  const char* id = "point-id";
  const char* targetid = "point-targetid";

  // Set all fields:
  point_->set_id(id);
  point_->set_targetid(targetid);
  point_->set_extrude(extrude);
  point_->set_altitudemode(altitudemode);
  point_->set_coordinates(coordinates);

  // Verify getter and has_xxx():
  ASSERT_TRUE(point_->has_id());
  ASSERT_EQ(id, point_->get_id());
  ASSERT_TRUE(point_->has_targetid());
  ASSERT_EQ(targetid, point_->get_targetid());
  ASSERT_TRUE(point_->has_extrude());
  ASSERT_EQ(extrude, point_->get_extrude());
  ASSERT_TRUE(point_->has_altitudemode());
  ASSERT_EQ(altitudemode, point_->get_altitudemode());
  ASSERT_TRUE(point_->has_coordinates());
  ASSERT_EQ(coordinates, point_->get_coordinates());

  // Clear all fields:
  point_->clear_id();
  point_->clear_targetid();
  point_->clear_extrude();
  point_->clear_altitudemode();
  point_->clear_coordinates();

}

TEST_F(PointTest, TestSerialize) {
  point_->set_id("point-id");
  point_->set_extrude(true);
  point_->set_coordinates(KmlFactory::GetFactory()->CreateCoordinates());
  std::string expected(
    "<Point id=\"point-id\">"
    "<extrude>1</extrude>"
    "<coordinates/>"
    "</Point>"
  );
  ASSERT_EQ(expected, SerializeRaw(point_));
}

// Test LineString.
class LineStringTest : public testing::Test {
 protected:
  virtual void SetUp() {
    linestring_ = KmlFactory::GetFactory()->CreateLineString();
  }

  LineStringPtr linestring_;
};

TEST_F(LineStringTest, TestType) {
  ASSERT_EQ(Type_LineString, linestring_->Type());
  ASSERT_TRUE(linestring_->IsA(Type_LineString));
  ASSERT_TRUE(linestring_->IsA(Type_Geometry));
  ASSERT_TRUE(linestring_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(LineStringTest, TestDefaults) {
  ASSERT_FALSE(linestring_->has_id());
  ASSERT_FALSE(linestring_->has_targetid());
  ASSERT_FALSE(linestring_->has_extrude());
  ASSERT_FALSE(linestring_->get_extrude());
  ASSERT_FALSE(linestring_->has_tessellate());
  ASSERT_FALSE(linestring_->get_tessellate());
  ASSERT_FALSE(linestring_->has_altitudemode());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, linestring_->get_altitudemode());
  ASSERT_FALSE(linestring_->has_coordinates());
}

// Verify setting default makes has_xxx() true:
TEST_F(LineStringTest, TestSetToDefaultValues) {
  linestring_->set_extrude(linestring_->get_extrude());
  linestring_->set_tessellate(linestring_->get_tessellate());
  linestring_->set_altitudemode(linestring_->get_altitudemode());
  linestring_->set_coordinates(NULL);  // should not crash
  ASSERT_TRUE(linestring_->has_extrude());
  ASSERT_TRUE(linestring_->has_tessellate());
  ASSERT_TRUE(linestring_->has_altitudemode());
  ASSERT_FALSE(linestring_->has_coordinates());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(LineStringTest, TestSetGetHasClear) {
  // Non-default values:
  bool extrude = true;
  bool tessellate = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();

  // Set all fields:
  linestring_->set_extrude(extrude);
  linestring_->set_tessellate(tessellate);
  linestring_->set_altitudemode(altitudemode);
  linestring_->set_coordinates(coordinates);

  // Verify getter and has_xxx():
  ASSERT_TRUE(linestring_->has_extrude());
  ASSERT_EQ(extrude, linestring_->get_extrude());
  ASSERT_TRUE(linestring_->has_tessellate());
  ASSERT_EQ(tessellate, linestring_->get_tessellate());
  ASSERT_TRUE(linestring_->has_altitudemode());
  ASSERT_EQ(altitudemode, linestring_->get_altitudemode());
  ASSERT_TRUE(linestring_->has_coordinates());
  ASSERT_EQ(coordinates, linestring_->get_coordinates());

  // Clear all fields:
  linestring_->clear_extrude();
  linestring_->clear_tessellate();
  linestring_->clear_altitudemode();
  linestring_->clear_coordinates();
}

TEST_F(LineStringTest, TestSerialize) {
  linestring_->set_id("linestring-id");
  linestring_->set_tessellate(true);
  linestring_->set_coordinates(KmlFactory::GetFactory()->CreateCoordinates());
  std::string expected(
    "<LineString id=\"linestring-id\">"
    "<tessellate>1</tessellate>"
    "<coordinates/>"
    "</LineString>"
  );
  ASSERT_EQ(expected, SerializeRaw(linestring_));
}

// Test LinearRing.
class LinearRingTest : public testing::Test {
 protected:
  virtual void SetUp() {
    linearring_ = KmlFactory::GetFactory()->CreateLinearRing();
  }

  LinearRingPtr linearring_;
};

TEST_F(LinearRingTest, TestType) {
  ASSERT_EQ(Type_LinearRing, linearring_->Type());
  ASSERT_TRUE(linearring_->IsA(Type_LinearRing));
  ASSERT_TRUE(linearring_->IsA(Type_Geometry));
  ASSERT_TRUE(linearring_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(LinearRingTest, TestDefaults) {
  ASSERT_FALSE(linearring_->has_id());
  ASSERT_FALSE(linearring_->has_targetid());
  ASSERT_FALSE(linearring_->has_extrude());
  ASSERT_FALSE(linearring_->get_extrude());
  ASSERT_FALSE(linearring_->has_tessellate());
  ASSERT_FALSE(linearring_->get_tessellate());
  ASSERT_FALSE(linearring_->has_altitudemode());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, linearring_->get_altitudemode());
  ASSERT_FALSE(linearring_->has_coordinates());
}

// Verify setting default makes has_xxx() true:
TEST_F(LinearRingTest, TestSetToDefaultValues) {
  linearring_->set_extrude(linearring_->get_extrude());
  linearring_->set_tessellate(linearring_->get_tessellate());
  linearring_->set_altitudemode(linearring_->get_altitudemode());
  linearring_->set_coordinates(NULL);  // should not crash
  ASSERT_TRUE(linearring_->has_extrude());
  ASSERT_TRUE(linearring_->has_tessellate());
  ASSERT_TRUE(linearring_->has_altitudemode());
  ASSERT_FALSE(linearring_->has_coordinates());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(LinearRingTest, TestSetGetHasClear) {
  // Non-default values:
  bool extrude = true;
  bool tessellate = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  CoordinatesPtr coordinates = KmlFactory::GetFactory()->CreateCoordinates();

  // Set all fields:
  linearring_->set_extrude(extrude);
  linearring_->set_tessellate(tessellate);
  linearring_->set_altitudemode(altitudemode);
  linearring_->set_coordinates(coordinates);

  // Verify getter and has_xxx():
  ASSERT_TRUE(linearring_->has_extrude());
  ASSERT_EQ(extrude, linearring_->get_extrude());
  ASSERT_TRUE(linearring_->has_tessellate());
  ASSERT_EQ(tessellate, linearring_->get_tessellate());
  ASSERT_TRUE(linearring_->has_altitudemode());
  ASSERT_EQ(altitudemode, linearring_->get_altitudemode());
  ASSERT_TRUE(linearring_->has_coordinates());
  ASSERT_EQ(coordinates, linearring_->get_coordinates());

  // Clear all fields:
  linearring_->clear_extrude();
  linearring_->clear_tessellate();
  linearring_->clear_altitudemode();
  linearring_->clear_coordinates();
}

TEST_F(LinearRingTest, TestSerialize) {
  linearring_->set_id("linearring-id");
  linearring_->set_tessellate(false);
  linearring_->set_coordinates(KmlFactory::GetFactory()->CreateCoordinates());
  std::string expected(
    "<LinearRing id=\"linearring-id\">"
    "<tessellate>0</tessellate>"
    "<coordinates/>"
    "</LinearRing>"
  );
  ASSERT_EQ(expected, SerializeRaw(linearring_));
}

// Test OuterBoundaryIs.
class OuterBoundaryIsTest : public testing::Test {
 protected:
  virtual void SetUp() {
    outerboundaryis_ = KmlFactory::GetFactory()->CreateOuterBoundaryIs();
  }

  OuterBoundaryIsPtr outerboundaryis_;
};

TEST_F(OuterBoundaryIsTest, TestType) {
  ASSERT_EQ(Type_outerBoundaryIs, outerboundaryis_->Type());
  ASSERT_TRUE(outerboundaryis_->IsA(Type_outerBoundaryIs));
}

// Verify proper defaults:
TEST_F(OuterBoundaryIsTest, TestDefaults) {
  ASSERT_FALSE(outerboundaryis_->has_linearring());
}

TEST_F(OuterBoundaryIsTest, TestSetGetHasClear) {
  // Give it the only possible child:
  outerboundaryis_->set_linearring(
      KmlFactory::GetFactory()->CreateLinearRing());
  ASSERT_TRUE(outerboundaryis_->has_linearring());
  ASSERT_TRUE(outerboundaryis_->get_linearring());
  // Clear it and verify we're back to the default state.
  outerboundaryis_->clear_linearring();
}

// Test InnerBoundaryIs.
class InnerBoundaryIsTest : public testing::Test {
 protected:
  virtual void SetUp() {
    innerboundaryis_ = KmlFactory::GetFactory()->CreateInnerBoundaryIs();
  }

  InnerBoundaryIsPtr innerboundaryis_;
};

TEST_F(InnerBoundaryIsTest, TestType) {
  ASSERT_EQ(Type_innerBoundaryIs, innerboundaryis_->Type());
  ASSERT_TRUE(innerboundaryis_->IsA(Type_innerBoundaryIs));
}

// Verify proper defaults:
TEST_F(InnerBoundaryIsTest, TestDefaults) {
  ASSERT_FALSE(innerboundaryis_->has_linearring());
}

TEST_F(InnerBoundaryIsTest, TestSetGetHasClear) {
  // Give it the only possible child:
  innerboundaryis_->set_linearring(
      KmlFactory::GetFactory()->CreateLinearRing());
  ASSERT_TRUE(innerboundaryis_->has_linearring());
  ASSERT_TRUE(innerboundaryis_->get_linearring());
  // Clear it and verify we're back to the default state.
  innerboundaryis_->clear_linearring();
}

// TestPolygon
class PolygonTest : public testing::Test {
 protected:
  virtual void SetUp() {
    polygon_ = KmlFactory::GetFactory()->CreatePolygon();
  }

  PolygonPtr polygon_;
};

// Verify proper Type.
TEST_F(PolygonTest, TestType) {
  ASSERT_EQ(Type_Polygon, polygon_->Type());
  ASSERT_TRUE(polygon_->IsA(Type_Polygon));
  ASSERT_TRUE(polygon_->IsA(Type_Geometry));
  ASSERT_TRUE(polygon_->IsA(Type_Object));
}

// Verify proper defaults.
TEST_F(PolygonTest, TestDefaults) {
  ASSERT_FALSE(polygon_->has_id());
  ASSERT_FALSE(polygon_->has_targetid());
  ASSERT_FALSE(polygon_->has_extrude());
  ASSERT_FALSE(polygon_->get_extrude());
  ASSERT_FALSE(polygon_->has_tessellate());
  ASSERT_FALSE(polygon_->get_tessellate());
  ASSERT_FALSE(polygon_->has_altitudemode());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, polygon_->get_altitudemode());
  ASSERT_FALSE(polygon_->has_outerboundaryis());
  ASSERT_EQ(static_cast<size_t>(0), polygon_->get_innerboundaryis_array_size());
}

// Verify setting default makes has_xxx() true:
TEST_F(PolygonTest, TestSetToDefaultValues) {
  polygon_->set_extrude(polygon_->get_extrude());
  polygon_->set_tessellate(polygon_->get_tessellate());
  polygon_->set_altitudemode(polygon_->get_altitudemode());
  polygon_->set_outerboundaryis(NULL);
  ASSERT_TRUE(polygon_->has_extrude());
  ASSERT_TRUE(polygon_->has_tessellate());
  ASSERT_TRUE(polygon_->has_altitudemode());
  ASSERT_FALSE(polygon_->has_outerboundaryis());  // ptr is null
}

// Verify set, get, has, clear:
TEST_F(PolygonTest, TestSetGetHasClear) {
  // Non-default values:
  bool extrude = true;
  bool tessellate = true;
  AltitudeModeEnum altitudemode = ALTITUDEMODE_RELATIVETOGROUND;
  OuterBoundaryIsPtr outerboundaryis =
    KmlFactory::GetFactory()->CreateOuterBoundaryIs();

  // Set all fields:
  polygon_->set_extrude(extrude);
  polygon_->set_tessellate(tessellate);
  polygon_->set_altitudemode(altitudemode);
  polygon_->set_outerboundaryis(outerboundaryis);

  // Verify getter and has_xxx():
  ASSERT_TRUE(polygon_->has_extrude());
  ASSERT_EQ(extrude, polygon_->get_extrude());
  ASSERT_TRUE(polygon_->has_tessellate());
  ASSERT_EQ(tessellate, polygon_->get_tessellate());
  ASSERT_TRUE(polygon_->has_altitudemode());
  ASSERT_EQ(altitudemode, polygon_->get_altitudemode());
  ASSERT_TRUE(polygon_->has_outerboundaryis());
  ASSERT_EQ(outerboundaryis, polygon_->get_outerboundaryis());

  // Clear all fields:
  polygon_->clear_extrude();
  polygon_->clear_tessellate();
  polygon_->clear_altitudemode();
  polygon_->clear_outerboundaryis();
}

TEST_F(PolygonTest, TestSerialize) {
  polygon_->set_id("polygon-id");
  polygon_->set_altitudemode(ALTITUDEMODE_ABSOLUTE);
  polygon_->set_outerboundaryis(
      KmlFactory::GetFactory()->CreateOuterBoundaryIs());
  std::string expected(
    "<Polygon id=\"polygon-id\">"
    "<altitudeMode>absolute</altitudeMode>"
    "<outerBoundaryIs/>"
    "</Polygon>"
  );
  ASSERT_EQ(expected, SerializeRaw(polygon_));
}

// Test MultiGeometry.
class MultiGeometryTest : public testing::Test {
 protected:
  virtual void SetUp() {
    multigeometry_ = KmlFactory::GetFactory()->CreateMultiGeometry();
  }

  MultiGeometryPtr multigeometry_;
};

TEST_F(MultiGeometryTest, TestType) {
  ASSERT_EQ(Type_MultiGeometry, multigeometry_->Type());
  ASSERT_TRUE(multigeometry_->IsA(Type_MultiGeometry));
  ASSERT_TRUE(multigeometry_->IsA(Type_Geometry));
  ASSERT_TRUE(multigeometry_->IsA(Type_Object));
}

TEST_F(MultiGeometryTest, TestDefaults) {
  ASSERT_FALSE(multigeometry_->has_id());
  ASSERT_FALSE(multigeometry_->has_targetid());
  ASSERT_EQ(static_cast<size_t>(0), multigeometry_->get_geometry_array_size());
}

TEST_F(MultiGeometryTest, TestAddGetGeometries) {
  // Create some Geometryies and give them to the MultiGeometry
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePoint());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateMultiGeometry());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePolygon());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateModel());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLineString());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLinearRing());

  // Verify the proper size and order of the geometry array:
  ASSERT_EQ(static_cast<size_t>(6), multigeometry_->get_geometry_array_size());
  ASSERT_EQ(Type_Point, multigeometry_->get_geometry_array_at(0)->Type());
  ASSERT_EQ(Type_MultiGeometry,
            multigeometry_->get_geometry_array_at(1)->Type());
  ASSERT_EQ(Type_Polygon, multigeometry_->get_geometry_array_at(2)->Type());
  ASSERT_EQ(Type_Model, multigeometry_->get_geometry_array_at(3)->Type());
  ASSERT_EQ(Type_LineString, multigeometry_->get_geometry_array_at(4)->Type());
  ASSERT_EQ(Type_LinearRing, multigeometry_->get_geometry_array_at(5)->Type());
}

TEST_F(MultiGeometryTest, TestSerialize) {
  multigeometry_->set_id("multigeometry-id");
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePoint());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLineString());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateLinearRing());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreatePolygon());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateModel());
  multigeometry_->add_geometry(KmlFactory::GetFactory()->CreateMultiGeometry());
  std::string expected(
    "<MultiGeometry id=\"multigeometry-id\">"
    "<Point/>"
    "<LineString/>"
    "<LinearRing/>"
    "<Polygon/>"
    "<Model/>"
    "<MultiGeometry/>"
    "</MultiGeometry>"
  );
  ASSERT_EQ(expected, SerializeRaw(multigeometry_));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
