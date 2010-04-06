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

// Build a Region-based NetworkLink hierarchy from a KML file.  Shared styles
// are preserved in the case of relative references within the input file.

#include <fstream>
#include <iostream>
#include <string>
#include "kml/base/file.h"
#include "kml/dom.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/kml_feature_list_saver.h"
#include "kml/engine.h"
#include "kml/regionator/feature_list_regionator.h"
#include "kml/regionator/regionator.h"

using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::StyleSelectorPtr;
using kmlconvenience::FeatureList;
using kmlconvenience::KmlFeatureListSaver;
using kmlengine::SharedStyleMap;
using kmlregionator::FeatureListRegionator;
using kmlregionator::Regionator;

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "usage: " << argv[0] << " input.kml output_directory" <<
      std::endl;
    return 1;
  }
  const char* kml_filename = argv[1];
  const char* output_dir = argv[2];

  std::ifstream input_file(kml_filename,
                           std::ios_base::in | std::ios_base::binary);
  if (!input_file.is_open() || !input_file.good()) {
    std::cerr << "Open failed: " << kml_filename << std::endl;
    return 1;
  }

  // Parse the Features and shared styles from the input KML file.
  FeatureList feature_list;
  SharedStyleMap shared_style_map;
  // When created with a style base all internal relative references are
  // rewritten as style.kml#style-id.
  const string kStyleKml("style.kml");
  KmlFeatureListSaver kml_saver(&feature_list, &shared_style_map,
                                kStyleKml.c_str());

  string errors;
  if (!kmlengine::KmlStream::ParseFromIstream(&input_file, &errors,
                                              &kml_saver)) {
    std::cerr << "Parse failed: " << kml_filename << std::endl;
    std::cerr << errors << std::endl;
    return 1;
  }

  std::cout << "Feature count: " << feature_list.Size() << std::endl;
  std::cout << "Style count: " << shared_style_map.size() << std::endl;

  // Give the FeatureList to the FeatureListRegionator which walks the
  // hierarchy starting at root.  The output is aligned to a quadtree rooted
  // at n=180, s=-180, e=180, w=-180.
  if (!FeatureListRegionator<>::Regionate(&feature_list, 10, NULL,
                                          output_dir)) {
    std::cerr << "Regionation failed" << std::endl;
    return 1;
  }

  // Gather up the shared styles and write them to $output_dir/style.kml.
  // TODO: move this into a KmlFile::CreateFromSharedStyleMap.
  kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
  kmldom::DocumentPtr document = kml_factory->CreateDocument();
  SharedStyleMap::const_iterator iter = shared_style_map.begin();
  for (; iter != shared_style_map.end(); ++iter) {
    document->add_styleselector(iter->second);
  }
  kmldom::KmlPtr kml = kml_factory->CreateKml();
  kml->set_feature(document);

  // Use KmlFile mostly for its nice XML header, etc.
  kmlengine::KmlFilePtr style_file(kmlengine::KmlFile::CreateFromImport(kml));
  string style_kml;
  style_file->SerializeToString(&style_kml);

  const string style_path(kmlbase::File::JoinPaths(output_dir, kStyleKml));
  if (!kmlbase::File::WriteStringToFile(style_kml, style_path)) {
    std::cerr << "Style output failed: " << style_path << std::endl;
    return 1;
  }

  return 0;
}
