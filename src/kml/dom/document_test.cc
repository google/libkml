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

#include "kml/dom/document.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_ptr.h"
#include "gtest/gtest.h"

namespace kmldom {

class DocumentTest : public testing::Test {
 protected:
  virtual void SetUp() {
    document_ = KmlFactory::GetFactory()->CreateDocument();
  }

  DocumentPtr document_;
};

TEST_F(DocumentTest, TestType) {
  ASSERT_TRUE(Type_Document == document_->Type());
  ASSERT_TRUE(document_->IsA(Type_Document));
  ASSERT_TRUE(document_->IsA(Type_Container));
  ASSERT_TRUE(document_->IsA(Type_Feature));
  ASSERT_TRUE(document_->IsA(Type_Object));
}

TEST_F(DocumentTest, TestSchema) {
  ASSERT_EQ(static_cast<size_t>(0), document_->get_schema_array_size());
  SchemaPtr s0(KmlFactory::GetFactory()->CreateSchema());
  SchemaPtr s1(KmlFactory::GetFactory()->CreateSchema());
  SchemaPtr s3(KmlFactory::GetFactory()->CreateSchema());
  document_->add_schema(s0);
  document_->add_schema(s1);
  document_->add_schema(s3);
  ASSERT_EQ(static_cast<size_t>(3), document_->get_schema_array_size());
  ASSERT_TRUE(Type_Schema == document_->get_schema_array_at(0)->Type());
  ASSERT_TRUE(Type_Schema == document_->get_schema_array_at(1)->Type());
  ASSERT_TRUE(Type_Schema == document_->get_schema_array_at(2)->Type());
}

TEST_F(DocumentTest, TestStyleSelectors) {
  ASSERT_EQ(static_cast<size_t>(0), document_->get_styleselector_array_size());
  StylePtr s0(KmlFactory::GetFactory()->CreateStyle());
  StylePtr s1(KmlFactory::GetFactory()->CreateStyle());
  StyleMapPtr sm(KmlFactory::GetFactory()->CreateStyleMap());
  document_->add_styleselector(s0);
  document_->add_styleselector(s1);
  document_->add_styleselector(sm);
  ASSERT_EQ(static_cast<size_t>(3), document_->get_styleselector_array_size());
  ASSERT_TRUE(Type_Style ==
                 document_->get_styleselector_array_at(0)->Type());
  ASSERT_TRUE(Type_Style ==
                 document_->get_styleselector_array_at(1)->Type());
  ASSERT_TRUE(Type_StyleMap ==
                 document_->get_styleselector_array_at(2)->Type());
}

// Verify serialization follows XSD order.
TEST_F(DocumentTest, TestParseSerialize) {
  const string kDocument(
      "<Document>"
      "<name>hi</name>"
      "<description>hello</description>"
      "<styleUrl>#style-id</styleUrl>"
      "<Style/>"
      "<StyleMap/>"
      "<Style/>"
      "<Schema id=\"schema0\"/>"
      "<Schema id=\"schema1\"/>"
      "<Placemark/>"
      "<Document/>"
      "<GroundOverlay/>"
      "</Document>");
  ASSERT_EQ(kDocument, kmldom::SerializeRaw(kmldom::Parse(kDocument, NULL)));
}

TEST_F(DocumentTest, TestDeleteSchemaAndStyleSelectorAt) {
  // This is mostly just to test that these methods exist.  See element_test.cc
  // for a full test of the underlying Element::DeleteFromArrayAt method.

  // Verify proper return for out of range index.
  ASSERT_FALSE(document_->DeleteSchemaAt(0));
  ASSERT_FALSE(document_->DeleteStyleSelectorAt(0));

  StylePtr orig_style(KmlFactory::GetFactory()->CreateStyle());
  orig_style->set_id("style0");
  document_->add_styleselector(orig_style);
  StyleSelectorPtr got_style = document_->DeleteStyleSelectorAt(0);
  ASSERT_TRUE(got_style);
  ASSERT_EQ(orig_style->get_id(), got_style->get_id());
  ASSERT_FALSE(document_->DeleteStyleSelectorAt(0));

  SchemaPtr orig_schema(KmlFactory::GetFactory()->CreateSchema());
  orig_schema->set_name("schema0");
  document_->add_schema(orig_schema);
  SchemaPtr got_schema = document_->DeleteSchemaAt(0);
  ASSERT_TRUE(got_schema);
  ASSERT_EQ(orig_schema->get_name(), got_schema->get_name());
  ASSERT_FALSE(document_->DeleteSchemaAt(0));
}

}  // end namespace kmldom
