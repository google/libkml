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

// This file contains the declaration of the ExpatHandlerNs class.

#ifndef KML_BASE_EXPAT_HANDLER_NS_H__
#define KML_BASE_EXPAT_HANDLER_NS_H__

#include <string>
#include "kml/base/expat_handler.h"

namespace kmlbase {

class Xmlns;

// Interface to namespace-enabled expat parser.  Overall usages is as follows:
// Xmlns* xmlns = Xmlns::Create(xmlns-default-and-prefix-namespace-pairs);
// class YourExpatHandler : public ExpatHandler {
//  public:
//   StartElement(const char* your_prefixed_name, const char** atts) {
//     // your_prefixed_name is either:
//     //  1) "foo" if "foo" is in the default namespace of the Xmlns.
//     //  2) "yourprefix:goo" if "goo" is in a namespace for which there
//     //     is a mapping to "yourprefix" in the xmlns.
//     //  3) "whoknows:bar" if "bar" is in a namespace unknown to the Xmlns.
//   }
//   EndElement(const char* your_prefixed_name) {
//   }
//   CharData(const XML_Char* s, int len) {
//   }
// };
// YourExpatHandler your_expat_handler;
// ExpatHandlerNs expat_handler_ns(&your_expat_handler, xmlns);
// bool status = ExpatParser(xml_data, &expat_handler_ns, errors, true);
class ExpatHandlerNs : public ExpatHandler {
public:
  // The given ExpatHandler is a prefix-aware but namespace-unaware handler.
  // The Xmlns describes the prefixes implemented by the handler.  For example,
  // if the ExpatHandler implements "atom:name" then Xmlns should have a
  // mapping from the "atom" prefix to the atom namespace.  ExpatHandler's
  // unprefixed elements "Placemark", for example, are in the Xmlns's default
  // namespace.
  ExpatHandlerNs(ExpatHandler* expat_handler, const Xmlns* xmlns);
  virtual ~ExpatHandlerNs() {}
  // This translates an expat-generated namespace qualified name into a
  // name with a prefix known to the Xmlns passed to the constructor.
  const std::string TranslatePrefixedName(
      const std::string prefixed_name) const;
  virtual void StartElement(const char* namespaced_named, const char** atts);
  virtual void EndElement(const char* namespaced_name);
  virtual void CharData(const XML_Char* s, int len);
  virtual void StartNamespace(const XML_Char* prefix, const XML_Char* uri);
  virtual void EndNamespace(const XML_Char* prefix);

private:
  ExpatHandler* expat_handler_;
  const Xmlns* xmlns_;
};

}  // end namespace kmlbase

#endif  // KML_BASE_EXPAT_HANDLER_NS_H__
