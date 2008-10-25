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

// This file contains the unit tests for the Vec3 class.

#include "kml/base/vec3.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"

namespace kmlbase {

class Vec3Test : public testing::Test {
};

TEST_F(Vec3Test, TestConstructEmpty) {
  Vec3 vec3;
  ASSERT_EQ(0.0, vec3.get_latitude());
  ASSERT_EQ(0.0, vec3.get_longitude());
  ASSERT_EQ(0.0, vec3.get_altitude());
  ASSERT_EQ(false, vec3.has_altitude());
}

TEST_F(Vec3Test, TestConstruct2d) {
  const double kLatitude(-12.12);
  const double kLongitude(23.46);
  Vec3 vec3(kLongitude, kLatitude);
  ASSERT_EQ(kLatitude, vec3.get_latitude());
  ASSERT_EQ(kLongitude, vec3.get_longitude());
  ASSERT_EQ(0.0, vec3.get_altitude());
  ASSERT_EQ(false, vec3.has_altitude());
}

TEST_F(Vec3Test, TestConstruct3d) {
  const double kLatitude(-12.12);
  const double kLongitude(23.46);
  const double kAltitude(54321.0987);
  Vec3 vec3(kLongitude, kLatitude, kAltitude);
  ASSERT_EQ(kLatitude, vec3.get_latitude());
  ASSERT_EQ(kLongitude, vec3.get_longitude());
  ASSERT_EQ(kAltitude, vec3.get_altitude());
  ASSERT_EQ(true, vec3.has_altitude());
}

TEST_F(Vec3Test, TestSetClearAltitude) {
  const double kAltitude(54321.0987);
  Vec3 vec3(1,2);
  vec3.set_altitude(kAltitude);
  ASSERT_EQ(kAltitude, vec3.get_altitude());
  ASSERT_EQ(true, vec3.has_altitude());
  vec3.clear_altitude();
  ASSERT_EQ(0.0, vec3.get_altitude());
  ASSERT_EQ(false, vec3.has_altitude());
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
