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

// Build a Region-based NetworkLink hierarchy from a CSV file.
// This creates a KML file from a CSV file whose first row is a schema such as:
//  Score,Name,Latitude,Longitude,Description,a,b,c
// And each of whose lines look like:
//  123,hello,37.1,-111.123,how are you,1,2,3
//  456,hi,37.2,-111.124,very fine thank you,2,3,4
// A Point Placemark is created for each line in the CSV file.  This example
// prints an error for and drops each line not exactly matching the schema.

#include <iostream>
#include <string>
#include "kml/base/csv_splitter.h"
#include "kml/base/file.h"
#include "kml/dom.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/csv_parser.h"
#include "kml/engine.h"
#include "kml/regionator/feature_list_region_handler.h"
#include "kml/regionator/regionator.h"

using kmlbase::File;
using kmldom::PlacemarkPtr;
using kmldom::RegionPtr;
using kmlconvenience::CsvParser;
using kmlconvenience::CsvParserHandler;
using kmlconvenience::CsvParserStatus;
using kmlconvenience::FeatureList;
using kmlengine::Bbox;
using kmlregionator::FeatureListRegionHandler;
using kmlregionator::RegionHandler;
using kmlregionator::Regionator;

// This CsvParserHandler saves each "OK" Placemark to the given FeatureList.
class FeatureListSaver : public kmlconvenience::CsvParserHandler {
 public:
  FeatureListSaver(FeatureList* feature_list)
    : feature_list_(feature_list) {
  }

  // This is the method called from within the CsvParser for each line in the
  // input CSV.  In this implementation we save all Placemarks from good lines
  // and noisily skip over imperfect lines.
  virtual bool HandleLine(int line, CsvParserStatus status, PlacemarkPtr p) {
    if (status == kmlconvenience::CSV_PARSER_STATUS_OK) {
      feature_list_->PushBack(p);
    } else {
      std::cerr << "Error on line " << line << std::endl;
    }
    return true;
  }

 private:
  FeatureList* feature_list_;
};

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " input.csv output_directory" <<
      std::endl;
    return 1;
  }
  const char* csv_filename = argv[1];
  const char* output_dir = argv[2];

  string csv_data;
  if (!File::ReadFileToString(csv_filename, &csv_data)) {
    std::cerr << "Read failed: " << csv_filename << std::endl;
    return 1;
  }
  kmlbase::CsvSplitter csv_splitter(csv_data);

  // Parse the CSV data into a FeatureList of Point Placemarks sorted by the
  // ExtendedData/Data element named "Score".
  FeatureList feature_list;
  FeatureListSaver feature_saver(&feature_list);
  if (!CsvParser::ParseCsv(&csv_splitter, &feature_saver)) {
    std::cerr << "Parse failed: " << csv_filename << std::endl;
    return 1;
  }
  std::cout << "Feature count: " << feature_list.Size() << std::endl;
  feature_list.Sort();

  // Give the FeatureList to the FeatureListRegionHandler.
  FeatureListRegionHandler feature_list_region_handler(&feature_list);

  // Create a root Region based on the bounding box of the FeatureList.
  Bbox bbox;
  feature_list.ComputeBoundingBox(&bbox);
  RegionPtr root = kmlconvenience::CreateRegion2d(bbox.get_north(),
                                                  bbox.get_south(),
                                                  bbox.get_east(),
                                                  bbox.get_west(),
                                                  256, -1);

  // Create a Regionator instance and walk the hierarchy starting at root.  The
  // output is aligned to a quadtree rooted at n=180, s=-180, e=180, w=-180.
  if (!Regionator::RegionateAligned(feature_list_region_handler, root,
                                    output_dir)) {
    std::cerr << "Regionation failed" << std::endl;
    return 1;
  }

  return 0;
}
