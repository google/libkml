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
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/dom/kml_handler.h"
#include "kml/dom/parser_observer.h"

namespace kmlbase {

// A basic ExpatParser handler that simply reconstructs parsed XML in a
// string.
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

// Verify basic usage of the static ParseString method.
TEST(ExpatParserTest, TestPassingParseString) {
  const std::string kXml("<Tom><dick>foo</dick><harry>bar</harry></Tom>");
  std::string errors;
  TestXmlHandler handler;
  ASSERT_TRUE(ExpatParser::ParseString(kXml, &handler, &errors, false));
  ASSERT_TRUE(errors.empty());
  ASSERT_EQ(kXml, handler.get_xml());
}

// Verify failure of ParseString on badly formed XML content.
TEST(ExpatParserTest, TestFailingParseString) {
  // kXml is not well-formed.
  const std::string kXml("<Tom><dick>foo</dick><harry>bar</harry>");
  std::string errors;
  TestXmlHandler handler;
  ASSERT_FALSE(ExpatParser::ParseString(kXml, &handler, &errors, false));
  ASSERT_FALSE(errors.empty());
  ASSERT_EQ(kXml, handler.get_xml());
}

// Verify basic usage of the ParseBuffer method.
TEST(ExpatParserTest, TestPassingParseBuffer) {
  const std::string kXml("<Tom><dick>foo</dick><harry>bar</harry></Tom>");
  std::string errors;
  TestXmlHandler handler;
  ExpatParser parser(&handler, false);

  // Parse the string one character at a time.
  for (size_t i = 0; i < kXml.length(); ++i) {
    ASSERT_TRUE(parser.ParseBuffer(kXml.substr(i, 1), &errors,
                                   i == kXml.length()-1));
  }
  ASSERT_EQ(kXml, handler.get_xml());
}

// Verify failure of ParseBuffer on badly formed XML content.
TEST(ExpatParserTest, TestFailingParseBuffer) {
  // kXml is not well-formed.
  const std::string kXml("<Tom><dick>foo</dick><harry>bar</harry>");
  std::string errors;
  TestXmlHandler handler;
  ExpatParser parser(&handler, false);

  // Parse the string one character at a time.
  for (size_t i = 0; i < kXml.length(); ++i) {
    ASSERT_TRUE(parser.ParseBuffer(kXml.substr(i, 1), &errors, false));
    ASSERT_TRUE(errors.empty());
  }
  // Now set the is_final bool to true to indicate that we believe parsing
  // is done. Expat will check and see that its own parsing state shows
  // more content is necessary because our XML is missing the closing
  // </Tom> tag.
  ASSERT_FALSE(parser.ParseBuffer("", &errors, true));
  ASSERT_FALSE(errors.empty());

  ASSERT_EQ(kXml, handler.get_xml());
}

// Assert that we detect a mid-stream parsing failure.
TEST(ExpatParserTest, TestMidstreamFailingParseBuffer) {
  const std::string k0("<A><B><C><D>");
  const std::string k1("</D>");  // This is fine.
  const std::string k2("</B>");  // XML is badly formed here, missing </C>.
  std::string errors;
  TestXmlHandler handler;
  ExpatParser parser(&handler, false);

  ASSERT_TRUE(parser.ParseBuffer(k0, &errors, false));
  ASSERT_TRUE(errors.empty());

  ASSERT_TRUE(parser.ParseBuffer(k1, &errors, false));
  ASSERT_TRUE(errors.empty());

  ASSERT_FALSE(parser.ParseBuffer(k2, &errors, false));
  ASSERT_FALSE(errors.empty());
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
