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
#include "gtest/gtest.h"

namespace kmlbase {

static const char kAttr0[] = "id";
static const char kAttr1[] = "fraction";
static const char kAttr2[] = "xunits";
static const char kNoSuchAttr[] = "no-such-attr";

class AttributesTest : public testing::Test {
 protected:
  virtual void SetUp() {
    attributes_.reset(new Attributes);
  }
  boost::scoped_ptr<Attributes> attributes_;
};

TEST_F(AttributesTest, TestCreate) {
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
  ASSERT_TRUE(attributes_.get());
  std::string got_val;
  ASSERT_TRUE(attributes_->GetString(atts[0], &got_val));
  ASSERT_EQ(std::string(atts[1]), got_val);
  ASSERT_TRUE(attributes_->GetString(atts[2], &got_val));
  ASSERT_EQ(std::string(atts[3]), got_val);
  ASSERT_TRUE(attributes_->GetString(atts[4], &got_val));
  ASSERT_EQ(std::string(atts[5]), got_val);
  ASSERT_FALSE(attributes_->GetString("no-such-attr", &got_val));
  // Verify null output is well behaved.
  ASSERT_FALSE(attributes_->GetString(atts[0], NULL));
}

TEST_F(AttributesTest, TestCreateOdd) {
  // A bad atts list.
  const char* atts[] = { "a0", "a1", "b0", 0 };
  // Call the method under test.
  attributes_.reset(Attributes::Create(atts));
  ASSERT_TRUE(attributes_.get());
  std::string got_val;
  ASSERT_TRUE(attributes_->GetString(atts[0], &got_val));
  ASSERT_EQ(std::string(atts[1]), got_val);
  ASSERT_FALSE(attributes_->GetString(atts[2], &got_val));
}

TEST_F(AttributesTest, TestSetGetString) {
  const std::string kVal0 = "val0";
  const std::string kVal1 = "val1";
  attributes_->SetString(kAttr0, kVal0);
  attributes_->SetString(kAttr1, kVal1);
  std::string got_val;
  ASSERT_TRUE(attributes_->GetString(kAttr0, &got_val));
  ASSERT_EQ(kVal0, got_val);
  ASSERT_TRUE(attributes_->GetString(kAttr1, &got_val));
  ASSERT_EQ(kVal1, got_val);
  ASSERT_TRUE(false == attributes_->GetString(kNoSuchAttr, &got_val));
  ASSERT_EQ(kVal1, got_val);
}

TEST_F(AttributesTest, TestSetGetDouble) {
  const double kVal0 = 123.456;
  const double kVal1 = 987.654321;
  attributes_->SetDouble(kAttr0, kVal0);
  attributes_->SetDouble(kAttr1, kVal1);
  double got_val;
  ASSERT_TRUE(attributes_->GetDouble(kAttr0, &got_val));
  ASSERT_EQ(kVal0, got_val);
  ASSERT_TRUE(attributes_->GetDouble(kAttr1, &got_val));
  ASSERT_EQ(kVal1, got_val);
  ASSERT_TRUE(false == attributes_->GetDouble(kNoSuchAttr, &got_val));
  ASSERT_EQ(kVal1, got_val);
}

TEST_F(AttributesTest, TestSetGetInt) {
  const int kVal0 = 123;
  const int kVal1 = -987;
  attributes_->SetInt(kAttr0, kVal0);
  attributes_->SetInt(kAttr1, kVal1);
  int got_val;
  ASSERT_TRUE(attributes_->GetInt(kAttr0, &got_val));
  ASSERT_EQ(kVal0, got_val);
  ASSERT_TRUE(attributes_->GetInt(kAttr1, &got_val));
  ASSERT_EQ(kVal1, got_val);
  ASSERT_TRUE(false == attributes_->GetInt(kNoSuchAttr, &got_val));
  ASSERT_EQ(kVal1, got_val);
}

TEST_F(AttributesTest, TestClone) {
  const std::string kVal0 = "val0";
  const double kVal1 = 123.456789;
  attributes_->SetString(kAttr0, kVal0);
  attributes_->SetDouble(kAttr1, kVal1);
  Attributes* clone = attributes_->Clone();
  std::string got_string;
  ASSERT_TRUE(clone->GetString(kAttr0, &got_string));
  ASSERT_EQ(kVal0, got_string);
  double got_double;
  ASSERT_TRUE(clone->GetDouble(kAttr1, &got_double));
  ASSERT_EQ(kVal1, got_double);
  delete clone;
}

TEST_F(AttributesTest, TestMerge) {
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
  ASSERT_TRUE(attributes_->GetString(kAttr0, &got_string));
  ASSERT_EQ(kVal0, got_string);
  double got_double;
  ASSERT_TRUE(attributes_->GetDouble(kAttr1, &got_double));
  ASSERT_EQ(kVal1b, got_double);
  ASSERT_TRUE(attributes_->GetString(kAttr2, &got_string));
  ASSERT_EQ(kVal2, got_string);
}

TEST_F(AttributesTest, TestSerialize) {
  const std::string kVal0 = "val0";
  const double kVal1 = 123.456789;
  attributes_->SetString(kAttr0, kVal0);
  attributes_->SetDouble(kAttr1, kVal1);
  std::string serialized;
  attributes_->Serialize(&serialized);
  const std::string expecting = " " + std::string(kAttr1) + "=\"123.456789\" "
    + std::string(kAttr0) + "=\"" + kVal0 + "\"";
  ASSERT_EQ(expecting, serialized);
}

TEST_F(AttributesTest, TestMatch) {
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
  ASSERT_EQ(static_cast<size_t>(1), xmlns.size());
  ASSERT_EQ(std::string(atts[3]), xmlns["ex"]);
}

TEST_F(AttributesTest, TestMatchNoDefault) {
  const char* atts[] = {
    "xmlns:kml", "http://www.opengis.net/kml/2.2",
    "xmlns:ex", "http://vendor.com/kml/2.2ext",
    NULL
  };
  attributes_.reset(Attributes::Create(atts));
  StringStringMap xmlns;
  // This is the method under test.
  attributes_->MatchSplitKey("xmlns:", &xmlns);
  ASSERT_EQ(static_cast<size_t>(2), xmlns.size());
  ASSERT_EQ(std::string(atts[1]), xmlns["kml"]);
  ASSERT_EQ(std::string(atts[3]), xmlns["ex"]);
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

