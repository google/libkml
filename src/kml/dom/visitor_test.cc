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

// This file contains the unit tests for the abstract Object element.

#include "kml/dom/visitor.h"
#include "kml/dom/kmldom.h"
#include "kml/dom/kml_factory.h"
#include "gtest/gtest.h"

namespace kmldom {

class TestVisitor : public Visitor {
 public:
  TestVisitor() { }
  virtual ~TestVisitor() { }

  virtual Visitor::Status VisitContainer(const ContainerPtr& node) {
    output_ += "C:" + node->get_name() + " { ";
    return Visitor::CONTINUE;
  }
  virtual void VisitContainerEnd(const ContainerPtr& node) {
    output_ += "}";
  }

  virtual Visitor::Status VisitFeature(const FeaturePtr& node) {
    output_ += "F:" + node->get_name() + " ";
    return Visitor::CONTINUE;
  }
  std::string output_;
};

TEST(Visitor, TestBasic) {
  KmlFactory* factory = KmlFactory::GetFactory();
  DocumentPtr doc = factory->CreateDocument();
  doc->set_name("doc");

  PlacemarkPtr p1 = factory->CreatePlacemark();
  doc->add_feature(p1);
  p1->set_name("first");

  PlacemarkPtr p2 = factory->CreatePlacemark();
  doc->add_feature(p2);
  p2->set_name("second");

  PlacemarkPtr p3 = factory->CreatePlacemark();
  doc->add_feature(p3);
  p3->set_name("last");

  TestVisitor v;
  doc->Accept(&v);
  EXPECT_EQ("C:doc { F:first F:second F:last }", v.output_);
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
