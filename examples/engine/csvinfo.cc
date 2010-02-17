// Copyright 2010, Google Inc. All rights reserved.
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

// This program parses the given CSV file and prints out some basic info.

#include <iostream>
#include <string>
#include "kml/base/csv_splitter.h"
#include "kml/base/file.h"
#include "kml/base/string_util.h"
#include "kml/convenience/csv_parser.h"
#include "kml/convenience/convenience.h"
#include "kml/dom.h"

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

// This CsvParserHandler saves all "OK" Placemarks to the passed Container.
class CsvInfo : public CsvParserHandler {
 public:
  CsvInfo()
    : good_(0),
      bad_(0) {
  }

  virtual bool HandleLine(int line, CsvParserStatus status, PlacemarkPtr p) {
    if (status == kmlconvenience::CSV_PARSER_STATUS_OK) {
      ++good_;
    } else {
      ++bad_;
    }
    return true;
  }

  unsigned int get_good() const {
    return good_;
  }
  unsigned int get_bad() const {
    return bad_;
  }

 private:
  unsigned int good_;
  unsigned int bad_;
};

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << "input.csv" << std::endl;
    return 1;
  }

  std::string csv_data;
  if (!kmlbase::File::ReadFileToString(argv[1], &csv_data)) {
    std::cerr << "Read failed: " << argv[1] << std::endl;
    return 1;
  }
  kmlbase::CsvSplitter csv_splitter(csv_data);

  CsvInfo csv_info;

  // Call the CsvParser in strict mode to convert each line of CSV data into
  // a Placemark saved into the supplied Folder.
  if (!kmlconvenience::CsvParser::ParseCsv(&csv_splitter, &csv_info)) {
    std::cerr << "ParseCsv failed: " << argv[1] << std::endl;
    return 1;
  }

  std::cout << "Good lines " << csv_info.get_good() << std::endl;
  std::cout << "Bad lines " << csv_info.get_bad() << std::endl;

}
