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

// This file contains the unit tests for the KmlFile class.

#include "kml/engine/kml_file.h"
#include <string>
#include "kml/base/net_cache.h"
#include "gtest/gtest.h"
#include "kml/dom.h"
#include "kml/engine/kml_cache.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmldom::ElementPtr;

namespace kmlengine {

class KmlFileTest : public testing::Test {
 protected:
  virtual void SetUp() {
    kml_file_ = KmlFile::Create();
  }

  void VerifyIsPlacemarkWithName(const ElementPtr& root,
                                 const std::string& name);
  void KmlToKmz(const std::string& kml_data, std::string* kmz_data);
  KmlFilePtr kml_file_;
};

// Verify the initial state of a freshly created empty KmlFile.
TEST_F(KmlFileTest, TestInitialState) {
  ASSERT_EQ(kmldom::ElementPtr(), kml_file_->root());
  ASSERT_EQ(std::string(kDefaultEncoding), kml_file_->get_encoding());
}

// Verify the encoding appears properly in the xml header.
TEST_F(KmlFileTest, TestEncoding) {
  ASSERT_EQ(std::string("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"),
            kml_file_->CreateXmlHeader());

  const std::string kIso_8859_1("iso-8859-1");
  kml_file_->set_encoding(kIso_8859_1);
  ASSERT_EQ(kIso_8859_1, kml_file_->get_encoding());

  ASSERT_EQ(std::string("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"),
            kml_file_->CreateXmlHeader());
}

// Verify basic usage of the ParseFromString() method.
TEST_F(KmlFileTest, TestBasicParseFromString) {
  kmldom::ElementPtr root = kml_file_->ParseFromString("<kml/>", NULL);
  ASSERT_EQ(kmldom::Type_kml, root->Type());

  std::string errors;
  root = kml_file_->ParseFromString("bad stuff", &errors);
  ASSERT_FALSE(root);
  ASSERT_FALSE(errors.empty());
}

TEST_F(KmlFileTest, TestRoot) {
  ASSERT_EQ(kmldom::ElementPtr(), kml_file_->root());
  kmldom::ElementPtr root = kml_file_->ParseFromString("<kml/>", NULL);
  ASSERT_EQ(kmldom::Type_kml, kml_file_->root()->Type());

  // Verify that a 2nd ParseFromString() on the same KmlFile resets root.
  root = kml_file_->ParseFromString("<Placemark/>", NULL);
  ASSERT_EQ(kmldom::Type_Placemark, kml_file_->root()->Type());
}

TEST_F(KmlFileTest, TestBasicObjectIdParse) {
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Folder id=\"folder\">"
    "<Placemark id=\"placemark\"/>"
    "</Folder>",
    NULL);
  ASSERT_TRUE(root);
  kmldom::ObjectPtr f = kml_file_->GetObjectById("folder");
  ASSERT_EQ(kmldom::Type_Folder, f->Type());
  kmldom::ObjectPtr p = kml_file_->GetObjectById("placemark");
  ASSERT_EQ(kmldom::Type_Placemark, p->Type());
}

TEST_F(KmlFileTest, TestObjectIdDupePassing) {
  std::string errors;
  const std::string kDupeId("DUPE");
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Folder id=\"" + kDupeId + "\">"
    "<Placemark id=\"" + kDupeId + "\"/>"
    "</Folder>",
    &errors);
  // By default the duplicate ids do not cause the parse to fail.
  ASSERT_TRUE(root);
  // Verify an element of the duplicate id exists.
  ASSERT_TRUE(kml_file_->GetObjectById(kDupeId));
}

TEST_F(KmlFileTest, TestObjectIdDupeFailing) {
  std::string errors;
  const std::string kDupeId("DUPE");
  kml_file_->set_strict_parse(true);
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Folder id=\"" + kDupeId + "\">"
    "<Placemark id=\"" + kDupeId + "\"/>"
    "</Folder>",
    &errors);
  // Verify failure of the parse.
  ASSERT_FALSE(root);
  // Verify no element of the duplicate id exists.
  ASSERT_FALSE(kml_file_->GetObjectById(kDupeId));
}

TEST_F(KmlFileTest, TestObjectIdMapReplaced) {
  const std::string kTestId("the-id");

  // Verify that the given id is not found.
  ASSERT_FALSE(kml_file_->GetObjectById(kTestId));

  // Parse a file with the id and verify that it now is found.
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Placemark id=\"the-id\"/>", NULL);
  kmldom::ObjectPtr p = kml_file_->GetObjectById(kTestId);
  ASSERT_EQ(kTestId, p->get_id());

  // Parse another file without the id and verify that the test id is no
  // longer found.
  root = kml_file_->ParseFromString("<Placemark id=\"some-other-id\"/>", NULL);
  ASSERT_FALSE(kml_file_->GetObjectById(kTestId));
}

// Verify NULL is returned for a non-existent shared style.
TEST_F(KmlFileTest, TestNullGetSharedStyleById) {
  ASSERT_FALSE(kml_file_->GetSharedStyleById("no-such-id"));
}

// Verify a basic shared style is found and a local style is not found.
TEST_F(KmlFileTest, TestBasicGetSharedStyleById) {
  // Use ParseFromString to insert shared styles into the shared style map.
  const std::string kStyleId("share-me");
  const std::string kStyleMapId("me-too");
  const std::string kFolderStyleId("not-me");
  kmldom::ElementPtr root = kml_file_->ParseFromString(
    "<Document>"
      "<Style id=\"" + kStyleId + "\"/>"
      "<StyleMap id=\"" + kStyleMapId + "\"/>"
      "<Folder>"
        "<Style id=\"" + kFolderStyleId + "\"/>"
      "</Folder>"
    "</Document>", NULL);
  ASSERT_TRUE(root);  // Verify the parse succeeded.

  // Verify both shared style selectors were found.
  kmldom::StyleSelectorPtr style = kml_file_->GetSharedStyleById(kStyleId);
  ASSERT_TRUE(AsStyle(style));  // Verify it's a <Style>
  ASSERT_EQ(kStyleId, style->get_id());
  kmldom::StyleSelectorPtr stylemap =
      kml_file_->GetSharedStyleById(kStyleMapId);
  ASSERT_TRUE(AsStyleMap(stylemap));  // Verify it's a <StyleMap>
  ASSERT_EQ(kStyleMapId, stylemap->get_id());

  // Verify that the local style is found as an Object...
  kmldom::ObjectPtr object = kml_file_->GetObjectById(kFolderStyleId);
  ASSERT_TRUE(AsStyle(object));  // Verify it's a <Style>
  ASSERT_EQ(kFolderStyleId, object->get_id());
  // ...but is not found as a shared style.
  ASSERT_FALSE(kml_file_->GetSharedStyleById(kFolderStyleId));
}

// Verify that a 2nd parse of the same KML with a shared style succeeds.  This
// verifies that both the object_id and shared_style_map are properly reset.
TEST_F(KmlFileTest, TestReparse) {
  const std::string kStyleWithId("<Style id=\"id\"/>");
  kmldom::ElementPtr root = kml_file_->ParseFromString(kStyleWithId, NULL);
  ASSERT_TRUE(root);
  ASSERT_TRUE(AsStyle(root));
  // Parse the same thing again.
  root = kml_file_->ParseFromString(kStyleWithId, NULL);
  ASSERT_TRUE(root);
  ASSERT_TRUE(AsStyle(root));
}

// This is an internal helper function to verify that the passed element
// is a Placemark with the given name.
void KmlFileTest::VerifyIsPlacemarkWithName(const ElementPtr& root,
                                            const std::string& name) {
  ASSERT_TRUE(root);
  ASSERT_TRUE(AsPlacemark(root));
  ASSERT_EQ(name, AsPlacemark(root)->get_name());
}

// Verify the CreateFromParse() static method using KML data.
TEST_F(KmlFileTest, TestCreateFromParseOfKml) {
  const std::string kName("my name");
  const std::string kKml("<Placemark><name>" + kName + "</name></Placemark>");
  std::string errors;
  kml_file_ = KmlFile::CreateFromParse(kKml, &errors);
  ASSERT_TRUE(errors.empty());
  VerifyIsPlacemarkWithName(kml_file_->root(), kName);
}

// Verify the CreateFromParse() static method on junk input.
TEST_F(KmlFileTest, TestCreateFromParseOfJunk) {
  const std::string kJunk("this is obviously neither KML nor KMZ");
  std::string errors;
  kml_file_ = KmlFile::CreateFromParse(kJunk, &errors);
}

// This is an internal helper function to create a KMZ memory buffer for the
// input KML memory buffer.
void KmlFileTest::KmlToKmz(const std::string& kml_data,
                           std::string* kmz_data) {
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  const char* tempname = tempfile->name().c_str();
  ASSERT_TRUE(KmzFile::WriteKmz(tempname, kml_data));
  KmzFilePtr kmz_file = KmzFile::OpenFromFile(tempname);
  ASSERT_TRUE(kmz_file);
  ASSERT_TRUE(kmz_file->ReadKml(kmz_data));
}

// Verify the CreateFromParse() static method using KMZ data.
TEST_F(KmlFileTest, TestCreateFromParseOfKmz) {
  const std::string kName("my name");
  const std::string kKml("<Placemark><name>" + kName + "</name></Placemark>");
  std::string kmz_data;
  KmlToKmz(kKml, &kmz_data);
  std::string errors;
  kml_file_ = KmlFile::CreateFromParse(kmz_data, &errors);
  ASSERT_TRUE(errors.empty());
  VerifyIsPlacemarkWithName(kml_file_->root(), kName);
}

// Verify that GetParentLinkParserObservers finds all kinds of parents of
// links in a KML file.
TEST_F(KmlFileTest, TestGetLinkParents) {
  const std::string kAllLinks = std::string(DATADIR) + "/links/alllinks.kml";
  std::string kml;
  ASSERT_TRUE(kmlbase::File::ReadFileToString(kAllLinks, &kml));
  std::string errors;
  kml_file_ = KmlFile::CreateFromParse(kml, &errors);
  ASSERT_TRUE(errors.empty());
  ASSERT_TRUE(kml_file_);
  const ElementVector& link_parents = kml_file_->get_link_parent_vector();
  // This is obviously exactly matched to the content of alllinks.kml.
  ASSERT_EQ(static_cast<size_t>(7), link_parents.size());
  ASSERT_EQ(kmldom::Type_IconStyle, link_parents[0]->Type());
  ASSERT_EQ(kmldom::Type_ItemIcon, link_parents[1]->Type());
  ASSERT_EQ(kmldom::Type_NetworkLink, link_parents[2]->Type());
  ASSERT_EQ(kmldom::Type_GroundOverlay, link_parents[3]->Type());
  ASSERT_EQ(kmldom::Type_ScreenOverlay, link_parents[4]->Type());
  ASSERT_EQ(kmldom::Type_PhotoOverlay, link_parents[5]->Type());
  ASSERT_EQ(kmldom::Type_Model, link_parents[6]->Type());
#if 0
  // TODO: handle styleUrl(?) and SchemaData
  ASSERT_EQ(std::string("style.kml#style"), href_vector[6]->Type());
  ASSERT_EQ(std::string("#myschema"), href_vector[7]);
#endif
}

// Verify const behavior.
TEST_F(KmlFileTest, TestConstNull) {
  const KmlFilePtr kml_file = KmlFile::Create();
  ASSERT_FALSE(kml_file->GetObjectById("blah"));
  ASSERT_FALSE(kml_file->GetSharedStyleById("blah"));
}

// Verify basic usage of the CreateFromString() static method.
TEST_F(KmlFileTest, TestBasicCreateFromString) {
  const std::string kName("my name");
  const std::string kPlacemark("<Placemark><name>" + kName +
                               "</name></Placemark>");
  kml_file_ = KmlFile::CreateFromString(kPlacemark);
  ASSERT_TRUE(kml_file_);
}

// Verify basic usage of the CreateFromStringWithUrl() static method.
TEST_F(KmlFileTest, TestBasicCreateFromStringWithUrl) {
  const std::string kName("my name");
  const std::string kPlacemark("<Placemark><name>" + kName +
                               "</name></Placemark>");
  const std::string kUrl("http://foo.com/goo/baz.kml");
  // There's no requirement a NetCache need exist.
  kml_file_ = KmlFile::CreateFromStringWithUrl(kPlacemark, kUrl, NULL);
  ASSERT_TRUE(kml_file_);
  ASSERT_EQ(kUrl, kml_file_->get_url());
  kmldom::PlacemarkPtr placemark = AsPlacemark(kml_file_->root());
  ASSERT_TRUE(placemark);
  ASSERT_EQ(kName, placemark->get_name());
  ASSERT_FALSE(kml_file_->get_kml_cache());
}

// Verify CreateFromStringWithUrl() with a KmlCache.
TEST_F(KmlFileTest, TestCreateWithKmlCache) {
  kmlbase::NetFetcher null_net_fetcher;
  KmlCache kml_cache(&null_net_fetcher, 1);
  const std::string kName("my name");
  const std::string kPlacemark("<Placemark><name>" + kName +
                               "</name></Placemark>");
  const std::string kUrl("http://foo.com/goo/baz.kml");
  // Typically the KmlCache is the one that created the KmlFile.
  kml_file_ = KmlFile::CreateFromStringWithUrl(kPlacemark, kUrl, &kml_cache);
  ASSERT_EQ(kUrl, kml_file_->get_url());
  ASSERT_EQ(&kml_cache, kml_file_->get_kml_cache());
}

// Verify KmlFile's default xmlns is set on the root element.
TEST_F(KmlFileTest, TestXmlnsOnRoot) {
  std::string xml;
  const std::string kExpectedKml(
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
      "<kml xmlns=\"http://www.opengis.net/kml/2.2\"/>\n");
  kml_file_ = KmlFile::CreateFromString("<kml/>");
  ASSERT_TRUE(kml_file_->SerializeToString(&xml));
  ASSERT_EQ(kExpectedKml, xml);

  const std::string kExpectedPlacemark(
      "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
      "<Placemark xmlns=\"http://www.opengis.net/kml/2.2\"/>\n");
  kml_file_ = KmlFile::CreateFromString("<Placemark/>");
  xml.clear();
  ASSERT_TRUE(kml_file_->SerializeToString(&xml));
  ASSERT_EQ(kExpectedPlacemark, xml);
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
