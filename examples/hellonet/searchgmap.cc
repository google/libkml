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

// This returns the part of a My Map within the specified bounds.  This is
// interactive program which lists the users maps, asks for one by name,
// and returns the features in that map within a bounding box specified by
// the bounds of a KML file the user supplies.  Any KML file can be used to
// specify these bounds, for example one with a 2 vertex line will specify
// the smallest north-up bounding box around that line.

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

// This returns the bounding box of the given KML file.
static bool GetMapBounds(const std::string map_name, kmlengine::Bbox* bbox) {
  if (!kmlbase::StringEndsWith(map_name, ".kml")) {
    std::cerr << "Specify a KML file." << std::endl;
    return false;
  }  // TODO: else look for a My Map by this name
  std::string kml_data;
  if (!kmlbase::File::ReadFileToString(map_name, &kml_data)) {
    std::cerr << "Read failed: " << map_name << std::endl;
    return false;
  }
  kmlengine::KmlFilePtr kml_file(
      kmlengine::KmlFile::CreateFromParse(kml_data, NULL));
  if (!kml_file.get()) {
    std::cerr << "Parse failed: " << map_name << std::endl;
    return false;
  }
  kmldom::FeaturePtr root = kmlengine::GetRootFeature(kml_file->get_root());
  if (!root.get()) {
    std::cerr << "No root feature? " << map_name << std::endl;
    return false;
  }
  if (!kmlengine::GetFeatureBounds(root, bbox)) {
    std::cerr << "No location info? " << map_name << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char** argv) {
  if (argc > 1 ) {
    std::cerr << "Warning: this is an interactive program." << std::endl;
    std::cerr << "All command line args are ignored." << std::endl;
  }

  std::string user;
  std::string password;

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
  kmldom::AtomFeedPtr meta_feed = google_maps_data->GetMetaFeed();
  if (!meta_feed.get()) {
    std::cerr << "GetMetaFeed failed" << std::endl;
    return 1;
  }

again:
  std::cout << "You have " << meta_feed->get_entry_array_size() << " maps."
            << std::endl;

  for (size_t l = 0; l < meta_feed->get_entry_array_size(); ++l) {
    const kmldom::AtomEntryPtr& entry = meta_feed->get_entry_array_at(l);
    std::cout << entry->get_title() << std::endl;
  }

  std::cout << "Title of map to search: ";
  // Use std::getline here to capture the entire line.
  std::string map_title;
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

kml_again:
  std::cout << "Bounds KML: ";
  // Use std::getline here to capture the entire line.
  std::string bounds_map;
  std::getline(std::cin, bounds_map);

  kmlengine::Bbox bbox;
  if (!GetMapBounds(bounds_map, &bbox)) {
    goto kml_again;
  }

  const kmldom::AtomFeedPtr feature_feed = google_maps_data->SearchMapByBbox(
      map_entry, bbox);

  std::cout << "There are " << feature_feed->get_entry_array_size()
            << " features in this bounding box." << std::endl;

  std::cout << "Output file: ";
  std::string output;
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
