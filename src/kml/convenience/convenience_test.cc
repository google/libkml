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

// This file contains the unit tests for the KML convenience functions.

#include "kml/convenience/convenience.h"
#include "boost/scoped_ptr.hpp"
#include "kml/dom/kml_factory.h"
#include "kml/base/date_time.h"
#include "gtest/gtest.h"
#include "kml/base/vec3.h"

using kmlbase::DateTime;
using kmldom::CoordinatesPtr;
using kmldom::DataPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::RegionPtr;

namespace kmlconvenience {

class ConvenienceTest : public testing::Test {
};

// This tests the AddExtendedDataValue() function.
TEST_F(ConvenienceTest, TestAddExtendedDataValue) {
  const std::string kName("population");
  const std::string kValue("42000");
  PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
  AddExtendedDataValue(kName, kValue, placemark);
  ASSERT_TRUE(placemark->has_extendeddata());
  ASSERT_EQ(static_cast<size_t>(1),
            placemark->get_extendeddata()->get_data_array_size());
  ASSERT_EQ(kName,
            placemark->get_extendeddata()->get_data_array_at(0)->get_name());
  ASSERT_EQ(kValue,
            placemark->get_extendeddata()->get_data_array_at(0)->get_value());
}

// This tests the CreateDataNameValue() function.
TEST_F(ConvenienceTest, TestCreateDataNameValue) {
  const std::string kName("par");
  const std::string kValue("5");
  DataPtr data = CreateDataNameValue(kName, kValue);
  ASSERT_TRUE(data);
  ASSERT_EQ(kName, data->get_name());
  ASSERT_EQ(kValue, data->get_value());
}

// This tests the CreatePointFromLatLonAtts() function.
TEST_F(ConvenienceTest, TestCreatePointFromLatLonAtts) {
  const char* atts[] = { "lat", "38.38", "lon", "-121.456", NULL };
  PointPtr point = CreatePointFromLatLonAtts(atts);
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  ASSERT_EQ(static_cast<size_t>(1),
                       point->get_coordinates()->get_coordinates_array_size());
  ASSERT_EQ(
    strtod(atts[1], NULL),
    point->get_coordinates()->get_coordinates_array_at(0).get_latitude());
  ASSERT_EQ(
    strtod(atts[3], NULL),
    point->get_coordinates()->get_coordinates_array_at(0).get_longitude());
  ASSERT_EQ(0.0,
    point->get_coordinates()->get_coordinates_array_at(0).get_altitude());
}

// This tests the CreatePointFromVec3() function.
TEST_F(ConvenienceTest, TestCreatePointFromVec3) {
  kmlbase::Vec3 vec3(1.1, 2.2, 3.3);
  PointPtr point = CreatePointFromVec3(vec3);
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  ASSERT_EQ(static_cast<size_t>(1),
            point->get_coordinates()->get_coordinates_array_size());
  ASSERT_EQ(
    2.2, point->get_coordinates()->get_coordinates_array_at(0).get_latitude());
  ASSERT_EQ(
    1.1, point->get_coordinates()->get_coordinates_array_at(0).get_longitude());
  ASSERT_EQ(
    3.3, point->get_coordinates()->get_coordinates_array_at(0).get_altitude());
}

// This tests the CreatePointLatLon() function.
TEST_F(ConvenienceTest, TestCreatePointLatLon) {
  PointPtr point = CreatePointLatLon(-42.42, 150.160);
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  ASSERT_EQ(static_cast<size_t>(1),
                       point->get_coordinates()->get_coordinates_array_size());
  ASSERT_EQ(
    -42.42,
    point->get_coordinates()->get_coordinates_array_at(0).get_latitude());
  ASSERT_EQ(
    150.160,
    point->get_coordinates()->get_coordinates_array_at(0).get_longitude());
  ASSERT_EQ(
    0.0, point->get_coordinates()->get_coordinates_array_at(0).get_altitude());
}

// This tests the CreatePointPlacemark() function.
TEST_F(ConvenienceTest, TestCreatePointPlacemark) {
  const std::string kName("my point placemark");
  const double kLat = 38.0987123;
  const double kLon = -123.123;
  PlacemarkPtr placemark = CreatePointPlacemark(kName, kLat, kLon);
  ASSERT_EQ(kName, placemark->get_name());
  ASSERT_TRUE(placemark->has_geometry());
  PointPtr point = kmldom::AsPoint(placemark->get_geometry());
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  CoordinatesPtr coordinates = point->get_coordinates();
  ASSERT_EQ(static_cast<size_t>(1),
                       coordinates->get_coordinates_array_size());
  kmldom::Vec3 vec = coordinates->get_coordinates_array_at(0);
  ASSERT_EQ(kLat, vec.get_latitude());
  ASSERT_EQ(kLon, vec.get_longitude());
}

// This tests the PointPlacemarkWithTimeStamp() function.
TEST_F(ConvenienceTest, TestCreatePointPlacemarkWithTimeStamp) {
  const std::string kWhen("2008-10-03T09:25:42Z");
  PointPtr point = KmlFactory::GetFactory()->CreatePoint();
  boost::scoped_ptr<DateTime> date_time(DateTime::Create(kWhen));
  const char* style_id("my-shared-style");
  PlacemarkPtr placemark = CreatePointPlacemarkWithTimeStamp(point, *date_time,
                                                             style_id);
  ASSERT_TRUE(placemark);
  ASSERT_TRUE(placemark->has_geometry());
  ASSERT_TRUE(AsPoint(placemark->get_geometry()));
  ASSERT_TRUE(placemark->has_timeprimitive());
  ASSERT_TRUE(AsTimeStamp(placemark->get_timeprimitive()));
  ASSERT_TRUE(placemark->has_styleurl());
}

// This tests the CreateRegion2d() function.
TEST_F(ConvenienceTest, TestCreateRegion2d) {
  const double kNorth(67.87);
  const double kSouth(-56.78);
  const double kEast(98.12);
  const double kWest(12.34);
  const double kMinlodpixels(128);
  const double kMaxlodpixels(512);
  RegionPtr region = CreateRegion2d(kNorth, kSouth, kEast, kWest,
                                    kMinlodpixels, kMaxlodpixels);
  ASSERT_TRUE(region->has_latlonaltbox());
  ASSERT_TRUE(region->has_lod());
  ASSERT_EQ(kNorth, region->get_latlonaltbox()->get_north());
  ASSERT_EQ(kSouth, region->get_latlonaltbox()->get_south());
  ASSERT_EQ(kEast, region->get_latlonaltbox()->get_east());
  ASSERT_EQ(kWest, region->get_latlonaltbox()->get_west());
  ASSERT_FALSE(region->get_latlonaltbox()->has_minaltitude());
  ASSERT_FALSE(region->get_latlonaltbox()->has_maxaltitude());
  ASSERT_FALSE(region->get_latlonaltbox()->has_altitudemode());
  ASSERT_EQ(kMinlodpixels, region->get_lod()->get_minlodpixels());
  ASSERT_EQ(kMaxlodpixels, region->get_lod()->get_maxlodpixels());
  ASSERT_FALSE(region->get_lod()->has_minfadeextent());
  ASSERT_FALSE(region->get_lod()->has_maxfadeextent());
}

// This tests the GetExtendedDataValue() function.
TEST_F(ConvenienceTest, TestGetExtendedDataValue) {
  const std::string kName("yardage");
  const std::string kValue("0");
  PlacemarkPtr placemark = CreatePointPlacemark("19", 38, -122);
  AddExtendedDataValue(kName, kValue, placemark);
  std::string value;
  ASSERT_TRUE(GetExtendedDataValue(placemark, kName, &value));
  ASSERT_EQ(kValue, value);
  const std::string kNoSuch("no-such-name");
  ASSERT_FALSE(GetExtendedDataValue(placemark, kNoSuch, &value));
}

// This tests the SetExtendedDataValue() function.
TEST_F(ConvenienceTest, TestSetExtendedDataValue) {
  const std::string kName0("name0");
  const std::string kValue0("value0");
  const std::string kName1("name1");
  const std::string kValue1("value1");
  PlacemarkPtr placemark = CreatePointPlacemark("19", 38, -122);
  SetExtendedDataValue(kName0, kValue0, placemark);
  std::string value;
  ASSERT_TRUE(GetExtendedDataValue(placemark, kName0, &value));
  ASSERT_EQ(kValue0, value);
  SetExtendedDataValue(kName1, kValue1, placemark);
  ASSERT_FALSE(GetExtendedDataValue(placemark, kName0, &value));
  ASSERT_TRUE(GetExtendedDataValue(placemark, kName1, &value));
  ASSERT_EQ(kValue1, value);
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
