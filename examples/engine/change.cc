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

// This program shows basic ProcessUpdate() with <Change>.

#include <iostream>
#include "kml/convenience/convenience.h"
#include "kml/dom.h"
#include "kml/engine.h"

using kmldom::ChangePtr;
using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::PlacemarkPtr;
using kmldom::UpdatePtr;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;

static void HelloUpdateChange() {
  // This is the same KML as in the kmlfile.cc example.
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  FolderPtr folder = kml_factory->CreateFolder();
  folder->set_id("f0");
  folder->set_name("Folder 0");
  PlacemarkPtr placemark = kml_factory->CreatePlacemark();
  placemark->set_id("pm0");
  placemark->set_name("Placemark 0");
  folder->add_feature(placemark);
  placemark = kml_factory->CreatePlacemark();
  placemark->set_id("pm1");
  placemark->set_name("Placemark 1");
  folder->add_feature(placemark);
  KmlPtr kml = kml_factory->CreateKml();
  kml->set_feature(folder);

  // Importing to a KmlFile creates an internal database of object id mappings.
  KmlFilePtr kml_file = KmlFile::CreateFromImport(kml);

  ChangePtr change = kml_factory->CreateChange();
  placemark = kmlconvenience::CreatePointPlacemark("new name", 38, -120);
  placemark->set_targetid("pm0");
  change->add_object(placemark);
  UpdatePtr update = kml_factory->CreateUpdate();
  update->add_updateoperation(change);

  kmlengine::ProcessUpdate(update, kml_file);

  std::string xml;
  kml_file->SerializeToString(&xml);
  std::cout << xml;
}

int main(int argc, char** argv) {
  HelloUpdateChange();
  return 0;
}
