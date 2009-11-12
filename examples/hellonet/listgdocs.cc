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
#include <string>
#include "boost/scoped_ptr.hpp"
#include "curlfetch.h"
#include "prompt.h"
#include "kml/base/file.h"
#include "kml/convenience/atom_util.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/http_client.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/google_doc_list.h"

using kmlconvenience::AtomUtil;
using kmlconvenience::GoogleDocList;
using kmldom::AtomEntryPtr;
using kmldom::AtomFeedPtr;
using kmldom::AsAtomEntry;
using kmldom::AsAtomFeed;
using kmldom::ParseAtom;

int main(int argc, char** argv) {
  std::string user;
  std::string password;
  std::string doc_title;

  // Get the user logged in first.
  std::cout << "Email: ";
  std::cin >> user;
  password = Prompt::PromptAndInputWithNoEcho("Password: ");

  CurlHttpClient* curl_http_client = new CurlHttpClient(
      "libkml:examples:hellonet:getgdoc");
  if (!curl_http_client->Login(GoogleDocList::get_service_name(), user,
                               password)) {
    std::cerr << "Login failed" << std::endl;
    return 1;
  }

  std::cout << "Login succeeded..." << std::endl;

  // Create a GoogleDocList client from the logged in HttpClient and
  // get the user's list of Google docs.
  boost::scoped_ptr<kmlconvenience::GoogleDocList> doc_list(
    kmlconvenience::GoogleDocList::Create(curl_http_client));

  kmldom::AtomFeedPtr meta_feed = doc_list->GetMetaFeed();
  if (!meta_feed.get()) {
    std::cerr << "GetMetaFeed failed" << std::endl;
    return 1;
  }

  std::cout << "You have " << meta_feed->get_entry_array_size()
            << " docs:" << std::endl;

  for (size_t l = 0; l < meta_feed->get_entry_array_size(); ++l) {
    const kmldom::AtomEntryPtr& entry = meta_feed->get_entry_array_at(l);
    // TODO: implement FindCategoryByScheme
#if 0
    kmldom::AtomCategoryPtr category = AtomUtil::FindCategoryByScheme(*entry,
                                                                      "kind");
    std::string kind = (category && category->has_label())
                       ? category->get_label() : "[unknown kind]";
    std::cout << kind << ": " << entry->get_title() << std::endl;
#else
    std::cout << entry->get_title() << std::endl;
#endif
  }

  return 0;
}
