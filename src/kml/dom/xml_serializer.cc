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

// This file contains the implementation of the public XML serializing
// functions and the implementation of the XmlSerialize class.

#include "kml/dom/xml_serializer.h"
#include "kml/dom/kml_funcs.h"
#include <cstring>
#include <stack>
#include <sstream>
#include "kml/base/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;

namespace kmldom {

// This function is in the public API for converting the given Element
// hierarchy to "pretty" xml.
string SerializePretty(const ElementPtr& root) {
  if (!root) {
    return string("");
  }
  string xml;
  StringAdapter string_adapter(&xml);
  XmlSerializer<StringAdapter> serializer("\n", "  ", &string_adapter);
  root->Serialize(serializer);
  return xml;
}

// This function is in the public API for converting the given Element
// hierarchy to xml with no additional whitespace for newlines or
// indentation.
string SerializeRaw(const ElementPtr& root) {
  if (!root) {
    return string("");
  }
  string xml;
  StringAdapter string_adapter(&xml);
  XmlSerializer<StringAdapter> serializer("", "", &string_adapter);
  root->Serialize(serializer);
  return xml;
}

string GetElementName(const ElementPtr& element) {
  return element ?  Xsd::GetSchema()->ElementName(element->Type()) : string("");
}

}  // namespace kmldom
