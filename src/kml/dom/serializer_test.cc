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

// This file contains the unit tests for the abstract Serializer base class.

#include "kml/dom/serializer.h"
#include <string>
#include "kml/base/attributes.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kmldom.h"
#include "gtest/gtest.h"

using kmlbase::Attributes;

namespace kmldom {

class SerializerTest : public testing::Test {
 protected:
  virtual void SetUp() {
    folder_ = KmlFactory::GetFactory()->CreateFolder();
    placemark_ = KmlFactory::GetFactory()->CreatePlacemark();
    point_ = KmlFactory::GetFactory()->CreatePoint();
    region_ = KmlFactory::GetFactory()->CreateRegion();
  }

  FolderPtr folder_;
  PlacemarkPtr placemark_;
  PointPtr point_;
  RegionPtr region_;
};

// The NullSerializer implementation overrides no Serializer virtual methods.
// This verifies that Serializer has no pure virtual methods.
class NullSerializer : public Serializer {
};

// This Serialier implementation provides implementations for all virtual
// methods.  This should build and run and do nothing.
class MaximalSerializer : public Serializer {
 public:
  virtual void BeginById(int type_id, const Attributes& attributes) {}
  virtual void End() {}
  virtual void SaveElement(const ElementPtr& element) {}
  virtual void SaveElementGroup(const ElementPtr& element, int group_id) {}
  virtual void SaveStringFieldById(int type_id, std::string value) {}
  virtual void SaveContent(const std::string& content, bool maybe_quote) {}
  virtual void SaveLonLatAlt(double longitude, double latitude,
                             double altitude) {}
  virtual void Indent() {}
};

typedef std::vector<KmlDomType> TypeIdVector;

// By default Serializer recurses down every complex element.  By appending
// each visited complex element to a list in its BeginById() method this
// serializer provides the means to verify proper depth-first order recursion.
class HierarchicalSerializer : public Serializer {
 public:
  virtual void BeginById(int type_id, const Attributes& attributes) {
    type_id_vector_.push_back(static_cast<KmlDomType>(type_id));
  }

  const TypeIdVector& get_type_id_vector() const {
    return type_id_vector_;
  }

 private:
  TypeIdVector type_id_vector_;
};

typedef std::vector<ElementPtr> ElementVector;

// This serializer implements SaveElement() and does not recurse on complex
// child elements, but it does append to the list of those visited.
class FlatSerializer : public Serializer {
 public:
  virtual void SaveElement(const ElementPtr& element) {
    element_vector_.push_back(element);
  }

  const ElementVector& get_element_vector() const {
    return element_vector_;
  }

 private:
  ElementVector element_vector_;
};

// This Serializer implementation counts begin and end tags of complex elements
// and a count of all simple elements (fields).
class StatsSerializer : public Serializer {
 public:
  StatsSerializer()
      : begin_count_(0),
        end_count_(0),
        field_count_(0),
        element_count_(0),
        element_group_count_(0),
        content_count_(0) {}
  virtual void BeginById(int type_id, const Attributes& attributes) {
    ++begin_count_;
  }
  virtual void End() {
    ++end_count_;
  }
  virtual void SaveStringFieldById(int type_id, std::string value) {
    ++field_count_;
  }
  virtual void SaveContent(const std::string& content, bool maybe_quote) {
    ++content_count_;
  }
  virtual void SaveElement(const ElementPtr& element) {
    ++element_count_;
    Serializer::SaveElement(element);
  }
  virtual void SaveElementGroup(const ElementPtr& element, int group_id) {
    ++element_group_count_;
    SaveElement(element);  // To count elements and recurse.
  }
  int get_begin_count() const {
    return begin_count_;
  }
  int get_end_count() const {
    return end_count_;
  }
  int get_field_count() const {
    return field_count_;
  }
  int get_element_count() const {
    return element_count_;
  }
  int get_element_group_count() const {
    return element_group_count_;
  }

 private:
  int begin_count_;
  int end_count_;
  int field_count_;
  int element_count_;
  int element_group_count_;
  int content_count_;
};

// This exists because Serialize is public only on Element.
static void CallSerializer(const ElementPtr& element, Serializer* serializer) {
  ASSERT_TRUE(element);  // This is basically an internal check.
  ASSERT_TRUE(serializer);  // This is basically an internal check.
  element->Serialize(*serializer);
}

// Verify that the default Serializer properly does nothing.
TEST_F(SerializerTest, TestNullSerializer) {
  Serializer serializer;
  CallSerializer(placemark_, &serializer);
  NullSerializer null_serializer;
  CallSerializer(placemark_, &null_serializer);
  MaximalSerializer max_serializer;
  CallSerializer(placemark_, &max_serializer);
}

// Verify that the framework calls out to the Serializer-based class the
// expected number of times for the very simple case of a complex element
// with no fields or child elements.
TEST_F(SerializerTest, TestStatsSerializerOnEmptyElement) {
  StatsSerializer stats_serializer;
  CallSerializer(placemark_, &stats_serializer);
  // Once for <Placemark>
  ASSERT_EQ(1, stats_serializer.get_begin_count());
  // Once for </Placemark>
  ASSERT_EQ(1, stats_serializer.get_end_count());
  // No child simple elements (fields).
  ASSERT_EQ(0, stats_serializer.get_field_count());
  // No child complex elements.
  ASSERT_EQ(0, stats_serializer.get_element_count());
  // No child complex elements in substitution groups.
  ASSERT_EQ(0, stats_serializer.get_element_group_count());
}

// Verify that the framework calls out to the Serializer-based class as
// expected for a complex element with some fields.
TEST_F(SerializerTest, TestStatsSerializerOnFields) {
  StatsSerializer stats_serializer;
  placemark_->set_id("id");  // This is known to be an attribute.
  placemark_->set_name("hi");  // This is known to be a field (<name>).
  // This is known to be a field (<visibility>).
  placemark_->set_visibility(true);
  CallSerializer(placemark_, &stats_serializer);
  // 1: <Placemark>
  ASSERT_EQ(1, stats_serializer.get_begin_count());
  // 1: </Placemark>
  ASSERT_EQ(1, stats_serializer.get_end_count());
  // 2: <name>, <visibility>
  ASSERT_EQ(2, stats_serializer.get_field_count());
  // No child complex elements.
  ASSERT_EQ(0, stats_serializer.get_element_count());
  // No child complex elements in substitution groups.
  ASSERT_EQ(0, stats_serializer.get_element_group_count());
}

// Verify that the framework calls out to the Serializer-based class as
// expected for a hierarchy of complex elements.
TEST_F(SerializerTest, TestStatsSerializerOnChildren) {
  StatsSerializer stats_serializer;
  placemark_->set_geometry(point_);
  placemark_->set_region(region_);
  folder_->add_feature(placemark_);
  CallSerializer(folder_, &stats_serializer);
  // 3: <Folder> <Placemark> <Region> <Point>
  ASSERT_EQ(4, stats_serializer.get_begin_count());
  // 3: </Point> </Region> </Placemark> </Folder>
  ASSERT_EQ(4, stats_serializer.get_end_count());
  // 0: none of the complex elements have attributes or fields
  ASSERT_EQ(0, stats_serializer.get_field_count());
  // 2: 1 for Folder's Placemark + 1 for Placemark's Point + 1 for Placemark's
  // Region.
  ASSERT_EQ(3, stats_serializer.get_element_count());
  // Placemark is a Feature in Folder, and Point is Geometry in Placemark.
  // Region is not in a group.
  ASSERT_EQ(2, stats_serializer.get_element_group_count());

  // Verify that a serializer which provides no implementation of SaveElement()
  // recurses down the hierarchy of complex elements.
  HierarchicalSerializer hier_serializer;
  CallSerializer(folder_, &hier_serializer);
  const TypeIdVector& type_id_vector = hier_serializer.get_type_id_vector();
  ASSERT_EQ(static_cast<size_t>(4), type_id_vector.size());
  ASSERT_EQ(Type_Folder, type_id_vector[0]);
  ASSERT_EQ(Type_Placemark, type_id_vector[1]);
  ASSERT_EQ(Type_Region, type_id_vector[2]);
  ASSERT_EQ(Type_Point, type_id_vector[3]);

  // Verify that a serializer which provides a non-recursing implementation
  // of SaveElement() merely visits each complex element.
  FlatSerializer flat_serializer;
  CallSerializer(placemark_, &flat_serializer);
  const ElementVector& element_vector = flat_serializer.get_element_vector();
  ASSERT_EQ(static_cast<size_t>(2), element_vector.size());
  ASSERT_EQ(Type_Region, element_vector[0]->Type());
  ASSERT_EQ(Type_Point, element_vector[1]->Type());
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
