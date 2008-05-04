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

// This file contains the unit tests for the SimpleData, SchemaData, Datai
// and ExtendedData elements.

#include <string>
#include "kml/dom/extendeddata.h"
#include "kml/dom.h"
#include "kml/util/unit_test.h"

namespace kmldom {

// <SimpleData>
class SimpleDataTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SimpleDataTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    simpledata_ = KmlFactory::GetFactory()->CreateSimpleData();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  SimpleDataPtr simpledata_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SimpleDataTest);

void SimpleDataTest::TestType() {
  CPPUNIT_ASSERT(Type_SimpleData == simpledata_->Type());
  CPPUNIT_ASSERT(true == simpledata_->IsA(Type_SimpleData));
}

void SimpleDataTest::TestDefaults() {
  CPPUNIT_ASSERT("" == simpledata_->name());
  CPPUNIT_ASSERT(false == simpledata_->has_name());
  CPPUNIT_ASSERT("" == simpledata_->text());
  CPPUNIT_ASSERT(false == simpledata_->has_text());
}

void SimpleDataTest::TestSetToDefaultValues() {
  TestDefaults();
  simpledata_->set_name(simpledata_->name());
  CPPUNIT_ASSERT(true == simpledata_->has_name());
  simpledata_->set_text(simpledata_->text());
  CPPUNIT_ASSERT(true == simpledata_->has_text());
}

void SimpleDataTest::TestSetGetHasClear() {
  std::string name("tom");
  simpledata_->set_name(name);
  CPPUNIT_ASSERT(true == simpledata_->has_name());
  CPPUNIT_ASSERT(name == simpledata_->name());
  simpledata_->clear_name();

  std::string text("dick");
  simpledata_->set_text(text);
  CPPUNIT_ASSERT(true == simpledata_->has_text());
  CPPUNIT_ASSERT(text == simpledata_->text());
  simpledata_->clear_text();

  TestDefaults();
}

// <SchemaData>
class SchemaDataTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SchemaDataTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestSchemaUrl);
  CPPUNIT_TEST(TestLists);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    schemadata_ = KmlFactory::GetFactory()->CreateSchemaData();
  }
  void tearDown() {
    // schemadata_'s destructor releases underlying SchemaData storage.
  }

 protected:
  void TestType();
  void TestSchemaUrl();
  void TestLists();

 private:
  SchemaDataPtr schemadata_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SchemaDataTest);

void SchemaDataTest::TestType() {
  CPPUNIT_ASSERT(Type_SchemaData == schemadata_->Type());
  CPPUNIT_ASSERT(true == schemadata_->IsA(Type_SchemaData));
}

void SchemaDataTest::TestSchemaUrl() {
  CPPUNIT_ASSERT(false == schemadata_->has_schemaurl());
  CPPUNIT_ASSERT("" == schemadata_->schemaurl());
  std::string schemaurl("#icanhasurl");
  schemadata_->set_schemaurl(schemaurl);
  CPPUNIT_ASSERT(true == schemadata_->has_schemaurl());
  CPPUNIT_ASSERT(schemaurl == schemadata_->schemaurl());
  schemadata_->clear_schemaurl();
  CPPUNIT_ASSERT(false == schemadata_->has_schemaurl());
  CPPUNIT_ASSERT("" == schemadata_->schemaurl());
}

void SchemaDataTest::TestLists() {
  // Vector is empty.
  CPPUNIT_ASSERT(0 == schemadata_->simpledata_array_size());
  // Add three <SimpleData> elements:
  schemadata_->add_simpledata(KmlFactory::GetFactory()->CreateSimpleData());
  schemadata_->add_simpledata(KmlFactory::GetFactory()->CreateSimpleData());
  schemadata_->add_simpledata(KmlFactory::GetFactory()->CreateSimpleData());
  // We have three items in the array:
  CPPUNIT_ASSERT(3 == schemadata_->simpledata_array_size());
  for (size_t i = 0; i < schemadata_->simpledata_array_size(); ++i) {
    CPPUNIT_ASSERT(
        Type_SimpleData == schemadata_->simpledata_array_at(i)->Type());
  }
}

// <Data>
class DataTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(DataTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    data_ = KmlFactory::GetFactory()->CreateData();
  }
  void tearDown() {
    // data_'s destructor releases underlying Data storage.
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  DataPtr data_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DataTest);

void DataTest::TestType() {
  CPPUNIT_ASSERT(Type_Data == data_->Type());
  CPPUNIT_ASSERT(true == data_->IsA(Type_Data));
}

void DataTest::TestDefaults() {
  CPPUNIT_ASSERT(false == data_->has_name());
  CPPUNIT_ASSERT("" == data_->name());
  CPPUNIT_ASSERT(false == data_->has_displayname());
  CPPUNIT_ASSERT("" == data_->displayname());
  CPPUNIT_ASSERT(false == data_->has_value());
  CPPUNIT_ASSERT("" == data_->value());
}

void DataTest::TestSetToDefaultValues() {
  data_->set_name(data_->name());
  CPPUNIT_ASSERT(true == data_->has_name());
  data_->set_displayname(data_->displayname());
  CPPUNIT_ASSERT(true == data_->has_displayname());
  data_->set_value(data_->value());
  CPPUNIT_ASSERT(true == data_->has_value());
}

void DataTest::TestSetGetHasClear() {
  std::string name("tom");
  data_->set_name(name);
  CPPUNIT_ASSERT(true == data_->has_name());
  CPPUNIT_ASSERT(name == data_->name());
  data_->clear_name();

  std::string displayname("dick");
  data_->set_displayname(displayname);
  CPPUNIT_ASSERT(true == data_->has_displayname());
  CPPUNIT_ASSERT(displayname == data_->displayname());
  data_->clear_displayname();

  std::string value("harry");
  data_->set_value(value);
  CPPUNIT_ASSERT(true == data_->has_value());
  CPPUNIT_ASSERT(value == data_->value());
  data_->clear_value();

  TestDefaults();
}

// <ExtendedData>
class ExtendedDataTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ExtendedDataTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestLists);
  CPPUNIT_TEST(TestParse);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    extendeddata_ = KmlFactory::GetFactory()->CreateExtendedData();
  }
  void tearDown() {
    // extendeddata_'s destructor releases underlying ExtendedData storage.
  }

 protected:
  void TestType();
  void TestLists();
  void TestParse();

 private:
  ExtendedDataPtr extendeddata_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ExtendedDataTest);

void ExtendedDataTest::TestType() {
  CPPUNIT_ASSERT(Type_ExtendedData == extendeddata_->Type());
  CPPUNIT_ASSERT(true == extendeddata_->IsA(Type_ExtendedData));
}

void ExtendedDataTest::TestLists() {
  // Vector is empty.
  CPPUNIT_ASSERT(0 == extendeddata_->extendeddatamember_array_size());
  // Add three <Data> and three <SchemaData> elements:
  extendeddata_->add_extendeddatamember(
      KmlFactory::GetFactory()->CreateData());
  extendeddata_->add_extendeddatamember(
      KmlFactory::GetFactory()->CreateData());
  extendeddata_->add_extendeddatamember(
      KmlFactory::GetFactory()->CreateSchemaData());
  extendeddata_->add_extendeddatamember(
      KmlFactory::GetFactory()->CreateSchemaData());
  extendeddata_->add_extendeddatamember(
      KmlFactory::GetFactory()->CreateData());
  extendeddata_->add_extendeddatamember(
      KmlFactory::GetFactory()->CreateSchemaData());
  // We have six items in the array:
  CPPUNIT_ASSERT(6 == extendeddata_->extendeddatamember_array_size());
  // Assert ordering is preserved:
  CPPUNIT_ASSERT(
      Type_Data == extendeddata_->extendeddatamember_array_at(0)->Type());
  CPPUNIT_ASSERT(
      Type_Data == extendeddata_->extendeddatamember_array_at(1)->Type());
  CPPUNIT_ASSERT(
      Type_SchemaData == extendeddata_->extendeddatamember_array_at(2)->Type());
  CPPUNIT_ASSERT(
      Type_SchemaData == extendeddata_->extendeddatamember_array_at(3)->Type());
  CPPUNIT_ASSERT(
      Type_Data == extendeddata_->extendeddatamember_array_at(4)->Type());
  CPPUNIT_ASSERT(
      Type_SchemaData == extendeddata_->extendeddatamember_array_at(5)->Type());
}

void ExtendedDataTest::TestParse() {
  const std::string schemaurl("http://example.com/index.kml#schemaId");
  const std::string d_name("myCoolDataName");
  const std::string displayname("my cool displayName");
  const std::string value("my cool value");
  const std::string sd_name("myCoolSimpleDataName");
  const std::string chardata("some char data");
  const std::string kml =
    "<ExtendedData>"
    "<Data name=\"" + d_name + "\">"
    "<displayName>" + displayname + "</displayName>"
    "<value>" + value + "</value>"
    "</Data>"
    "<SchemaData schemaUrl=\"" + schemaurl + "\">"
    "<SimpleData name=\"" + sd_name + "\">" + chardata + "</SimpleData>"
    "</SchemaData>"
    "</ExtendedData>";
  std::string errors;
  ElementPtr root = Parse(kml, &errors);
  CPPUNIT_ASSERT(root);
  CPPUNIT_ASSERT(errors.empty());
  const ExtendedDataPtr extendeddata = AsExtendedData(root);
  CPPUNIT_ASSERT(extendeddata);
  CPPUNIT_ASSERT(2 == extendeddata->extendeddatamember_array_size());
  const DataPtr data = AsData(extendeddata->extendeddatamember_array_at(0));
  CPPUNIT_ASSERT(data);
  CPPUNIT_ASSERT(data->has_name());
  CPPUNIT_ASSERT_EQUAL(d_name, data->name());
  CPPUNIT_ASSERT(data->has_displayname());
  CPPUNIT_ASSERT_EQUAL(displayname, data->displayname());
  CPPUNIT_ASSERT(data->has_value());
  CPPUNIT_ASSERT_EQUAL(value, data->value());
  const SchemaDataPtr schemadata = AsSchemaData(
      extendeddata->extendeddatamember_array_at(1));
  CPPUNIT_ASSERT(schemadata);
  CPPUNIT_ASSERT(schemadata->has_schemaurl());
  CPPUNIT_ASSERT_EQUAL(schemaurl, schemadata->schemaurl());
  CPPUNIT_ASSERT(1 == schemadata->simpledata_array_size());
  const SimpleDataPtr simpledata = AsSimpleData(
      schemadata->simpledata_array_at(0));
  CPPUNIT_ASSERT(simpledata);
  CPPUNIT_ASSERT(simpledata->has_name());
  CPPUNIT_ASSERT(simpledata->has_text());
  CPPUNIT_ASSERT_EQUAL(sd_name, simpledata->name());
  CPPUNIT_ASSERT_EQUAL(chardata, simpledata->text());
}

}  // end namespace kmldom

TEST_MAIN
