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

// This file implements the KML ParseNS() function.  The parser uses expat in
// its namespace-aware mode.  See kml_handler.cc for more details.

#include "kml/dom/kml_handler_ns.h"
#include "kml/base/string_util.h"
#include "kml/dom/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/parser_observer.h"
#include "kml/dom/xsd.h"

namespace kmldom {

// The uri:local_name separator used in expat's ParserCreateNS.
static const char kXmlnsSeparator[] = "|";

KmlHandlerNS::KmlHandlerNS(parser_observer_vector_t& observers)
  : KmlHandler(observers) {
}

KmlHandlerNS::~KmlHandlerNS() {
}

void KmlHandlerNS::StartElement(const char *name, const char **atts) {
  std::string uri, localname;
  SplitPrefixUriString(name, &uri, &localname);
  KmlHandler::StartElement(localname.c_str(), atts);
}

void KmlHandlerNS::EndElement(const char *name) {
  std::string uri, localname;
  SplitPrefixUriString(name, &uri, &localname);
  KmlHandler::EndElement(localname.c_str());
}

void KmlHandlerNS::CharData(const XML_Char *s, int len) {
  KmlHandler::CharData(s, len);
}

void KmlHandlerNS::StartNamespace(const XML_Char *prefix, const XML_Char *uri) {
  // TODO: save the uri:prefix mappings.
  // TODO: enforce the rule that the ONLY default namespace is KML's?
  // TODO: enforce that there must be a 1:1 uri:prefix mapping per file?
}

void KmlHandlerNS::EndNamespace(const XML_Char *prefix) {
}

// Private.
// TODO: optimize.
void KmlHandlerNS::SplitPrefixUriString(const std::string& str,
                                        std::string* uri,
                                        std::string* localname) {
  if (str.find(kXmlnsSeparator) != std::string::npos) {
    std::vector<std::string> string_vec;
    kmlbase::SplitStringUsing(str, kXmlnsSeparator, &string_vec);
    assert(string_vec.size() == 2);
    if (uri) {
      *uri = string_vec.at(0);
    }
    if (localname) {
      *localname = string_vec.at(1);
    }
  } else {  // No namespace separator found.
    *localname = str;
  }
}

}  // end namespace kmldom
