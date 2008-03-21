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

// Sample of parse and dom access of attributes.

#include <assert.h>
#include <iostream>
#include <string>
#include "kml/dom.h"

using kmldom::Element;
using kmldom::HotSpot;
using kmldom::IconStyle;
using kmldom::Placemark;

void CheckHotSpot() {
  const std::string kml(
      "<IconStyle id=\"is42\">"
      "<hotSpot x=\"0.5\" y=\"123\" xunits=\"fraction\" yunits=\"pixels\"/>"
      "</IconStyle>");
  Element* root = kmldom::Parse(kml, NULL);

  // Verify the parse went fine.
  assert(root);  // The kml is perfect.

  const IconStyle* iconstyle = kmldom::AsIconStyle(root);
  assert(true == iconstyle->has_hotspot());

  const HotSpot* hotspot = iconstyle->hotspot();
  assert(true == hotspot->has_x());
  assert(0.5 == hotspot->x());
  assert(true == hotspot->has_y());
  assert(123 == hotspot->y());
  assert(true == hotspot->has_xunits());
  assert(kmldom::UNITS_FRACTION == hotspot->xunits());
  assert(true == hotspot->has_yunits());
  assert(kmldom::UNITS_PIXELS == hotspot->yunits());

  // Serialize the sample.
  std::string parsed_kml = kmldom::SerializePretty(*hotspot);
  std::cout << parsed_kml << std::endl;

  // Free all resources allocated by the parser.
  delete root;
}

void CheckId() {
  // Parse some KML with an attribute.
  const std::string kml(
      "<Placemark id=\"placemark123\"><name>x</name></Placemark>");
  Element* root = kmldom::Parse(kml, NULL);

  // Verify the parse went fine.
  assert(root);  // The kml is perfect.

  const Placemark* placemark = kmldom::AsPlacemark(root);

  assert("placemark123" == placemark->id());

  // Serialize the sample.
  std::string parsed_kml = kmldom::SerializePretty(*placemark);
  std::cout << parsed_kml << std::endl;

  // Free all resources allocated by the parser.
  delete root;
}

int main(int argc, char** argv) {
  CheckId();
  CheckHotSpot();
  return 0;  // All is well if we got this far.
}
