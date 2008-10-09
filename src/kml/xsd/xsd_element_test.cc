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

// This file contains the unit tests for the XsdElement class.

#include "kml/xsd/xsd_element.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/unit_test.h"
#include "kml/base/attributes.h"
#include "kml/xsd/xsd_util.h"

namespace kmlxsd {

// This class is the unit test fixture for the XsdElement class.
class XsdElementTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(XsdElementTest);
  CPPUNIT_TEST(TestBasicCreate);
  CPPUNIT_TEST(TestCreateNameType);
  CPPUNIT_TEST(TestCreateNameTypeDefault);
  CPPUNIT_TEST(TestCreateNameAbstract);
  CPPUNIT_TEST(TestCreateNameTypeDefaultSubstitutionGroup);
  CPPUNIT_TEST(TestGetTypeId);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestBasicCreate();
  void TestCreateNameType();
  void TestCreateNameTypeDefault();
  void TestCreateNameAbstract();
  void TestCreateNameTypeDefaultSubstitutionGroup();
  void TestGetTypeId();

 private:
  kmlbase::Attributes attributes_;
  boost::scoped_ptr<XsdElement> xsd_element_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(XsdElementTest);

void XsdElementTest::TestBasicCreate() {
  // <xs:element name="sjokolade"/>
  const std::string kSjokolade("sjokolade");
  attributes_.SetString(kName, kSjokolade);
  xsd_element_.reset(XsdElement::Create(attributes_));
  CPPUNIT_ASSERT(xsd_element_.get());
  CPPUNIT_ASSERT_EQUAL(kSjokolade, xsd_element_->get_name());
  CPPUNIT_ASSERT(!xsd_element_->is_ref());
}

void XsdElementTest::TestCreateNameType() {
  // <element name="address" type="string"/>, for example.
  const std::string kAddress("address");
  const std::string kString("string");
  attributes_.SetString(kName, kAddress);
  attributes_.SetString(kType, kString);
  xsd_element_.reset(XsdElement::Create(attributes_));
  CPPUNIT_ASSERT(xsd_element_.get());
  CPPUNIT_ASSERT_EQUAL(kAddress, xsd_element_->get_name());
  CPPUNIT_ASSERT_EQUAL(kString, xsd_element_->get_type());
  CPPUNIT_ASSERT(!xsd_element_->is_ref());
}

void XsdElementTest::TestCreateNameTypeDefault() {
  // <element name="altitude" type="double" default="0.0"/>
  const std::string kAltitude("altitude");
  const std::string kDouble("double");
  const std::string kZZ("0.0");
  attributes_.SetString(kName, kAltitude);
  attributes_.SetString(kType, kDouble);
  attributes_.SetString(kDefault, kZZ);

  xsd_element_.reset(XsdElement::Create(attributes_));

  CPPUNIT_ASSERT(xsd_element_.get());
  CPPUNIT_ASSERT_EQUAL(kAltitude, xsd_element_->get_name());
  CPPUNIT_ASSERT_EQUAL(kDouble, xsd_element_->get_type());
  CPPUNIT_ASSERT_EQUAL(kZZ, xsd_element_->get_default());
  CPPUNIT_ASSERT(!xsd_element_->is_ref());
}

void XsdElementTest::TestCreateNameAbstract() {
  // <element name="altitudeModeGroup" abstract="true"/>
  const std::string kAltitudeModeGroup("altitudeModeGroup");
  attributes_.SetString(kName, kAltitudeModeGroup);
  attributes_.SetString(kAbstract, "true");

  xsd_element_.reset(XsdElement::Create(attributes_));

  CPPUNIT_ASSERT(xsd_element_.get());
  CPPUNIT_ASSERT_EQUAL(kAltitudeModeGroup, xsd_element_->get_name());
  CPPUNIT_ASSERT_EQUAL(true, xsd_element_->is_abstract());
  CPPUNIT_ASSERT(!xsd_element_->is_ref());
}

void XsdElementTest::TestCreateNameTypeDefaultSubstitutionGroup() {
  // <element name="altitudeMode" type="kml:altitudeModeEnumType"
  //  default="clampToGround" substitutionGroup="kml:altitudeModeGroup"/>
  const std::string kAltitudeMode("altitudeMode");
  const std::string kAltitudeEnum("kml:altitudeModeEnumType");
  const std::string kClampToGround("clampToGround");
  const std::string kAltitudeModeGroup("kml:altitudeModeGroup");
  attributes_.SetString(kName, kAltitudeMode);
  attributes_.SetString(kType, kAltitudeEnum);
  attributes_.SetString(kDefault, kClampToGround);
  attributes_.SetString(kSubstitutionGroup, kAltitudeModeGroup);

  xsd_element_.reset(XsdElement::Create(attributes_));

  CPPUNIT_ASSERT(xsd_element_.get());
  CPPUNIT_ASSERT_EQUAL(kAltitudeMode, xsd_element_->get_name());
  CPPUNIT_ASSERT_EQUAL(kAltitudeEnum, xsd_element_->get_type());
  CPPUNIT_ASSERT_EQUAL(kClampToGround, xsd_element_->get_default());
  CPPUNIT_ASSERT_EQUAL(kAltitudeModeGroup,
                       xsd_element_->get_substitution_group());
  CPPUNIT_ASSERT(!xsd_element_->is_ref());
}

// Verify the parse of <xs:element type="..."/> and get_type_id().
void XsdElementTest::TestGetTypeId() {
  xsd_element_.reset(CreateXsdElement("somethingIntegral", "int"));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_INT, xsd_element_->get_type_id());
  xsd_element_.reset(CreateXsdElement("somethingDouble", "double"));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_DOUBLE,
                       xsd_element_->get_type_id());
  xsd_element_.reset(CreateXsdElement("yesOrNo", "boolean"));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_BOOLEAN,
                       xsd_element_->get_type_id());
  xsd_element_.reset(CreateXsdElement("saySomething", "string"));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_STRING,
                       xsd_element_->get_type_id());
  // A user defined type such as a complexType is properly not a primitive.
  xsd_element_.reset(CreateXsdElement("Placemark", "PlacemarkType"));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_INVALID,
                       xsd_element_->get_type_id());
}


}  // end namespace kmlxsd

TEST_MAIN
