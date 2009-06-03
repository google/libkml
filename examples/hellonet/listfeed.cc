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

// This prints every link and title in every entry in the given (Atom) feed.
// See RFC 4287.  The Atom file can either be a local file or a network URL.
// (There is no direct relationship to KML in this program.)

#include <iostream>
#include <string>
#include "curlfetch.h"
#include "kml/base/file.h"
#include "kml/dom.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " feed-file-or-url.atom" << std::endl;
    return 1;
  }

  const char* feed_url = argv[1];

  std::string feed_data;
  if (!kmlbase::File::ReadFileToString(feed_url, &feed_data) &&
      !CurlToString(feed_url, &feed_data)) {
    std::cerr << "read/fetch failed " << feed_url << std::endl;
    return 1;
  }

  // This program expects a <feed> root element.
  kmldom::AtomFeedPtr feed = kmldom::AsAtomFeed(
      kmldom::ParseAtom(feed_data, NULL));
  if (!feed.get()) {
    std::cerr << "parse failed " << feed_url << std::endl;
    return 1;
  }

  for (size_t e = 0; e < feed->get_entry_array_size(); ++e) {
    // For each <entry> in the feed...
    const kmldom::AtomEntryPtr& entry = feed->get_entry_array_at(e);
    // Print the <title>:
    std::cout << "[title] " << entry->get_title() << std::endl;
    for (size_t l = 0; l < entry->get_link_array_size(); ++l) {
      // For each <link> in the entry...
      const kmldom::AtomLinkPtr& link = entry->get_link_array_at(l);
      // Print the href=:
      std::cout << " [link] " << link->get_href() << std::endl;
    }
  }

  return 0;
}
