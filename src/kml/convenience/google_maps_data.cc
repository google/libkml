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

// This file contains the implementation of the GoogleMapsData class.

#include "kml/convenience/google_maps_data.h"

#include <iostream>
#include <string>
#include <vector>
#include "kml/convenience/atom_util.h"
#include "kml/convenience/http_client.h"
#include "kml/dom.h"
#include "kml/engine.h"

namespace kmlconvenience {

static const char* kScope = "http://maps.google.com";

static const char* kMapFeedUri = "/maps/feeds/maps/default/full";

// static
GoogleMapsData* GoogleMapsData::Create(const std::string& scope,
                                       HttpClient* http_client) {
  GoogleMapsData* mds = new GoogleMapsData(scope.empty() ? kScope : scope);
  // The HttpClient must exist.
  if (http_client) {
    mds->http_client_.reset(http_client);
    return mds;
  }
  delete http_client;
  delete mds;
  return NULL;
}

GoogleMapsData::GoogleMapsData(const std::string& scope_)
  : scope_(scope_) {
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
kmldom::AtomEntryPtr GoogleMapsData::FindEntryByTitle(
    const kmldom::AtomFeedPtr& meta_feed, const std::string& title) {
  for (size_t e = 0; e < meta_feed->get_entry_array_size(); ++e) {
    const kmldom::AtomEntryPtr& entry = meta_feed->get_entry_array_at(e);
    if (entry->get_title() == title) {
      return entry;
    }
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

}  // end namespace kmlconvenience
