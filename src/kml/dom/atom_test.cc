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

// This file contains the unit tests for the <atom:author> and <atom:link>
// elements.

#include "kml/dom/atom.h"
#include "kml/base/xml_namespaces.h"
#include "kml/dom/kml_factory.h"
#include "gtest/gtest.h"

namespace kmldom {

class AtomAuthorTest : public testing::Test {
 protected:
  virtual void SetUp() {
    atomauthor_ = KmlFactory::GetFactory()->CreateAtomAuthor();
  }

  AtomAuthorPtr atomauthor_;
};

TEST_F(AtomAuthorTest, TestType) {
  ASSERT_EQ(Type_AtomAuthor, atomauthor_->Type());
  ASSERT_FALSE(atomauthor_->IsA(Type_Object));
  ASSERT_TRUE(atomauthor_->IsA(Type_AtomAuthor));
}

TEST_F(AtomAuthorTest, TestXmlNamespace) {
  ASSERT_EQ(kmlbase::XMLNS_ATOM, atomauthor_->get_xmlns());
}

class AtomLinkTest : public testing::Test {
 protected:
  virtual void SetUp() {
    atomlink_ = KmlFactory::GetFactory()->CreateAtomLink();
  }

  AtomLinkPtr atomlink_;
};

TEST_F(AtomLinkTest, TestType) {
  ASSERT_EQ(Type_AtomLink, atomlink_->Type());
  ASSERT_FALSE(atomlink_->IsA(Type_Object));
  ASSERT_TRUE(atomlink_->IsA(Type_AtomLink));
}

TEST_F(AtomLinkTest, TestXmlNamespace) {
  ASSERT_EQ(kmlbase::XMLNS_ATOM, atomlink_->get_xmlns());
}

}  // namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
