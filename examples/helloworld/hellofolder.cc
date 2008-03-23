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

#include <assert.h>
#include <iostream>
#include <string>
#include "kml/dom.h"

using std::cout;
using std::endl;
using std::string;
using kmldom::Coordinates;
using kmldom::Feature;
using kmldom::Folder;
using kmldom::KmlFactory;
using kmldom::Placemark;
using kmldom::Point;

Placemark* CreatePlacemark(kmldom::KmlFactory* factory,
                           const string& name,
                           double lat, double lon) {
  Placemark* placemark(factory->CreatePlacemark());
  placemark->set_name(name);

  Coordinates* coordinates(factory->CreateCoordinates());
  coordinates->add_point2(lon, lat);

  Point* point(factory->CreatePoint());
  point->set_coordinates(coordinates);

  placemark->set_geometry(point);

  return placemark;
}

static const unsigned int kHowManyPoints = 1001;

int main(int argc, char** argv) {
  KmlFactory* factory(KmlFactory::GetFactory());

  // Create a Folder, fill it with Placemarks.
  cout << "Creating " << kHowManyPoints << " Placemarks:" << endl;
  Folder* folder = factory->CreateFolder();
  for (int i = 0 ; i < kHowManyPoints ; ++i) {
    folder->add_feature(CreatePlacemark(factory, "hi", 1.1, 2.2));
  }

  // Read back all the Placemarks in the Folder.
  cout << "Iterating " << kHowManyPoints << " Placemarks:" << endl;
  assert(kHowManyPoints == folder->feature_array_size());
  for (int i = 0; i < folder->feature_array_size(); ++i) {
    assert("hi" == folder->feature_array_at(i)->name());
    assert(kmldom::Type_Placemark == folder->feature_array_at(i)->Type());
  }

  // Deletes everything within.
  cout << "Deleting Folder" << endl;
  delete folder;

  return 0;
}
