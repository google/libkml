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

// This program prints all concrete elements of or derived from a given type.
// For example, this prints all 7 concrete feature element names in KML:
//   xsdfind ogckml22.xsd AbstractFeatureType
// (Get ogckml22.xsd from http://schemas.opengis.net/kml/2.2.0/ogckml22.xsd).

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/xsd/xsd_file.h"

using std::cerr;
using std::cout;
using std::endl;

int main(int argc, char** argv) {
  if (argc != 3) {
    cerr << "usage: " << argv[0] << " file.xsd type" << endl;
    return 1;
  }

  const char* xsd_pathname = argv[1];
  const char* type_name = argv[2];

  std::string xsd_data;
  if (!kmlbase::File::ReadFileToString(xsd_pathname, &xsd_data)) {
    cerr << "read failed " << xsd_pathname << endl;
    return 1;
  }

  std::string errors;
  boost::scoped_ptr<kmlxsd::XsdFile> xsd_file(
      kmlxsd::XsdFile::CreateFromParse(xsd_data, &errors));
  if (!xsd_file.get()) {
    cerr << "parse failed " << errors;
    return 1;
  }

  kmlxsd::XsdElementVector elements;
  xsd_file->GetElementsOfTypeByName(type_name, &elements);
  for (size_t i = 0; i < elements.size(); ++i) {
    if (!elements[i]->is_abstract()) {
      cout << elements[i]->get_name() << endl;
    }
  }

  return 0;
}
