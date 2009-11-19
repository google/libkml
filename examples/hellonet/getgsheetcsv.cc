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

// This downloads the specified Google Spreadsheet as a CSV file.

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
#include "kml/convenience/google_doc_list.h"
#include "kml/convenience/google_spreadsheets.h"

using kmlconvenience::AtomUtil;
using kmlconvenience::GoogleDocList;
using kmlconvenience::GoogleSpreadsheets;
using kmldom::AtomEntryPtr;
using kmldom::AtomFeedPtr;
using kmldom::AsAtomEntry;
using kmldom::AsAtomFeed;
using kmldom::ParseAtom;

int main(int argc, char** argv) {
  std::string user;
  std::string password;
  std::string spreadsheet_title;
  std::string output;

  // Get the user logged in first.
  std::cout << "Email: ";
  std::cin >> user;
  password = Prompt::PromptAndInputWithNoEcho("Password: ");

  // Since GoogleSpreadsheets doesn't (yet?) provide a resource id we get that
  // from GoogleDocList.
  CurlHttpClient* doc_list_http_client = new CurlHttpClient(
      "libkml:examples:hellonet:getgsheetcsv");
  if (!doc_list_http_client->Login(GoogleDocList::get_service_name(), user,
                                   password)) {
    std::cerr << "Login failed" << std::endl;
    return 1;
  }
  boost::scoped_ptr<GoogleDocList> google_doc_list(
      GoogleDocList::Create(doc_list_http_client));

  // Login also to GoogleSpreadsheets for the actual CSV fetch.
  CurlHttpClient* spreadsheets_http_client = new CurlHttpClient(
      "libkml:examples:hellonet:getgsheetcsv");
  if (!spreadsheets_http_client->Login(GoogleSpreadsheets::get_service_name(),
                                       user, password)) {
    std::cerr << "Login failed" << std::endl;
    return 1;
  }
  boost::scoped_ptr<GoogleSpreadsheets> google_spreadsheets(
      GoogleSpreadsheets::Create(spreadsheets_http_client));

  std::cout << "Login succeeded..." << std::endl;

  kmldom::AtomFeedPtr meta_feed = google_doc_list->GetMetaFeed();
  if (!meta_feed.get()) {
    std::cerr << "GetMetaFeed failed" << std::endl;
    return 1;
  }

again:
  std::cout << "You have " << meta_feed->get_entry_array_size()
            << " docs:" << std::endl;

  for (size_t l = 0; l < meta_feed->get_entry_array_size(); ++l) {
    const kmldom::AtomEntryPtr& entry = meta_feed->get_entry_array_at(l);
    kmldom::AtomCategoryPtr category = AtomUtil::FindCategoryByScheme(*entry,
                                                                      "kind");
    std::string kind = (category && category->has_label())
                       ? category->get_label() : "[unknown kind]";
    std::cout << kind << ": " << entry->get_title() << std::endl;
  }

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

  string spreadsheet_data;
  if (!google_spreadsheets->DownloadSpreadsheet(spreadsheet_entry, "csv",
                                                &spreadsheet_data)) {
    std::cout << "Download failed: " << spreadsheet_title
              << std::endl;
    if (Prompt::PromptAgain("Try another title? ")) {
      goto again;
    }
    return 1;
  }

  std::cout << "Output file: ";
  std::cin >> output;

  kmlbase::File::WriteStringToFile(spreadsheet_data, output.c_str());

  std::cin.get();
  if (Prompt::PromptAgain("Get CSV for another spreadsheet? ")) {
    goto again;
  }

  return 0;
}
