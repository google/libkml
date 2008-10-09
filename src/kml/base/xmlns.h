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

#ifndef KML_BASE_XMLNS_H__
#define KML_BASE_XMLNS_H__

#include <map>
#include <string>
#include "kml/base/attributes.h"

namespace kmlbase {

// This class holds the default namespace and the set of prefix-namespace
// mappings for an XML file.  For example, if these attributes of the root
// element are used with this class's Create...
//  <schema xmlns="http://www.w3.org/2001/XMLSchema"
//          xmlns:kml="http://www.opengis.net/kml/2.2"
//          xmlns:atom="http://www.w3.org/2005/Atom"
//          xmlns:xal="urn:oasis:names:tc:ciq:xsdschema:xAL:2.0"
//          targetNamespace="http://www.opengis.net/kml/2.2"
//          elementFormDefault="qualified"
//          version="2.2.0">
// ...the get_default() will return "http://www.w3.org/2001/XMLSchema", and the
// following prefix-namespaces pairs will be returned by get_namespace:
//          kml="http://www.opengis.net/kml/2.2"
//          atom="http://www.w3.org/2005/Atom"
//          xal="urn:oasis:names:tc:ciq:xsdschema:xAL:2.0"
// If there are no "xmlns*" attribute names in the passed attributes Create()
// returns NULL.
class Xmlns {
 public:
  // The caller owns the created Xmlns object.
  static Xmlns* Create(const kmlbase::Attributes& attributes) {
    Xmlns* xmlns = new Xmlns;
    if (xmlns->Parse(attributes)) {
      return xmlns;
    }
    delete xmlns;
    return NULL;
  }

  const std::string& get_default() const {
    return default_;
  }

  const std::string GetNamespace(const std::string& prefix) const {
    StringStringMap::const_iterator iter = prefix_map_.begin();
    for (; iter != prefix_map_.end(); ++iter) {
      if (prefix == iter->first) {
        return iter->second;
      }
    }
    return "";  // Empty string.
  }

  const std::string GetKey(const std::string& value) const {
    StringStringMap::const_iterator iter = prefix_map_.begin();
    for (; iter != prefix_map_.end(); ++iter) {
      if (value == iter->second) {
        return iter->first;
      }
    }
    return "";  // Empty string.
  }

 private:
  Xmlns() {}
  bool Parse(const kmlbase::Attributes& attributes) {
    const std::string kXmlns("xmlns");
    attributes.MatchSplitKey(kXmlns + ":", &prefix_map_);
    // Return true if there is a default xmlns or if there are any
    // xmlns:prefx="ns" pairs.
    return attributes.GetString(kXmlns, &default_) || !prefix_map_.empty();
  }
  std::string default_;
  StringStringMap prefix_map_;
};

}  // end namespace kmlbase

#endif // KML_BASE_XMLNS_H__
