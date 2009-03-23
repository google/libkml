// Copyright 2009, Google Inc. All rights reserved.
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

// This file contains the unit tests for the GetElementsById() function
// and the IdMapper class.

#include "kml/engine/find_xml_namespaces.h"
#include "gtest/gtest.h"
#include "kml/base/attributes.h"
#include "kml/dom.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

using kmlbase::Attributes;
using kmldom::DocumentPtr;
using kmldom::KmlFactory;

namespace kmlengine {

// If either or both args to FindXmlNamespaces() is NULL nothing happens and
// there is no crash.
TEST(FindXmlNamepacesTest, TestNull) {
  FindXmlNamespaces(NULL, NULL);
  Attributes xmlns_attributes;
  FindXmlNamespaces(NULL, &xmlns_attributes);
  FindXmlNamespaces(KmlFactory::GetFactory()->CreateDocument(), NULL);
}

TEST(FindXmlNamepacesTest, TestNoNamespaces) {
  kmldom::DocumentPtr document = KmlFactory::GetFactory()->CreateDocument();
  Attributes xmlns_attributes;
  FindXmlNamespaces(document, &xmlns_attributes);
  ASSERT_EQ(static_cast<size_t>(0), xmlns_attributes.GetSize());
}

TEST(FindXmlNamepacesTest, TestOneGx) {
  KmlFactory* factory = KmlFactory::GetFactory();
  kmldom::DocumentPtr document = factory->CreateDocument();
  document->add_feature(factory->CreateGxTour());
  Attributes xmlns_attributes;
  FindXmlNamespaces(document, &xmlns_attributes);
  ASSERT_EQ(static_cast<size_t>(1), xmlns_attributes.GetSize());
  std::string xml_namespace;
  ASSERT_TRUE(xmlns_attributes.GetValue("gx", &xml_namespace));
}

TEST(FindXmlNamepacesTest, TestOneGxAndOneAtom) {
  KmlFactory* factory = KmlFactory::GetFactory();
  kmldom::DocumentPtr document = factory->CreateDocument();
  document->add_feature(factory->CreateGxTour());
  document->set_atomauthor(factory->CreateAtomAuthor());
  Attributes xmlns_attributes;
  FindXmlNamespaces(document, &xmlns_attributes);
  ASSERT_EQ(static_cast<size_t>(2), xmlns_attributes.GetSize());
  std::string xml_namespace;
  ASSERT_TRUE(xmlns_attributes.GetValue("gx", &xml_namespace));
  ASSERT_TRUE(xmlns_attributes.GetValue("atom", &xml_namespace));
}

TEST(FindXmlNamepacesTest, TestOneGxAndOneAtomAndOneXal) {
  KmlFactory* factory = KmlFactory::GetFactory();
  kmldom::DocumentPtr document = factory->CreateDocument();
  document->add_feature(factory->CreateGxTour());
  document->set_atomauthor(factory->CreateAtomAuthor());
  document->set_xaladdressdetails(factory->CreateXalAddressDetails());
  Attributes xmlns_attributes;
  FindXmlNamespaces(document, &xmlns_attributes);
  ASSERT_EQ(static_cast<size_t>(3), xmlns_attributes.GetSize());
  std::string xml_namespace;
  ASSERT_TRUE(xmlns_attributes.GetValue("gx", &xml_namespace));
  ASSERT_TRUE(xmlns_attributes.GetValue("atom", &xml_namespace));
  ASSERT_TRUE(xmlns_attributes.GetValue("xal", &xml_namespace));
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
