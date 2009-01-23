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

#include "kml/base/math_util.h"

// The mean radius of the Earth in meters.
// Equatorial = 6378137, polar = 6356752.
static const unsigned int kEarthRadius = 6366710;

namespace kmlbase {

Vec3 LatLngOnRadialFromPoint(double lat, double lng,
                             double distance, double radial) {
  const double lat_r = DegToRad(lat);
  const double lng_r = DegToRad(lng);
  const double distance_r = MetersToRadians(distance);
  const double radial_r = DegToRad(radial);
  const double radial_lat = asin(sin(lat_r) * cos(distance_r) +
                    cos(lat_r) * sin(distance_r) * cos(radial_r));
  const double delta_lng = atan2(sin(radial_r) * sin(distance_r) * cos(lat_r),
                           cos(distance_r) - sin(lat_r) * sin(radial_lat));
  const double radial_lng = fmod(lng_r + delta_lng + M_PI, 2 * M_PI) - M_PI;
  return Vec3(RadToDeg(radial_lng), RadToDeg(radial_lat));
}

double DistanceBetweenPoints(double lat1, double lng1,
                             double lat2, double lng2) {
  double lat1_r = DegToRad(lat1);
  double lng1_r = DegToRad(lng1);
  double lat2_r = DegToRad(lat2);
  double lng2_r = DegToRad(lng2);
  double d = 2 * asin(sqrt(pow(sin((lat1_r - lat2_r)/2), 2) +
             cos(lat1_r) * cos(lat2_r) * pow(sin((lng1_r - lng2_r)/2), 2)));
  return RadiansToMeters(d);
}

double DegToRad(double degrees) { return degrees * M_PI / 180.0; }
double RadToDeg(double radians) {  return radians * 180.0 / M_PI; }
double MetersToRadians(double meters) {  return meters / kEarthRadius; }
double RadiansToMeters(double radians) {  return radians * kEarthRadius; }

}  // end namespace kmlbase
