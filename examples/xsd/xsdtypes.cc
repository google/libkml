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

// This program prints all the types defined in the XSD.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/xsd/xsd_file.h"

using kmlxsd::XsdElementVector;
using kmlxsd::XsdFile;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "usage: " << argv[0] << " file.xsd" << endl;
    return 1;
  }

  const char* xsd_pathname = argv[1];

  std::string xsd_data;
  if (!kmlbase::File::ReadFileToString(xsd_pathname, &xsd_data)) {
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

  kmlxsd::XsdTypeVector types;
  xsd_file->GetAllTypes(&types);
  for (size_t i = 0; i < types.size(); ++i) {
    cout << types[i]->get_name() << endl;
  }

  return 0;
}
