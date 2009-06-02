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

// This is the SWIG interface file for the KML Engine.
// This was developed with SWIG version 1.3.35.

%module kmlengine

%import "kmldom.i"

// Include the headers needed to build kmlengine_wrap.cc.
%{
#include "kml/dom.h"
#include "kml/engine.h"
%}

// Tell SWIG about C++ Standard Library std::string.
%include "std_string.i"

// Tell SWIG about the KML DOM intrusive_ptr typedefs.
%include "kml/dom/kml_ptr.h"

%include "typemaps.i"

namespace kmlengine {

class Bbox {
 public:
  Bbox();
  Bbox(double north, double south, double east, double west);
  bool Contains(double latitude, double longitude) const;
  void ExpandFromBbox(const Bbox& bbox);
  void ExpandLatitude(double latitude);
  void ExpandLongitude(double longitude);
  void ExpandLatLon(double latitude, double longitude);
  double get_north() const;
  double get_south() const;
  double get_east() const;
  double get_west() const;
};

kmldom::ElementPtr Clone(const kmldom::ElementPtr& element);

bool GetFeatureBounds(const kmldom::FeaturePtr& feature, Bbox* bbox);

%apply double* OUTPUT { double* lat };
%apply double* OUTPUT { double* lon };
bool GetFeatureLatLon(const kmldom::FeaturePtr& feature,
                      double* lat, double* lon);

const kmldom::FeaturePtr GetRootFeature(const kmldom::ElementPtr& root);

%nodefaultctor KmlFile;
%apply std::string* OUTPUT { std::string* errors };
%apply std::string* OUTPUT { std::string* xml_output };
class KmlFile {
 public:
  static KmlFile* CreateFromParse(const std::string& kml_data,
                                  std::string* errors);
  static KmlFile* CreateFromImport(const kmldom::ElementPtr& root);
  const kmldom::ElementPtr get_root() const;
  bool SerializeToString(std::string* xml_output) const;
  kmldom::ObjectPtr GetObjectById(const std::string& id) const;
  kmldom::StyleSelectorPtr GetSharedStyleById(const std::string& id) const;
};

%nodefaultctor KmzFile;
%apply std::string* OUTPUT { std::string* output };
class KmzFile {
 public:
  static KmzFile* OpenFromFile(const char* kmz_filepath);
  static KmzFile* CreateFromString(const std::string& kmz_data);
  bool ReadKml(std::string* output) const;
  bool ReadFile(const char* subfile, std::string* output) const;
};

%apply std::string* OUTPUT { std::string* kmz_url };
%apply std::string* OUTPUT { std::string* kmz_path };
bool KmzSplit(const std::string& kml_url, std::string* kmz_url,
              std::string* kmz_path);

%apply std::string* OUTPUT { std::string* scheme };
%apply std::string* OUTPUT { std::string* host };
%apply std::string* OUTPUT { std::string* port };
%apply std::string* OUTPUT { std::string* path };
%apply std::string* OUTPUT { std::string* query };
%apply std::string* OUTPUT { std::string* fragment };
bool SplitUri(const std::string& uri, std::string* scheme, std::string* host,
              std::string* port, std::string* path, std::string* query,
              std::string* fragment);

// TODO:
// void ProcessUpdate(const kmldom::UpdatePtr& update, KmlFile* kml_file);

// TODO:
// std::string CreateBalloonText(const KmlFilePtr& kml_file,
//                              const kmldom::FeaturePtr& feature);

// TODO:
// kmldom::StylePtr CreateResolvedStyle(const kmldom::FeaturePtr& feature,
//                                     const KmlFilePtr& kml_file,
//                                     kmldom::StyleStateEnum style_state);


}  // end namespace kmlengine
