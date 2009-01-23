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

// This file contains the declarations of some KML convenience functions.

#ifndef KML_CONVENIENCE_CONVENIENCE_H__
#define KML_CONVENIENCE_CONVENIENCE_H__

#include <string>
#include "kml/base/vec3.h"
#include "kml/dom.h"

namespace kmlbase {
class DateTime;
}

namespace kmlconvenience {

// NOTE: this collection of convenience routines is expected to grow.
// NOTE: for now these are all in one file in alphabetical order.

// This creates a Data element with the given name and value and appends
// this to the Feature's ExtendedData.  An ExtendedData is created in the
// Feature if one does not already exist.
void AddExtendedDataValue(const std::string& name, const std::string& value,
                          kmldom::FeaturePtr feature);

// Creates a simple Polygon from a LinearRing.
kmldom::PlacemarkPtr CreateBasicPolygonPlacemark(
    const kmldom::LinearRingPtr& lr);

// Creates a <coordinates> element filled with the lng, lat[, alt] tuples
// describing a great circle of radius around a point lat, lng. The
// antemeridian is not considered here.
kmldom::CoordinatesPtr CreateCoordinatesCircle(double lat, double lng,
                                               double radius, size_t segments);

// This creates a Data element with the name and value specified:
// <Data name="NAME><value>VALUE</value></Data>
kmldom::DataPtr CreateDataNameValue(const std::string& name,
                                    const std::string& value);

// If the atts contains both a double "lat" and double "lon" then create
// a KML <Point> with <coordinates> set from these attributes.
kmldom::PointPtr CreatePointFromLatLonAtts(const char** atts);

// Create a <Point> with <coordinates> from the given Vec3.
kmldom::PointPtr CreatePointFromVec3(const kmlbase::Vec3& vec);

// This creates a Point coordinates set as indicated.
kmldom::PointPtr CreatePointLatLon(double lat, double lon);

// This is a convenience function to create a Point Placemark.
kmldom::PlacemarkPtr CreatePointPlacemark(const std::string& name,
                                          double lat, double lon);

// Create a <Placemark> with the given <Point>, DateTime and <styleUrl>.
// A <TimeStamp> is created from the DateTime and <ExtendedData> fields are
// created for date and time.
kmldom::PlacemarkPtr CreatePointPlacemarkWithTimeStamp(
    const kmldom::PointPtr& point, const kmlbase::DateTime& date_time,
    const char* style_id);

// Create a Region with LatLonAltBox set to the given bounds and Lod
// set to the given values.  This is a "2D" Region because no altitude
// mode is set which defaults the LatLonAltBox to clampToGround.
kmldom::RegionPtr CreateRegion2d(double north, double south,
                                 double east, double west,
                                 double minlodpixels, double maxlodpixels);

// This gets the value of the given name from the ExtendedData/Data as
// described above.  If there is no ExtendedData or no Data element with
// the given name false is returned.
bool GetExtendedDataValue(const kmldom::FeaturePtr& feature,
                          const std::string& name,
                          std::string* value);

// This sets the ExtendedData element of the feature to hold the given name
// value as a Data element as described above.  NOTE: Any previous ExtendedData
// is delete from this feature.
void SetExtendedDataValue(const std::string& name, const std::string& value,
                          kmldom::FeaturePtr feature);

}  // end namespace kmlconvenience

#endif  // KML_CONVENIENCE_CONVENIENCE_H__
