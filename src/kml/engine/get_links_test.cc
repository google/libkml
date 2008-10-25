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

// This file contains the unit tests for the GetLinks function.

#include "kml/engine/get_links.h"
#include "kml/base/file.h"
#include "gtest/gtest.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#else
static const std::string kDataDir = DATADIR;
#endif

namespace kmlengine {

class GetLinksTest : public testing::Test {
};

// Verify that GetLinks() returns false if given no output vector.
TEST_F(GetLinksTest, TestNull) {
  const std::string nothing;
  ASSERT_FALSE(GetLinks(nothing, NULL));
}

// Verify very basic usage.
TEST_F(GetLinksTest, TestBasic) {
  const std::string kHref = "foo.kml";
  const std::string kNetworkLink = "<NetworkLink><Link><href>" + kHref +
                                   "</href></Link></NetworkLink>";
  href_vector_t href_vector;
  ASSERT_TRUE(GetLinks(kNetworkLink, &href_vector));
  ASSERT_EQ(static_cast<size_t>(1), href_vector.size());
  ASSERT_EQ(kHref, href_vector[0]);
}

// Verify that GetLinks finds all kinds of hrefs in a KML file.
TEST_F(GetLinksTest, TestAll) {
  const std::string kAllLinks = kDataDir + "/links/alllinks.kml";
  std::string kml;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(kAllLinks, &kml));
  href_vector_t href_vector;
  ASSERT_TRUE(GetLinks(kml, &href_vector));
  // This is obviously exactly matched to the content of alllinks.kml.
  ASSERT_EQ(static_cast<size_t>(9), href_vector.size());
  ASSERT_EQ(std::string("http://example.com/icon.jpg"),
                       href_vector[0]);
  ASSERT_EQ(std::string("itemicon.png"), href_vector[1]);
  ASSERT_EQ(std::string("../more.kml"), href_vector[2]);
  ASSERT_EQ(std::string("go.jpeg"), href_vector[3]);
  ASSERT_EQ(std::string("so.jpeg"), href_vector[4]);
  ASSERT_EQ(std::string("po.jpeg"), href_vector[5]);
  ASSERT_EQ(std::string("style.kml#style"), href_vector[6]);
  ASSERT_EQ(std::string("#myschema"), href_vector[7]);
  ASSERT_EQ(std::string("model.dae"), href_vector[8]);
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
