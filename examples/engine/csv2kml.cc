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

// This creates a KML file from a CSV file whose first row is a schema such as:
//  name,latitude,longitude,description,a,b,c
// And each of whose lines look like:
//  hello,37.1,-111.123,how are you,1,2,3
// A Point Placemark is created for each line in the CSV file.  This example
// prints an error for and drops each line not exactly matching the schema.
// For very very large CSV files see: examples/regionator/csvregionator.cc.
// For very very large KML files see: streamkml.cc.

#include <iostream>
#include <string>
#include "kml/base/csv_splitter.h"
#include "kml/base/file.h"
#include "kml/base/string_util.h"
#include "kml/convenience/csv_parser.h"
#include "kml/convenience/convenience.h"
#include "kml/dom.h"
#include "kml/engine.h"

using kmlbase::File;
using kmlconvenience::CreatePointPlacemark;
using kmlconvenience::CsvParser;
using kmlconvenience::CsvParserHandler;
using kmlconvenience::CsvParserStatus;
using kmldom::ContainerPtr;
using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::PlacemarkPtr;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;

// This CsvParserHandler saves all "OK" Placemarks to the passed Container.
class ContainerSaver : public CsvParserHandler {
 public:
  ContainerSaver(ContainerPtr container)
    : container_(container) {
  }

  virtual bool HandleLine(int line, CsvParserStatus status, PlacemarkPtr p) {
    if (status == kmlconvenience::CSV_PARSER_STATUS_OK) {
      container_->add_feature(p);
    } else {
      if (line == 1 && status == kmlconvenience::CSV_PARSER_STATUS_NO_LAT_LON) {
        std::cerr << "Schema line must have both \"latitude\" and "
          <<  "\"longitude\"." << std::endl;
      } else {
        std::cerr << "Error on line " << line << std::endl;
      }
    }
    return true;
  }

 private:
  ContainerPtr container_;
};

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " input.csv output.kml" << std::endl;
    return 1;
  }

  std::string csv_data;
  if (!kmlbase::File::ReadFileToString(argv[1], &csv_data)) {
    std::cerr << "Read failed: " << argv[1] << std::endl;
    return 1;
  }
  kmlbase::CsvSplitter csv_splitter(csv_data);

  // Create a <Folder> and a ContainerSaver to write to it.
  FolderPtr folder = kmldom::KmlFactory::GetFactory()->CreateFolder();
  ContainerSaver container_saver(folder);

  // Call the CsvParser in strict mode to convert each line of CSV data into
  // a Placemark saved into the supplied Folder.
  if (!kmlconvenience::CsvParser::ParseCsv(&csv_splitter, &container_saver)) {
    std::cerr << "ParseCsv failed: " << argv[1] << std::endl;
    return 1;
  }

  std::cout << "Feature count " << folder->get_feature_array_size()
            << std::endl;

  // Import to kmlengine::KmlFile to get a nice xml header and xml namespace on
  // the root element.
  kmldom::KmlPtr kml = kmldom::KmlFactory::GetFactory()->CreateKml();
  kml->set_feature(folder);
  KmlFilePtr kml_file = KmlFile::CreateFromImport(kml);
  std::string xml;
  kml_file->SerializeToString(&xml);
  return File::WriteStringToFile(xml, argv[2]) ? 1 : 0;
}
