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

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "curlfetch.h"
#include "kml/base/file.h"
#include "kml/xsd/xsd_complex_type.h"
#include "kml/xsd/xsd_file.h"
#include "kml/xsd/xsd_simple_type.h"
#include "kml/xsd/xsd_util.h"

using kmlxsd::XsdComplexType;
using kmlxsd::XsdComplexTypePtr;
using kmlxsd::XsdElementPtr;
using kmlxsd::XsdElementVector;
using kmlxsd::XsdFile;
using kmlxsd::XsdSimpleType;
using kmlxsd::XsdSimpleTypePtr;
using kmlxsd::XsdType;
using kmlxsd::XsdTypePtr;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

static const char kOgcKml22Xsd[] =
    "http://schemas.opengis.net/kml/2.2.0/ogckml22.xsd";

static void PrintElement(const XsdFile& xsd_file, const XsdElementPtr element) {
  const XsdTypePtr& xsd_type = xsd_file.FindElementType(element);
  if (XsdComplexTypePtr complex_type =
          XsdComplexType::AsComplexType(xsd_type)) {
    cout << element->get_name() << " " << element->get_type() << endl;
    if (const XsdComplexTypePtr base = xsd_file.GetBaseType(complex_type)) {
      cout << "    is-a " << base->get_name() << endl;
    }
    XsdElementVector children;
    xsd_file.GetChildElements(element->get_name(), &children);
    for (size_t i = 0; i < children.size(); ++i) {
      if (children[i]) {  // TODO hack due to atom:author
        cout << "  " << children[i]->get_name() << " "
          << children[i]->get_type() << " [" << children[i]->get_default()
          << "]" << endl;
      }
    }
    return;
  }

  if (XsdSimpleTypePtr simple_type = XsdSimpleType::AsSimpleType(xsd_type)) {
    cout << element->get_name() << " " << element->get_type() << endl;
    if (simple_type->IsEnumeration()) {
      cout << "    is an enum" << endl;
      for (size_t i = 0; i < simple_type->get_enumeration_size(); ++i) {
        cout << "  "  << simple_type->get_enumeration_at(i) << endl;
      }
    }
  }
}

int main(int argc, char** argv) {
  if (argc > 2) {
    cerr << "usage: " << argv[0] << " [url-to-kml.xsd]" << endl;
    return 1;
  }
        
  const char* kml_xsd_url = argc > 1 ? argv[1] : kOgcKml22Xsd;
 
  std::string xsd_data;
  if (!kmlbase::File::ReadFileToString(kml_xsd_url, &xsd_data) &&
      !CurlToString(kml_xsd_url, &xsd_data)) {
    // TODO: parse kml_xsd_url to determine local/remote
    cerr << "read/fetch failed " << kml_xsd_url << endl;
    return 1;
  }

  std::string errors;
  boost::scoped_ptr<XsdFile> xsd_file(
      XsdFile::CreateFromParse(xsd_data, &errors));
  if (!xsd_file.get()) {
    cerr << "parse failed " << errors;
    return 1;
  }

  kmlxsd::XsdElementVector elements;
  xsd_file->GetAllElements(&elements);
  for (size_t i = 0; i < elements.size(); ++i) {
    PrintElement(*xsd_file, elements[i]);
  }

  return 0;
}
