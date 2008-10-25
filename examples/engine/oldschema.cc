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

// This example uses the kmlengine::SchemaParserObserver and
// kmlengine::OldSchemaParserObserver to convert "old-style" <Schema> to valid
// OGC KML 2.2.  See kml/engine/parse_old_schema.h for details.

#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/engine/old_schema_parser_observer.h"
#include "kml/engine/schema_parser_observer.h"
#include "kml/engine/engine_types.h"
#include "kml/base/file.h"

using kmlengine::KmzFile;
using std::cout;
using std::endl;

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " kmlfile" << endl;
    return 1;
  }

  // Read the file.
  std::string file_data;
  if (!kmlbase::File::ReadFileToString(argv[1], &file_data)) {
    cout << argv[1] << " read failed" << endl;
    return 1;
  }

  // If the file was KMZ, extract the KML file.
  std::string kml;
  if (KmzFile::IsKmz(file_data)) {
    boost::scoped_ptr<KmzFile> kmz_file(KmzFile::OpenFromString(argv[1]));
    if (!kmz_file.get()) {
      cout << "Failed opening KMZ file" << endl;
      return 1;
    }
    if (!kmz_file->ReadKml(&kml)) {
      cout << "Failed to read KML from KMZ" << endl;
      return 1;
    }
  } else {
    kml = file_data;
  }

  // This holds mappings from <Schema name="..."> to SchemaPtr.
  kmlengine::SchemaNameMap schema_name_map;
  // This finds all <Schema>'s in the parse and saves them in schema_name_map.
  kmlengine::SchemaParserObserver schema_parser_observer(&schema_name_map);
  // This uses mappings in schema_name_map to convert each "old-style" <Schema>
  // instances into an OGC KML 2.2 <Placemark>.
  kmlengine::OldSchemaParserObserver old_schema_parser_observer(
      schema_name_map);
  // Configure the parser with the two observers.
  kmldom::Parser parser;
  parser.AddObserver(&schema_parser_observer);
  parser.AddObserver(&old_schema_parser_observer);
  std::string errors;
  kmldom::ElementPtr root = parser.Parse(kml, &errors);
  if (!root) {
    cout << errors << endl;
    return 1;
  }

  cout << kmldom::SerializePretty(root);

  return 0;
}
