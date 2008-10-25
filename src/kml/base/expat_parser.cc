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

#include "kml/base/expat_parser.h"
#include <sstream>
#include "expat.h"
#include "kml/base/expat_handler.h"

namespace kmlbase {

static void XMLCALL
startElement(void *userData, const char *name, const char **atts) {
  ((ExpatHandler*)userData)->StartElement(name, atts);
}

static void XMLCALL
endElement(void *userData, const char *name) {
  ((ExpatHandler*)userData)->EndElement(name);
}

static void XMLCALL
charData(void *userData, const XML_Char *s, int len) {
  ((ExpatHandler*)userData)->CharData(s, len);
}

static void XMLCALL
startNamespace(void *userData, const XML_Char *prefix, const XML_Char *uri) {
  ((ExpatHandler*)userData)->StartNamespace(prefix, uri);
}

static void XMLCALL
endNamespace(void *userData, const XML_Char *prefix) {
  ((ExpatHandler*)userData)->EndNamespace(prefix);
}

bool ExpatParser(const std::string& xml, ExpatHandler* expat_handler,
                 std::string* errors, bool namespace_aware) {
  XML_Parser parser =
    namespace_aware ? XML_ParserCreateNS(NULL, kExpatNsSeparator)
                    : XML_ParserCreate(NULL);
  expat_handler->set_parser(parser);
  XML_SetUserData(parser, expat_handler);
  XML_SetElementHandler(parser, startElement, endElement);
  XML_SetCharacterDataHandler(parser, charData);
  if (namespace_aware) {
    XML_SetNamespaceDeclHandler(parser, startNamespace, endNamespace);
  }
  int xml_size = static_cast<int>(xml.size());
  XML_Status status = XML_Parse(parser, xml.c_str(), xml_size, xml_size);
  if (status != XML_STATUS_OK && errors) {
    // This is the other half of XML_StopParser() which is our way of
    // stopping expat if the root element is not KML.
    if (status == XML_STATUS_SUSPENDED) {
      *errors = "Invalid root element";
    } else {
      std::stringstream strstream;
      strstream << XML_ErrorString(XML_GetErrorCode(parser));
      strstream << " on line ";
      strstream << XML_GetCurrentLineNumber(parser);
      strstream << " at offset ";
      strstream << XML_GetCurrentColumnNumber(parser);
      *errors = strstream.str();
    }
  }
  XML_ParserFree(parser);
  return status == XML_STATUS_OK;
}

bool RunExpat(const std::string& xml, ExpatHandler* expat_handler,
              std::string* errors) {
  return ExpatParser(xml, expat_handler, errors, false);
}

}  // end namespace kmlbase
