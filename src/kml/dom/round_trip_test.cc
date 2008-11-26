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

// This file "round-trips" test KML files through parse and serialize.

#include "kml/dom/kml_funcs.h"
#include "kml/base/file.h"
#include "gtest/gtest.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlbase::File;

namespace kmldom {

static struct {
  const char* subdir;
  const char* input_kml;  
  // Golden is expected output of SerializePretty which strips comments and
  // orders known elements in XSD order.
  const char* golden_kml;
} kTestCases[] = {
  { "kml", "all-unknown-input.kml", "all-unknown-golden.kml" },
  { "kml", "all-unknown-attrs-input.kml", "all-unknown-attrs-golden.kml" }
};

static std::string ReadTestCase(const char *subdir, const char* filename) {
  std::string content;
  File::ReadFileToString(
      File::JoinPaths(DATADIR, File::JoinPaths(subdir, filename)), &content);
  return content;
}

static std::string ParseSerialize(const std::string& kml) {
  return kmldom::SerializePretty(kmldom::Parse(kml, NULL));
}

TEST(RoundTripTest, RunTestCases) {
  size_t size = sizeof(kTestCases)/sizeof(kTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    const char* subdir = kTestCases[i].subdir;
    const char* input_kml = kTestCases[i].input_kml;
    const char* golden_kml = kTestCases[i].golden_kml;
    std::string actual_kml = ParseSerialize(ReadTestCase(subdir, input_kml));
    std::string expected_kml = ReadTestCase(subdir, golden_kml);
    EXPECT_EQ(expected_kml, actual_kml);
  }
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
