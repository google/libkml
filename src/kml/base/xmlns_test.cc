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

// This file contains the unit tests for the Xmlns class.

#include "kml/base/xmlns.h"
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/unit_test.h"

namespace kmlbase {

class XmlnsTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(XmlnsTest);
  CPPUNIT_TEST(TestCreate);
  CPPUNIT_TEST(TestNullCreate);
  CPPUNIT_TEST(TestGetKey);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestCreate();
  void TestNullCreate();
  void TestGetKey();

 private:
  boost::scoped_ptr<Attributes> attributes_;
  boost::scoped_ptr<Xmlns> xmlns_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(XmlnsTest);

// Just to pick a random example test case...
// <schema xmlns="http://www.w3.org/2001/XMLSchema"
//         xmlns:kml="http://www.opengis.net/kml/2.2"
//         xmlns:atom="http://www.w3.org/2005/Atom"
//         xmlns:xal="urn:oasis:names:tc:ciq:xsdschema:xAL:2.0"
//         targetNamespace="http://www.opengis.net/kml/2.2"
//         elementFormDefault="qualified"
//         version="2.2.0">
void XmlnsTest::TestCreate() {
  // Expat turns the above into this list.
  const char* schema_attrs[] = {
    "xmlns", "http://www.w3.org/2001/XMLSchema",
    "xmlns:kml", "http://www.opengis.net/kml/2.2",
    "xmlns:atom", "http://www.w3.org/2005/Atom",
    "xmlns:xal", "urn:oasis:names:tc:ciq:xsdschema:xAL:2.0",
    "targetNamespace", "http://www.opengis.net/kml/2.2",
    "elementFormDefault", "qualified",
    "version", "2.2.0",
    NULL
  };
  attributes_.reset(Attributes::Create(schema_attrs));
  CPPUNIT_ASSERT(attributes_.get());
  // This is the method under test.
  xmlns_.reset(Xmlns::Create(*attributes_));
  // The default namespace is the value of the "xmlns" attribute.
  CPPUNIT_ASSERT_EQUAL(std::string(schema_attrs[1]), xmlns_->get_default());
  CPPUNIT_ASSERT_EQUAL(std::string(schema_attrs[3]),
                       xmlns_->GetNamespace("kml"));
  CPPUNIT_ASSERT_EQUAL(std::string(schema_attrs[5]),
                       xmlns_->GetNamespace("atom"));
}

// Verify the NULL return path of Create().
void XmlnsTest::TestNullCreate() {
  attributes_.reset(new Attributes);  // Empty attributes.
  xmlns_.reset(Xmlns::Create(*attributes_));
  // No attributes, no Xmlns.
  CPPUNIT_ASSERT(!xmlns_.get());
}

void XmlnsTest::TestGetKey() {
  attributes_.reset(new Attributes);
  const std::string kPrefix("mcn");
  const std::string kNamespace("my:cool:namespace");
  attributes_->SetString(std::string("xmlns:") + kPrefix, kNamespace);
  xmlns_.reset(Xmlns::Create(*attributes_));
  CPPUNIT_ASSERT_EQUAL(kPrefix, xmlns_->GetKey(kNamespace));
}

}  // end namespace kmlbase

TEST_MAIN
