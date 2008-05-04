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

// This file contains the unit tests for the KmlHandler class.

#include "kml/dom/kml_handler.h"
#include <stdlib.h>  // For calloc() and free().
#include "kml/dom/element.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/placemark.h"
#include "kml/util/unit_test.h"

namespace kmldom {

// This class is the unit test fixture for the KmlHandler class.
class KmlHandlerTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmlHandlerTest);
  CPPUNIT_TEST(TestInitialState);
  CPPUNIT_TEST(TestStartSimpleElement);
  CPPUNIT_TEST(TestEndSimpleElement);
  CPPUNIT_TEST(TestBasicCharData);
  CPPUNIT_TEST(TestStartComplexElement);
  CPPUNIT_TEST(TestEndComplexElement);
  CPPUNIT_TEST(TestStartComplexElementWithAtts);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    // Emulate expat's xmlparse.c:startAtts().
    // 16 == xmlparse.c's INIT_ATTS_SIZE
    atts_ = static_cast<const char**>(calloc(16, sizeof(char*)));
    kml_handler_ = new KmlHandler;
  }

  // Called after each test.
  void tearDown() {
    free(atts_);
    delete kml_handler_;
  }

 protected:
  void TestInitialState();
  void TestStartSimpleElement();
  void TestEndSimpleElement();
  void TestBasicCharData();
  void TestStartComplexElement();
  void TestEndComplexElement();
  void TestStartComplexElementWithAtts();

 private:
  const char** atts_;
  KmlHandler* kml_handler_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmlHandlerTest);

// This verifies the initial state of a freshly constructed KmlHandler.
void KmlHandlerTest::TestInitialState() {
  // No elements have been processed, but the PopRoot() method should
  // be well behaved.
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of the StartElement() method for a known simple element.
void KmlHandlerTest::TestStartSimpleElement() {
  // This is what expat sends to StartElement() on "<name>".
  kml_handler_->StartElement("name", atts_);

  // Since "name" is known we will find it as the root element.
  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_name);

  // PopRoot() is destructive so now there is nothing.
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of the EndElement() method for a known simple element.
void KmlHandlerTest::TestEndSimpleElement() {
  // This is what expat sends to KmlHandler on "<name/>":
  kml_handler_->StartElement("name", atts_);
  kml_handler_->EndElement("name");

  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_name);

  // PopRoot() is destructive so now there is nothing.
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of the CharData() method for a known simple element.
void KmlHandlerTest::TestBasicCharData() {
  // This is what expat sends to KmlHandler on "<name>what is in a</name>":
  const char* kTagName = "name";
  const char* kContent = "what is in a name";

  kml_handler_->StartElement(kTagName, atts_);
  kml_handler_->CharData(kContent, strlen(kContent));
  kml_handler_->EndElement(kTagName);

  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_name);
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
  CPPUNIT_ASSERT(kContent == root->char_data());
}

// This is a test of the StartElement() method for a known complex element.
void KmlHandlerTest::TestStartComplexElement() {
  kml_handler_->StartElement("Placemark", atts_);
  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_Placemark);
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of the EndElement() method for a known complex element.
void KmlHandlerTest::TestEndComplexElement() {
  // This is what expat does for "<Placemark/>".
  kml_handler_->StartElement("Placemark", atts_);
  kml_handler_->EndElement("Placemark");
  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_Placemark);
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
}

// This is a test of StartElement() for a known complex element with known
// attributes.
void KmlHandlerTest::TestStartComplexElementWithAtts() {
  const char* kAttrName = "id";
  const char* kAttrVal = "foo";
  atts_[0] = kAttrName;
  atts_[1] = kAttrVal;
  kml_handler_->StartElement("Placemark", atts_);
  ElementPtr root = kml_handler_->PopRoot();
  CPPUNIT_ASSERT(root->Type() == Type_Placemark);
  CPPUNIT_ASSERT(NULL == kml_handler_->PopRoot());
  PlacemarkPtr placemark = AsPlacemark(root);
  CPPUNIT_ASSERT(kAttrVal == placemark->id());
}

}  // end namespace kmldom

TEST_MAIN
