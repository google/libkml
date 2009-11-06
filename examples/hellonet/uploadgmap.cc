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

// This creates a new Google My Map from the given KML file using the Google
// Maps Data API.  The URI of feed to the new map is printed on output.
// The created map has only the Placemarks from the original file.

#include <termios.h>
#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "curlfetch.h"
#include "prompt.h"
#include "kml/base/file.h"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/convenience/atom_util.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/google_maps_data.h"
#include "kml/convenience/http_client.h"

using kmlconvenience::GoogleMapsData;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " file.kml" << std::endl;
    return 1;
  }

  std::string kml_data;
  if (!kmlbase::File::ReadFileToString(argv[1], &kml_data)) {
    std::cerr << "Read failed: " << argv[1] << std::endl;
    return 1;
  }

  // Parse the KML and inline all StyleSelectors.
  const kmldom::FeaturePtr root_feature =
      kmlengine::GetRootFeature(kmlengine::InlineStyles(kml_data, NULL));
  if (!root_feature.get()) {
    std::cerr << "No root feature?" << std::endl;
    return -1;
  }

  std::string user;
  std::string password;

  std::cout << "Email: ";
  std::cin >> user;
  password = Prompt::PromptAndInputWithNoEcho("Password: ");

  CurlHttpClient* curl_http_client =
      new CurlHttpClient("libkml:examples:hellonet::uploadgmap");
  if (!curl_http_client->Login(GoogleMapsData::get_service_name(), user,
                               password)) {
    std::cerr << "Login failed" << std::endl;
    return 1;
  }

  boost::scoped_ptr<GoogleMapsData> google_maps_data(
      GoogleMapsData::Create(curl_http_client));

  std::string map_entry_xml;
  google_maps_data->CreateMap(root_feature->get_name(),
                              root_feature->get_description(), &map_entry_xml);

  kmldom::AtomEntryPtr map_entry =
      kmldom::AsAtomEntry(kmldom::ParseAtom(map_entry_xml, NULL));
  if (!map_entry.get()) {
    // A parse failure usually means a non-xml error string was returned as
    // the response.
    std::cerr << "CreateMap failed: " << map_entry_xml << std::endl;
    return 1;
  }

  // Print the map feed URI now because Google Maps Data API does not show
  // maps with no features in the list of maps.
  // TODO: delete the map if no features were added?
  std::string map_feed_uri;
  kmlconvenience::AtomUtil::FindRelUrl(*map_entry, "self", &map_feed_uri);
  std::cout << "Map feed URI: " << map_feed_uri << std::endl;

  std::string feature_feed_uri;
  if (!GoogleMapsData::GetFeatureFeedUri(map_entry, &feature_feed_uri)) {
    std::cerr << "No Feature feed URI?" << std::endl;
    return 1;
  }

  int placemark_count = google_maps_data->PostPlacemarks(root_feature,
                                                         feature_feed_uri);
  std::cout << "Uploaded " << placemark_count << " placemarks." << std::endl;

  return 0;
}
