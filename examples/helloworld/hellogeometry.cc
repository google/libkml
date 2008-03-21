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

// This file contains some sample code for Geometry elements:
// Point, LineString, LinearRing, Polygon, MultiGeometry

#include <iostream>
#include <sstream>
#include <string>
#include "kml/dom.h"

using std::stringstream;
using kmldom::Coordinates;
using kmldom::Folder;
using kmldom::InnerBoundaryIs;
using kmldom::Kml;
using kmldom::KmlFactory;
using kmldom::LinearRing;
using kmldom::LineString;
using kmldom::MultiGeometry;
using kmldom::OuterBoundaryIs;
using kmldom::Placemark;
using kmldom::Point;
using kmldom::Polygon;

Placemark* CreatePointPlacemark() {
  KmlFactory* factory = KmlFactory::GetFactory();

  Coordinates* coordinates = factory->CreateCoordinates();
  coordinates->add_point2(-123.32, 36.36);

  Point* point = factory->CreatePoint();
  point->set_coordinates(coordinates);

  Placemark* placemark = factory->CreatePlacemark();
  placemark->set_name("Point");
  placemark->set_geometry(point);

  return placemark;
}

Placemark* CreateLineStringPlacemark() {
  KmlFactory* factory = KmlFactory::GetFactory();

  Coordinates* coordinates = factory->CreateCoordinates();
  coordinates->add_point3(-121.12, 38.38, 123.456);
  coordinates->add_point3(-122.22, 37.37, 122.345);
  coordinates->add_point3(-123.32, 36.36, 121.234);
  coordinates->add_point2(-123.32, 36.36);

  LineString* linestring = factory->CreateLineString();
  linestring->set_extrude(true);
  linestring->set_tessellate(true);
  linestring->set_altitudemode(kmldom::ALTITUDEMODE_ABSOLUTE);
  linestring->set_coordinates(coordinates);

  Placemark* placemark = factory->CreatePlacemark();
  placemark->set_geometry(linestring);
  placemark->set_name("LineString");

  return placemark;
}

Placemark* CreateSimplePolygon() {
  KmlFactory* factory = KmlFactory::GetFactory();

  Coordinates* coordinates = factory->CreateCoordinates();
  coordinates->add_point3(-121.12, 38.38, 123.456);
  coordinates->add_point3(-122.22, 37.37, 122.345);
  coordinates->add_point3(-123.32, 36.36, 121.234);
  coordinates->add_point3(-121.12, 38.38, 123.456);

  LinearRing* linearring = factory->CreateLinearRing();
  linearring->set_coordinates(coordinates);

  OuterBoundaryIs* outerboundaryis = factory->CreateOuterBoundaryIs();
  outerboundaryis->set_linearring(linearring);

  Polygon* polygon = factory->CreatePolygon();
  polygon->set_tessellate(true);
  polygon->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  polygon->set_outerboundaryis(outerboundaryis);

  Placemark* placemark = factory->CreatePlacemark();
  placemark->set_name("Simple Polygon");
  placemark->set_geometry(polygon);

  return placemark;
}

// Corners presumed to be the corners of the ring.
template<int N>
LinearRing* CreateBoundary(const double (&corners)[N]) {
  KmlFactory* factory = KmlFactory::GetFactory();
  Coordinates* coordinates = factory->CreateCoordinates();
  for (int i = 0; i < N; i+=2) {
    coordinates->add_point2(corners[i], corners[i+1]);
  }
  // Last must be the same as first in a LinearRing.
  coordinates->add_point2(corners[0], corners[1]);

  LinearRing* linearring = factory->CreateLinearRing();
  linearring->set_coordinates(coordinates);
  return linearring;
}

Placemark* CreateTwoHolePolygon() {
  KmlFactory* factory = KmlFactory::GetFactory();
  Placemark* placemark = factory->CreatePlacemark();
  placemark->set_name("Polygon with 2 holes");

  Polygon* polygon = factory->CreatePolygon();

  const double outer_corners[] = {
    -122.4319382787491,37.80198570954779,
    -122.4318730681021,37.80166118304026,
    -122.4314979385389,37.8017138829201,
    -122.4315644851293,37.80202995372478
  };

  const double inner_west_corners[] = {
    -122.4318860160652,37.80193182195953,
    -122.4318421173957,37.80172085332186,
    -122.4317194993788,37.80173660441158,
    -122.4317632243884,37.8019475540673
  };

  const double inner_east_corners[] = {
    -122.4317133795781,37.80195363606838,
    -122.4316718851426,37.80174288373532,
    -122.4315492963515,37.8017586463012,
    -122.4315926416269,37.80196848593953
  };
  OuterBoundaryIs* outerboundaryis = factory->CreateOuterBoundaryIs();
  outerboundaryis->set_linearring(CreateBoundary(outer_corners));
  polygon->set_outerboundaryis(outerboundaryis);

  InnerBoundaryIs* innerboundaryis = factory->CreateInnerBoundaryIs();
  innerboundaryis->set_linearring(CreateBoundary(inner_west_corners));
  polygon->add_innerboundaryis(innerboundaryis);

  innerboundaryis = factory->CreateInnerBoundaryIs();
  innerboundaryis->set_linearring(CreateBoundary(inner_east_corners));
  polygon->add_innerboundaryis(innerboundaryis);

  placemark->set_geometry(polygon);

  return placemark;
}

Point* CreatePoint(int id, double longitude, double latitude) {
  KmlFactory* factory = KmlFactory::GetFactory();

  Coordinates* coordinates = factory->CreateCoordinates();
  coordinates->add_point2(longitude, latitude);

  Point* point = factory->CreatePoint();
  stringstream ss;
  ss << "pt_" << id;
  point->set_id(ss.str());
  point->set_coordinates(coordinates);
  return point;
}

Placemark* CreateMultiGeometry() {
  KmlFactory* factory = KmlFactory::GetFactory();

  Placemark* placemark = factory->CreatePlacemark();
  placemark->set_name("MultiGeometry");
  placemark->set_id("pmmg");
  MultiGeometry* multigeometry = factory->CreateMultiGeometry();
  placemark->set_geometry(multigeometry);

  const int num_points = 3;
  for (int i = 0; i < num_points; ++i) {
    multigeometry->add_geometry(CreatePoint(i, -120 + i, 37 + i));
  }

  return placemark;
}

void CreateGeometry(bool verbose) {
  KmlFactory* factory = KmlFactory::GetFactory();

  Folder* folder = factory->CreateFolder();
  folder->add_feature(CreatePointPlacemark());
  folder->add_feature(CreateLineStringPlacemark());
  folder->add_feature(CreateSimplePolygon());
  folder->add_feature(CreateTwoHolePolygon());
  folder->add_feature(CreateMultiGeometry());

  Kml* kml = factory->CreateKml();
  kml->set_feature(folder);

  if (verbose) {
    std::cout << kmldom::SerializePretty(*kml);
  }

  // Free's kml, Folder, all Feature's in Folder, etc.
  delete kml;
}

int main(int argc, char** argv) {
  // Silent by default.
  bool verbose = argc == 2 && argv[1][0] == '-' && argv[1][1] == 'v';
  CreateGeometry(verbose);
  return 0;  // All is well if we got this far.
}
