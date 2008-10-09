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

// This file contains the unit tests for the XsdSchema class.

#include "kml/xsd/xsd_schema.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/attributes.h"
#include "kml/base/unit_test.h"
#include "kml/xsd/xsd_util.h"

using kmlbase::Attributes;

namespace kmlxsd {

// This class is the unit test fixture for the XsdSchema class.
class XsdSchemaTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(XsdSchemaTest);
  CPPUNIT_TEST(TestNullCreateNoAttributes);
  CPPUNIT_TEST(TestNullCreateNoTargetNamespace);
  CPPUNIT_TEST(TestCreate);
  CPPUNIT_TEST(TestSplitNsName);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestNullCreateNoAttributes();
  void TestNullCreateNoTargetNamespace();
  void TestNullCreate();
  void TestCreate();
  void TestSplitNsName();

 private:
  XsdSchemaPtr xsd_schema_;
  Attributes attributes_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(XsdSchemaTest);

// Verify NULL is returned if no attributes are supplied.
void XsdSchemaTest::TestNullCreateNoAttributes() {
  xsd_schema_ = XsdSchema::Create(attributes_);
  CPPUNIT_ASSERT(!xsd_schema_);
}

// Verify NULL is returned from Create() if attributes has no targetNamespace.
void XsdSchemaTest::TestNullCreateNoTargetNamespace() {
  attributes_.SetString("random", "junk");
  xsd_schema_ = XsdSchema::Create(attributes_);
  CPPUNIT_ASSERT(!xsd_schema_);
}

// Verify processing of Create method.
void XsdSchemaTest::TestCreate() {
  // Send down the attributes from:
  //   <schema xmlns:mcn="my:cool:namespace"
  //           targetNamespace="my:cool:namespace"/>
  const std::string kNamespace("my:cool:namespace");
  const std::string kPrefix("mcn");
  attributes_.SetString(std::string("xmlns:") + kPrefix, kNamespace);
  attributes_.SetString(kTargetNamespace, kNamespace);
  xsd_schema_ = XsdSchema::Create(attributes_);
  CPPUNIT_ASSERT(xsd_schema_);
  CPPUNIT_ASSERT_EQUAL(kNamespace, xsd_schema_->get_target_namespace());
  CPPUNIT_ASSERT_EQUAL(kPrefix, xsd_schema_->get_target_namespace_prefix());
}

// Verify SplitNsName().
void XsdSchemaTest::TestSplitNsName() {
  const std::string kPrefix("myml");
  xsd_schema_ = CreateXsdSchema(kPrefix, "who:cares");
  const std::string kMyElement("MyElement");
  std::string ncname;
  CPPUNIT_ASSERT(xsd_schema_->SplitNsName(kPrefix + ":" + kMyElement, &ncname));
  CPPUNIT_ASSERT_EQUAL(kMyElement, ncname);
}

}  // end namespace kmlxsd

TEST_MAIN
