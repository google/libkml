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

// This file contains the declaration of the GoogleMapsData class.
// Very distantly related to:
// http://code.google.com/p/gdata-cpp-util/source/browse/trunk/gdata/client/
//     doc_list_service.h
// TODO: this interface and implemenation are under construction, expect
// additions and changes.

#ifndef KML_CONVENIENCE_GOOGLE_MAPS_DATA_H_
#define KML_CONVENIENCE_GOOGLE_MAPS_DATA_H_

#include <string>
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"

namespace kmlconvenience {

class HttpClient;

// This class provides an API to the Google Maps Data API.
// See the "Google Maps Data API HTTP Protocol Guide":
// http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html
// Overall usage:
//   class YourHttpClient : public kmlconvenience::HttpClient {
//    public:
//     virtual bool SendRequest(...) const {
//       ...your network I/O code goes here...
//     };
//   };
//   YourHttpClient* your_http_client = new YourHttpClient;
//   your_http_client->Login("user@gmail.com", "users-password");
//   GoogleMapsData* maps_data = GoogleMapsData::Create("your-app", "",
//                                                      &your_http_client);
//   std::string map_feed_atom;
//   maps_data->GetMapFeed(&map_feed_atom);
//   kmldom::ElementPtr root = kmldom::ParseAtom(map_feed_atom);
//   kmlconvenience::AtomUtil... for common Atom inspection.
class GoogleMapsData {
 public:
  // Create a GoogleMapsData object and login with the given email and password.
  // The scope is the hostname and port; use an empty string to use the default
  // of Google's Maps Data API hostname and port.  The GoogleMapsData object
  // uses the HttpClient class to handle all network communication.  The
  // HttpClient must be created and logged in before passing it to Create.
  static GoogleMapsData* Create(const std::string& scope,
                                HttpClient* http_client);
  ~GoogleMapsData();

  const char* get_map_feed_uri() const;

  const std::string& get_scope() const {
    return scope_;
  }

  // This returns the "meta feed" for the authenticated user.  The result is an
  // Atom <feed> containing an <entry> for each of the user's maps.  See:
  // http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#RetrievingMetafeed
  bool GetMetaFeedXml(std::string* atom_feed) const;

  // This calls GetMetaFeedXml and returns the parsed result.
  kmldom::AtomFeedPtr GetMetaFeed() const;

  // This creates a new map with the given title and summary.  This is simply
  // an HTTP POST to the user's maps meta feed.  On success true is returned.
  // If a map_entry_xml string is supplied the <atom:entry> for the new map
  // is saved there.  See:
  // http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#CreatingMaps
  bool CreateMap(const std::string& title, const std::string& summary,
                 std::string* map_entry_xml);

  // TODO:
  // http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#UpdatingMaps

  // TODO:
  // http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#DeletingMaps

  // This returns the URI of the Feature Feed of the given map.
  // "A map's feature feed is published in the map's <content> tag within its
  // src attibute." See:
  // http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#RetrievingMaps
  static bool GetFeatureFeedUri(const kmldom::AtomEntryPtr& map_entry,
                                std::string* feature_feed_uri);

  // This fetches the given URI and saves the result in the supplied string.
  // http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#RetrievingFeatures
  bool GetFeatureFeedXml(const std::string& feature_feed_uri,
                         std::string* atom_feed) const;

  // This calls GetFeatureFeedXml and returns the parsed result.
  kmldom::AtomFeedPtr GetFeatureFeedByUri(
      const std::string& feature_feed_uri) const;

  // Return the <entry> of the first map in the feed with the given title.
  // This returns NULL if no <entry>'s have this exact <title>.
  static kmldom::AtomEntryPtr FindEntryByTitle(
      const kmldom::AtomFeedPtr& meta_feed, const std::string& title);

  // Return the KML Feature child of the Atom <entry>'s <content>.  This
  // returns NULL if the <entry>'s <content> has no KML Feature.
  static kmldom::FeaturePtr GetEntryFeature(const kmldom::AtomEntryPtr& entry);

  // This appends the KML Feature in each of the feed's entry's to the
  // given container.  The number of KML Features appended is returned.
  // Each Feature added to the Container is a full clone from the feed entry.
  static int GetMapKml(const kmldom::AtomFeedPtr& feature_feed,
                       kmldom::ContainerPtr container);

  // Creates a <Document>, sets the <atom:link> and calls GetMapKml.
  static kmldom::DocumentPtr CreateDocumentOfMapFeatures(
      const kmldom::AtomFeedPtr& feature_feed);

  // This adds a new feature to a map.  This is simply an HTTP POST to the
  // given feature feed URI which can be retrieved from the map entry using
  // GetFeatureFeedUri.  On success true is returned.  If a feature_entry_xml
  // string is supplied the <atom:entry> for the new feature is saved there.
  // http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#CreatingFeatures
  bool AddFeature(const std::string& feature_feed_post_uri,
                  const kmldom::FeaturePtr& feature,
                  std::string* feature_entry_xml);

  // TODO:
  // http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#UpdatingFeatures

  // TODO:
  // http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#DeletingFeatures

 private:
  // Use static Create().
  GoogleMapsData(const std::string& maps_data_scope);
  boost::scoped_ptr<HttpClient> http_client_;
  const std::string scope_;
};

}  // end namespace kmlconvenience

#endif  // KML_CONVENIENCE_GOOGLE_MAPS_DATA_H_
