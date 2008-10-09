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

// This program prints the elements not present in the given set of KML files.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/convenience/element_counter.h"
#include "kml/dom/xsd.h"
#include "kml/xsd/xsd_file.h"

using kmlconvenience::ElementCountMap;
using kmlconvenience::ElementCounter;
using kmldom::KmlDomType;
using kmldom::Parser;
using kmldom::Xsd;
using kmlxsd::XsdElementVector;
using kmlxsd::XsdFile;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

static bool GetConcreteElements(const char* xsd_pathname,
                                XsdElementVector* elements) {
  std::string xsd_data;
  if (!kmlbase::File::ReadFileToString(xsd_pathname, &xsd_data)) {
    cerr << "read failed " << xsd_pathname << endl;
    return false;
  }

  std::string errors;
  boost::scoped_ptr<XsdFile> xsd_file(
      XsdFile::CreateFromParse(xsd_data, &errors));
  if (!xsd_file.get()) {
    cerr << "parse failed " << xsd_pathname << endl;
    cerr << errors << endl;
    return false;
  }

  xsd_file->GetComplexElements(elements);
  xsd_file->GetSimpleElements(elements);
  return true;
}

static bool ParseAndCountElements(const char* kml_pathname,
                                  ElementCountMap* element_count_map) {
  std::string kml_data;
  if (!kmlbase::File::ReadFileToString(kml_pathname, &kml_data)) {
    cerr << "read failed " << kml_pathname << endl;
    return false;
  }
  ElementCounter element_counter(element_count_map);
  Parser parser;
  parser.AddObserver(&element_counter);
  std::string errors;
  if (!parser.Parse(kml_data, &errors)) {
    cerr << "parse failed: " << kml_pathname << endl;
    cerr << errors << endl;
    return false;
  }
  return true;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << "usage: " << argv[0] << " file.xsd file.kml..." << endl;
    return 1;
  }

  const char* xsd_pathname = argv[1];
  XsdElementVector elements;
  if (!GetConcreteElements(xsd_pathname, &elements)) {
    return 1;
  }

  ElementCountMap element_count_map;
  for (argc -= 2, argv +=2; argc; --argc, ++argv) {
    ParseAndCountElements(*argv, &element_count_map);
  }

  for (size_t i = 0; i < elements.size(); ++i) {
    const std::string& element_name = elements[i]->get_name();
    const KmlDomType kml_element_id = static_cast<KmlDomType>(
        Xsd::GetSchema()->ElementId(element_name));
    if (element_count_map.find(kml_element_id) == element_count_map.end()) {
      cout << element_name << endl;
    }
  }

  return 0;
}
