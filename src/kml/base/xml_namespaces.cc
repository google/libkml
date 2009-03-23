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

#include "kml/base/xml_namespaces.h"

namespace kmlbase {

// TODO: how/if to handle http://earth.google.com/kml/2.[12]...
const XmlNamespace XmlNamespaces[] = {
  { XMLNS_NONE, NULL, NULL },
  { XMLNS_ATOM, "atom", "http://www.w3.org/2005/Atom" },
  { XMLNS_GPX, "gpx", "http://www.topografix.com/GPX/1/0" },
  { XMLNS_GX22, "gx", "http://www.google.com/kml/ext/2.2" },
  { XMLNS_KML22, "kml", "http://www.opengis.net/kml/2.2" },
  { XMLNS_XAL, "xal", "urn:oasis:names:tc:ciq:xsdschema:xAL:2.0" },
  { XMLNS_XML, "xml", "http://www.w3.org/XML/1998/namespace" },
  { XMLNS_XSD, "xsd", "http://www.w3.org/2001/XMLSchema" },
  { XMLNS_XSI, "xsi", "http://www.w3.org/2001/XMLSchema-instance" }
};

bool FindXmlNamespaceAndPrefix(XmlnsId xmlns_id,
                               std::string* prefix,
                               std::string* xml_namespace) {
  if (xmlns_id == XMLNS_NONE) {
    return false;
  }
  const size_t num_namespaces = sizeof(XmlNamespaces)/sizeof(XmlNamespaces[0]);
  for (size_t i = 0; i < num_namespaces; ++i) {
    if (XmlNamespaces[i].xmlns_id_ == xmlns_id) {
      if (prefix) {
        *prefix = XmlNamespaces[i].prefix_;
      }
      if (xml_namespace) {
        *xml_namespace = XmlNamespaces[i].xml_namespace_;
      }
      return true;
    }
  }
  return false;
}

}  // end namespace kmlbase
