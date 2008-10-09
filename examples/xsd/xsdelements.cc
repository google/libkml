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

// This program prints the abstract, complex or simple elements of the XSD.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/xsd/xsd_file.h"
#include "kml/xsd/xst_parser.h"

using kmlxsd::XsdElementVector;
using kmlxsd::XsdFile;
using kmlxsd::XstParser;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

static int usage(const char* app) {
  cerr << "usage: " << app << "abstract|complex|simple" << " file.xsd"
    << " [file.xst]" << endl;
  return 1;
}

int main(int argc, char** argv) {
  if (argc != 3 && argc != 4) {
    return usage(argv[0]);
  }

  const char which = argv[1][0];
  const char* xsd_pathname = argv[2];
  const char* xst_pathname = argc == 4 ? argv[3] : NULL;
 
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

  kmlxsd::XsdElementVector elements;
  switch (which) {
   case 'a':
    xsd_file->GetAbstractElements(&elements);
    break;
   case 'c':
    xsd_file->GetComplexElements(&elements);
    break;
   case 's':
    xsd_file->GetSimpleElements(&elements);
    break;
   default:
    return usage(argv[0]);
  }

  if (xst_pathname) {
    std::string xst_data;
    if (!kmlbase::File::ReadFileToString(xst_pathname, &xst_data)) {
      cerr << "read failed " << xst_pathname << endl;
      return 1;
    }
    XstParser xst_parser(xsd_file.get());
    xst_parser.ParseXst(xst_data);
  }
  for (size_t i = 0; i < elements.size(); ++i) {
    const std::string& element_name = elements[i]->get_name();
    std::string alias = xsd_file->get_alias(element_name);
    cout << (alias.empty() ? element_name : alias) << " "
      << elements[i]->get_type() << endl;
  }

  return 0;
}
