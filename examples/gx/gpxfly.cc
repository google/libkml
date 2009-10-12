// Copyright 2009, Google Inc. All rights reserved.
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

// This program converts a GPX file to a KML Tour of FlyTo's.  Each <trkpt>
// is translated to a <gx:FlyTo>/<LookAt> to the lat/lon of the trkpt and
// all <gx:FlyTo>'s are put in one <gx:Tour>/<gx:Playlist>.

#include <iostream>
#include "kml/base/expat_parser.h"
#include "kml/base/date_time.h"
#include "kml/base/file.h"
#include "kml/base/math_util.h"
#include "kml/base/vec3.h"
#include "kml/base/xml_namespaces.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/gpx_trk_pt_handler.h"
#include "kml/dom.h"
#include "kml/engine.h"

using kmlbase::AzimuthBetweenPoints;
using kmlbase::DateTime;
using kmlbase::DistanceBetweenPoints;
using kmlbase::ExpatParser;
using kmlbase::GroundDistanceFromRangeAndElevation;
using kmlbase::HeightFromRangeAndElevation;
using kmlbase::Vec3;
using kmlconvenience::CreateAnimatedUpdateChangePoint;
using kmlconvenience::CreatePointLatLon;
using kmldom::CameraPtr;
using kmldom::ChangePtr;
using kmldom::ContainerPtr;
using kmldom::DocumentPtr;
using kmldom::GxAnimatedUpdatePtr;
using kmldom::GxFlyToPtr;
using kmldom::GxPlaylistPtr;
using kmldom::GxTourPtr;
using kmldom::GxWaitPtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::PlacemarkPtr;
using kmldom::UpdatePtr;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;

static const double kThreshold = 60.0;  // seconds

// Return a <gx:FlyTo> with <Camera> as specified by the lat, lon and heading
// and with a range scaled by speed.  The duration is used as the <gx:duration>
// of the <gx:FlyTo>.
static GxFlyToPtr CreateGxFlyTo(double lat, double lon, double heading,
                                double duration, double speed) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();

  // Tilt the camera back as speed increases, but clamp max tilt.
  double tilt =  0 + speed * 4;
  if (tilt > 65.) tilt = 65.;

  // The distance along the vector from the gpx point to eyepoint.
  double range = 100.0 + tilt * 7;

  // The ground distance between the gpx point and the subpoint of where
  // we'll place the camera.
  double distance = GroundDistanceFromRangeAndElevation(range, 90 - tilt);

  // The lat,lon of the camera.
  double reverse_heading = fmod(heading + 180.0,  360.0);
  Vec3 camera_position =
    kmlbase::LatLngOnRadialFromPoint(lat, lon, distance, reverse_heading);

  CameraPtr camera = kml_factory->CreateCamera();
  camera->set_latitude(camera_position.get_latitude());
  camera->set_longitude(camera_position.get_longitude());
  camera->set_altitude(HeightFromRangeAndElevation(range, 90 - tilt));
  camera->set_heading(heading);
  // Nudge the tilt up a little to move the icon towards the bottom of the
  // screen.
  camera->set_tilt(tilt *= 1.1);
  GxFlyToPtr flyto = kml_factory->CreateGxFlyTo();
  flyto->set_abstractview(camera);
  flyto->set_gx_duration(duration);
  // TODO: the first FlyTo in a Playlist often better as bounce
  flyto->set_gx_flytomode(kmldom::GX_FLYTOMODE_SMOOTH);
  return flyto;
}

// This specialization of the GpxTrkPtHandler converts each GPX <trkpt> to
// a KML <gx:AnimatedUpdate> + <gx:FlyTo>.
class TourTrkPtHandler : public kmlconvenience::GpxTrkPtHandler {
 public:
  TourTrkPtHandler(ContainerPtr container)
    : placemark_id_("moving-placemark"),
      previous_when_(0),
      container_(container) {
  }

  void NewTour() {
    GxTourPtr tour(KmlFactory::GetFactory()->CreateGxTour());
    tour->set_name("Play me!");
    playlist_ = KmlFactory::GetFactory()->CreateGxPlaylist();
    tour->set_gx_playlist(playlist_);
    container_->add_feature(tour);
  }

  // This is called for each <trkpt>.
  virtual void HandlePoint(const kmlbase::Vec3& where,
                           const std::string& when) {
    time_t when_timet = DateTime::ToTimeT(when);
    if (when_timet == 0) {
      std::cerr << "Ignoring point with no time" << std::endl;
      return;
    }
    if (previous_when_ == 0) {  // First point
      PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
      placemark->set_id(placemark_id_);
      placemark->set_geometry(CreatePointLatLon(where.get_latitude(),
                                                where.get_longitude()));
      container_->add_feature(placemark);
      NewTour();
    } else {
      // Convert the GPX <trkpt> to a <gx:AnimatedUpdate> + <gx:FlyTo>.
      // Note, it's quite important that the AnimatedUpdate appear _before_
      // the FlyTo given that a FlyTo will happen during an AnimatedUpdate
      // but an AnimatedUpdate will _not_ start until a FlyTo is done.
      const double duration = when_timet - previous_when_;
      if (duration < 0) {
        std::cerr << "Ignoring point out of time order." << std::endl;
      } else if (duration > kThreshold) {
        NewTour();
      } else {
        playlist_->add_gx_tourprimitive(
            CreateAnimatedUpdateChangePoint(placemark_id_, where, duration));
        const double heading = AzimuthBetweenPoints(
            previous_where_.get_latitude(), previous_where_.get_longitude(),
            where.get_latitude(), where.get_longitude());
        const double speed = DistanceBetweenPoints(
            previous_where_.get_latitude(), previous_where_.get_longitude(),
            where.get_latitude(), where.get_longitude()) / duration;
        playlist_->add_gx_tourprimitive(
            CreateGxFlyTo(where.get_latitude(), where.get_longitude(),
                          heading, duration, speed));
        // Wait 0 to create smooth animated update...
        playlist_->add_gx_tourprimitive(kmlconvenience::CreateWait(0));
      }
    }
    previous_when_ = when_timet;
    previous_where_ = where;
  }

 private:
  const std::string placemark_id_;
  kmlbase::Vec3 previous_where_;
  time_t previous_when_;
  ContainerPtr container_;
  GxPlaylistPtr playlist_;
};

// Parse the gpx file and make a KML tour to save out to the kml file.
static bool CreateGpxTour(const char* gpx_pathname, const char* kml_pathname) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  // Create a <Document> and hand it to the GPX parser.
  DocumentPtr document = kml_factory->CreateDocument();
  document->set_open(1);
  TourTrkPtHandler tour_maker(document);

  // Read the GPX file contents.
  std::string gpx_data;
  if (!kmlbase::File::ReadFileToString(gpx_pathname, &gpx_data)) {
    std::cerr << "read failed: " << gpx_pathname << std::endl;
    return false;
  }

  // Parse the GPX data writing a Tour into the <Document>.
  std::string errors;
  if (!ExpatParser::ParseString(gpx_data, &tour_maker, &errors, false)) {
    std::cerr << "parse failed: " << gpx_pathname << std::endl;
    return false;
  }

  // Put the <Document> in a <kml> element and write everything out to the
  // supplied pathname.
  KmlPtr kml = kml_factory->CreateKml();
  kml->set_feature(document);
  KmlFilePtr kml_file = KmlFile::CreateFromImport(kml);
  std::string kml_data;
  kml_file->SerializeToString(&kml_data);

  if (!kmlbase::File::WriteStringToFile(kml_data, kml_pathname)) {
    std::cerr << "write failed: " << kml_pathname << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " input.gpx output.kml" << std::endl;
    return 1;
  }
  return CreateGpxTour(argv[1], argv[2]) ? 0 : 1;
}
