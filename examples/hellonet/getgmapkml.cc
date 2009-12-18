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

// This returns the KML of the specific Google My Maps.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "curlfetch.h"
#include "prompt.h"
#include "kml/base/file.h"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/convenience/atom_util.h"
#include "kml/convenience/http_client.h"
#include "kml/convenience/google_maps_data.h"

using kmlconvenience::GoogleMapsData;

int main(int argc, char** argv) {
  std::string user;
  std::string password;
  std::string map_title;
  std::string output;

  // Get the user logged in first.
  std::cout << "Email: ";
  std::cin >> user;
  password = Prompt::PromptAndInputWithNoEcho("Password: ");

  CurlHttpClient* curl_http_client = new CurlHttpClient(
      "libkml:examples:hellonet:getmapkml");
  if (!curl_http_client->Login(GoogleMapsData::get_service_name(), user,
                               password)) {
    std::cerr << "Login failed" << std::endl;
    return 1;
  }

  std::cout << "Login succeeded..." << std::endl;

  // Create a GoogleMapsData client from the logged in HttpClient and
  // get the user's list of maps.
  boost::scoped_ptr<GoogleMapsData> google_maps_data(
      GoogleMapsData::Create(curl_http_client));

again:
  kmldom::AtomFeedPtr meta_feed = google_maps_data->GetMetaFeed();
  if (!meta_feed.get()) {
    std::cerr << "GetMetaFeed failed" << std::endl;
    return 1;
  }

  std::cout << "You have " << meta_feed->get_entry_array_size() << " maps."
            << std::endl;

  // For each <entry> in the <feed>...
  for (size_t e = 0; e < meta_feed->get_entry_array_size(); ++e) {
    const kmldom::AtomEntryPtr& entry = meta_feed->get_entry_array_at(e);
    // Print the <title>:
    std::cout << "[title] " << entry->get_title() << std::endl;
    std::string feature_feed_uri;
    if (GoogleMapsData::GetFeatureFeedUri(entry, &feature_feed_uri)) {
      std::cout << " [feature feed] " << feature_feed_uri << std::endl;
    }
  }

  std::cout << "Map title: ";
  // Use std::getline here to capture the entire line.
  std::getline(std::cin, map_title);

  const kmldom::AtomEntryPtr& map_entry =
      kmlconvenience::AtomUtil::FindEntryByTitle(meta_feed, map_title);

  if (!map_entry.get()) {
    std::cout << "No map with title: " << map_title << std::endl;
    if (Prompt::PromptAgain("Try another title? ")) {
      goto again;
    }
    return 1;
  }

  std::string feature_feed_uri;
  if (!GoogleMapsData::GetFeatureFeedUri(map_entry, &feature_feed_uri)) {
    std::cout << "No Feature Feed for this map?" << std::endl;
    return 1;
  }

  // A map with this title was found.  Go fetch
  std::cout << "Feature Feed URI: " << feature_feed_uri << std::endl;
  const kmldom::AtomFeedPtr feature_feed =
      google_maps_data->GetFeatureFeedByUri(feature_feed_uri);

  std::cout << "There are " << feature_feed->get_entry_array_size()
            << " entries in this map." << std::endl;

  std::cout << "Output file: ";
  std::cin >> output;

  // Dig out the KML features in the feed and put them in a <Document>.
  kmldom::KmlPtr kml = kmldom::KmlFactory::GetFactory()->CreateKml();
  kmldom::ContainerPtr container =
      google_maps_data->CreateDocumentOfMapFeatures(feature_feed);
  kml->set_feature(container);

  std::cout << "There are " << container->get_feature_array_size()
            << " Features in the map saved to " << output << "." << std::endl;

  // Use KmlFile's serialize to get xml proper header and xmlns.
  boost::scoped_ptr<kmlengine::KmlFile> kml_file(
      kmlengine::KmlFile::CreateFromImport(kml));
  std::string xml;
  kml_file->SerializeToString(&xml);
  kmlbase::File::WriteStringToFile(xml, output.c_str());

  std::cin.get();
  if (Prompt::PromptAgain("Get KML for another map? ")) {
    goto again;
  }

  return 0;
}
