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

TEST(AtomUtilTest, TestIsOfLinkRel) {
  kmldom::AtomLinkPtr link = kmldom::KmlFactory::GetFactory()->CreateAtomLink();
  // A link with no rel= at all.
  ASSERT_FALSE(AtomUtil::LinkIsOfRel(link, "post"));
  link->set_rel("post-is-not-at-the-end");
  ASSERT_FALSE(AtomUtil::LinkIsOfRel(link, "post"));
  link->set_rel("http://foo.com/goo/blah#post");
  ASSERT_TRUE(AtomUtil::LinkIsOfRel(link, "post"));
  link->set_rel("post");
  ASSERT_TRUE(AtomUtil::LinkIsOfRel(link, "post"));
  std::string empty;
  ASSERT_FALSE(AtomUtil::LinkIsOfRel(NULL, empty));
}

TEST(AtomUtilTest, TestFindLink) {
  kmldom::AtomFeedPtr feed = kmldom::KmlFactory::GetFactory()->CreateAtomFeed();
  const std::string kEmpty;
  // Empty/NULL everything just returns NULL w/o crashing.
  ASSERT_FALSE(AtomUtil::FindLink(*feed, kEmpty, kEmpty));
  const std::string kRel("alternate");
  const std::string kMimeType("text/html");
  // NULL AtomFeePtr just returns NULL w/o crashing.
  ASSERT_FALSE(AtomUtil::FindLink(*feed, kRel, kMimeType));
  kmldom::AtomLinkPtr link = kmldom::KmlFactory::GetFactory()->CreateAtomLink();
  feed->add_link(link);
  link->set_rel(kRel);
  // Have rel=, but not type=.
  ASSERT_FALSE(AtomUtil::FindLink(*feed, kRel, kMimeType));
  link->clear_rel();
  link->set_type(kMimeType);
  // Have type=, but not rel=.
  ASSERT_FALSE(AtomUtil::FindLink(*feed, kRel, kMimeType));
  link->set_rel(kRel);
  // Have both rel= and type=.
  kmldom::AtomLinkPtr got_link = AtomUtil::FindLink(*feed, kRel, kMimeType);
  ASSERT_TRUE(got_link.get());
  ASSERT_EQ(kRel, got_link->get_rel());
  ASSERT_EQ(kMimeType, got_link->get_type());
  kmldom::AtomEntryPtr entry =
      kmldom::KmlFactory::GetFactory()->CreateAtomEntry();
  ASSERT_FALSE(AtomUtil::FindLink(*entry, kRel, kMimeType));
  link = kmldom::KmlFactory::GetFactory()->CreateAtomLink();
  entry->add_link(link);
  link->set_rel(kRel);
  link->set_type(kMimeType);
  got_link = AtomUtil::FindLink(*feed, kRel, kMimeType);
  ASSERT_TRUE(got_link.get());
  ASSERT_EQ(kRel, got_link->get_rel());
  ASSERT_EQ(kMimeType, got_link->get_type());
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
