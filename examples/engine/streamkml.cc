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

// This example shows how KmlStream is used with a ParserObserver.  Given that
// the ParserObserver inhibits adding Features to the DOM this program can be
// used to scan extremely large files such as those where DOM representation
// would exceed available memmory.

#include <fstream>
#include <iostream>
#include "kml/dom.h"
#include "kml/engine.h"

using kmldom::AsFeature;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::KmlFactory;
using kmldom::LatLonBoxPtr;
using kmldom::SerializePretty;
using kmlengine::Bbox;
using kmlengine::GetFeatureLatLon;
using kmlengine::KmlStream;

// This ParserObserver inhibits adding of any Feature to the DOM.  A bounding
// box of all features is maintained.
class FeatureStreamer : public kmldom::ParserObserver {
 public:
  FeatureStreamer()
    : feature_count_(0) {
  }

  // ParserObserver::EndElement()
  virtual bool EndElement(const ElementPtr& parent, const ElementPtr& child) {
    if (FeaturePtr feature = AsFeature(child)) {
      // This is roughly 1hz in a 2 ghz MacBook.
      if (++feature_count_ % 10000 == 0 ) {
        std::cout << feature_count_ << " " << feature->get_name();
        std::cout << std::endl;
      }
      double lat, lon;
      if (GetFeatureLatLon(feature, &lat, &lon) && lat && lon) {
        bbox_.ExpandLatLon(lat, lon);
      }
      // Do not add this feature to the DOM (note that this discards both
      // <Documents> and <Folders>).
      return false;
    }
    // Preserve all other parent-child relationships (such as those _within_
    // a feature such as the Point child of Placemark and coordinates child
    // of Point as expected in GetFeatureLatLon).
    return true;
  }

  const Bbox& get_bbox() const {
    return bbox_;
  }
  int get_feature_count() const {
    return feature_count_;
  }
 private:
  Bbox bbox_;
  int feature_count_;
};

void StreamKml(std::istream* input) {
  FeatureStreamer feature_streamer;
  std::string errors;
  boost::scoped_ptr<KmlStream> kml_stream(
      KmlStream::ParseFromIstream(input, &errors, &feature_streamer));
  if (!kml_stream.get()) {
    std::cerr << "KmlStream error " << errors << std::endl;
    return;
  }

  std::cout << "Streamed parse completed, ";
  std::cout << feature_streamer.get_feature_count() << " features";
  std::cout << std::endl;

  // Emit the bounding box as KML.
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  LatLonBoxPtr llab = kml_factory->CreateLatLonBox();
  const Bbox& bbox = feature_streamer.get_bbox();
  llab->set_north(bbox.get_north());
  llab->set_south(bbox.get_south());
  llab->set_east(bbox.get_east());
  llab->set_west(bbox.get_west());
  std::cout << SerializePretty(llab);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " huge.kml" << std::endl;
    return 1;
  }
  const char* filename = argv[1];
  std::ifstream input_file(filename, std::ios_base::in|std::ios_base::binary);
  if (!input_file.is_open() || !input_file.good()) {
    std::cerr << "open failed: " << filename << std::endl;
    return 1;
  }
  StreamKml(&input_file);
}
