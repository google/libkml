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
#include "kml/convenience/atom_util.h"
#include "kml/convenience/convenience.h"
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

// This class simply returns the post_data as the response.
class EchoHttpClient : public HttpClient {
 public:
  EchoHttpClient()
    : HttpClient("EchoHttpClient") {
  }

  virtual bool SendRequest(HttpMethodEnum http_method,
                           const std::string& request_uri,
                           const StringPairVector* request_headers,
                           const std::string* post_data,
                           std::string* response) const {
    if (post_data && response) {
      *response = *post_data;
    }
    return true;
  }
};

// This HttpClient logs each request to the supplied vector.
struct HttpRequest {
  HttpMethodEnum http_method_;
  std::string request_uri_;
  StringPairVector request_headers_;
  std::string post_data_;
};
typedef std::vector<HttpRequest> HttpRequestVector;

class LoggingHttpClient : public HttpClient {
 public:
  LoggingHttpClient(HttpRequestVector* request_log)
    : HttpClient("LoggingHttpClient"),
      request_log_(request_log) {
  }

  virtual bool SendRequest(HttpMethodEnum http_method,
                           const std::string& request_uri,
                           const StringPairVector* request_headers,
                           const std::string* post_data,
                           std::string* response) const {
    HttpRequest http_request;
    http_request.http_method_ = http_method;
    http_request.request_uri_ = request_uri;
    if (request_headers) {
      http_request.request_headers_ = *request_headers;
    }
    if (post_data) {
      http_request.post_data_ = *post_data;
    }
    request_log_->push_back(http_request);
    return true;
  }

 private:
  HttpRequestVector* request_log_;
};

// This tests NULL use of the Create method.
TEST_F(GoogleMapsDataTest, TestNullCreate) {
  // NULL/empty args?  Null return (no crash, no hang, no exception, etc).
  ASSERT_FALSE(GoogleMapsData::Create(NULL));
}

// This tests basic use of the Create method and the get_scope() method.
TEST_F(GoogleMapsDataTest, TestBasicCreate) {
  const std::string kScope = "http://host.com:123";
  ASSERT_EQ(0, setenv("GOOGLE_MAPS_DATA_SCOPE", kScope.c_str(), 1));
  google_maps_data_.reset(
      GoogleMapsData::Create(new HttpClient("TestBasicCreate")));
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
  HttpClient* http_client = new HttpClient("TestGetMetaFeedXml");
  http_client->Login("local", "user@gmail.com", "user-password");
  google_maps_data_.reset(GoogleMapsData::Create(http_client));
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
  google_maps_data_.reset(GoogleMapsData::Create(new TestDataHttpClient));
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
  google_maps_data_.reset(
      GoogleMapsData::Create(new HttpClient("TestHttpClient")));
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
  google_maps_data_.reset(
      GoogleMapsData::Create(new FakeAtomFeedHttpClient()));
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

TEST_F(GoogleMapsDataTest, TestCreateMap) {
  google_maps_data_.reset(GoogleMapsData::Create(new EchoHttpClient));
  ASSERT_TRUE(google_maps_data_.get());
  const std::string kTitle("The Girl With the Dragon Tattoo");
  const std::string kSummary("Wildly suspenseful... an intelligent thriller");
  std::string map_entry_xml;
  ASSERT_TRUE(google_maps_data_->CreateMap(kTitle, kSummary, &map_entry_xml));
  const kmldom::AtomEntryPtr entry =
      kmldom::AsAtomEntry(kmldom::ParseAtom(map_entry_xml, NULL));
  ASSERT_TRUE(entry.get());
  ASSERT_TRUE(entry->has_title());
  ASSERT_EQ(kTitle, entry->get_title());
  ASSERT_TRUE(entry->has_summary());
  ASSERT_EQ(kSummary, entry->get_summary());
}

TEST_F(GoogleMapsDataTest, TestAddFeature) {
  google_maps_data_.reset(GoogleMapsData::Create(new EchoHttpClient));
  ASSERT_TRUE(google_maps_data_.get());
  const std::string kName("Stieg Larsson");
  const std::string kDescription("At once a murder mystery, love story and...");
  const double kLat(38.38);
  const double kLon(101.101);
  kmldom::PlacemarkPtr placemark =
      kmlconvenience::CreatePointPlacemark(kName, kLat, kLon);
  placemark->set_description(kDescription);
  std::string feature_entry_xml;
  ASSERT_TRUE(google_maps_data_->AddFeature("", placemark, &feature_entry_xml));
  const kmldom::AtomEntryPtr entry =
      kmldom::AsAtomEntry(kmldom::ParseAtom(feature_entry_xml, NULL));
  ASSERT_TRUE(entry.get());
  ASSERT_TRUE(entry->has_title());
  ASSERT_EQ(kName, entry->get_title());
  ASSERT_TRUE(entry->has_summary());
  ASSERT_EQ(kDescription, entry->get_summary());
  const kmldom::AtomContentPtr content = entry->get_content();
  ASSERT_TRUE(content.get());
  ASSERT_EQ(static_cast<size_t>(1),
            content->get_misplaced_elements_array_size());
  kmldom::PlacemarkPtr got_placemark = kmldom::AsPlacemark(
      content->get_misplaced_elements_array_at(0));
  ASSERT_EQ(kName, got_placemark->get_name());
  ASSERT_EQ(kDescription, got_placemark->get_description());
  double got_lat, got_lon;
  kmlengine::GetFeatureLatLon(placemark, &got_lat, &got_lon);
  ASSERT_EQ(kLat, got_lat);
  ASSERT_EQ(kLon, got_lon);
}

TEST_F(GoogleMapsDataTest, TestPostPlacemarksOnAFolder) {
  HttpRequestVector request_log;
  google_maps_data_.reset(
      GoogleMapsData::Create(new LoggingHttpClient(&request_log)));
  // PostPlacemarks() ignores things that aren't <Placemarks>
  kmldom::FolderPtr folder = kmldom::KmlFactory::GetFactory()->CreateFolder();
  ASSERT_TRUE(request_log.empty());
  ASSERT_EQ(0, google_maps_data_->PostPlacemarks(folder, "http://foo.com/"));
}

TEST_F(GoogleMapsDataTest, TestPostPlacemarksOnOnePlacemark) {
  HttpRequestVector request_log;
  google_maps_data_.reset(
      GoogleMapsData::Create(new LoggingHttpClient(&request_log)));
  const std::string kFeatureFeedUri("http://host.com/anything/will/do");
  const std::string kName("Stieg Larsson");
  const std::string kDescription("At once a murder mystery, love story and...");
  const double kLat(38.38);
  const double kLon(101.101);
  kmldom::PlacemarkPtr placemark =
      kmlconvenience::CreatePointPlacemark(kName, kLat, kLon);
  ASSERT_EQ(1, google_maps_data_->PostPlacemarks(placemark, kFeatureFeedUri));
  ASSERT_EQ(static_cast<size_t>(1), request_log.size());
  ASSERT_EQ(HTTP_POST, request_log[0].http_method_);
  ASSERT_EQ(kFeatureFeedUri, request_log[0].request_uri_);
}

TEST_F(GoogleMapsDataTest, TestPostPlacemarksOnKmlSamples) {
  HttpRequestVector request_log;
  google_maps_data_.reset(
      GoogleMapsData::Create(new LoggingHttpClient(&request_log)));

  // Read and parse the kmlsamples.kml file.
  std::string kml_samples;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(
      std::string(DATADIR) + "/kml/kmlsamples.kml", &kml_samples));
  kmlengine::KmlFilePtr kml_file(
      kmlengine::KmlFile::CreateFromString(kml_samples));
  kmldom::FeaturePtr root_feature =
      kmlengine::GetRootFeature(kml_file->get_root());

  // Make up some feature feed uri.
  const std::string kFeatureFeedUri("http://host.com/anything/will/do");

  // Call the method under test.
  // There are 20 <Placemark>'s in the file, but one has no Geometry.
  ASSERT_EQ(19, google_maps_data_->PostPlacemarks(root_feature,
                                                  kFeatureFeedUri));
  // Verify there was a request for eqch placemark.
  ASSERT_EQ(static_cast<size_t>(19), request_log.size());
  for (size_t i = 0; i < request_log.size(); ++i) {
    // Every request is a POST...
    ASSERT_EQ(HTTP_POST, request_log[i].http_method_);
    // ... to the given uri...
    ASSERT_EQ(kFeatureFeedUri, request_log[i].request_uri_);
    // ... of an <atom:entry>...
    const kmldom::AtomEntryPtr entry =
        kmldom::AsAtomEntry(kmldom::ParseAtom(request_log[i].post_data_, NULL));
    ASSERT_TRUE(entry.get());
    // ... whose content is a <Placemark>.
    const kmldom::FeaturePtr feature = AtomUtil::GetEntryFeature(entry);
    ASSERT_TRUE(feature.get());
    ASSERT_EQ(kmldom::Type_Placemark, feature->Type());
  }
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
