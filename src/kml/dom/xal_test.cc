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

// This file contains the unit tests for the <xal:AddressDetails> elements.

#include "kml/dom/xal.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "gtest/gtest.h"

namespace kmldom {

class XalAddressDetailsTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xaladdressdetails_ = KmlFactory::GetFactory()->CreateXalAddressDetails();
  }

  XalAddressDetailsPtr xaladdressdetails_;
};

TEST_F(XalAddressDetailsTest, TestType) {
  ASSERT_EQ(Type_XalAddressDetails, xaladdressdetails_->Type());
  ASSERT_TRUE(xaladdressdetails_->IsA(Type_XalAddressDetails));
}

TEST_F(XalAddressDetailsTest, TestDefault) {
  ASSERT_FALSE(xaladdressdetails_->has_country());
  ASSERT_FALSE(xaladdressdetails_->get_country());
}

TEST_F(XalAddressDetailsTest, TestSetGetHasClear) {
  XalCountryPtr country(KmlFactory::GetFactory()->CreateXalCountry());
  xaladdressdetails_->set_country(country);
  ASSERT_TRUE(xaladdressdetails_->has_country());
  ASSERT_EQ(country, xaladdressdetails_->get_country());
  xaladdressdetails_->clear_country();
  ASSERT_FALSE(xaladdressdetails_->has_country());
  ASSERT_FALSE(xaladdressdetails_->get_country());
}

TEST_F(XalAddressDetailsTest, TestParseSerialize) {
  const std::string kAddressDetails(
      "<xal:AddressDetails>"
      "<xal:Country/>"
      "</xal:AddressDetails>");
  ASSERT_EQ(kAddressDetails,
            kmldom::SerializeRaw(kmldom::Parse(kAddressDetails, NULL)));
}

class XalCountryTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xalcountry_ = KmlFactory::GetFactory()->CreateXalCountry();
  }

  XalCountryPtr xalcountry_;
};

TEST_F(XalCountryTest, TestType) {
  ASSERT_EQ(Type_XalCountry, xalcountry_->Type());
  ASSERT_TRUE(xalcountry_->IsA(Type_XalCountry));
}

TEST_F(XalCountryTest, TestDefault) {
  ASSERT_FALSE(xalcountry_->has_countrynamecode());
  ASSERT_FALSE(xalcountry_->has_administrativearea());
  ASSERT_TRUE(xalcountry_->get_countrynamecode().empty());
  ASSERT_FALSE(xalcountry_->get_administrativearea());
}

TEST_F(XalCountryTest, TestSetGetHasClear) {
  XalAdministrativeAreaPtr administrativearea(
      KmlFactory::GetFactory()->CreateXalAdministrativeArea());
  const std::string kCountryCodeName("US");
  xalcountry_->set_countrynamecode(kCountryCodeName);
  xalcountry_->set_administrativearea(administrativearea);
  ASSERT_TRUE(xalcountry_->has_countrynamecode());
  ASSERT_TRUE(xalcountry_->has_administrativearea());
  ASSERT_EQ(kCountryCodeName, xalcountry_->get_countrynamecode());
  ASSERT_EQ(administrativearea, xalcountry_->get_administrativearea());
  xalcountry_->clear_countrynamecode();
  xalcountry_->clear_administrativearea();
  ASSERT_FALSE(xalcountry_->has_countrynamecode());
  ASSERT_FALSE(xalcountry_->has_administrativearea());
  ASSERT_TRUE(xalcountry_->get_countrynamecode().empty());
  ASSERT_FALSE(xalcountry_->get_administrativearea());
}

TEST_F(XalCountryTest, TestParseSerialize) {
  const std::string kCountry(
      "<xal:Country>"
      "<xal:CountryNameCode>US</xal:CountryNameCode>"
      "<xal:AdministrativeArea/>"
      "</xal:Country>");
  ASSERT_EQ(kCountry,
            kmldom::SerializeRaw(kmldom::Parse(kCountry, NULL)));
}

class XalAdministrativeAreaTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xaladministrativearea_ =
        KmlFactory::GetFactory()->CreateXalAdministrativeArea();
  }

  XalAdministrativeAreaPtr xaladministrativearea_;
};

TEST_F(XalAdministrativeAreaTest, TestType) {
  ASSERT_EQ(Type_XalAdministrativeArea, xaladministrativearea_->Type());
  ASSERT_TRUE(xaladministrativearea_->IsA(Type_XalAdministrativeArea));
}

TEST_F(XalAdministrativeAreaTest, TestDefault) {
  ASSERT_FALSE(xaladministrativearea_->has_subadministrativearea());
  ASSERT_FALSE(xaladministrativearea_->get_subadministrativearea());
}

}  // namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
