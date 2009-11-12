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
#include "kml/convenience/google_doc_list.h"
#include "kml/convenience/http_client.h"

using kmlconvenience::GoogleDocList;

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " file.csv" << std::endl;
    return 1;
  }

  std::string spreadsheet_data;
  if (!kmlbase::File::ReadFileToString(argv[1], &spreadsheet_data)) {
    std::cerr << "Read failed: " << argv[1] << std::endl;
    return 1;
  }

  std::string user;
  std::string password;

  std::cout << "Email: ";
  std::cin >> user;
  password = Prompt::PromptAndInputWithNoEcho("Password: ");

  // Yes, you use Doc List API to upload a spreadsheet.
  CurlHttpClient* curl_http_client =
      new CurlHttpClient("libkml:examples:hellonet::uploadgsheet");
  if (!curl_http_client->Login(GoogleDocList::get_service_name(), user,
                               password)) {
    std::cerr << "Login failed" << std::endl;
    return 1;
  }

  boost::scoped_ptr<GoogleDocList> google_doc_list(
      GoogleDocList::Create(curl_http_client));

  // TODO: add a Slug: header to set the title.
  std::string doc_entry_xml;
  if (!google_doc_list->UploadSpreadsheet(spreadsheet_data, "text/csv",
                                         &doc_entry_xml)) {
    std::cerr << "UploadSpreadsheet failed: " << doc_entry_xml << std::endl;
    return 1;
  }

  kmldom::AtomEntryPtr map_entry =
      kmldom::AsAtomEntry(kmldom::ParseAtom(doc_entry_xml, NULL));
  if (!map_entry.get()) {
    // A parse failure usually means a non-xml error string was returned as
    // the response.
    std::cerr << "CreateMap failed: " << doc_entry_xml << std::endl;
    return 1;
  }

  std::string doc_feed_uri;
  kmlconvenience::AtomUtil::FindRelUrl(*map_entry, "self", &doc_feed_uri);
  std::cout << "Upload succeeded.  Doc feed URI: " << doc_feed_uri
            << std::endl;

  return 0;
}
