// Copyright 2009, Google Inc. All rights reserved.
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

// This file contains the unit test for the GoogleMapsData class.

#include "kml/convenience/google_maps_data.h"

#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/convenience/http_client.h"

namespace kmlconvenience {

class GoogleMapsDataTest : public testing::Test {
 protected:
  void SetUp() {
  }
  boost::scoped_ptr<GoogleMapsData> google_maps_data_;
};

// This tests NULL use of the Create method.
TEST_F(GoogleMapsDataTest, TestNullCreate) {
  const std::string empty;
  // NULL/empty args?  Null return (no crash, no hang, no exception, etc).
  ASSERT_FALSE(GoogleMapsData::Create(empty, NULL));
}

// This tests basic use of the Create method.
TEST_F(GoogleMapsDataTest, TestBasicCreate) {
  const std::string empty;
  google_maps_data_.reset(
      GoogleMapsData::Create(empty, new HttpClient("TestBasicCreate")));
  // An HttpClient was supplied so a GoogleMapsData was created.
  ASSERT_TRUE(google_maps_data_.get());
}

// This tests that the GoogleMapsData instance destroys the HttpClient when
// the GoogleMapsData instance is destroyed.
TEST_F(GoogleMapsDataTest, VerifyGoogleMapsDataDestroysHttpClient) {
  // TODO:
}

// This tests basic normal use of the GetMapFeed() method.
TEST_F(GoogleMapsDataTest, TestGetMapFeed) {
  const std::string empty;
  HttpClient* http_client = new HttpClient("TestGetMapFeed");
  http_client->Login("local", "user@gmail.com", "user-password");
  google_maps_data_.reset(GoogleMapsData::Create(empty, http_client));
  ASSERT_TRUE(google_maps_data_.get());
  std::string map_feed;
  ASSERT_TRUE(google_maps_data_->GetMapFeed(&map_feed));
  size_t end_of_first_line = map_feed.find('\n');
  ASSERT_NE(std::string::npos, end_of_first_line);
  ASSERT_EQ(std::string("GET ") + google_maps_data_->get_scope() +
                google_maps_data_->get_map_feed_uri(),
            map_feed.substr(0, end_of_first_line));
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

