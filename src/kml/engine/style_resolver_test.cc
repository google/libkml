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

// This file contains the unit tests for the CreateResolvedStyle() function.
// TODO: styleUrl to external file.

#include "kml/engine/style_resolver.h"
#include <string>
#include "kml/dom.h"
#include "kml/util/file.h"
#include "kml/util/unit_test.h"
#include "kml/engine/kml_file.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#else
static const std::string kDataDir = DATADIR;
#endif

using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::StylePtr;

namespace kmlengine {

class StyleResolverTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(StyleResolverTest);
  CPPUNIT_TEST(TestFiles);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestFiles();

 public:
  // Called before each test.
  void setUp() {
  }

  // Called after each test.
  void tearDown() {
  }

 private:
  // This is an internal utility to read a testdata file.
  bool ReadDataDirFileToString(const std::string& filename,
                               std::string* content) const;
  // Parse the given KML file into the test fixture's KmlFile.
  void ParseFromDataDirFile(const std::string& filename);
  // This returns 0 if element serializes pretty to exactly the contents of
  // the check file.
  int ComparePretty(const ElementPtr& element, const char* check_file) const;
  // KmlFile is used for its GetSharedStyleById().
  KmlFile kml_file_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(StyleResolverTest);

// This is a table of style resolution test cases.  The resolved style for the
// the feature given by the id in feature_id_ for the given style state in the
// source_file_ is as found in the check_file_.  SerializePretty() is used as
// the comparison so the check_file_ should be "pretty".
static const struct {
  const char* source_file_;
  const char* feature_id_;
  const kmldom::StyleStateEnum style_state_;
  const char* check_file_;
} kTestCases [] = {
  { "/style/simple.kml", "pm0", kmldom::STYLESTATE_NORMAL,
    "/style/simple-pm0-check.kml" },
  { "/style/inline-style.kml", "f0", kmldom::STYLESTATE_NORMAL,
    "/style/inline-style-f0-check.kml" },
  { "/style/inline-stylemap.kml", "pm0", kmldom::STYLESTATE_HIGHLIGHT,
    "/style/inline-stylemap-pm0-highlight-check.kml" },
  { "/style/inline-stylemap.kml", "pm0", kmldom::STYLESTATE_NORMAL,
    "/style/inline-stylemap-pm0-normal-check.kml" },
  { "/style/shared-stylemap.kml", "pm0", kmldom::STYLESTATE_HIGHLIGHT,
    "/style/shared-stylemap-pm0-highlight-check.kml" },
  { "/style/shared-stylemap.kml", "pm0", kmldom::STYLESTATE_NORMAL,
    "/style/shared-stylemap-pm0-normal-check.kml" },
  { "/style/allstyles.kml", "f0", kmldom::STYLESTATE_NORMAL,
    "/style/allstyles-f0-normal-check.kml" },
#if 0 // TODO
  { "/style/allstyles.kml", "f0", kmldom::STYLESTATE_HIGHLIGHT,
    "/style/allstyles-f0-highlight-check.kml" },
#endif
  { "/style/allstyles.kml", "f1", kmldom::STYLESTATE_NORMAL,
    "/style/empty-style-check.kml" },
  { "/style/allstyles.kml", "f1", kmldom::STYLESTATE_HIGHLIGHT,
    "/style/empty-style-check.kml" }
};

// This is a utility function to read a file relative to the testdata directory.
bool StyleResolverTest::ReadDataDirFileToString(const std::string& filename,
                                                std::string* content) const {
  return kmlutil::File::ReadFileToString(std::string(kDataDir + filename),
                                         content);
}

// This is a utility function to parse the given string of KML data into
// the test fixture's KmlFile.
void StyleResolverTest::ParseFromDataDirFile(const std::string& filename) {
  std::string kml_data;
  bool status = ReadDataDirFileToString(filename, &kml_data);
  CPPUNIT_ASSERT(status);
  kml_file_.ParseFromString(kml_data, NULL);
  CPPUNIT_ASSERT(kml_file_.root());
}

// This is a utility function to compare the given element to the KML in the
// given file.  The comparison uses "pretty" XML serialization.
int StyleResolverTest::ComparePretty(const ElementPtr& element,
                                     const char* check_file) const {
  std::string kml_data;
  CPPUNIT_ASSERT(ReadDataDirFileToString(check_file, &kml_data));
  return kml_data.compare(kmldom::SerializePretty(element));
}

// This function verifies all test cases in the kTestCases table.
void StyleResolverTest::TestFiles() {
  for (size_t i = 0; i < sizeof(kTestCases)/sizeof(kTestCases[0]); ++i) {
    // Read the file and find the feature.
    ParseFromDataDirFile(kTestCases[i].source_file_);
    FeaturePtr feature = kmldom::AsFeature(
        kml_file_.GetObjectById(kTestCases[i].feature_id_));
    CPPUNIT_ASSERT(feature);  // This is internal to the test.

    // This is the function under test.
    StylePtr style = CreateResolvedStyle(feature, kml_file_,
                                         kTestCases[i].style_state_);
    CPPUNIT_ASSERT(style);  // This helps debugging.

    // A text comparison is used as that detects issues with unknown elements.
    CPPUNIT_ASSERT(!ComparePretty(style, kTestCases[i].check_file_));
  }
}

}  // end namespace kmlengine

TEST_MAIN
