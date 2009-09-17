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

#include <iostream>
#include <string>
#include <vector>
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

bool GoogleMapsData::GetMapFeed(std::string* atom_feed) const {
  return http_client_->SendRequest(HTTP_GET, scope_ + kMapFeedUri, NULL, NULL,
                                   atom_feed);
}

}  // end namespace kmlconvenience
