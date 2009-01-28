// Copyright 2009, Google Inc. All rights reserved.
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

// This creates a KML file from a CSV file whose lines are in this form:
//  score|lat|lon|name|description[|style_url]
// A Point Placemark is created for each line in the CSV file.
// This program ignores the score and any style_url in the CSV file.
// For very very large CSV files see: examples/regionator/csvregionator.cc.
// For very very large KML files see: streamkml.cc.

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/base/string_util.h"
#include "kml/convenience/convenience.h"
#include "kml/dom.h"
#include "kml/engine.h"

using kmlbase::SplitStringUsing;
using kmlbase::File;
using kmlbase::ToString;
using kmlconvenience::CreatePointPlacemark;
using kmldom::DocumentPtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::PlacemarkPtr;
using kmlengine::KmlFile;

PlacemarkPtr CsvLineToPlacemark(const std::string& csv_line) {
  if (csv_line.empty()) {
    return NULL;
  }
  std::vector<std::string> csv_parts;
  SplitStringUsing(csv_line, "|", &csv_parts);
  if (csv_parts.size() < 5) {
    std::cerr << "Skipping bad csv line: " << csv_line << std::endl;
    return NULL;
  }
  const double lat = strtod(csv_parts[1].c_str(), NULL);
  const double lon = strtod(csv_parts[2].c_str(), NULL);
  PlacemarkPtr placemark = CreatePointPlacemark(csv_parts[3], lat, lon);
  placemark->set_description(csv_parts[4]);
  return placemark;
}

int CsvFileToKmlFile(const char* csv_filename, const char* kml_filename) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  DocumentPtr document = kml_factory->CreateDocument();
  document->set_name(csv_filename);

  std::ifstream csv_file;
  csv_file.open(csv_filename);
  for (int line_count = 0; csv_file.good(); ++line_count) {
    std::string csv_line;
    getline(csv_file, csv_line);
    if (PlacemarkPtr placemark = CsvLineToPlacemark(csv_line)) {
      placemark->set_id(std::string("line") + ToString(line_count));
      document->add_feature(placemark);
    }
  }

  KmlPtr kml = KmlFactory::GetFactory()->CreateKml();
  kml->set_feature(document);

  boost::scoped_ptr<KmlFile> kml_file(KmlFile::CreateFromImport(kml));
  if (!kml_file.get()) {
    return 1;
  }

  document->set_description(
     ToString(document->get_feature_array_size()) + " Placemarks." + "<br/>" +
     "From CSV file: " + csv_filename + ".\n");

  std::string xml;
  kml_file->SerializeToString(&xml);
  return File::WriteStringToFile(xml, kml_filename) ? 1 : 0;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << "input.csv output.kml" << std::endl;
    return 1;
  }
  return CsvFileToKmlFile(argv[1], argv[2]);
}



