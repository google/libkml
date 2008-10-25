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

// This file contains the unit tests for the abstract Container element.

#include "kml/dom/container.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/placemark.h"
#include "gtest/gtest.h"

namespace kmldom {

class ContainerTest : public testing::Test {
 protected:
  virtual void SetUp() {
    container_ = new TestContainer();
  }

  // Container is abstract, hence its constructor is protected.
  class TestContainer : public Container {
  };
  boost::intrusive_ptr<TestContainer> container_;
};

TEST_F(ContainerTest, TestType) {
  ASSERT_TRUE(container_->IsA(Type_Container));
  ASSERT_TRUE(container_->IsA(Type_Feature));
  ASSERT_TRUE(container_->IsA(Type_Object));
}

TEST_F(ContainerTest, TestAddGetFeatures) {
  // Put each type of Feature in the Container.
  KmlFactory* factory = KmlFactory::GetFactory();
  container_->add_feature(factory->CreateDocument());
  container_->add_feature(factory->CreateFolder());
  container_->add_feature(factory->CreateGroundOverlay());
  container_->add_feature(factory->CreateNetworkLink());
  container_->add_feature(factory->CreatePhotoOverlay());
  container_->add_feature(factory->CreatePlacemark());
  container_->add_feature(factory->CreateScreenOverlay());
  // Verify order is preserved.
  ASSERT_EQ((size_t)7, container_->get_feature_array_size());
  ASSERT_EQ(Type_Document,
            container_->get_feature_array_at(0)->Type());
  ASSERT_EQ(Type_Folder,
            container_->get_feature_array_at(1)->Type());
  ASSERT_EQ(Type_GroundOverlay,
            container_->get_feature_array_at(2)->Type());
  ASSERT_EQ(Type_NetworkLink,
            container_->get_feature_array_at(3)->Type());
  ASSERT_EQ(Type_PhotoOverlay,
            container_->get_feature_array_at(4)->Type());
  ASSERT_EQ(Type_Placemark,
            container_->get_feature_array_at(5)->Type());
  ASSERT_EQ(Type_ScreenOverlay,
            container_->get_feature_array_at(6)->Type());
  // Deleting the container deletes all Features in the array.
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
