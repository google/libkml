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

// This file contains the declarations of some mathematical functions useful
// when working with KML or, more generally, geometry on a Great Circle.
//
// Many of the formulae here were cribbed from the excellent Aviation
// Formulary: http://williams.best.vwh.net/avform.htm
//
// NOTE: with the exception of the functions explicitly for converting between
// units, all functions here accept and return coordinates and angles in
// decimal degrees, and distances in meters.

#ifndef KML_BASE_MATH_UTIL_H__
#define KML_BASE_MATH_UTIL_H__

#include <math.h>
#include <utility>
#include "kml/base/vec3.h"

namespace kmlbase {

// Returns a Vec3 containing the latitude and longitude of a point at a
// distance (meters) out on the radial (degrees) from a center point lat, lng.
// The radial is measured clockwise from north. The antemeridian is not
// considered here.
Vec3 LatLngOnRadialFromPoint(double lat, double lng,
                             double distance, double radial);

// Returns the great circle distance in meters between two points. The
// antemeridian is not considered here.
double DistanceBetweenPoints(double lat1, double lng1,
                             double lat2, double lng2);

// These functions are mostly internal, used in converting between degrees and
// radians.
double DegToRad(double degrees);
double RadToDeg(double radians);
double MetersToRadians(double meters);
double RadiansToMeters(double radians);

}  // end namespace kmlbase

#endif  // KML_BASE_MATH_UTIL_H__
