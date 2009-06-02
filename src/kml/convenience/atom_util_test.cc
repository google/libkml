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

// This file contains the unit test for the AtomUtil API functions.

#include "kml/convenience/atom_util.h"
#include "gtest/gtest.h"
#include "kml/convenience/convenience.h"
#include "kml/dom.h"
#include "kml/engine/location_util.h"

namespace kmlconvenience {

TEST(AtomUtilTest, TestCreateBasicEntry) {
  const std::string kTitle("War And Peace");
  const std::string kSummary("First there was war, then there was peace");
  const kmldom::AtomEntryPtr entry =
      AtomUtil::CreateBasicEntry(kTitle, kSummary);
  ASSERT_TRUE(entry.get());
  ASSERT_EQ(kTitle, entry->get_title());
  ASSERT_EQ(kSummary, entry->get_summary());
}

TEST(AtomUtilTest, TestCreateAtomEntryForFeature) {
  const std::string kName("a place");
  const std::string kDescription("This is a really great place");
  const double kLat(12.12);
  const double kLon(-38.38);
  kmldom::PlacemarkPtr placemark = CreatePointPlacemark(kName, kLat, kLon);
  placemark->set_description(kDescription);
  const kmldom::AtomEntryPtr entry = AtomUtil::CreateEntryForFeature(placemark);
  ASSERT_TRUE(entry.get());
  ASSERT_EQ(kName, entry->get_title());
  ASSERT_EQ(kDescription, entry->get_summary());
  ASSERT_TRUE(entry->has_content());
  const kmldom::AtomContentPtr content = entry->get_content();
  ASSERT_TRUE(content.get());
  ASSERT_EQ(static_cast<size_t>(1),
            content->get_misplaced_elements_array_size());
  placemark = kmldom::AsPlacemark(content->get_misplaced_elements_array_at(0));
  ASSERT_TRUE(placemark.get());
  double lat;
  double lon;
  ASSERT_TRUE(kmlengine::GetFeatureLatLon(placemark, &lat, &lon));
  ASSERT_EQ(kLat, lat);
  ASSERT_EQ(kLon, lon);
  ASSERT_EQ(kName, placemark->get_name());
  ASSERT_EQ(kDescription, placemark->get_description());
}

TEST(AtomUtilTest, TestBasicGetContentSrc) {
  kmldom::AtomContentPtr content =
      kmldom::KmlFactory::GetFactory()->CreateAtomContent();
  const std::string kSrc("http://somehost.com/some/path/feed/blah");
  content->set_src(kSrc);
  kmldom::AtomEntryPtr entry =
      kmldom::KmlFactory::GetFactory()->CreateAtomEntry();
  entry->set_content(content);
  std::string src;
  ASSERT_TRUE(AtomUtil::GetContentSrc(entry, &src));
  ASSERT_EQ(kSrc, src);
}

TEST(AtomUtilTest, TestFindRelUrl) {
  kmldom::AtomFeedPtr feed = kmldom::KmlFactory::GetFactory()->CreateAtomFeed();
  const std::string kRelType("fish");
  ASSERT_FALSE(AtomUtil::FindRelUrl(*feed.get(), kRelType, NULL));
  std::string href;
  ASSERT_FALSE(AtomUtil::FindRelUrl(*feed.get(), kRelType, &href));
}

TEST(AtomUtilTest, TestCloneEntryFeature) {
  const std::string kName("a place");
  const double kLat(12.12);
  const double kLon(-38.38);
  const kmldom::PlacemarkPtr placemark =
      CreatePointPlacemark(kName, kLat, kLon);
  kmldom::AtomEntryPtr entry = AtomUtil::CreateEntryForFeature(placemark);
  kmldom::FeaturePtr feature = AtomUtil::GetEntryFeature(entry);
  ASSERT_TRUE(feature.get());
}

TEST(AtomUtilTest, TestGetEntryFeature) {
  kmldom::AtomEntryPtr entry;
  kmldom::FeaturePtr feature = AtomUtil::GetEntryFeature(entry);
}

TEST(AtomUtilTest, TestGetFeedFeatures) {
  kmldom::AtomFeedPtr feed;
  kmldom::FolderPtr folder;
  AtomUtil::GetFeedFeatures(feed, folder);
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

