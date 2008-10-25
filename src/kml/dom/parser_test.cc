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

// This file contains the unit tests for the Parse() function and targets
// various internals of the KmlHandler class.

#include "kml/dom/kml_funcs.h"
#include <string>
#include "kml/dom/element.h"
#include "kml/dom/kml.h"
#include "kml/dom/kml_cast.h"
#include "gtest/gtest.h"

namespace kmldom {

class ParserTest : public testing::Test {
};

// Verify proper behavior for good KML.  The XML is valid and all elements
// are known and contained by proper parents.
TEST_F(ParserTest, TestValidKml) {
  std::string errors;
  ElementPtr root = Parse("<kml>"
                          "<Placemark>"
                          "<name>a good Placemark</name>"
                          "<Point>"
                          "<coordinates>1,2,3</coordinates>"
                          "</Point>"
                          "</Placemark>"
                          "</kml>",
                          &errors);
  // KML is valid so there are no errors and the root is <kml>.
  ASSERT_TRUE(errors.empty());
  ASSERT_TRUE(root);

  const KmlPtr kml = AsKml(root);
  ASSERT_TRUE(kml);

  errors.clear();
  // Assigning to root releases storage allocated in Parse above.
  root = Parse(" <kml/>", &errors);  // Note leading space.
  ASSERT_TRUE(errors.empty());
  ASSERT_TRUE(root);

  ASSERT_TRUE(AsKml(root));

  // ElementPtr root going out of scope releases storage allocated in 2nd
  // Parse.
}

TEST_F(ParserTest, TestJunkInput) {
  // Parse a garbage string.
  std::string errors;
  ElementPtr root = Parse("This is not even xml", &errors);
  // Since the parse failed there will be an error string and NULL is returned.
  ASSERT_FALSE(errors.empty());
  ASSERT_FALSE(root);

  // An unescaped ampersand is a basic XML parse error.
  root = Parse("<Document><name>&</name></Document>", &errors);
  ASSERT_FALSE(errors.empty());
  ASSERT_FALSE(root);
}

TEST_F(ParserTest, TestFullyUnknownXml) {
  // Parse perfectly valid, but fully unknown XML.  "Fully unknown" means
  // the root element is not known.  When Parse returns NULL an error string
  // is set.  The error string is considered human readable and not
  // examined further by this test.
  std::string errors;
  ElementPtr root;

  // These test some subtle variations of the inner workings of expat.
  root = Parse("<gml/>", &errors);
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());

  errors.clear();
  root = Parse("<gml></gml>", &errors);
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());

  errors.clear();
  root = Parse("<gml/>\n", &errors);
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());

  errors.clear();
  root = Parse("<gml></gml>\n", &errors);
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());

  errors.clear();
  root = Parse("<gml><this>is not<kml/></this>is also not</gml>", &errors);
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());

  errors.clear();
  root = Parse("<gml>"
               "<Placemark><name>still not kml</name></Placemark>"
               "</gml>",
               &errors);
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());
}

TEST_F(ParserTest, TestPartlyValidKml) {
  // This pushes several elements onto the stack within the parser to
  // excercise the destructor which frees them all.
  std::string errors;
  ElementPtr root = Parse("<kml>"
                          "<Folder>"
                          "<Document>"
                          "<Placemark>"
                          "<name>a good Placemark</name>"
                          "<MultiGeometry>"
                          "<MultiGeometry>"
                          "<Point>"
                          "<coordinates>",
                          &errors);
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
