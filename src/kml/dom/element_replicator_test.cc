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

// This file contains the unit tests for the Clone() function and the
// ElementReplicator class.

#include "kml/dom/element_replicator.h"
#include <string>
#include "kml/dom/attributes.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kmldom.h"
#include "kml/util/unit_test.h"

namespace kmldom {

class ElementReplicatorTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ElementReplicatorTest);
  CPPUNIT_TEST(TestEmptyClone);
  CPPUNIT_TEST(TestCloneFields);
  CPPUNIT_TEST(TestCloneChildren);
  CPPUNIT_TEST(TestCloneArray);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestEmptyClone();
  void TestCloneFields();
  void TestCloneChildren();
  void TestCloneArray();

 public:
  // Called before each test.
  void setUp() {
    coordinates_ = KmlFactory::GetFactory()->CreateCoordinates();
    folder_ = KmlFactory::GetFactory()->CreateFolder();
    groundoverlay_ = KmlFactory::GetFactory()->CreateGroundOverlay();
    placemark_ = KmlFactory::GetFactory()->CreatePlacemark();
    point_ = KmlFactory::GetFactory()->CreatePoint();
  }

  // Called after each test.
  void tearDown() {
    // Smart pointers free everything.
  }

 private:
  CoordinatesPtr coordinates_;
  FolderPtr folder_;
  GroundOverlayPtr groundoverlay_;
  PlacemarkPtr placemark_;
  PointPtr point_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ElementReplicatorTest);

// Verify that a empty complex element is cloned properly.
void ElementReplicatorTest::TestEmptyClone() {
  ElementPtr clone = Clone(placemark_);
  CPPUNIT_ASSERT_EQUAL(clone->Type(), placemark_->Type());
}

// Verify that a complex element with some fields clones properly.
void ElementReplicatorTest::TestCloneFields() {
  const std::string kName("clone my name");
  const std::string kId("clone-my-id");
  const bool kVisibility = false;

  // Set the fields.
  placemark_->set_name(kName);
  placemark_->set_id(kId);
  placemark_->set_visibility(kVisibility);

  // Clone it.
  ElementPtr clone = Clone(placemark_);

  // Verify all values were set in the clone.
  CPPUNIT_ASSERT_EQUAL(clone->Type(), placemark_->Type());
  PlacemarkPtr cloned_placemark = AsPlacemark(clone);
  CPPUNIT_ASSERT(cloned_placemark->has_id());
  CPPUNIT_ASSERT_EQUAL(kId, cloned_placemark->get_id());
  CPPUNIT_ASSERT(cloned_placemark->has_name());
  CPPUNIT_ASSERT_EQUAL(kName, cloned_placemark->get_name());
  CPPUNIT_ASSERT(cloned_placemark->has_visibility());
  CPPUNIT_ASSERT_EQUAL(kVisibility, cloned_placemark->get_visibility());
}

// Verify that a complex element with both some fields and complex children
// clones properly.
void ElementReplicatorTest::TestCloneChildren() {
  const std::string kDescription("clone my description");
  const std::string kId("clone-my-id");
  const bool kOpen = false;

  // Set some fields.
  folder_->set_description(kDescription);
  folder_->set_id(kId);
  folder_->set_open(kOpen);
  folder_->add_feature(placemark_);

  // Clone it.
  ElementPtr clone = Clone(folder_);

  // Verify all values were set in the clone.
  CPPUNIT_ASSERT_EQUAL(clone->Type(), folder_->Type());
  FolderPtr cloned_folder = AsFolder(clone);
  CPPUNIT_ASSERT(cloned_folder->has_id());
  CPPUNIT_ASSERT_EQUAL(kId, cloned_folder->get_id());
  CPPUNIT_ASSERT(!cloned_folder->has_name());
  CPPUNIT_ASSERT(cloned_folder->has_description());
  CPPUNIT_ASSERT_EQUAL(kDescription, cloned_folder->get_description());
  CPPUNIT_ASSERT(!cloned_folder->has_visibility());
  CPPUNIT_ASSERT(cloned_folder->has_open());
  CPPUNIT_ASSERT_EQUAL(kOpen, cloned_folder->get_open());
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1),
                       cloned_folder->get_feature_array_size());
  CPPUNIT_ASSERT_EQUAL(placemark_->Type(),
                       cloned_folder->get_feature_array_at(0)->Type());
}

// Verify that an element with an array of complex children and fields
// clones properly.
void ElementReplicatorTest::TestCloneArray() {
  // <Folder><Placemark><Point/><Placemark/><GroundOverlay/></Folder>
  placemark_->set_geometry(point_);
  folder_->add_feature(placemark_);
  folder_->add_feature(groundoverlay_);

  // Clone the Folder.
  ElementPtr clone = Clone(folder_);

  // Verify the contents of the Feature array child in the Folder.
  FolderPtr cloned_folder = AsFolder(clone);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2),
                       cloned_folder->get_feature_array_size());
  CPPUNIT_ASSERT_EQUAL(placemark_->Type(),
                       cloned_folder->get_feature_array_at(0)->Type());
  CPPUNIT_ASSERT_EQUAL(groundoverlay_->Type(),
                       cloned_folder->get_feature_array_at(1)->Type());

  // Verify the Placemark has a Point Geometry.
  PlacemarkPtr cloned_placemark =
      AsPlacemark(cloned_folder->get_feature_array_at(0));
  CPPUNIT_ASSERT(cloned_placemark->has_geometry());
  CPPUNIT_ASSERT_EQUAL(point_->Type(),
                       cloned_placemark->get_geometry()->Type());
}

}  // end namespace kmldom

TEST_MAIN
