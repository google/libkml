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

#include "kml/engine/update.h"
#include "gtest/gtest.h"
#include "kml/dom.h"
#include "kml/engine/kml_file.h"

using kmldom::ContainerPtr;
using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::UpdatePtr;

namespace kmlengine {

static const char source_change[] =
    "<Update>"
    "<targetHref/>"
    "<Change>"
    "<Placemark targetId=\"p\">"
    "<name>NEW NAME</name>"
    "</Placemark>"
    "</Change>"
    "</Update>";

static const char target_change[] = 
    "<Placemark id=\"p\"><name>hi</name></Placemark>";

TEST(UpdateTest, TestSingleSimpleChange) {
  KmlFilePtr target_file = KmlFile::CreateFromParse(target_change, NULL);
  ASSERT_TRUE(target_file);
  PlacemarkPtr target_placemark =
      kmldom::AsPlacemark(target_file->GetObjectById("p"));
  ASSERT_EQ(std::string("hi"), target_placemark->get_name());
  ASSERT_TRUE(target_placemark);
  UpdatePtr update = AsUpdate(kmldom::Parse(source_change, NULL));
  ASSERT_TRUE(update);
  ProcessUpdate(update, target_file);
  ASSERT_EQ(std::string("NEW NAME"), target_placemark->get_name());
  ASSERT_FALSE(target_placemark->has_targetid());
}

static const char source_create[] =
    "<Update>"
    "<targetHref/>"
    "<Create>"
    "<Folder targetId=\"f\">"
    "<Placemark id=\"px\">"
    "<name>Update-Created Placemark</name>"
    "<Point>"
    "<coordinates>-11.11,22,22</coordinates>"
    "</Point>"
    "</Placemark>"
    "</Folder>"
    "</Create>"
    "</Update>";

static const char target_create[] = "<Folder id=\"f\"/>";

TEST(UpdateTest, TestSingleSimpleCreate) {
  KmlFilePtr target_file = KmlFile::CreateFromParse(target_create, NULL);
  ASSERT_TRUE(target_file);
  FolderPtr folder = kmldom::AsFolder(target_file->get_root());
  ASSERT_TRUE(folder);
  ASSERT_EQ(static_cast<size_t>(0), folder->get_feature_array_size());
  UpdatePtr update = AsUpdate(kmldom::Parse(source_create, NULL));
  ASSERT_TRUE(update);
  ProcessUpdate(update, target_file);
  ASSERT_EQ(static_cast<size_t>(1), folder->get_feature_array_size());
  ASSERT_EQ(std::string("Update-Created Placemark"),
            folder->get_feature_array_at(0)->get_name());
}

// Add one of each kind of Feature to the Container.
static void AddFeatures(ContainerPtr container) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  container->add_feature(kml_factory->CreateDocument());
  container->add_feature(kml_factory->CreateFolder());
  container->add_feature(kml_factory->CreateGroundOverlay());
  container->add_feature(kml_factory->CreateNetworkLink());
  container->add_feature(kml_factory->CreatePhotoOverlay());
  container->add_feature(kml_factory->CreatePlacemark());
  container->add_feature(kml_factory->CreateScreenOverlay());
}

static void CheckFeatures(const ContainerPtr& container, size_t offset) {
  ASSERT_LT(offset + 6, container->get_feature_array_size());
  ASSERT_EQ(kmldom::Type_Document,
            container->get_feature_array_at(offset)->Type());
  ASSERT_EQ(kmldom::Type_Folder,
            container->get_feature_array_at(offset + 1)->Type());
  ASSERT_EQ(kmldom::Type_GroundOverlay,
            container->get_feature_array_at(offset + 2)->Type());
  ASSERT_EQ(kmldom::Type_NetworkLink,
            container->get_feature_array_at(offset + 3)->Type());
  ASSERT_EQ(kmldom::Type_PhotoOverlay,
            container->get_feature_array_at(offset + 4)->Type());
  ASSERT_EQ(kmldom::Type_Placemark,
            container->get_feature_array_at(offset + 5)->Type());
  ASSERT_EQ(kmldom::Type_ScreenOverlay,
            container->get_feature_array_at(offset + 6)->Type());
}

TEST(UpdateTest, TestCopyFeatures) {
  const std::string kTargetName("target name");
  const std::string kTargetFeature0Name("0th feature");
  const std::string kTargetFeature1Name("1th feature");
  const std::string kTargetFeature9Name("9th feature");
  const std::string kSourceName("source name");
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  ContainerPtr source = kml_factory->CreateDocument();
  source->set_name(kSourceName);
  ContainerPtr target = kml_factory->CreateFolder();
  target->set_name(kTargetName);
  PlacemarkPtr placemark = kml_factory->CreatePlacemark();
  placemark->set_name(kTargetFeature0Name);
  target->add_feature(placemark);
  placemark = kml_factory->CreatePlacemark();
  placemark->set_name(kTargetFeature1Name);
  target->add_feature(placemark);
  AddFeatures(source);
  // Run the function under test.
  CopyFeatures(source, target);
  ASSERT_EQ(static_cast<size_t>(9), target->get_feature_array_size());
  // Only Features are copied.
  ASSERT_EQ(kTargetName, target->get_name());
  ASSERT_EQ(kSourceName, source->get_name());
  // Verify that original features are intact in target.
  ASSERT_EQ(kTargetFeature0Name, target->get_feature_array_at(0)->get_name());
  ASSERT_EQ(kTargetFeature1Name, target->get_feature_array_at(1)->get_name());
  // Verify that the copied features are now in the target.
  CheckFeatures(target, 2);
  // Add another Feature to the target directly.
  placemark = kml_factory->CreatePlacemark();
  placemark->set_name(kTargetFeature9Name);
  target->add_feature(placemark);
  // Copy from the source once again.
  CopyFeatures(source, target);
  ASSERT_EQ(static_cast<size_t>(17), target->get_feature_array_size());
  // Verify that original features are intact in target.
  ASSERT_EQ(kTargetFeature0Name, target->get_feature_array_at(0)->get_name());
  ASSERT_EQ(kTargetFeature1Name, target->get_feature_array_at(1)->get_name());
  ASSERT_EQ(kTargetFeature9Name, target->get_feature_array_at(9)->get_name());
  // Verify that the copied features are now in the target.
  CheckFeatures(target, 2);
  CheckFeatures(target, 10);
}

static const char source_delete[] =
    "<Update>"
    "<targetHref/>"
    "<Delete>"
    "<Placemark id=\"p\"/>"
    "</Delete>"
    "</Update>";

static const char target_delete[] = "<Folder><Placemark id=\"p\"/></Folder>";

TEST(UpdateTest, TestSingleSimpleDelete) {
  KmlFilePtr target_file = KmlFile::CreateFromParse(target_delete, NULL);
  ASSERT_TRUE(target_file);
  UpdatePtr update = AsUpdate(kmldom::Parse(source_delete, NULL));
  ASSERT_TRUE(update);
  ProcessUpdate(update, target_file);
  // TODO:
  // Verify the Placemark has been removed from the Folder.
  // ASSERT_EQ(static_cast<size_t>(0),
  //           AsFeature(kml_file->get_root())->get_feature_array_size());
  // Verify the KmlFile's id mapping for the Placemark is gone.
  // ASSERT_FALSE(target_file->GetObjectById("p"));
}

// TODO: a table of a bunch of data-driven test case files

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
