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

// This creates KML from a suitable Google spreadsheet.  This looks exactly
// only for the "name", "latitude", and "longitude" rows and creates a
// <Point> <Placemark> for each one.

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
#include "kml/convenience/http_client.h"
#include "kml/convenience/google_spreadsheets.h"

using kmlconvenience::AtomUtil;
using kmldom::AtomEntryPtr;
using kmldom::AtomFeedPtr;
using kmldom::AsAtomEntry;
using kmldom::AsAtomFeed;
using kmldom::ParseAtom;

// Parse a string of this form: <namempace|key>val</namespace|key>.
static bool ParseGsxElement(const std::string& element, std::string* key,
                            std::string* val) {
  size_t pipe = element.find('|');
  if (pipe == std::string::npos) {
    return false;
  }
  size_t gt = element.find('>', pipe + 1);
  if (gt == std::string::npos) {
    return false;
  }
  size_t lt = element.find('<', gt + 1);
  if (lt == std::string::npos) {
    return false;
  }
  *key = element.substr(pipe + 1, gt - pipe - 1);
  *val = element.substr(gt + 1, lt - gt - 1);
  return true;
}

// Each Google Spreadsheets listfeed <entry> holds a spreadsheet row in a
// <gsx:COLUMN> element for each cell.
// For example:
// If the spreadsheet is...
// 0 | name       | longitude        | latitude          |
// 1 | croc hotel | 132.831455817379 | -12.6704922515546 |
// ...then this appears in the entry for the 1th row:
// <entry>
//   <gsx:name>croc hotel</gsx:name>
//   <gsx:longitude>132.831455817379</gsx:longitude>
//   <gsx:latitude>-12.6704922515546</gsx:latitude>
// <entry>
// See: http://code.google.com/apis/spreadsheets/data/3.0/developers_guide_protocol.html#ListFeeds
// TODO: create a more general form suitable for inclusion in the library itself
kmldom::PlacemarkPtr CreatePlacemarkFromEntry(
    const kmldom::AtomEntryPtr& entry) {
  std::string name;
  std::string longitude;
  std::string latitude;
  kmldom::ExtendedDataPtr extended_data =
      kmldom::KmlFactory::GetFactory()->CreateExtendedData();
  size_t usize = entry->get_unknown_elements_array_size();
  for (size_t u = 0; u < usize; ++u) {
    const std::string& us = entry->get_unknown_elements_array_at(u);
    std::string key;
    std::string val;
    if (ParseGsxElement(us, &key, &val)) {
      if (key == "name") {
        name = val;
      } else if (key == "longitude") {
        longitude = val;
      } else if (key == "latitude") {
        latitude = val;
      } else {
        extended_data->add_data(kmlconvenience::CreateDataNameValue(key, val));
      }
    }
  }
  if (!name.empty() && !longitude.empty() && !latitude.empty()) {
    kmldom::PlacemarkPtr placemark = kmlconvenience::CreatePointPlacemark(
        name, strtod(latitude.c_str(), NULL), strtod(longitude.c_str(), NULL));
    if (extended_data->get_data_array_size() > 0) {
      placemark->set_extendeddata(extended_data);
    }
    // TODO: set <atom:link> back to this spreadsheet cell.
    return placemark;
  }
  return NULL;
}

static AtomFeedPtr GetContentSrcFeed(CurlHttpClient* curl_http_client,
                                    const AtomEntryPtr& entry) {
  // worksheet feed = <content src="..."/> of meta feed entry
  std::string content_src_uri;
  if (AtomUtil::GetContentSrc(entry, &content_src_uri)) {
    std::string content_src_xml;
    if (curl_http_client->SendRequest(kmlconvenience::HTTP_GET,
                                      content_src_uri, NULL, NULL,
                                      &content_src_xml)) {
      return AsAtomFeed(ParseAtom(content_src_xml, NULL));
    }
  }
  return NULL;
}

int main(int argc, char** argv) {
  std::string user;
  std::string password;
  std::string spreadsheet_title;
  std::string output;

  // Get the user logged in first.
  std::cout << "Email: ";
  std::cin >> user;
  password = Prompt::PromptAndInputWithNoEcho("Password: ");

  CurlHttpClient* curl_http_client = new CurlHttpClient(
      "libkml:examples:hellonet:getgsheetkml");
  if (!curl_http_client->Login("wise", user, password)) {
    std::cerr << "Login failed" << std::endl;
    return 1;
  }

  std::cout << "Login succeeded..." << std::endl;

  // Create a GoogleMapsData client from the logged in HttpClient and
  // get the user's list of spreadsheets.
  boost::scoped_ptr<kmlconvenience::GoogleSpreadsheets> google_spreadsheets(
    kmlconvenience::GoogleSpreadsheets::Create(curl_http_client));
  kmldom::AtomFeedPtr meta_feed = google_spreadsheets->GetMetaFeed();
  if (!meta_feed.get()) {
    std::cerr << "GetMetaFeed failed" << std::endl;
    return 1;
  }

  std::cout << "You have " << meta_feed->get_entry_array_size()
            << " spreadsheets:" << std::endl;

  for (size_t l = 0; l < meta_feed->get_entry_array_size(); ++l) {
    const kmldom::AtomEntryPtr& entry = meta_feed->get_entry_array_at(l);
    std::cout << entry->get_title() << std::endl;
  }

again:
  std::cout << "Spreadsheet title: ";
  // Use std::getline here to capture the entire line.
  std::getline(std::cin, spreadsheet_title);

  const kmldom::AtomEntryPtr& spreadsheet_entry =
      kmlconvenience::AtomUtil::FindEntryByTitle(meta_feed, spreadsheet_title);

  if (!spreadsheet_entry.get()) {
    std::cout << "No spreadsheet with title: " << spreadsheet_title
              << std::endl;
    if (Prompt::PromptAgain("Try another title? ")) {
      goto again;
    }
    return 1;
  }

  const kmldom::AtomFeedPtr worksheet_feed =
      GetContentSrcFeed(curl_http_client, spreadsheet_entry);
  if (!worksheet_feed.get() || worksheet_feed->get_entry_array_size() < 1) {
    std::cerr << "Worksheet feed has no entry?" << std::endl;
    return 1;
  }

  // The 0th <entry>'s <content src="..."/> in the worksheet <feed> points to
  // the list feed.
  const kmldom::AtomFeedPtr list_feed =
      GetContentSrcFeed(curl_http_client,
                        worksheet_feed->get_entry_array_at(0));
  if (!list_feed.get() || list_feed->get_entry_array_size() < 1) {
    std::cerr << "No list feed?" << std::endl;
    return 1;
  }

  std::cout << "This spreadsheet has " << list_feed->get_entry_array_size()
            << " rows." << std::endl;

  kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
  kmldom::DocumentPtr document = kml_factory->CreateDocument();
  // TODO: set <atom:link> back to this spreadsheet.

  for (size_t l = 0; l < list_feed->get_entry_array_size(); ++l) {
    document->add_feature(CreatePlacemarkFromEntry(
        list_feed->get_entry_array_at(l)));
  }

  kmldom::KmlPtr kml = kml_factory->CreateKml();
  kml->set_feature(document);

  std::cout << "Output file: ";
  std::cin >> output;

  std::cout << "There are " << document->get_feature_array_size()
            << " Features in the KML saved to " << output << "." << std::endl;

  // Use KmlFile's serialize to get xml proper header and xmlns.
  boost::scoped_ptr<kmlengine::KmlFile> kml_file(
      kmlengine::KmlFile::CreateFromImport(kml));
  std::string xml;
  kml_file->SerializeToString(&xml);
  kmlbase::File::WriteStringToFile(xml, output.c_str());

  std::cin.get();
  if (Prompt::PromptAgain("Create KML for another spreadsheet? ")) {
    goto again;
  }

  return 0;
}
