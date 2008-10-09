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

#include "kml/xsd/xsd_primitive_type.h"
#include "kml/base/unit_test.h"

namespace kmlxsd {

// This class is the unit test fixture for the XsdTypes class.
class XsdPrimitiveTypeTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(XsdPrimitiveTypeTest);
  CPPUNIT_TEST(TestXsdPrimitiveTypeNameId);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestXsdPrimitiveTypeNameId();
};

CPPUNIT_TEST_SUITE_REGISTRATION(XsdPrimitiveTypeTest);

// Verify NULL is returned if no attributes are supplied.
void XsdPrimitiveTypeTest::TestXsdPrimitiveTypeNameId() {
  const std::string kBoolean("boolean");
  const std::string kDouble("double");
  const std::string kInt("int");
  const std::string kString("string");

  CPPUNIT_ASSERT_EQUAL(kBoolean,
      XsdPrimitiveType::GetTypeName(XsdPrimitiveType::XSD_BOOLEAN));
  CPPUNIT_ASSERT_EQUAL(kDouble,
      XsdPrimitiveType::GetTypeName(XsdPrimitiveType::XSD_DOUBLE));
  CPPUNIT_ASSERT_EQUAL(kInt,
      XsdPrimitiveType::GetTypeName(XsdPrimitiveType::XSD_INT));
  CPPUNIT_ASSERT_EQUAL(kString,
      XsdPrimitiveType::GetTypeName(XsdPrimitiveType::XSD_STRING));

  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_BOOLEAN,
                       XsdPrimitiveType::GetTypeId(kBoolean));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_DOUBLE,
                       XsdPrimitiveType::GetTypeId(kDouble));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_INT,
                       XsdPrimitiveType::GetTypeId(kInt));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_STRING,
                       XsdPrimitiveType::GetTypeId(kString));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_INVALID,
                       XsdPrimitiveType::GetTypeId("no-such-xsd-type"));
  CPPUNIT_ASSERT_EQUAL(XsdPrimitiveType::XSD_INVALID,
                       XsdPrimitiveType::GetTypeId("PlacemarkType"));
}

}  // end namespace kmlxsd

TEST_MAIN
