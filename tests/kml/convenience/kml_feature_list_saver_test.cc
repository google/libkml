// Copyright 2010, Google Inc. All rights reserved.
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

#include "kml/convenience/kml_feature_list_saver.h"
#include "gtest/gtest.h"

namespace kmlconvenience {

class KmlFeatureListSaverTest : public testing::Test {
 protected:
};

TEST_F(KmlFeatureListSaverTest, TestBasicSaveToFeatureList) {
  FeatureList feature_list;
  KmlFeatureListSaver kml_saver(&feature_list, NULL, NULL);
  kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = kml_factory->CreatePlacemark();
  const string id("placemark-id");
  placemark->set_id(id);
  ASSERT_TRUE(kml_saver.StartElement(placemark));
  kmldom::PointPtr point = kml_factory->CreatePoint();
  // Verify that a Feature would be constructed.
  kmldom::FolderPtr folder = kml_factory->CreateFolder();
  // Verify that a Feature is not given to a parent...
  ASSERT_FALSE(kml_saver.EndElement(folder, placemark));
  ASSERT_EQ(static_cast<size_t>(0), folder->get_feature_array_size());
  // ... and that it winds up in the FeatureList.
  ASSERT_EQ(static_cast<size_t>(1), feature_list.Size());
  ASSERT_EQ(static_cast<size_t>(1), feature_list.Save(folder));
  ASSERT_EQ(static_cast<size_t>(1), folder->get_feature_array_size());
  // Verify that this is our Placemark.
  kmldom::PlacemarkPtr got_placemark =
      kmldom::AsPlacemark(folder->get_feature_array_at(0));
  ASSERT_TRUE(placemark);
  ASSERT_EQ(id, placemark->get_id());
}

TEST_F(KmlFeatureListSaverTest, TestBasicSaveToSharedStyleMap) {
  FeatureList feature_list;
  kmlengine::SharedStyleMap shared_style_map;
  KmlFeatureListSaver kml_saver(&feature_list, &shared_style_map, NULL);
  kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
  kmldom::StylePtr style = kml_factory->CreateStyle();
  const string id("style-id");
  style->set_id(id);
  kmldom::DocumentPtr document = kml_factory->CreateDocument();
  // Verify that a Style is not given to a Document...
  ASSERT_FALSE(kml_saver.EndElement(document, style));
  // ...and that it winds up in the SharedStyleMap.
  ASSERT_EQ(static_cast<size_t>(1), shared_style_map.size());
  ASSERT_EQ(id, shared_style_map[id]->get_id());
}

TEST_F(KmlFeatureListSaverTest, TestBasicStyleUrlChange) {
  FeatureList feature_list;
  const string kStyleKml("styles-are-here.kml");
  KmlFeatureListSaver kml_saver(&feature_list, NULL, kStyleKml.c_str());
  kmldom::KmlFactory* kml_factory = kmldom::KmlFactory::GetFactory();
  kmldom::FolderPtr folder = kml_factory->CreateFolder();
  kmldom::PlacemarkPtr placemark = kml_factory->CreatePlacemark();
  const string kStyleId("#style-id");
  placemark->set_styleurl(kStyleId);
  ASSERT_FALSE(kml_saver.EndElement(folder, placemark));
  ASSERT_EQ(kStyleKml + kStyleId, placemark->get_styleurl());
}

}  // end namespace kmlconvenience
