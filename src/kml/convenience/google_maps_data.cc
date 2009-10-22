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

// This file contains the implementation of the GoogleMapsData class.

#include "kml/convenience/google_maps_data.h"

#include <string>
#include <vector>
#include "kml/base/mimetypes.h"
#include "kml/convenience/atom_util.h"
#include "kml/convenience/http_client.h"
#include "kml/dom.h"
#include "kml/engine.h"

namespace kmlconvenience {

static const char* kScope = "http://maps.google.com";

static const char* kMapFeedUri = "/maps/feeds/maps/default/full";

// static
GoogleMapsData* GoogleMapsData::Create(HttpClient* http_client) {
  GoogleMapsData* mds = new GoogleMapsData;
  // The HttpClient must exist.
  if (http_client) {
    http_client->AddHeader("GData-Version", "2.0");
    mds->http_client_.reset(http_client);
    return mds;
  }
  delete http_client;
  delete mds;
  return NULL;
}

static std::string GetScope() {
  if (const char* scope = getenv("GOOGLE_MAPS_DATA_SCOPE")) {
    return scope;
  }
  return kScope;
}

GoogleMapsData::GoogleMapsData()
  : scope_(GetScope()) {
}

// Keep POI of scoped_ptr<GoogleHttpClient>'s dtor out of .h
GoogleMapsData::~GoogleMapsData() {
}

const char* GoogleMapsData::get_map_feed_uri() const {
  return kMapFeedUri;
}

bool GoogleMapsData::GetMetaFeedXml(std::string* atom_feed) const {
  return http_client_->SendRequest(HTTP_GET, scope_ + kMapFeedUri, NULL, NULL,
                                   atom_feed);
}

kmldom::AtomFeedPtr GoogleMapsData::GetMetaFeed() const {
  std::string meta_feed;
  if (GetMetaFeedXml(&meta_feed)) {
    return kmldom::AsAtomFeed(kmldom::ParseAtom(meta_feed, NULL));
  }
  return NULL;
}

// static
bool GoogleMapsData::GetFeatureFeedUri(const kmldom::AtomEntryPtr& map_entry,
                                       std::string* feature_feed_uri) {
  if (map_entry.get() && map_entry->has_content()) {
    const kmldom::AtomContentPtr& content = map_entry->get_content();
    if (content->has_src()) {
      if (feature_feed_uri) {
        *feature_feed_uri = content->get_src();
      }
      return true;
    }
  }
  return false;
}

bool GoogleMapsData::GetFeatureFeedXml(const std::string& feature_feed_uri,
                                       std::string* atom_feed) const {
  return http_client_->SendRequest(HTTP_GET, feature_feed_uri, NULL, NULL,
                                   atom_feed);
}

kmldom::AtomFeedPtr GoogleMapsData::GetFeatureFeedByUri(
    const std::string& feature_feed_uri) const {
  std::string feature_feed;
  if (GetFeatureFeedXml(feature_feed_uri, &feature_feed)) {
    return kmldom::AsAtomFeed(kmldom::ParseAtom(feature_feed, NULL));
  }
  return NULL;
}

// static
kmldom::FeaturePtr GoogleMapsData::GetEntryFeature(
    const kmldom::AtomEntryPtr& entry) {
  if (entry.get() && entry->has_content() &&
      entry->get_content()->get_misplaced_elements_array_size() > 0) {
    return kmldom::AsFeature(
        entry->get_content()->get_misplaced_elements_array_at(0));
  }
  return NULL;
}

// static
int GoogleMapsData::GetMapKml(const kmldom::AtomFeedPtr& feature_feed,
                              kmldom::ContainerPtr container) {
  if (!container.get() || !feature_feed.get()) {
    return -1;  // Not much to do w/o both a feature feed and container.
  }
  int feature_count = 0;
  // TODO: set container's <atom:link> to the feature feed?
  // A Google My Maps Feature Feed has one KML Feature in each <entry>.
  for (size_t i = 0; i < feature_feed->get_entry_array_size(); ++i) {
    // TODO: set <atom:link> in the cloned Feature?
    const kmldom::FeaturePtr feature =
        GetEntryFeature(feature_feed->get_entry_array_at(i));
    if (feature.get()) {
      ++feature_count;
      // Must clone because libkml strictly prevents any element from having
      // more than one parent element.
      container->add_feature(kmldom::AsFeature(kmlengine::Clone(feature)));
    }
  }
  return feature_count;
}

// static
kmldom::DocumentPtr GoogleMapsData::CreateDocumentOfMapFeatures(
    const kmldom::AtomFeedPtr& feature_feed) {
  if (!feature_feed.get()) {
    return NULL;
  }
  kmldom::DocumentPtr document =
      kmldom::KmlFactory::GetFactory()->CreateDocument();
  // TODO: set <atom:link>
  GetMapKml(feature_feed, document);
  return document;
}

bool GoogleMapsData::CreateMap(const std::string& title,
                               const std::string& summary,
                               std::string* entry) {
  // Create the <atom:entry> for the new map.
  kmlengine::KmlFilePtr kml_file = kmlengine::KmlFile::CreateFromImport(
      AtomUtil::CreateBasicEntry(title, summary));
  if (!kml_file.get()) {
    return NULL;
  }

  // Get the Atom in XML form.
  std::string post_data;
  kml_file->SerializeToString(&post_data);

  // Indicate that we're posting XML.
  StringPairVector headers;
  HttpClient::PushHeader("Content-Type", kmlbase::kAtomMimeType, &headers);

  // Send off the HTTP POST and save the result to the user supplied buffer.
  return http_client_->SendRequest(HTTP_POST, scope_ + kMapFeedUri, &headers,
                                   &post_data, entry);
}

bool GoogleMapsData::AddFeature(const std::string& feature_feed_post_uri,
                                const kmldom::FeaturePtr& feature,
                                std::string* feature_entry_xml) {
  // Create an <atom:content> to hold the Feature.
  kmldom::AtomContentPtr content =
      kmldom::KmlFactory::GetFactory()->CreateAtomContent();
  content->AddElement(feature);
  content->set_type(kmlbase::kKmlMimeType);

  // Create an <atom:entry> to hold the <atom:content>.  Set the <atom:title>
  // from the Feature's <name> and <atom:description> from the Feature's
  // <description>.
  kmldom::AtomEntryPtr entry =
      AtomUtil::CreateBasicEntry(feature->get_name(),
                                 feature->get_description());
  entry->set_content(content);

  // Get the Atom in XML form.  Use KmlFile's serializer to get proper xmlns
  // headers.
  kmlengine::KmlFilePtr kml_file =
      kmlengine::KmlFile::CreateFromImport(entry);
  std::string post_data;
  kml_file->SerializeToString(&post_data);

  // Indicate that we're posting XML.
  StringPairVector headers;
  HttpClient::PushHeader("Content-Type", kmlbase::kAtomMimeType, &headers);

  // Send off the HTTP POST and save the result to the user supplied buffer.
  return http_client_->SendRequest(HTTP_POST, feature_feed_post_uri,
                                   &headers, &post_data, feature_entry_xml);
}

int GoogleMapsData::PostPlacemarks(const kmldom::FeaturePtr& root_feature,
                                   const std::string& feature_feed_uri) {
  // Because GetElementsById looks only _below_ the root feature.
  if (kmldom::Type_Placemark == root_feature->Type()) {
    return AddFeature(feature_feed_uri, root_feature, NULL) ? 1 : 0;
  }
 
  // Dig out all <Placemarks>.  Everything else is ignored, essentially
  // flattening Container hierarchies.
  kmlengine::ElementVector placemarks;
  kmlengine::GetElementsById(root_feature, kmldom::Type_Placemark, &placemarks);

  int placemark_count = 0;
  for (size_t i = 0; i < placemarks.size(); ++i) {
    const kmldom::PlacemarkPtr placemark = kmldom::AsPlacemark(placemarks[i]);
    if (placemark->has_geometry()) {
      if (AddFeature(feature_feed_uri, placemark, NULL)) {
        ++placemark_count;
      }
    }
  }
  return placemark_count;
}

}  // end namespace kmlconvenience
