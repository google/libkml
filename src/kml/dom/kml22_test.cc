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

#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/xsd.h"
#include "gtest/gtest.h"

namespace kmldom {

class Kml22Test : public testing::Test {
 protected:
  virtual void SetUp() {
    xsd_ = Xsd::GetSchema();
  }

  Xsd* xsd_;
};

// Verify proper enum defaults:
TEST_F(Kml22Test, TestEnumDefaults) {
  ASSERT_EQ(std::string("clampToGround"),
            xsd_->EnumValue(Type_altitudeMode, ALTITUDEMODE_CLAMPTOGROUND));
  ASSERT_EQ(std::string("normal"),
            xsd_->EnumValue(Type_colorMode, COLORMODE_NORMAL));
  ASSERT_EQ(std::string("default"),
            xsd_->EnumValue(Type_displayMode, DISPLAYMODE_DEFAULT));
  ASSERT_EQ(std::string("lowerLeft"),
            xsd_->EnumValue(Type_gridOrigin, GRIDORIGIN_LOWERLEFT));
  ASSERT_EQ(std::string("open"),
            xsd_->EnumValue(Type_state, ITEMICONSTATE_OPEN));
  ASSERT_EQ(std::string("check"),
            xsd_->EnumValue(Type_listItemType, LISTITEMTYPE_CHECK));
  ASSERT_EQ(std::string("onChange"),
            xsd_->EnumValue(Type_refreshMode, REFRESHMODE_ONCHANGE));
  ASSERT_EQ(std::string("rectangle"),
            xsd_->EnumValue(Type_shape, SHAPE_RECTANGLE));
  ASSERT_EQ(std::string("normal"),
            xsd_->EnumValue(Type_key, STYLESTATE_NORMAL));
  ASSERT_EQ(std::string("fraction"),
            xsd_->EnumValue(Type_units, UNITS_FRACTION));
  ASSERT_EQ(std::string("never"),
            xsd_->EnumValue(Type_viewRefreshMode, VIEWREFRESHMODE_NEVER));
  ASSERT_EQ(std::string("bounce"),
            xsd_->EnumValue(Type_GxFlyToMode, GX_FLYTOMODE_BOUNCE));
  ASSERT_EQ(std::string("pause"),
            xsd_->EnumValue(Type_GxPlayMode, GX_PLAYMODE_PAUSE));
}

// Test NULL arg on AddElement(), ParseAttributes(), and SerializeAttributes()
// on all complex elements.  These are all methods that take pointer arguments.
TEST_F(Kml22Test, TestElementNull) {
  int complex_count = 0;
  int element_type_id = static_cast<int>(Type_Unknown) + 1;
  const int end_id = static_cast<int>(Type_Invalid);
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  for (; element_type_id != end_id; ++element_type_id) {
    // Only complex elements return non-NULL.
    if (ElementPtr element = kml_factory->CreateElementById(
            static_cast<KmlDomType>(element_type_id))) {
      // All KmlFactory-created elements are known.
      ASSERT_FALSE(element->IsA(kmldom::Type_Unknown));
      // Verify the element's abstract base types.
      if (element->IsA(kmldom::Type_ColorStyle)) {
        ASSERT_TRUE(element->IsA(kmldom::Type_SubStyle));
      }
      if (element->IsA(kmldom::Type_Container)) {
        ASSERT_TRUE(element->IsA(kmldom::Type_Feature));
      }
      if (element->IsA(kmldom::Type_Overlay)) {
        ASSERT_TRUE(element->IsA(kmldom::Type_Feature));
      }
      if (element->IsA(kmldom::Type_AbstractLatLonBox) ||
          element->IsA(kmldom::Type_AbstractView) ||
          element->IsA(kmldom::Type_BasicLink) ||
          element->IsA(kmldom::Type_Feature) ||
          element->IsA(kmldom::Type_Geometry) ||
          element->IsA(kmldom::Type_StyleSelector) ||
          element->IsA(kmldom::Type_SubStyle) ||
          element->IsA(kmldom::Type_TimePrimitive)) {
        ASSERT_TRUE(element->IsA(kmldom::Type_Object));
      }
      // Verify NULL pointers do no harm.
      element->AddElement(NULL);
      element->ParseAttributes(NULL);
      element->SerializeAttributes(NULL);
      ++complex_count;
    }
  }
  ASSERT_EQ(84, complex_count);  // Yes, must exactly match kml22.h
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
