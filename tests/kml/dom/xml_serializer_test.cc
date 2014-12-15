// Copyright 2010, Google Inc. All rights reserved.
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

// This file contains the unit tests for the XmlSerializer class and
// the SerializePretty and SerializeRaw public API functions.

#include "kml/dom/xml_serializer.h"
#include <sstream>
#include "boost/scoped_ptr.hpp"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kmldom.h"
#include "gtest/gtest.h"

using kmlbase::ToString;

namespace kmldom {

class XmlSerializerTest : public testing::Test {
 protected:
  virtual void SetUp() {
    string_adapter_.reset(new StringAdapter(&output_));
    xml_serializer_.reset(
        new XmlSerializer<StringAdapter>("", "",
                                                  string_adapter_.get()));
    placemark_ = KmlFactory::GetFactory()->CreatePlacemark();
  }

  boost::scoped_ptr<XmlSerializer<StringAdapter> > xml_serializer_;
  // If string were strictly std::string we could instead use
  // std::ostringstream.
  string output_;
  boost::scoped_ptr<StringAdapter> string_adapter_;
  PlacemarkPtr placemark_;
};

TEST_F(XmlSerializerTest, TestToString) {
  double pi = 3.14159;
  unsigned int dna = 42;
  ASSERT_EQ(string("3.14159"), ToString(pi));
  ASSERT_EQ("42", ToString(dna));
}

TEST_F(XmlSerializerTest, TestSaveEmptyStringFieldById) {
  // Assert that the <name/> field serializes as expected.
  const int type_id = Type_name;
  const string expected_result("<name/>");
  const string empty;
  xml_serializer_->SaveFieldById(type_id, empty);
  ASSERT_EQ(expected_result, output_);
}

TEST_F(XmlSerializerTest, TestSaveStringFieldById) {
  // Assert that the <name> field serializes as expected.
  const int type_id = Type_name;
  const string txt("some feature name");
  const string expected_result("<name>some feature name</name>");
  xml_serializer_->SaveFieldById(type_id, txt);
  ASSERT_EQ(expected_result, output_);
}

TEST_F(XmlSerializerTest, TestCdataHandling) {
  // The underlying parser itself won't pass CDATA sections through to
  // element char data, but it is possible that raw CDATA will be passed
  // through directly by feature->set_name() or similar. If the serializer
  // sees this, we entity-escape the entire string to prevent the output
  // of invalid XML. Otherwise, if we see any invalid characters in the string,
  // we wrap them with CDATA.
  struct TestStruct {
    const string chardata;
    const string expected;
  } testdata[] = {
    {"simple text", "<name>simple text</name>\n"},
    {"<![CDATA[...]]>", "<name>&lt;![CDATA[...]]&gt;</name>\n"},
    {"invalid & char", "<name><![CDATA[invalid & char]]></name>\n"},
    {"invalid ' char", "<name><![CDATA[invalid ' char]]></name>\n"},
    {"invalid < char", "<name><![CDATA[invalid < char]]></name>\n"},
    {"invalid > char", "<name><![CDATA[invalid > char]]></name>\n"},
    {"invalid \" char", "<name><![CDATA[invalid \" char]]></name>\n"},
    {"goo <![CDATA[goo]]> goo", "<name>goo &lt;![CDATA[goo]]&gt; goo</name>\n"},
    {"<x><![CDATA[goo]]></x>",
     "<name>&lt;x&gt;&lt;![CDATA[goo]]&gt;&lt;/x&gt;</name>\n"}
  };

  const size_t size = sizeof(testdata) / sizeof(testdata[0]);

  for (size_t i = 0; i < size; ++i) {
    output_.clear();
    XmlSerializer<StringAdapter> s_("\n","", string_adapter_.get());
    s_.SaveFieldById(Type_name, testdata[i].chardata);
    ASSERT_EQ(testdata[i].expected, output_);
  }
}

TEST_F(XmlSerializerTest, TestCdataEscaping) {
  // Assert that data that should be escaped in a CDATA is so quoted.
  placemark_->set_name("<i>One</i> two");
  string xml = SerializePretty(placemark_);
  string expected("<Placemark>\n  "
                  "<name><![CDATA[<i>One</i> two]]></name>\n"
                  "</Placemark>\n");
  ASSERT_EQ(expected, xml);
}

TEST_F(XmlSerializerTest, TestCdataPassedBySetter) {
  string crazy_name("foo <b> goo <![CDATA[xxx<i>yyy</i>xxx]]> </b> goo");
  placemark_->set_name(crazy_name);
  string xml = SerializePretty(placemark_);
  string expected("<Placemark>\n"
                  "  <name>foo &lt;b&gt; goo &lt;![CDATA["
                  "xxx&lt;i&gt;yyy&lt;/i&gt;xxx]]&gt; &lt;/b&gt; goo</name>\n"
                  "</Placemark>\n");
  ASSERT_EQ(expected, xml);
}


TEST_F(XmlSerializerTest, TestSaveBoolFieldByIdAsBool) {
  // Assert that <open> is serialized correctly.
  const bool bool_state = true;
  string expected_result("<open>1</open>");
  // A parsed bool is serialized as an int:
  xml_serializer_->SaveFieldById(Type_open, bool_state);
  ASSERT_EQ(expected_result, output_);
}

TEST_F(XmlSerializerTest, TestSaveBoolFieldByIdAsInt) {
  // Assert that <open> is serialized correctly.
  const unsigned int int_state = 1;
  string expected_result("<open>1</open>");
  // A parsed int is serialized as an int:
  xml_serializer_->SaveFieldById(Type_open, int_state);
  ASSERT_EQ(expected_result, output_);
}

TEST_F(XmlSerializerTest, TestSaveContent) {
  // Ensure a simple string is serialized exactly.
  const string s("tom, dick");
  xml_serializer_->SaveContent(s, false);
  ASSERT_EQ(s, output_);
  // SaveContent will append continued calls.
  string t(" and harry");
  xml_serializer_->SaveContent(t, false);
  string expected_result(s + t);
  ASSERT_EQ(expected_result, output_);
}

TEST_F(XmlSerializerTest, TestSaveColor) {
  const kmlbase::Color32 kRed(0xff0000ff);
  const string kExpected("<color>ff0000ff</color>");
  xml_serializer_->SaveColor(Type_color, kRed);
  ASSERT_EQ(kExpected, output_);
}

TEST_F(XmlSerializerTest, TestPrecision) {
  double a = 1.0;
  // Will round down to int:
  string expected = "1";
  ASSERT_EQ(expected, ToString(a));
  double b = 1.1;
  // Will preserve at current level of precision:
  expected = "1.1";
  ASSERT_EQ(expected, ToString(b));
  double c = 1.2345678901234567890;
  // Will round down to 15 decimals of precision:
  expected = "1.23456789012346";
  ASSERT_EQ(expected, ToString(c));
}

// Tests the internal Indent() method.
TEST_F(XmlSerializerTest, TestSerializePretty) {
  placemark_->set_name("hello");
  string xml = SerializePretty(placemark_);
  string expected("<Placemark>\n  <name>hello</name>\n</Placemark>\n");
  ASSERT_EQ(expected, xml);
}

// This tests the pretty serialization of an element with no content.
TEST_F(XmlSerializerTest, TestSerializePrettyNil) {
  ASSERT_EQ(string("<Placemark/>\n"),
                       SerializePretty(placemark_));
}

// This tests the pretty serialization of an element with attributes but
// no content.
TEST_F(XmlSerializerTest, TestSerializePrettyNilWithAttrs) {
  placemark_->set_id("hi");  // Adds the id= attribute.
  ASSERT_EQ(string("<Placemark id=\"hi\"/>\n"),
                       SerializePretty(placemark_));
}

// This tests the raw serialization of an element a child element.
TEST_F(XmlSerializerTest, TestSerializeRaw) {
  placemark_->set_name("hello");
  string xml = SerializeRaw(placemark_);
  string expected("<Placemark><name>hello</name></Placemark>");
  ASSERT_EQ(expected, xml);
}

// This tests the raw serialization of an element with no content.
TEST_F(XmlSerializerTest, TestSerializeRawNil) {
  ASSERT_EQ(string("<Placemark/>"),
                       SerializeRaw(placemark_));
}

// This tests the raw serialization of an element with attributes but
// no content.
TEST_F(XmlSerializerTest, TestSerializeRawNilWithAttrs) {
  placemark_->set_id("hi");  // Adds the id= attribute.
  ASSERT_EQ(string("<Placemark id=\"hi\"/>"),
                       SerializeRaw(placemark_));
}

TEST_F(XmlSerializerTest, TestSerializeUnknowns) {
  // Unrecognised elements:
  const string unknown1("<unknown>zzz<Foo/></unknown>");
  const string unknown2("<unknownBar/>");
  placemark_->AddUnknownElement(unknown1);
  placemark_->AddUnknownElement(unknown2);
  ASSERT_EQ(static_cast<size_t>(2),
                       placemark_->get_unknown_elements_array_size());
  ASSERT_EQ(unknown1, placemark_->get_unknown_elements_array_at(0));
  ASSERT_EQ(unknown2, placemark_->get_unknown_elements_array_at(1));
  ASSERT_EQ(string("<Placemark>") + unknown1 + unknown2 + "</Placemark>",
            SerializeRaw(placemark_));
}

TEST_F(XmlSerializerTest, TestSerializeNull) {
  const string empty;
  ASSERT_EQ(empty, SerializePretty(NULL));
  ASSERT_EQ(empty, SerializeRaw(NULL));
}

// This test verifies that SerializeRaw remains compatible with some slightly
// unfortunate and non-obvious behavior in libkml 1.2.  In libkml 1.2 the
// serialization of <coordinates> _always_ emits lon,lat,alt and always
// uses "\n" to separate each tuple _even_ using SerializeRaw.
TEST_F(XmlSerializerTest, SerializeRawCoordinates) {
  placemark_ = AsPlacemark(ParseKml(
      "<Placemark>"
      "  <LineString>"
      "    <coordinates>1.2,3.4,5.6 9.8,7.6</coordinates>"
      "  </LineString>"
      "</Placemark>"));
  ASSERT_TRUE(placemark_);
  const string want(
      "<Placemark>"
      "<LineString>"
      "<coordinates>1.2,3.4,5.6\n"
      "9.8,7.6,0\n"
      "</coordinates>"
      "</LineString>"
      "</Placemark>");
  ASSERT_EQ(want, SerializeRaw(placemark_));
}

TEST_F(XmlSerializerTest, BasicSerializePrettyToOstream) {
  kmldom::CoordinatesPtr coordinates =
      kmldom::KmlFactory::GetFactory()->CreateCoordinates();
  coordinates->add_latlng(1,2);
  kmldom::PointPtr point = kmldom::KmlFactory::GetFactory()->CreatePoint();
  point->set_coordinates(coordinates);
  placemark_->set_geometry(point);
  placemark_->set_id("pm123");
  placemark_->set_name("placemark name");
  XmlSerializer<StringAdapter>::Serialize(placemark_, "\r", " ",
                                                    string_adapter_.get());
  const string want("<Placemark id=\"pm123\">\r"
                    " <name>placemark name</name>\r"
                    " <Point>\r"
                    "  <coordinates>\r"
                    "   2,1,0\r"
                    "  </coordinates>\r"
                    " </Point>\r"
                    "</Placemark>\r");
  ASSERT_EQ(want, output_);
}

TEST_F(XmlSerializerTest, TestGetElementName) {
  ASSERT_EQ(string(""), GetElementName(NULL));
  ASSERT_EQ(string("Placemark"), GetElementName(placemark_));
  ASSERT_EQ(string("atom:author"),
            GetElementName(KmlFactory::GetFactory()->CreateAtomAuthor()));
  ASSERT_EQ(string("gx:Tour"),
            GetElementName(KmlFactory::GetFactory()->CreateGxTour()));
}

}  // end namespace kmldom
