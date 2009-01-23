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

// This file contains the unit tests for the mathematical functions.

#include "kml/base/math_util.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"

namespace kmlbase {

TEST(BaseMathTest, TestLatLngOnRadialFromPoint) {
  // See http://williams.best.vwh.net/avform.htm#Example
  // Coordinates of LAX.
  const double kLAX_lat = 33.944066;
  const double kLAX_lng = -118.408294;
  // A distance and radial.
  const double kDistance = 185200.0;
  const double kRadial = 66.0;
  // The known-accurate coordinates of the point along that radial from LAX.
  const double kRadial_lat = 34.608154;
  const double kRadial_lng = -116.558327;

  Vec3 v = LatLngOnRadialFromPoint(kLAX_lat, kLAX_lng, kDistance, kRadial);
  ASSERT_NEAR(kRadial_lat, v.get_latitude(), 0.000001);
  ASSERT_NEAR(kRadial_lng, v.get_longitude(), 0.000001);
}

TEST(BaseMathTest, TestDistanceBetweenPoints) {
  const double kLAX_lat = 33.944066;
  const double kLAX_lng = -118.408294;
  const double kJFK_lat = 40.642480;
  const double kJFK_lng = -73.788071;
  // The known great circle distance in meters between LAX and JFK.
  const double kDistance = 3970683.0;
  double d = DistanceBetweenPoints(kLAX_lat, kLAX_lng, kJFK_lat, kJFK_lng);
  ASSERT_NEAR(kDistance, d, 0.1);
}

// Tese test the conversion functions.
TEST(BaseMathTest, TestDegToRad) {
  ASSERT_DOUBLE_EQ(0.0, DegToRad(0.0));
  ASSERT_DOUBLE_EQ(M_PI, DegToRad(180.0));
  ASSERT_DOUBLE_EQ(M_PI / 2, DegToRad(90.0));
  ASSERT_DOUBLE_EQ(M_PI / -2, DegToRad(-90.0));
}

TEST(BaseMathTest, TestRadToDeg) {
  ASSERT_DOUBLE_EQ(0.0, RadToDeg(0));
  ASSERT_DOUBLE_EQ(360.0, RadToDeg(2 * M_PI));
  ASSERT_DOUBLE_EQ(90.0, RadToDeg(M_PI / 2));
  ASSERT_DOUBLE_EQ(-90.0, RadToDeg(M_PI / -2));
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
