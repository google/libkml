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

// This file contains the unit tests for the internal Attributes class.

#include "kml/base/attributes.h"
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/unit_test.h"

namespace kmlbase {

const char* kAttr0 = "id";
const char* kAttr1 = "fraction";
const char* kAttr2 = "xunits";
const char* kNoSuchAttr = "no-such-attr";

class AttributesTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(AttributesTest);
  CPPUNIT_TEST(TestCreate);
  CPPUNIT_TEST(TestSetGetString);
  CPPUNIT_TEST(TestSetGetDouble);
  CPPUNIT_TEST(TestClone);
  CPPUNIT_TEST(TestMerge);
  CPPUNIT_TEST(TestSerialize);
  CPPUNIT_TEST(TestMatch);
  CPPUNIT_TEST(TestMatchNoDefault);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestCreate();
  void TestSetGetString();
  void TestSetGetDouble();
  void TestClone();
  void TestMerge();
  void TestSerialize();
  void TestMatch();
  void TestMatchNoDefault();

 public:
  // Called before each test.
  void setUp() {
    attributes_.reset(new Attributes);
  }

  // Called after each test.
  void tearDown() {
    // attributes_ managed by scoped_ptr.
  }

 private:
  boost::scoped_ptr<Attributes> attributes_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(AttributesTest);

void AttributesTest::TestCreate() {
  // A list of name-value pairs as expat might produce.
  const char* atts[] = {
    "name",
    "Placemark",
    "type",
    "kml:PlacemarkType",
    "substitutionGroup",
    "kml:AbstractFeatureGroup",
    NULL
  };
  // Call the method under test.
  attributes_.reset(Attributes::Create(atts));
  CPPUNIT_ASSERT(attributes_.get());
  std::string got_val;
  CPPUNIT_ASSERT(attributes_->GetString(atts[0], &got_val));
  CPPUNIT_ASSERT_EQUAL(std::string(atts[1]), got_val);
  CPPUNIT_ASSERT(attributes_->GetString(atts[2], &got_val));
  CPPUNIT_ASSERT_EQUAL(std::string(atts[3]), got_val);
  CPPUNIT_ASSERT(attributes_->GetString(atts[4], &got_val));
  CPPUNIT_ASSERT_EQUAL(std::string(atts[5]), got_val);
  CPPUNIT_ASSERT(!attributes_->GetString("no-such-attr", &got_val));
  // Verify null output is well behaved.
  CPPUNIT_ASSERT(!attributes_->GetString(atts[0], NULL));
}

void AttributesTest::TestSetGetString() {
  const std::string kVal0 = "val0";
  const std::string kVal1 = "val1";
  attributes_->SetString(kAttr0, kVal0);
  attributes_->SetString(kAttr1, kVal1);
  std::string got_val;
  CPPUNIT_ASSERT(attributes_->GetString(kAttr0, &got_val));
  CPPUNIT_ASSERT_EQUAL(kVal0, got_val);
  CPPUNIT_ASSERT(attributes_->GetString(kAttr1, &got_val));
  CPPUNIT_ASSERT_EQUAL(kVal1, got_val);
  CPPUNIT_ASSERT(false == attributes_->GetString(kNoSuchAttr, &got_val));
  CPPUNIT_ASSERT_EQUAL(kVal1, got_val);
}

void AttributesTest::TestSetGetDouble() {
  const double kVal0 = 123.456;
  const double kVal1 = 987.654321;
  attributes_->SetDouble(kAttr0, kVal0);
  attributes_->SetDouble(kAttr1, kVal1);
  double got_val;
  CPPUNIT_ASSERT(attributes_->GetDouble(kAttr0, &got_val));
  CPPUNIT_ASSERT_EQUAL(kVal0, got_val);
  CPPUNIT_ASSERT(attributes_->GetDouble(kAttr1, &got_val));
  CPPUNIT_ASSERT_EQUAL(kVal1, got_val);
  CPPUNIT_ASSERT(false == attributes_->GetDouble(kNoSuchAttr, &got_val));
  CPPUNIT_ASSERT_EQUAL(kVal1, got_val);
}

void AttributesTest::TestClone() {
  const std::string kVal0 = "val0";
  const double kVal1 = 123.456789;
  attributes_->SetString(kAttr0, kVal0);
  attributes_->SetDouble(kAttr1, kVal1);
  Attributes* clone = attributes_->Clone();
  std::string got_string;
  CPPUNIT_ASSERT(clone->GetString(kAttr0, &got_string));
  CPPUNIT_ASSERT_EQUAL(kVal0, got_string);
  double got_double;
  CPPUNIT_ASSERT(clone->GetDouble(kAttr1, &got_double));
  CPPUNIT_ASSERT_EQUAL(kVal1, got_double);
  delete clone;
}

void AttributesTest::TestMerge() {
  const std::string kVal0 = "val0";
  const double kVal1a = 123.456789;
  const double kVal1b = 78.90123;
  const std::string kVal2 = "fraction";
  attributes_->SetString(kAttr0, kVal0);
  attributes_->SetDouble(kAttr1, kVal1a);
  Attributes attributes;
  attributes.SetDouble(kAttr1, kVal1b);
  attributes.SetString(kAttr2, kVal2);
  attributes_->MergeAttributes(attributes);
  std::string got_string;
  CPPUNIT_ASSERT(attributes_->GetString(kAttr0, &got_string));
  CPPUNIT_ASSERT_EQUAL(kVal0, got_string);
  double got_double;
  CPPUNIT_ASSERT(attributes_->GetDouble(kAttr1, &got_double));
  CPPUNIT_ASSERT_EQUAL(kVal1b, got_double);
  CPPUNIT_ASSERT(attributes_->GetString(kAttr2, &got_string));
  CPPUNIT_ASSERT_EQUAL(kVal2, got_string);
}

void AttributesTest::TestSerialize() {
  const std::string kVal0 = "val0";
  const double kVal1 = 123.456789;
  attributes_->SetString(kAttr0, kVal0);
  attributes_->SetDouble(kAttr1, kVal1);
  std::string serialized;
  attributes_->Serialize(&serialized);
  const std::string expecting = " " + std::string(kAttr1) + "=\"123.456789\" "
    + std::string(kAttr0) + "=\"" + kVal0 + "\"";
  CPPUNIT_ASSERT_EQUAL(expecting, serialized);
}

void AttributesTest::TestMatch() {
  // <kml xmlns="http://www.opengis.net/kml/2.2"
  //      xmlns:ex="http://vendor.com/kml/2.2ext">
  // Expat turns the above XML into this:
  const char* atts[] = {
    "xmlns", "http://www.opengis.net/kml/2.2",
    "xmlns:ex", "http://vendor.com/kml/2.2ext",
    NULL
  };
  attributes_.reset(Attributes::Create(atts));
  StringStringMap xmlns;
  // This is the method under test.
  attributes_->MatchSplitKey("xmlns:", &xmlns);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), xmlns.size());
  CPPUNIT_ASSERT_EQUAL(std::string(atts[3]), xmlns["ex"]);
}

void AttributesTest::TestMatchNoDefault() {
  const char* atts[] = {
    "xmlns:kml", "http://www.opengis.net/kml/2.2",
    "xmlns:ex", "http://vendor.com/kml/2.2ext",
    NULL
  };
  attributes_.reset(Attributes::Create(atts));
  StringStringMap xmlns;
  // This is the method under test.
  attributes_->MatchSplitKey("xmlns:", &xmlns);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), xmlns.size());
  CPPUNIT_ASSERT_EQUAL(std::string(atts[1]), xmlns["kml"]);
  CPPUNIT_ASSERT_EQUAL(std::string(atts[3]), xmlns["ex"]);
}

}  // end namespace kmlbase

TEST_MAIN
