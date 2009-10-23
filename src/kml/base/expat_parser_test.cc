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

// This file contains the unit tests for the ExpatParser class.

#include "kml/base/expat_parser.h"
#include "kml/base/file.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/dom/kml_handler.h"
#include "kml/dom/parser_observer.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlbase {

// A basic ExpatParser handler that simply reconstructs parsed XML in a string.
class TestXmlHandler : public ExpatHandler {
 public:
  virtual void StartElement(const char* name, const char** atts) {
    xml_.append("<" + std::string(name) + ">");
  }
  virtual void EndElement(const char* name) {
    xml_.append("</" + std::string(name) + ">");
  }
  virtual void CharData(const char* data, int len) {
    xml_.append(data, len);
  }
  const std::string& get_xml() const { return xml_; }

 private:
  std::string xml_;
};

class ExpatParserTest : public testing::Test {
 protected:
  std::string errors_;
  TestXmlHandler handler_;
};

// Verify basic usage of the static ParseString method.
TEST_F(ExpatParserTest, TestPassingParseString) {
  const std::string kXml("<Tom><dick>foo</dick><harry>bar</harry></Tom>");
  ASSERT_TRUE(ExpatParser::ParseString(kXml, &handler_, &errors_, false));
  ASSERT_TRUE(errors_.empty());
  ASSERT_EQ(kXml, handler_.get_xml());
}

// Verify failure of ParseString on badly formed XML content.
TEST_F(ExpatParserTest, TestFailingParseString) {
  // kXml is not well-formed.
  const std::string kXml("<Tom><dick>foo</dick><harry>bar</harry>");
  ASSERT_FALSE(ExpatParser::ParseString(kXml, &handler_, &errors_, false));
  ASSERT_FALSE(errors_.empty());
  ASSERT_EQ(kXml, handler_.get_xml());
}

// Verify basic usage of the ParseBuffer method.
TEST_F(ExpatParserTest, TestPassingParseBuffer) {
  const std::string kXml("<Tom><dick>foo</dick><harry>bar</harry></Tom>");
  ExpatParser parser(&handler_, false);

  // Parse the string one character at a time.
  for (size_t i = 0; i < kXml.length(); ++i) {
    ASSERT_TRUE(parser.ParseBuffer(kXml.substr(i, 1), &errors_,
                                   i == kXml.length()-1));
  }
  ASSERT_EQ(kXml, handler_.get_xml());
}

// Verify failure of ParseBuffer on badly formed XML content.
TEST_F(ExpatParserTest, TestFailingParseBuffer) {
  // kXml is not well-formed.
  const std::string kXml("<Tom><dick>foo</dick><harry>bar</harry>");
  ExpatParser parser(&handler_, false);

  // Parse the string one character at a time.
  for (size_t i = 0; i < kXml.length(); ++i) {
    ASSERT_TRUE(parser.ParseBuffer(kXml.substr(i, 1), &errors_, false));
    ASSERT_TRUE(errors_.empty());
  }
  // Now set the is_final bool to true to indicate that we believe parsing
  // is done. Expat will check and see that its own parsing state shows
  // more content is necessary because our XML is missing the closing
  // </Tom> tag.
  ASSERT_FALSE(parser.ParseBuffer("", &errors_, true));
  ASSERT_FALSE(errors_.empty());

  ASSERT_EQ(kXml, handler_.get_xml());
}

// Assert that we detect a mid-stream parsing failure.
TEST_F(ExpatParserTest, TestMidstreamFailingParseBuffer) {
  const std::string k0("<A><B><C><D>");
  const std::string k1("</D>");  // This is fine.
  const std::string k2("</B>");  // XML is badly formed here, missing </C>.
  ExpatParser parser(&handler_, false);

  ASSERT_TRUE(parser.ParseBuffer(k0, &errors_, false));
  ASSERT_TRUE(errors_.empty());

  ASSERT_TRUE(parser.ParseBuffer(k1, &errors_, false));
  ASSERT_TRUE(errors_.empty());

  ASSERT_FALSE(parser.ParseBuffer(k2, &errors_, false));
  ASSERT_FALSE(errors_.empty());
}

// Verify basic usage of the GetInternalBuffer and ParseInternalBuffer methods.
TEST_F(ExpatParserTest, TestPassingParseInternalBuffer) {
  const std::string kXml("<Tom><dick>foo</dick><harry>bar</harry></Tom>");
  ExpatParser parser(&handler_, false);

  // Parse the string one character at a time.
  for (size_t i = 0; i < kXml.length(); ++i) {
    char* buf = static_cast<char*>(parser.GetInternalBuffer(1));
    *buf = kXml[i];
    ASSERT_TRUE(parser.ParseInternalBuffer(1, &errors_, i == kXml.length()-1));
  }
  ASSERT_EQ(kXml, handler_.get_xml());
  ASSERT_TRUE(errors_.empty());
}

TEST_F(ExpatParserTest, TestFailingInternalBuffer) {
  const std::string k0("<A><B><C><D>");
  const std::string k1("</D>");  // This is fine.
  const std::string k2("</B>");  // XML is badly formed here, missing </C>.
  ExpatParser parser(&handler_, false);

  void* buf = parser.GetInternalBuffer(k0.size());
  memcpy(buf, k0.data(), k0.size());
  ASSERT_TRUE(parser.ParseInternalBuffer(k0.size(), &errors_, false));
  ASSERT_TRUE(errors_.empty());

  buf = parser.GetInternalBuffer(k1.size());
  memcpy(buf, k1.data(), k1.size());
  ASSERT_TRUE(parser.ParseInternalBuffer(k1.size(), &errors_, false));
  ASSERT_TRUE(errors_.empty());

  buf = parser.GetInternalBuffer(k2.size());
  memcpy(buf, k2.data(), k2.size());
  ASSERT_FALSE(parser.ParseInternalBuffer(k2.size(), &errors_, true));
  ASSERT_FALSE(errors_.empty());
}

TEST_F(ExpatParserTest, TestBillionLaughsAttack) {
  // Ensure that the "billion laughs" buffer overflow attack is handled.
  // Previously, this would hang libkml.
  const std::string kBadXml = std::string(DATADIR) + "/kml/billion.kml";
  std::string file_data;
  ASSERT_TRUE(File::ReadFileToString(kBadXml.c_str(), &file_data));

  ASSERT_FALSE(ExpatParser::ParseString(file_data, &handler_, &errors_, false));
  ASSERT_FALSE(errors_.empty());
  ASSERT_TRUE(handler_.get_xml().empty());
}

TEST_F(ExpatParserTest, TestEntitiesStopParser) {
  // This is malformed XML.
  const std::string kBadXml(
    "<Placemark>"
    "<!DOCTYPE billion ["
    "<!ELEMENT billion (#PCDATA)>"
    "<!ENTITY laugh0 \"ha\">"
    "<!ENTITY laugh1 \"&laugh0;&laugh0;\">"
    "<!ENTITY laugh2 \"&laugh1;&laugh1;\">"
    "<!ENTITY laugh3 \"&laugh2;&laugh2;\">"
    "<!ENTITY laugh4 \"&laugh3;&laugh3;\">"
    "<!ENTITY laugh5 \"&laugh4;&laugh4;\">"
    "<!ENTITY laugh6 \"&laugh5;&laugh5;\">"
    "<!ENTITY laugh7 \"&laugh6;&laugh6;\">"
    "<!ENTITY laugh8 \"&laugh7;&laugh7;\">"
    "<!ENTITY laugh9 \"&laugh8;&laugh8;\">"
    "<!ENTITY laugh10 \"&laugh9;&laugh9;\">"
    "<!ENTITY laugh11 \"&laugh10;&laugh10;\">"
    "<!ENTITY laugh12 \"&laugh11;&laugh11;\">"
    "<!ENTITY laugh13 \"&laugh12;&laugh12;\">"
    "<!ENTITY laugh14 \"&laugh13;&laugh13;\">"
    "<!ENTITY laugh15 \"&laugh14;&laugh14;\">"
    "<!ENTITY laugh16 \"&laugh15;&laugh15;\">"
    "<!ENTITY laugh17 \"&laugh16;&laugh16;\">"
    "<!ENTITY laugh18 \"&laugh17;&laugh17;\">"
    "<!ENTITY laugh19 \"&laugh18;&laugh18;\">"
    "<!ENTITY laugh20 \"&laugh19;&laugh19;\">"
    "<!ENTITY laugh21 \"&laugh20;&laugh20;\">"
    "<!ENTITY laugh22 \"&laugh21;&laugh21;\">"
    "<!ENTITY laugh23 \"&laugh22;&laugh22;\">"
    "<!ENTITY laugh24 \"&laugh23;&laugh23;\">"
    "<!ENTITY laugh25 \"&laugh24;&laugh24;\">"
    "<!ENTITY laugh26 \"&laugh25;&laugh25;\">"
    "<!ENTITY laugh27 \"&laugh26;&laugh26;\">"
    "<!ENTITY laugh28 \"&laugh27;&laugh27;\">"
    "<!ENTITY laugh29 \"&laugh28;&laugh28;\">"
    "<!ENTITY laugh30 \"&laugh29;&laugh29;\">"
    "]>"
    "<billion>&laugh30;</billion>"
    "</Placemark>");
  ASSERT_FALSE(ExpatParser::ParseString(kBadXml, &handler_, &errors_, false));
  ASSERT_FALSE(errors_.empty());
  ASSERT_EQ(std::string("<Placemark>"), handler_.get_xml());
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
