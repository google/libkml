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

// This file contains the unit tests for the Href class.

#include "kml/engine/href.h"
#include "kml/util/unit_test.h"

namespace kmlengine {

class HrefTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(HrefTest);
  CPPUNIT_TEST(TestHrefNullFragment);
  CPPUNIT_TEST(TestHrefFragmentSetGetHasClear);
  CPPUNIT_TEST(TestHrefFragmentSimple);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestHrefNullFragment();
  void TestHrefFragmentSetGetHasClear();
  void TestHrefFragmentSimple();

 public:
  // Called before each test.
  void setUp() {
  }

  // Called after each test.
  void tearDown() {
  }
};

CPPUNIT_TEST_SUITE_REGISTRATION(HrefTest);

// Verify has_fragment() returns false on a URL with no fragment.
void HrefTest::TestHrefNullFragment() {
  Href href("http://foo.goo/index.kml");
  CPPUNIT_ASSERT(!href.has_fragment());
}

// Verify basic usage of the get,set,has,clear_fragment methods.
void HrefTest::TestHrefFragmentSetGetHasClear() {
  const std::string kFragment("the-fragment");
  Href href;
  href.set_fragment(kFragment);
  CPPUNIT_ASSERT(href.has_fragment());
  CPPUNIT_ASSERT_EQUAL(kFragment, href.get_fragment());
  href.clear_fragment();
  CPPUNIT_ASSERT(!href.has_fragment());
}

void HrefTest::TestHrefFragmentSimple() {
  const std::string kId("hi");
  Href href("#" + kId);
  CPPUNIT_ASSERT(href.has_fragment());
  CPPUNIT_ASSERT_EQUAL(kId, href.get_fragment());
}

}  // end namespace kmlengine

TEST_MAIN
