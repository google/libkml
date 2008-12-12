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

// This program converts a GPX track log to KML.  Each GPX <trkpt> is converted
// to a KML <Point> <Placemark> with <TimeStamp> and <ExtendedData>.   All
// other GPX elements are ignored.  The <Placemark>'s are arranged in
// <Folder>'s by date and "trip".  Note that "trip" is not the same as the GPX
// file <trk> or <trkseg>; "trip" is determined solely by point timestamp and
// an internal threshold for determining when a new trip begins.

#include <time.h>
#include <iostream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/date_time.h"
#include "kml/base/expat_parser.h"
#include "kml/base/file.h"
#include "kml/base/vec3.h"
#include "kml/convenience/convenience.h"
#include "kml/convenience/gpx_trk_pt_handler.h"
#include "kml/dom.h"

using kmlbase::ExpatParser;
using kmlbase::DateTime;
using kmlbase::Vec3;
using kmldom::ContainerPtr;
using kmldom::FolderPtr;
using kmldom::IconStylePtr;
using kmldom::IconStyleIconPtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::LabelStylePtr;
using kmldom::ListStylePtr;
using kmldom::PairPtr;
using kmldom::PointPtr;
using kmldom::PlacemarkPtr;
using kmldom::TimeStampPtr;
using kmldom::StylePtr;
using kmldom::StyleMapPtr;

static const char kDotIcon[] =
    "http://maps.google.com/mapfiles/kml/shapes/shaded_dot.png";

static const int kTripThreshold = 600;  // 10 minutes.

static IconStylePtr CreateIconStyle(double scale) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  IconStyleIconPtr icon = kml_factory->CreateIconStyleIcon();
  icon->set_href(kDotIcon);
  IconStylePtr icon_style = kml_factory->CreateIconStyle();
  icon_style->set_icon(icon);
  icon_style->set_scale(scale);
  return icon_style;
}

static LabelStylePtr CreateLabelStyle(double scale) {
  LabelStylePtr label_style = KmlFactory::GetFactory()->CreateLabelStyle();
  label_style->set_scale(scale);
  return label_style;
}

static PairPtr CreatePair(int style_state, double icon_scale) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  PairPtr pair = kml_factory->CreatePair();
  pair->set_key(style_state);
  StylePtr style = kml_factory->CreateStyle();
  style->set_iconstyle(CreateIconStyle(icon_scale));
  // Hide the label in normal style state, visible in highlight.
  style->set_labelstyle(CreateLabelStyle(
      style_state == kmldom::STYLESTATE_NORMAL ? 0 : 1 ));
  pair->set_styleselector(style);
  return pair;
}

static StylePtr CreateRadioFolder(const char* id) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  ListStylePtr list_style = kml_factory->CreateListStyle();
  list_style->set_listitemtype(kmldom::LISTITEMTYPE_RADIOFOLDER);
  StylePtr style = kml_factory->CreateStyle();
  style->set_liststyle(list_style);
  style->set_id(id);
  return style;
}

static StyleMapPtr CreateStyleMap(const char* id) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  StyleMapPtr style_map = kml_factory->CreateStyleMap();
  style_map->set_id(id);
  style_map->add_pair(CreatePair(kmldom::STYLESTATE_NORMAL, 1.1));
  style_map->add_pair(CreatePair(kmldom::STYLESTATE_HIGHLIGHT, 1.3));
  return style_map;
}

// Convert GPX <trkpt>'s to KML <Placemark>'s.
// For example, this GPX <trkpt>:
// <trkpt lat="-33.911973070" lon="18.422974152">
//   <ele>4.943848</ele>
//   <time>2008-10-11T14:55:41Z</time>
// </trkpt>
// ...is translated to this KML <Placemark>:
// <Placemark>
//   <name>15:50:48</name>
//   <TimeStamp><when>2008-10-01T15:50:48Z</when></TimeStamp>
//   <ExtendedData>
//     <Data name="date"><value>2008-10-01</value></Data>
//     <Data name="time"><value>15:50:48</value></Data>
//   </ExtendedData>
//   <Point><coordinates>18.427167954,-33.911966113,0</coordinates></Point>
// </Placemark>
// There is a <Folder> for each day named in xsd:date format (YYYY-MM-DD) and
// a sub-<Folder> within each day for each segment of <trkpt>'s.  Note that
// this is not the same as the GPX <trgseg>.  Instead a threshold is used to
// determine a break in a segment.  (In practice a GPS unit may lose signal
// for far longer than is represented in a <trkseg>).  This threshold is
// intended to represent a best guess for when one real-world trip ends and a
// new one begins.  This handler ignores all other elements in GPX.
// TODO: Ideally each user-visible time is converted to the timezone of the
// point.
class TrkPtHandler : public kmlconvenience::GpxTrkPtHandler {
 public:
  // Create the TrkPtHandler with a KML Container into which all KML generated
  // here is appended.  Each <trkpt> is saved as a <Point> <Placemark> whose
  // <styleUrl> is set to the given shared style.
  TrkPtHandler(ContainerPtr container, const char* point_style_id)
    : root_container_(container),
      point_style_id_(point_style_id),
      last_time_(0) {
  }

  // This is called for each <trkpt>.
  // Create a <Point> <Placemark> at the location specified by Vec3 and give
  // it a <TimeStamp> based on when.
  virtual void HandlePoint(const kmlbase::Vec3& where,
                           const std::string& when) {
    boost::scoped_ptr<DateTime> date_time(DateTime::Create(when));
    if (!date_time.get()) {
      std::cerr << "bad DateTime " << when << std::endl;
      return;
    }
    PointPtr point = kmlconvenience::CreatePointFromVec3(where);
    FolderPtr folder = ManageFolders(*date_time);
    folder->add_feature(kmlconvenience::CreatePointPlacemarkWithTimeStamp(
        point, *date_time, point_style_id_));
  }

 private:
  // Create and/or return the folder for this DateTime.  This method owns the
  // creation of the folder hierarchy.  The first level of hierarchy is a
  // folder for each date within which is a folder for each "trip".  It is
  // presumed that the DateTime's are presented to this method in time-order.
  FolderPtr ManageFolders(/*const*/ DateTime& date_time) {
    if (date_time.GetTimeT() - last_time_ > kTripThreshold) {
      // Threshold exceeded.  Save trip and start a new folder.
      if (!date_folder_) {  // First time around.
        date_folder_ = kmldom::KmlFactory::GetFactory()->CreateFolder();
        date_folder_->set_name(last_date_ = date_time.GetXsdDate());
      }
      date_folder_->add_feature(trip_folder_);
      trip_folder_ = kmldom::KmlFactory::GetFactory()->CreateFolder();
      trip_folder_->set_name(date_time.GetXsdTime());
    }
    last_time_ = date_time.GetTimeT();
    if (last_date_ != date_time.GetXsdDate()) {
      // Dawn.  Save the previous day.
      root_container_->add_feature(date_folder_);
      date_folder_ = kmldom::KmlFactory::GetFactory()->CreateFolder();
      date_folder_->set_name(last_date_ = date_time.GetXsdDate());
    }
    return trip_folder_;
  }

  ContainerPtr root_container_;
  const char* point_style_id_;
  FolderPtr date_folder_;
  FolderPtr trip_folder_;
  PointPtr point_;
  boost::scoped_ptr<DateTime> date_time_;
  bool gather_char_data_;
  std::string char_data_;
  time_t last_time_;
  std::string last_date_;
};

static bool ConvertGpxTrkPtsToKml(const char* gpx_pathname,
                                  const char* kml_pathname) {
  kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
  // Create a <Document> to hand to the GPX parser.
  kmldom::DocumentPtr document = kml_factory->CreateDocument();
  // Make the <Document> a radio folder.
  const char* kRadioFolderId = "radio-folder-style";
  document->add_styleselector(CreateRadioFolder(kRadioFolderId));
  // <Document> has no inline style of its own so point its styleUrl to the
  // shared style.
  document->set_styleurl(std::string("#") + kRadioFolderId);
  // Add a shared style for all the Placemark's to use.
  const char* kStyleMapId = "style-map";
  document->add_styleselector(CreateStyleMap(kStyleMapId));

  // Create the GPX parser directing its output to the <Document>.
  TrkPtHandler trk_pt_handler(document, kStyleMapId);

  // Read the GPX file contents.
  std::string gpx_data;
  if (!kmlbase::File::ReadFileToString(gpx_pathname, &gpx_data)) {
    std::cerr << "read failed: " << gpx_pathname << std::endl;
    return false;
  }

  // Parse the GPX data into the <Document>.
  std::string errors;

  if (!ExpatParser::ParseString(gpx_data, &trk_pt_handler, &errors, false)) {
    std::cerr << "parse failed: " << gpx_pathname << std::endl;
    return false;
  }

  // Put the <Document> in a <kml> element and write everything out to the
  // supplied pathname.
  kmldom::KmlPtr kml = kml_factory->CreateKml();
  kml->set_feature(document);
  std::string kml_data = kmldom::SerializePretty(kml);

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
  return ConvertGpxTrkPtsToKml(argv[1], argv[2]) ? 0 : 1;
}

