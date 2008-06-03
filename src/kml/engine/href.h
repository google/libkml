// Copyright 2008, Google Inc. All rights reserved.
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

// This file contains the definition of the Href class used to parse a
// KML-style URL such as that found in <href> and <styleUrl>.
// TODO: methods to parse/assemble a full RFC 1808 URL:
// TODO: <scheme>://<net_loc>/<path>;<params>?<query>#<fragment>
// TODO: "...;params..." is not used in KML

#ifndef KML_ENGINE_HREF_H__
#define KML_ENGINE_HREF_H__

#include <string>

namespace kmlengine {

// This class parses a KML URL.  Basic usage at present:
//   Href href("#object-id");
//   if (href.HasFragment()) {
//     return href.Fragment();  // "object-id"
//   }

class Href {
 public:
  Href() {}
  // Construct from the contents of <href>
  Href(const std::string& href) {
    // TODO: only does fragment at this time
    if (!href.empty() && href[0] == '#' && href.size() > 2) {
      fragment_ = href.substr(1);
    }
  }
  const std::string& get_fragment() {
    return fragment_;
  }
  bool has_fragment() {
    return !fragment_.empty();
  }
  void set_fragment(const std::string& fragment) {
    fragment_ = fragment;
  }
  void clear_fragment() {
    fragment_.clear();
  }

 private:
  // These names match RFC 1808:
  // <scheme>://<net_loc>/<path>;<params>?<query>#<fragment>
  // Note: params is unused in a KML URL.
  std::string scheme_;
  std::string net_loc_;
  std::string path_;
  std::string query_;
  std::string fragment_;
};

}  // end namespace kmlengine

#endif  // KML_ENGINE_HREF_H__
