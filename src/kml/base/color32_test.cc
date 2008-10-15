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

// This file contains the unit tests for the Color32 class.

#include "kml/base/color32.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/unit_test.h"

namespace kmlbase {

class ColorTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ColorTest);
  CPPUNIT_TEST(TestConstruction);
  CPPUNIT_TEST(TestGetSet);
  CPPUNIT_TEST(TestOperators);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestConstruction();
  void TestGetSet();
  void TestOperators();

 private:
  boost::scoped_ptr<Color32> color_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ColorTest);

// Colors we'll use in multiple tests, given as AABBGGRR.
static const uint32 kOpaqueWhite = 0xffffffff;
static const uint32 kOpaqueBlack = 0xff000000;
static const uint32 kOpaqueBlue = 0xffff0000;
static const uint32 kOpaqueGreen = 0xff00ff00;
static const uint32 kOpaqueRed = 0xff0000ff;

void ColorTest::TestConstruction() {
  // Verify basic instantiation of a default Color32 object.
  color_.reset(new Color32());
  CPPUNIT_ASSERT_EQUAL(kOpaqueWhite, color_->get_color_abgr());

  // Verify construction from an int.
  const int32 kRedInt = 0xff0000ff;
  color_.reset(new Color32(kRedInt));
  CPPUNIT_ASSERT_EQUAL(kOpaqueRed, color_->get_color_abgr());

  // Verify construction from a specific color.
  color_.reset(new Color32(kOpaqueBlack));
  CPPUNIT_ASSERT_EQUAL(kOpaqueBlack, color_->get_color_abgr());

  // Verify construction from a bunch of RGBA uchars.
  const unsigned char kA = 0xff;
  const unsigned char kB = 0x00;
  const unsigned char kG = 0x00;
  const unsigned char kR = 0xff;  // The components of kOpaqueRed.
  color_.reset(new Color32(kA, kB, kG, kR));
  CPPUNIT_ASSERT_EQUAL(kOpaqueRed, color_->get_color_abgr());

  // Verify construction from another Color32.
  Color32 color(kOpaqueBlue);
  color_.reset(new Color32(color));
  CPPUNIT_ASSERT_EQUAL(kOpaqueBlue, color_->get_color_abgr());

  // Verify construction from a string.
  const std::string kOpaqueRedStr("ff0000ff");
  color_.reset(new Color32(kOpaqueRedStr));
  CPPUNIT_ASSERT_EQUAL(kOpaqueRed, color_->get_color_abgr());

  // Verify correct behaviour with poorly formed string data.
  //
  // Any string supplied that is less than 8 chars is filled from the front
  // with zeros (and will thus be completely transparent).

  // An fully empty string initalizes to all zeroes (transparent black).
  const std::string kEmptyString("");
  color_.reset(new Color32(kEmptyString));
  std::string expected = "00000000";
  CPPUNIT_ASSERT_EQUAL(expected, color_->to_string_abgr());

  const std::string kOnlySixCharsGiven("ffffff");
  color_.reset(new Color32(kOnlySixCharsGiven));
  expected = "00ffffff";
  CPPUNIT_ASSERT_EQUAL(expected, color_->to_string_abgr());

  const std::string kOnly2CharsGiven("ff");
  color_.reset(new Color32(kOnly2CharsGiven));
  expected = "000000ff";
  CPPUNIT_ASSERT_EQUAL(expected, color_->to_string_abgr());

  // Only the last eight chars are used for construction from string. Extra
  // chars at the start of the input string are ignored.
  const std::string kTenCharsGiven("aabbccddee");
  color_.reset(new Color32(kTenCharsGiven));
  expected = "bbccddee";
  CPPUNIT_ASSERT_EQUAL(expected, color_->to_string_abgr());

  // The input string here has only two valid hex values in the last eight
  // chars ( the "a" and "e" in "or value") and those are the only chars that
  // won't be replaced with zeroes.
  const std::string kBadString("This isn't even close to a color value");
  color_.reset(new Color32(kBadString));
  expected = "0000a00e";
  CPPUNIT_ASSERT_EQUAL(expected, color_->to_string_abgr());
}

void ColorTest::TestGetSet() {
  // Verify getters of default state.
  color_.reset(new Color32());
  const uint32 kFF = 0xff;
  CPPUNIT_ASSERT_EQUAL(kFF, color_->get_alpha());
  CPPUNIT_ASSERT_EQUAL(kFF, color_->get_blue());
  CPPUNIT_ASSERT_EQUAL(kFF, color_->get_green());
  CPPUNIT_ASSERT_EQUAL(kFF, color_->get_red());

  // Verify getters of newly set state.
  const uint32 kAB = 0xab;
  color_->set_alpha(kAB);
  color_->set_blue(kAB);
  color_->set_green(kAB);
  color_->set_red(kAB);
  CPPUNIT_ASSERT_EQUAL(kAB, color_->get_alpha());
  CPPUNIT_ASSERT_EQUAL(kAB, color_->get_blue());
  CPPUNIT_ASSERT_EQUAL(kAB, color_->get_green());
  CPPUNIT_ASSERT_EQUAL(kAB, color_->get_red());

  // Verify get_color_abgr and get_color_rgba.
  color_.reset(new Color32(kOpaqueGreen));
  const uint32 kOpaqueGreenARGB = 0xff00ff00;
  CPPUNIT_ASSERT_EQUAL(kOpaqueGreen, color_->get_color_abgr());
  CPPUNIT_ASSERT_EQUAL(kOpaqueGreenARGB, color_->get_color_argb());

  // Verify reconstruction to AABBGGRR string.
  const std::string kOpaqueRedStr("ff0000ff");
  color_.reset(new Color32(kOpaqueRedStr));
  CPPUNIT_ASSERT_EQUAL(kOpaqueRedStr, color_->to_string_abgr());

  // Verify to_argb_string.
  CPPUNIT_ASSERT_EQUAL(kOpaqueRedStr, color_->to_string_abgr());
  const std::string kOpaqueRedStr_argb("ffff0000");
  CPPUNIT_ASSERT_EQUAL(kOpaqueRedStr_argb, color_->to_string_argb());
}

void ColorTest::TestOperators() {
  // Verify correct behavior of overidden class operators.
  // Assignment from uint32.
  color_.reset(new Color32());
  *color_.get() = kOpaqueGreen;  // Same as: Color32 c; c = kOpaqueGreen;
  CPPUNIT_ASSERT_EQUAL(kOpaqueGreen, color_->get_color_abgr());

  // Assignment from int.
  color_.reset(new Color32());
  *color_.get() = kOpaqueRed;
  CPPUNIT_ASSERT_EQUAL(kOpaqueRed, color_->get_color_abgr());

  // Assignment from reference to object.
  color_.reset(new Color32());
  Color32 color(kOpaqueGreen);
  *color_.get() = color;
  CPPUNIT_ASSERT_EQUAL(kOpaqueGreen, color_->get_color_abgr());

  // Inequality operator.
  Color32 black(kOpaqueBlack);
  Color32 green(kOpaqueGreen);
  CPPUNIT_ASSERT(black != green);
  
  // Equality operator.
  CPPUNIT_ASSERT(black == black);
  CPPUNIT_ASSERT(green == green);

  // Greater-than operator.
  CPPUNIT_ASSERT(green > black);

  // Less-than operator.
  CPPUNIT_ASSERT(black < green);
}

}  // end namespace kmlbase

TEST_MAIN
