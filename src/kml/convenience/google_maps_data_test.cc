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

#include <iostream>

// This file contains the unit test for the GoogleMapsData class.

#include "kml/convenience/google_maps_data.h"

#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/file.h"
#include "kml/convenience/http_client.h"
#include "kml/engine.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

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

// This tests basic use of the Create method and the get_scope() method.
TEST_F(GoogleMapsDataTest, TestBasicCreate) {
  const std::string kScope("http://host.com:123");
  google_maps_data_.reset(
      GoogleMapsData::Create(kScope, new HttpClient("TestBasicCreate")));
  // An HttpClient was supplied so a GoogleMapsData was created.
  ASSERT_TRUE(google_maps_data_.get());
  ASSERT_EQ(kScope, google_maps_data_->get_scope());
}

// This tests that the GoogleMapsData instance destroys the HttpClient when
// the GoogleMapsData instance is destroyed.
TEST_F(GoogleMapsDataTest, VerifyGoogleMapsDataDestroysHttpClient) {
  // TODO:
}

// This tests basic normal use of the GetMetaFeedXml() method.
TEST_F(GoogleMapsDataTest, TestGetMetaFeedXml) {
  const std::string empty;
  HttpClient* http_client = new HttpClient("TestGetMetaFeedXml");
  http_client->Login("local", "user@gmail.com", "user-password");
  google_maps_data_.reset(GoogleMapsData::Create(empty, http_client));
  ASSERT_TRUE(google_maps_data_.get());
  std::string map_feed;
  ASSERT_TRUE(google_maps_data_->GetMetaFeedXml(&map_feed));
  size_t end_of_first_line = map_feed.find('\n');
  ASSERT_NE(std::string::npos, end_of_first_line);
  ASSERT_EQ(std::string("GET ") + google_maps_data_->get_scope() +
                google_maps_data_->get_map_feed_uri(),
            map_feed.substr(0, end_of_first_line));
}

TEST_F(GoogleMapsDataTest, TestGetMetaFeed) {
  // This HttpClient always "fetches" testdata/gmaps/metafeed.xml.
  class TestDataHttpClient : public HttpClient {
   public:
    TestDataHttpClient()
      : HttpClient("MetaFeedHttpClient") {
    }
  
    virtual bool SendRequest(HttpMethodEnum http_method,
                             const std::string& request_uri,
                             const StringPairVector* request_headers,
                             const std::string* post_data,
                             std::string* response) const {
      return kmlbase::File::ReadFileToString(
          std::string(DATADIR) + "/gmaps/metafeed.xml", response);
    }
  };
  google_maps_data_.reset(GoogleMapsData::Create("", new TestDataHttpClient));
  ASSERT_TRUE(google_maps_data_.get());
  // Call the method under test.
  kmldom::AtomFeedPtr atom_feed = google_maps_data_->GetMetaFeed();
  ASSERT_TRUE(atom_feed.get());
  // Check a few things known to be in metafeed.xml:
  ASSERT_EQ(std::string("My maps"), atom_feed->get_title());
  ASSERT_EQ(static_cast<size_t>(5), atom_feed->get_link_array_size());
  ASSERT_EQ(static_cast<size_t>(3), atom_feed->get_entry_array_size());
  ASSERT_EQ(std::string("alaska"),
            atom_feed->get_entry_array_at(0)->get_title());
  ASSERT_EQ(std::string("canada"),
            atom_feed->get_entry_array_at(1)->get_title());
  ASSERT_EQ(std::string("this is the title"),
            atom_feed->get_entry_array_at(2)->get_title());
}

TEST_F(GoogleMapsDataTest, TestGetFeatureFeedUri) {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::AtomContentPtr content = factory->CreateAtomContent();
  const std::string kSrc("http://hello.com/how/are/you");
  content->set_src(kSrc);
  kmldom::AtomEntryPtr entry = factory->CreateAtomEntry();
  entry->set_content(content);
  std::string feature_feed_uri;
  ASSERT_TRUE(GoogleMapsData::GetFeatureFeedUri(entry, &feature_feed_uri));
  ASSERT_EQ(kSrc, feature_feed_uri);
}

TEST_F(GoogleMapsDataTest, TestFindEntryByTitle) {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::AtomEntryPtr entry = factory->CreateAtomEntry();
  const std::string kTitle("War and Peace");
  entry->set_title(kTitle);
  kmldom::AtomFeedPtr feed = factory->CreateAtomFeed();
  feed->add_entry(entry);
  kmldom::AtomEntryPtr got_entry = GoogleMapsData::FindEntryByTitle(feed,
                                                                    kTitle);
  ASSERT_TRUE(got_entry.get());
  ASSERT_EQ(kTitle, got_entry->get_title());
}

TEST_F(GoogleMapsDataTest, TestCreateDocumentOfMapFeatures) {
  std::string feature_feed_xml;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(
      std::string(DATADIR) + "/gmaps/feature-feed.xml", &feature_feed_xml));
  const kmldom::AtomFeedPtr feature_feed =
     AsAtomFeed(kmldom::ParseAtom(feature_feed_xml, NULL));
  ASSERT_TRUE(feature_feed.get());
  const kmldom::ContainerPtr container =
      GoogleMapsData::CreateDocumentOfMapFeatures(feature_feed);
  ASSERT_TRUE(container.get());
  ASSERT_EQ(static_cast<size_t>(4), container->get_feature_array_size());
  // entry 0 is a Point Placemark with IconStyle.
  kmldom::PlacemarkPtr placemark =
      kmldom::AsPlacemark(container->get_feature_array_at(0));
  ASSERT_TRUE(placemark.get());
  ASSERT_EQ(std::string("Montana"), placemark->get_name());
  kmldom::PointPtr point = kmldom::AsPoint(placemark->get_geometry());
  ASSERT_TRUE(point.get());
  kmldom::StylePtr style = kmldom::AsStyle(placemark->get_styleselector());
  ASSERT_TRUE(style->has_iconstyle());
  // entry 1 is LineString Placemark with LineStyle
  placemark = kmldom::AsPlacemark(container->get_feature_array_at(1));
  ASSERT_TRUE(placemark.get());
  kmldom::LineStringPtr linestring =
      kmldom::AsLineString(placemark->get_geometry());
  ASSERT_TRUE(linestring.get());
  style = kmldom::AsStyle(placemark->get_styleselector());
  ASSERT_TRUE(style->has_linestyle());
  // entry 2 is LineString Placemark with LineStyle
  placemark = kmldom::AsPlacemark(container->get_feature_array_at(2));
  ASSERT_TRUE(placemark.get());
  ASSERT_EQ(std::string("I90 from Missoula to Butte"), placemark->get_name());
  linestring = kmldom::AsLineString(placemark->get_geometry());
  ASSERT_TRUE(linestring.get());
  style = kmldom::AsStyle(placemark->get_styleselector());
  ASSERT_TRUE(style->has_linestyle());
  // entry 3 is Polygon Placemark with LineStyle and PolyStyle
  placemark = kmldom::AsPlacemark(container->get_feature_array_at(3));
  ASSERT_TRUE(placemark.get());
  ASSERT_EQ(std::string("Glacier National Park"), placemark->get_name());
  kmldom::PolygonPtr polygon = kmldom::AsPolygon(placemark->get_geometry());
  ASSERT_TRUE(polygon.get());
  style = kmldom::AsStyle(placemark->get_styleselector());
  ASSERT_TRUE(style->has_linestyle());
  ASSERT_TRUE(style->has_polystyle());
}

TEST_F(GoogleMapsDataTest, TestGetFeatureFeedXml) {
  const std::string empty;
  google_maps_data_.reset(
      GoogleMapsData::Create(empty, new HttpClient("TestHttpClient")));
  const std::string kFeatureFeedUri = "http://host.com/a/b/c";
  std::string http_response;
  ASSERT_TRUE(google_maps_data_->GetFeatureFeedXml(kFeatureFeedUri,
                                                   &http_response));
  // This test verifies only that GetFeatureFeedXml makes a call on
  // the installed HttpClient to GET the provided URL.
  kmlbase::StringVector lines;
  kmlbase::SplitStringUsing(http_response, "\n", &lines);
  ASSERT_EQ(std::string("GET ") + kFeatureFeedUri, lines[0]);
}

TEST_F(GoogleMapsDataTest, TestGetFeatureFeedByUri) {
  // This HttpClient always returns an Atom <feed> in the http response.
  // The requested URI is set in a <link> child of the feed.
  class FakeAtomFeedHttpClient : public HttpClient {
   public:
    FakeAtomFeedHttpClient()
      : HttpClient("FakeAtomFeedHttpClient") {
    }
  
    virtual bool SendRequest(HttpMethodEnum http_method,
                             const std::string& request_uri,
                             const StringPairVector* request_headers,
                             const std::string* post_data,
                             std::string* response) const {
      kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
      kmldom::AtomLinkPtr link = factory->CreateAtomLink();
      link->set_href(request_uri);
      kmldom::AtomFeedPtr feed = factory->CreateAtomFeed();
      feed->add_link(link);
      // Use kmlengine::KmlFile to emit xmlns'd xml.
      kmlengine::KmlFilePtr kml_file(
        kmlengine::KmlFile::CreateFromImport(feed));
      kml_file->SerializeToString(response);
      return true;
    }
  };
  const std::string empty;
  google_maps_data_.reset(
      GoogleMapsData::Create(empty, new FakeAtomFeedHttpClient()));
  const std::string kFeatureFeedUri = "http://host.com/a/b/c";
  // Call the method under test.  This is nearly a pure I/O method so we check
  // here that it makes the expected I/O request: an HTTP GET of the URI.
  const kmldom::AtomFeedPtr feed =
      google_maps_data_->GetFeatureFeedByUri(kFeatureFeedUri);
  ASSERT_TRUE(feed.get());
  ASSERT_EQ(static_cast<size_t>(1), feed->get_link_array_size());
  const kmldom::AtomLinkPtr link = feed->get_link_array_at(0);
  ASSERT_TRUE(link->has_href());
  ASSERT_EQ(kFeatureFeedUri, link->get_href());
}

static kmldom::AtomEntryPtr CreatePlacemarkEntry(const std::string& name) {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_name(name);
  kmldom::AtomContentPtr content = factory->CreateAtomContent();
  // Cast <atom:entry> to Element to gain access to the AddElement() method.
  kmldom::AsElement(content)->AddElement(placemark);
  kmldom::AtomEntryPtr entry = factory->CreateAtomEntry();
  entry->set_content(content);
  return entry;
}

TEST_F(GoogleMapsDataTest, TestGetEntryFeature) {
  const std::string kName("my name");
  kmldom::FeaturePtr feature =
    GoogleMapsData::GetEntryFeature(CreatePlacemarkEntry(kName));
  ASSERT_TRUE(feature.get());
  kmldom::PlacemarkPtr placemark = kmldom::AsPlacemark(feature);
  ASSERT_TRUE(placemark.get());
  ASSERT_EQ(kName, placemark->get_name());
}

TEST_F(GoogleMapsDataTest, TestGetMapKml) {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::AtomFeedPtr feed = factory->CreateAtomFeed();
  const std::string kName0("the name of the 0th feature");
  feed->add_entry(CreatePlacemarkEntry(kName0));
  const std::string kName1("the name of the 1th feature");
  feed->add_entry(CreatePlacemarkEntry(kName1));
  const std::string kName2("the name of the 2th feature");
  feed->add_entry(CreatePlacemarkEntry(kName2));
  // Create a <Folder> to save to.
  kmldom::FolderPtr folder = factory->CreateFolder();
  // Call the method under test.
  ASSERT_EQ(3, GoogleMapsData::GetMapKml(feed, folder));
  ASSERT_EQ(static_cast<size_t>(3), folder->get_feature_array_size());
  kmldom::PlacemarkPtr got_placemark = kmldom::AsPlacemark(
      folder->get_feature_array_at(0));
  ASSERT_TRUE(got_placemark.get());
  ASSERT_EQ(kName0, got_placemark->get_name());
  got_placemark = kmldom::AsPlacemark(folder->get_feature_array_at(1));
  ASSERT_TRUE(got_placemark.get());
  ASSERT_EQ(kName1, got_placemark->get_name());
  got_placemark = kmldom::AsPlacemark(folder->get_feature_array_at(2));
  ASSERT_TRUE(got_placemark.get());
  ASSERT_EQ(kName2, got_placemark->get_name());
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

