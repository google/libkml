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

// Walks a Feature hierarchy, looking for Placemarks with Geometry, printing
// the type of Geometry encountered.

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/util/fileio.h"

using kmldom::Container;
using kmldom::Element;
using kmldom::Feature;
using kmldom::Geometry;
using kmldom::Kml;
using kmldom::MultiGeometry;
using kmldom::Placemark;
using std::cout;
using std::endl;

static void WalkGeometry(const Placemark* placemark);
static void WalkFeature(const Feature* feature);
static void WalkContainer(const Container& container);
static const Feature* GetRootFeature(const Element* root);

static void WalkGeometry(const Geometry* geometry) {
  if (!geometry) {
    return;
  }
  // Print the Geometry type.
  cout << "Found a";
  switch(geometry->Type()) {
    case kmldom::Type_Point:
      cout << " Point";
      break;
    case kmldom::Type_LineString:
      cout << " LineString";
      break;
    case kmldom::Type_LinearRing:
      cout << " LinearRing";
      break;
    case kmldom::Type_Polygon:
      cout << " Polygon";
      break;
    case kmldom::Type_MultiGeometry:
      cout << " MultiGeometry";
      break;
    case kmldom::Type_Model:
      cout << " Model";
      break;
    default:  // KML has 6 types of Geometry.
      break;
  }
  cout << endl;
  // Recurse into <MultiGeometry>.
  if (const MultiGeometry* multigeometry = kmldom::AsMultiGeometry(geometry)) {
    for (size_t i = 0; i < multigeometry->geometry_array_size(); ++i) {
      WalkGeometry(multigeometry->geometry_array_at(i));
    }
  }
}

static void WalkFeature(const Feature* feature) {
  if (feature) {
    if (const Container* container = kmldom::AsContainer(feature)) {
      WalkContainer(*container);
    } else if (const Placemark* placemark = kmldom::AsPlacemark(feature)) {
      WalkGeometry(placemark->geometry());
    }
  }
}

static void WalkContainer(const Container& container) {
  for (size_t i = 0; i < container.feature_array_size(); ++i) {
    WalkFeature(container.feature_array_at(i));
  }
}

static const Feature* GetRootFeature(const Element* root) {
  if (root) {
    const Kml* kml = kmldom::AsKml(root);
    if (kml && kml->has_feature()) {
      return kml->feature();
    }
    return kmldom::AsFeature(root);
  }
  return NULL;
}

int main(int argc, char** argv) {
  std::string kml;
  ReadFileToString(argv[1], &kml);
  std::string errors;
  Element* root = kmldom::Parse(kml, &errors);
  if (root) {
    WalkFeature(GetRootFeature(root));
    delete root;
  }
  return 0;
}

