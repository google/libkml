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

// This file contains the unit tests for the GetElementsById() function
// and the IdMapper class.

#include "kml/engine/id_mapper.h"
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kmldom.h"
#include "kml/dom/xsd.h"
#include "gtest/gtest.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlbase::File;
using kmldom::CoordinatesPtr;
using kmldom::ElementPtr;
using kmldom::FolderPtr;
using kmldom::GroundOverlayPtr;
using kmldom::KmlFactory;
using kmldom::ObjectPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;

namespace kmlengine {

class IdMapperTest : public testing::Test {
 protected:
  virtual void SetUp() {
    coordinates_ = KmlFactory::GetFactory()->CreateCoordinates();
    folder0_ = KmlFactory::GetFactory()->CreateFolder();
    folder1_ = KmlFactory::GetFactory()->CreateFolder();
    groundoverlay_ = KmlFactory::GetFactory()->CreateGroundOverlay();
    placemark0_ = KmlFactory::GetFactory()->CreatePlacemark();
    placemark1_ = KmlFactory::GetFactory()->CreatePlacemark();
    placemark2_ = KmlFactory::GetFactory()->CreatePlacemark();
    point0_ = KmlFactory::GetFactory()->CreatePoint();
    point1_ = KmlFactory::GetFactory()->CreatePoint();
    id_mapper_.reset(new IdMapper(&object_id_map_, &dup_id_vector_));
  }

  CoordinatesPtr coordinates_;
  FolderPtr folder0_;
  FolderPtr folder1_;
  GroundOverlayPtr groundoverlay_;
  PlacemarkPtr placemark0_;
  PlacemarkPtr placemark1_;
  PlacemarkPtr placemark2_;
  PointPtr point0_;
  PointPtr point1_;
  ObjectIdMap object_id_map_;
  ElementVector dup_id_vector_;
  boost::scoped_ptr<IdMapper> id_mapper_;
};

TEST_F(IdMapperTest, TestIdMapperEmpty) {
  // Save an element with no id.
  id_mapper_->SaveElement(folder0_);
  ASSERT_TRUE(dup_id_vector_.empty());
  ASSERT_TRUE(object_id_map_.empty());
}

TEST_F(IdMapperTest, TestIdMapperOne) {
  // Save an element with an id.
  const std::string kPlacemark0Id("id-of-placemark0");
  placemark0_->set_id(kPlacemark0Id);
  id_mapper_->SaveElement(placemark0_);
  ASSERT_TRUE(dup_id_vector_.empty());
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  ASSERT_EQ(kPlacemark0Id, object_id_map_[kPlacemark0Id]->get_id());
}

// Verify MapIds() on an element with an id.
TEST_F(IdMapperTest, TestMapIdsOne) {
  // Save an element with an id.
  const std::string kPlacemark0Id("id-of-placemark0");
  placemark0_->set_id(kPlacemark0Id);
  MapIds(placemark0_, &object_id_map_, &dup_id_vector_);
  ASSERT_TRUE(dup_id_vector_.empty());
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  ASSERT_EQ(kPlacemark0Id, object_id_map_[kPlacemark0Id]->get_id());
}

// Verify MapIds() is well behaved when presented with no dup vector pointer.
TEST_F(IdMapperTest, TestMapIdsNoDupVector) {
  // Save an element with an id.
  const std::string kPlacemark0Id("id-of-placemark0");
  placemark0_->set_id(kPlacemark0Id);
  MapIds(placemark0_, &object_id_map_, NULL);
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  ASSERT_EQ(kPlacemark0Id, object_id_map_[kPlacemark0Id]->get_id());
}

TEST_F(IdMapperTest, TestIdMapperDupe) {
  // Save an element whose child has the same id.
  const std::string kDupeId("xml-does-not-like-duplicate-ids");
  point0_->set_id(kDupeId);
  placemark0_->set_id(kDupeId);
  placemark0_->set_geometry(point0_);
  id_mapper_->SaveElement(placemark0_);
  // The dupe will be the parent due element simply because it comes first.
  ASSERT_EQ(static_cast<size_t>(1), dup_id_vector_.size());
  ASSERT_EQ(kmldom::Type_Placemark, dup_id_vector_[0]->Type());
  // The id map will hold the last item of this id.
  ASSERT_EQ(kDupeId, object_id_map_[kDupeId]->get_id());
  ASSERT_EQ(kmldom::Type_Point, object_id_map_[kDupeId]->Type());
}

TEST_F(IdMapperTest, TestNoIds) {
  MapIds(folder0_, &object_id_map_, &dup_id_vector_);
  ASSERT_TRUE(object_id_map_.empty());
  ASSERT_TRUE(dup_id_vector_.empty());
  folder0_->add_feature(placemark0_);
  folder0_->add_feature(placemark1_);
  folder0_->add_feature(placemark2_);
  // Still nothing with an id.
  MapIds(folder0_, &object_id_map_, &dup_id_vector_);
  ASSERT_TRUE(object_id_map_.empty());
  ASSERT_TRUE(dup_id_vector_.empty());
}

TEST_F(IdMapperTest, TestSomeIds) {
  const std::string kId0("id0");
  const std::string kName0("name0");
  placemark0_->set_id(kId0);
  placemark0_->set_name(kName0);
  const std::string kId1("id1");
  const std::string kName1("name1");
  placemark1_->set_id(kId1);
  placemark1_->set_name(kName1);
  folder0_->add_feature(placemark0_);
  folder0_->add_feature(placemark1_);
  MapIds(folder0_, &object_id_map_, &dup_id_vector_);
  // Verify the map has exactly the two expected mappings.
  ASSERT_EQ(static_cast<size_t>(2), object_id_map_.size());
  // Verifies 1) it was mapped, 2) it's a Placemark, 3) it has the right name.
  ASSERT_EQ(kName0, AsPlacemark(object_id_map_[kId0])->get_name());
  ASSERT_EQ(kName1, AsPlacemark(object_id_map_[kId1])->get_name());
  ASSERT_TRUE(dup_id_vector_.empty());
}

TEST_F(IdMapperTest, TestAllObjects) {
  std::string kml;
  ASSERT_TRUE(File::ReadFileToString(
      File::JoinPaths(DATADIR, File::JoinPaths("kml",
                                               "all-unknown-attrs-input.kml")),
      &kml));
  ElementPtr root = kmldom::Parse(kml, NULL);
  MapIds(root, &object_id_map_, &dup_id_vector_);
  ASSERT_TRUE(dup_id_vector_.empty());
  ASSERT_EQ(static_cast<size_t>(44), object_id_map_.size());
  ObjectIdMap::const_iterator iter = object_id_map_.begin();
  for (; iter != object_id_map_.end(); ++iter) {
    const ObjectPtr& object = iter->second;
    const std::string& id = iter->first;
    // The id of the object is the id that maps to the object.
    ASSERT_EQ(id, object->get_id());
    // Each id in all-unknown-attrs-input.kml is the same as the element name.
    ASSERT_EQ(kmldom::Xsd::GetSchema()->ElementId(id), object->Type());
  }
}

TEST_F(IdMapperTest, TestDupeDetection) {
  // Create a KML hierarchy with duplicate ids.
  const std::string kDupeId("some-id-to-duplicate");
  placemark0_->set_id(kDupeId);
  folder0_->set_id(kDupeId);
  folder0_->add_feature(placemark0_);
  // Call the method under test.
  MapIds(folder0_, &object_id_map_, &dup_id_vector_);
  ASSERT_EQ(static_cast<size_t>(1), dup_id_vector_.size());
}

TEST_F(IdMapperTest, TestClearIds) {
  placemark0_->set_id("placemark123");
  const std::string kFolderName("folder name");
  folder0_->set_id("folder123");
  folder0_->set_name(kFolderName);
  folder0_->add_feature(placemark0_);
  FolderPtr folder = AsFolder(ClearIds(folder0_));
  ASSERT_TRUE(folder);
  ASSERT_TRUE(folder->has_name());
  ASSERT_EQ(kFolderName, folder->get_name());
  ASSERT_EQ(static_cast<size_t>(1), folder->get_feature_array_size());
  ASSERT_FALSE(placemark0_->has_id());
  ASSERT_FALSE(folder0_->has_id());
}

TEST_F(IdMapperTest, TestClearManyIds) {
  std::string kml;
  ASSERT_TRUE(File::ReadFileToString(
      File::JoinPaths(DATADIR, File::JoinPaths("kml",
                                               "all-unknown-attrs-input.kml")),
      &kml));
  ElementPtr root = kmldom::Parse(kml, NULL);
  MapIds(root, &object_id_map_, NULL);
  ASSERT_EQ(static_cast<size_t>(44), object_id_map_.size());

  // Call the method under test.
  ClearIds(root);
  object_id_map_.clear();
  MapIds(root, &object_id_map_, NULL);
  ASSERT_TRUE(object_id_map_.empty());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
