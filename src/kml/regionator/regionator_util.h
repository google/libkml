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

// This file contains the declarations of utility functions used in creating
// Region KML.
// TODO: the bulk of these are general purpose and should find their way to
// some more central place.

#ifndef KML_REGIONATOR_REGIONATOR_UTIL_H__
#define KML_REGIONATOR_REGIONATOR_UTIL_H__

#include <string>
#include "kml/dom.h"
#include "kml/regionator/regionator_qid.h"

namespace kmlregionator {

// Creates a copy of the given LatLonAltBox.
// TODO: a general Clone() in kmldom
kmldom::LatLonAltBoxPtr CloneLatLonAltBox(const kmldom::LatLonAltBoxPtr& orig);

// Creates a copy of the given Lod.
// TODO: a general Clone() in kmldom
kmldom::LodPtr CloneLod(const kmldom::LodPtr& orig);

// Creates a deep copy of the given Region.
// TODO: a general Clone() in kmldom
kmldom::RegionPtr CloneRegion(const kmldom::RegionPtr& orig);

// Creates a Region whose LatLonAltBox is the specified quadrant of
// that in the parent.  The created Region's Lod is cloned from the parent.
kmldom::RegionPtr CreateChildRegion(const kmldom::RegionPtr& parent,
                                    quadrant_t quadrant);

// Create a Placemark with LineString based on the LatLonAltBox in the Region.
kmldom::PlacemarkPtr CreateLineStringBox(const std::string& name,
                                         const kmldom::RegionPtr& region);

// Create a Placemark with a Point at the given coordinates.
kmldom::PlacemarkPtr CreatePointPlacemark(const std::string& name,
                                          double lat, double lon);

// Create a Region with LatLonAltBox set to the given bounds and Lod
// set to the given values.  This is a "2D" Region because no altitude
// mode is set which defaults the LatLonAltBox to clampToGround.
kmldom::RegionPtr CreateRegion2d(double north, double south,
                                 double east, double west,
                                 double minlodpixels, double maxlodpixels);

// Create a NetworkLink to the given URL with a Region cloned from the
// given Region.
kmldom::NetworkLinkPtr CreateRegionNetworkLink(const kmldom::RegionPtr& region,
                                               const std::string& href);

// Create a Document with a Region cloned from the given Region.
kmldom::DocumentPtr CreateRegionDocument(const kmldom::RegionPtr& region);

// Return the center of the LatLonBox or LatLonAltBox
void GetCenter(const kmldom::AbstractLatLonBoxPtr& latlonbox,
               double* lat, double* lon);

}  // end namespace kmlregionator

#endif  // KML_REGIONATOR_REGIONATOR_UTIL_H__
