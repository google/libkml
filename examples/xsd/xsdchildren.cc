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

// This program prints the children of all complex elements and the enumeration
// values of all enumerated simple elements in the XSD.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/xsd/xsd_complex_type.h"
#include "kml/xsd/xsd_file.h"
#include "kml/xsd/xsd_simple_type.h"
#include "kml/xsd/xsd_util.h"
#include "kml/xsd/xst_parser.h"

using kmlbase::File;
using kmlxsd::XsdComplexType;
using kmlxsd::XsdComplexTypePtr;
using kmlxsd::XsdElementPtr;
using kmlxsd::XsdElementVector;
using kmlxsd::XsdFile;
using kmlxsd::XsdSimpleType;
using kmlxsd::XsdSimpleTypePtr;
using kmlxsd::XsdTypePtr;
using kmlxsd::XstParser;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

void PrintAlias(const XsdFile& xsd_file, const std::string& real_name) {
  const std::string alias = xsd_file.get_alias(real_name);
  cout << (alias.empty() ? real_name : alias);
}

// This prints each child of each complex element in this format:
//  parent_element_name element_type element_base child_element_name
// For example:
//  "BalloonStyle string xsd:primitive text"
// If the type is not known or if there is no base type "None": appears:
//  "Point coordinatesType None coordinates"
static void PrintComplex(const std::string& parent_element_name,
                         const XsdFile& xsd_file) {
  XsdElementVector children;
  xsd_file.GetChildElements(parent_element_name, &children);
  for (size_t i = 0; i < children.size(); ++i) {
    const XsdElementPtr& child_element = children[i];
    // NOTE: child_element can be NULL for import'ed elements (atom:author).
    if (child_element && !child_element->get_name().empty()) {
      if (const XsdTypePtr child_type =
              xsd_file.FindElementType(child_element)) {
          const std::string child_type_base = child_type->get_base().empty() ?
            "None" : child_type->get_base();
          PrintAlias(xsd_file, parent_element_name);
          cout  << " " << child_type->get_name() << " "
            << child_type_base << " ";
          PrintAlias(xsd_file, child_element->get_name());
          cout << endl;
      }
    }
  }
}

// This prints each possible enumeration value of each enumerated element:
//   "altitudeMode clampToGround"
static void PrintEnum(const std::string& element_name,
                      const XsdSimpleTypePtr& simple_type) {
  if (!simple_type->IsEnumeration()) {
    return;
  }
  size_t size = simple_type->get_enumeration_size();
  for (size_t i = 0; i < size; ++i) {
    cout << element_name << " " << simple_type->get_enumeration_at(i) << endl;
  }
}

static void PrintChildren(const XsdFile& xsd_file,
                          const XsdElementPtr parent_element) {
  const XsdTypePtr& xsd_type = xsd_file.FindElementType(parent_element);
  if (XsdComplexTypePtr complex_type =
          XsdComplexType::AsComplexType(xsd_type)) {
    PrintComplex(parent_element->get_name(), xsd_file);
  } else if (XsdSimpleTypePtr simple_type =
                 XsdSimpleType::AsSimpleType(xsd_type)) {
    PrintEnum(parent_element->get_name(), simple_type);
  }
}

int main(int argc, char** argv) {
  if (argc != 2 && argc != 3) {
    cerr << "usage: " << argv[0] << " file.xsd [file.xst]" << endl;
    return 1;
  }

  const char* xsd_pathname = argv[1];
  const char* xst_pathname = argc == 3 ? argv[2] : NULL;

  std::string xsd_data;
  if (!File::ReadFileToString(xsd_pathname, &xsd_data)) {
    cerr << "read failed " << xsd_pathname << endl;
    return 1;
  }

  std::string errors;
  boost::scoped_ptr<XsdFile> xsd_file(
      XsdFile::CreateFromParse(xsd_data, &errors));
  if (!xsd_file.get()) {
    cerr << "parse failed " << errors;
    return 1;
  }

  std::string xst_data;
  if (xst_pathname) {
    if (!File::ReadFileToString(xst_pathname, &xst_data)) {
      cerr << "read failed " << xst_pathname << endl;
      return 1;
    }
    XstParser xst_parser(xsd_file.get());
    xst_parser.ParseXst(xst_data);
  }

  kmlxsd::XsdElementVector elements;
  xsd_file->GetAllElements(&elements);
  for (size_t i = 0; i < elements.size(); ++i) {
    PrintChildren(*xsd_file, elements[i]);
  }

  return 0;
}
