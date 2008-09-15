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

// This file contains the unit tests for the ElementCounter class.

#include "kml/convenience/element_counter.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/unit_test.h"
#include "kml/dom.h"
#include "kml/dom/xsd.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

namespace kmlconvenience {

using kmldom::ElementPtr;
using kmldom::Parser;
using kmldom::Xsd;

class ElementCounterTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ElementCounterTest);
  CPPUNIT_TEST(TestEmpty);
  CPPUNIT_TEST(TestBasicParse);
  CPPUNIT_TEST(TestMultipleElements);
  CPPUNIT_TEST(TestRepeatedParse);
  CPPUNIT_TEST(TestEachComplex);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    element_counter_.reset(new ElementCounter(&element_count_map_));
    parser_.reset(new Parser);
    parser_->AddObserver(element_counter_.get());
    xsd_ = Xsd::GetSchema();
  }

  void tearDown() {
  }

 protected:
  void TestEmpty();
  void TestBasicParse();
  void TestMultipleElements();
  void TestRepeatedParse();
  void TestEachComplex();

 private:
  ElementCountMap element_count_map_;
  boost::scoped_ptr<ElementCounter> element_counter_;
  boost::scoped_ptr<Parser> parser_;
  Xsd* xsd_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ElementCounterTest);

void ElementCounterTest::TestEmpty() {
  CPPUNIT_ASSERT(element_count_map_.empty());
}

void ElementCounterTest::TestBasicParse() {
  ElementPtr root = parser_->Parse("<Placemark/>", NULL);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), element_count_map_.size());
  CPPUNIT_ASSERT_EQUAL(1, element_count_map_[kmldom::Type_Placemark]);
}

void ElementCounterTest::TestMultipleElements() {
  const std::string kKml("<Folder><Placemark/><Placemark/></Folder>");
  ElementPtr root = parser_->Parse(kKml, NULL);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), element_count_map_.size());
  CPPUNIT_ASSERT_EQUAL(1, element_count_map_[kmldom::Type_Folder]);
  CPPUNIT_ASSERT_EQUAL(2, element_count_map_[kmldom::Type_Placemark]);
}

void ElementCounterTest::TestRepeatedParse() {
  const std::string kXml("<Placemark/>");
  ElementPtr root = parser_->Parse(kXml, NULL);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), element_count_map_.size());
  CPPUNIT_ASSERT_EQUAL(1, element_count_map_[kmldom::Type_Placemark]);
  root = parser_->Parse(kXml, NULL);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), element_count_map_.size());
  CPPUNIT_ASSERT_EQUAL(2, element_count_map_[kmldom::Type_Placemark]);
}

void ElementCounterTest::TestEachComplex() {
  const int kBegin = static_cast<int>(kmldom::Type_Alias);
  const int kEnd = static_cast<int>(kmldom::Type_ViewVolume) + 1;
  for (int i = kBegin; i != kEnd; ++i) {
    kmldom::KmlDomType type_id = static_cast<kmldom::KmlDomType>(i);
    if (type_id == kmldom::Type_IconStyleIcon ||
        type_id == kmldom::Type_Metadata) {
      continue;
    }
    const std::string kXml(std::string("<") + xsd_->ElementName(i) + "/>");
    ElementPtr root = parser_->Parse(kXml, NULL);
    CPPUNIT_ASSERT(root);
    CPPUNIT_ASSERT_EQUAL(type_id, root->Type());
    CPPUNIT_ASSERT_EQUAL(1, element_count_map_[type_id]);
  }
  const size_t kExpectedSize = static_cast<size_t>(kEnd - kBegin - 2);
  CPPUNIT_ASSERT_EQUAL(kExpectedSize, element_count_map_.size());
  for (int i = kBegin; i != kEnd; ++i) {
    kmldom::KmlDomType type_id = static_cast<kmldom::KmlDomType>(i);
    if (type_id == kmldom::Type_IconStyleIcon ||
        type_id == kmldom::Type_Metadata) {
      continue;
    }
    CPPUNIT_ASSERT_EQUAL(1, element_count_map_[type_id]);
  }
}

}  // namespace kmlconvenience

TEST_MAIN
