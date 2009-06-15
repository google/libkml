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

// This sample program generates a gx:Tour of all visible features in the given
// KML or KMZ file. You supply two times (in seconds): a flyto_duration, used
// as the time taken to fly between features, and a wait_duration, used as the
// time spent dwelling at a feature before flying to the next.
// Example usage: ./featuretour /my/cool/file.kml 10.0  5.0 tour.kml

#include <iostream>
#include "kml/convenience/convenience.h"
#include "kml/base/file.h"
#include "kml/dom.h"
#include "kml/engine.h"

using kmlconvenience::CreateFlyToForFeature;
using kmlconvenience::CreateWait;
using kmldom::FeaturePtr;
using kmldom::GxFlyToPtr;
using kmldom::GxPlaylistPtr;
using kmldom::GxTourPtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmlengine::GetRootFeature;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;
using kmlengine::VisitFeatureHierarchy;
using std::cerr;
using std::cout;
using std::endl;

// This subclass of FeatureVisitor takes each Feature in the KmlFile and
// attempts to generate a <gx:FlyTo> for that feature's view, appending it
// to a supplied gx:Playlist. The GxPlaylistPtr must be non-NULL.
class TourComposer : public kmlengine::FeatureVisitor {
 public:
  TourComposer(GxPlaylistPtr playlist, double flyto_duration,
               double dwell_duration)
    : playlist_(playlist), flyto_duration_(flyto_duration),
      dwell_duration_(dwell_duration) {
  }

  virtual ~TourComposer() {}

  // The callback from VisitFeatureHierarchy.
  virtual void VisitFeature(const FeaturePtr& feature) {
    if (!feature->get_visibility()) {
      return;  // Skip if not visible.
    }
    // CreateFlyToForFeature may return NULL.
    GxFlyToPtr flyto = CreateFlyToForFeature(feature, flyto_duration_);
    if (!flyto) {
      return;
    }
    playlist_->add_gx_tourprimitive(flyto);
    playlist_->add_gx_tourprimitive(CreateWait(dwell_duration_));
  }

 private:
  GxPlaylistPtr playlist_;
  double flyto_duration_;
  double dwell_duration_;
};

static GxTourPtr GenerateTour(const KmlFilePtr& kml_file,
                              double flyto_duration, double dwell_duration) {
  KmlFactory* factory = KmlFactory::GetFactory();
  GxTourPtr tour = factory->CreateGxTour();
  tour->set_name("Play me!");
  GxPlaylistPtr playlist = factory->CreateGxPlaylist();
  TourComposer tour_composer(playlist, flyto_duration, dwell_duration);
  // Both VisitFeatureHierarchy and GetRootFeature are well-behaved with
  // respect to NULL returns.
  VisitFeatureHierarchy(GetRootFeature(kml_file->get_root()),
                        tour_composer);
  tour->set_gx_playlist(playlist);
  return tour;
}

static bool WriteTour(const GxTourPtr& tour, const char* outfile) {
  KmlPtr kml = KmlFactory::GetFactory()->CreateKml();
  kml->set_feature(tour);
  KmlFilePtr kmlfile = KmlFile::CreateFromImport(kml);
  if (!kmlfile) {
    cerr << "Error: could not create KmlFile from tour" << endl;
    return false;
  }

  std::string kml_data;
  kmlfile->SerializeToString(&kml_data);
  if (!kmlbase::File::WriteStringToFile(kml_data, outfile)) {
    cerr << "write failed: " << outfile << endl;
    return false;
  }
  cout << "wrote " << outfile << endl;
  return true;
}

int main(int argc, char** argv) {
  if (argc != 5) {
    cerr << "usage: " << argv[0] <<
      " input.kml flyto_duration wait_duration output.kml" << endl;
    return -1;
  }

  std::string kml_data;
  if (!kmlbase::File::ReadFileToString(argv[1], &kml_data)) {
    cerr << "error: read of " << argv[1] << " failed" << endl;
    return false;
  }

  std::string errors;
  KmlFilePtr kml_file = KmlFile::CreateFromParse(kml_data, &errors);
  if (!kml_file) {
    cerr << "parse failed: " << errors << endl;;
    return 1;
  }

  const double flyto_duration = strtod(argv[2], NULL);
  const double dwell_duration = strtod(argv[3], NULL);
  GxTourPtr tour = GenerateTour(kml_file, flyto_duration, dwell_duration);
  if (!tour) {
    cerr << "Could not create tour from " << argv[1] << endl;;
    return 1;
  }
  return WriteTour(tour, argv[4]) ? 0 : 1;
}

