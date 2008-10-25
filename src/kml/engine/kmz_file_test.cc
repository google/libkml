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

// This file contains the unit tests for the KmzFile class.

#include "kml/engine/kmz_file.h"
#include <string>
#include <vector>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "gtest/gtest.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#else
static const std::string kDataDir = DATADIR;
#endif

using kmlbase::File;

namespace kmlengine {

class KmzTest : public testing::Test {
 protected:
  boost::scoped_ptr<KmzFile> kmz_file_;
};

TEST_F(KmzTest, TestOpenFromFile) {
  // doc.kmz contains a simple doc.kml and is a valid zip archive.
  const std::string kGoodKmz = kDataDir + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kGoodKmz.c_str()));
  ASSERT_TRUE(kmz_file_);
  std::string kml_data;
  // doc.kml can be read.
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  // nokml.kmz is a valid zip archive, but does not contain any KML files
  const std::string kBadKmz = kDataDir + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kBadKmz.c_str()));
  ASSERT_TRUE(kmz_file_);
  kml_data.clear();
  // There is no KML file to read.
  ASSERT_FALSE(kmz_file_->ReadKml(&kml_data));
  ASSERT_TRUE(kml_data.empty());
}

TEST_F(KmzTest, TestOpenFromBadFile) {
  // Two kinds of bad file.
  // 1: a non-existant file:
  const std::string kNoSuchFile("nosuchfile.kmz");
  kmz_file_.reset(KmzFile::OpenFromFile(kNoSuchFile.c_str()));
  // The file cannot be opened.
  ASSERT_TRUE(kmz_file_ == NULL);
  // 2: a file that is not a valid KMZ archive.
  const std::string kBadKmz= kDataDir + "/kmz/bad.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kBadKmz.c_str()));
  // The file could not be read.
  ASSERT_TRUE(kmz_file_ == NULL);
}

TEST_F(KmzTest, TestOpenFromString) {
  // doc.kmz contains a simple doc.kml and is a valid zip archive.
  const std::string kGoodKmz = kDataDir + "/kmz/doc.kmz";
  std::string kmz_file_data;
  ASSERT_TRUE(File::ReadFileToString(kGoodKmz.c_str(), &kmz_file_data));
  ASSERT_FALSE(kmz_file_data.empty());
  kmz_file_.reset(KmzFile::OpenFromString(kmz_file_data));
  ASSERT_TRUE(kmz_file_);
  std::string kml_data;
  // doc.kml can be read.
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  // nokml.kmz is a valid zip archive, but does not contain any KML files
  const std::string kBadKmz = kDataDir + "/kmz/nokml.kmz";
  kmz_file_data.clear();
  ASSERT_TRUE(File::ReadFileToString(kBadKmz.c_str(), &kmz_file_data));
  ASSERT_FALSE(kmz_file_data.empty());
  kmz_file_.reset(KmzFile::OpenFromString(kmz_file_data));
  ASSERT_TRUE(kmz_file_);
  kml_data.clear();
  // There is no KML file to read.
  ASSERT_FALSE(kmz_file_->ReadKml(&kml_data));
  ASSERT_TRUE(kml_data.empty());
}

TEST_F(KmzTest, TestReadKml) {
  // doc.kmz has two KML files at the root level, a.kml and doc.kml, which were
  // added to the archive in that order. Assert that doc.kml is read instead
  // of a.kml.
  const std::string kDoc = kDataDir + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kDoc.c_str()));
  ASSERT_TRUE(kmz_file_);
  std::string kml_data;
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  ASSERT_TRUE(std::string::npos != kml_data.find("doc.kml"));
  // nokml.kmz is a valid zip archive, but does not contain any KML files
  const std::string kNokml = kDataDir + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNokml.c_str()));
  ASSERT_TRUE(kmz_file_);
  kml_data.clear();
  ASSERT_FALSE(kmz_file_->ReadKml(&kml_data));
  ASSERT_TRUE(kml_data.empty());
  // multikml-nodoc.kmz has three kml files added in the following order:
  // - z/c.kml
  // - b.kml
  // - a/a.kml
  // Each file has a placemark whose <name> is the archived filename.
  // Assert that z/c.kml is read first.
  const std::string kMulti1 = kDataDir + "/kmz/multikml-nodoc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kMulti1.c_str()));
  ASSERT_TRUE(kmz_file_);
  kml_data.clear();
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  ASSERT_TRUE(std::string::npos != kml_data.find("c.kml"));
  // multikml-doc.kmz has four kml files added in the following order:
  // - z/c.kml
  // - b.kml
  // - a/a.kml
  // - doc/doc.kml
  // Assert that z/c.kml is read because the default file (doc.kml at root
  // level) cannot be found.
  const std::string kMulti2 = kDataDir + "/kmz/multikml-doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kMulti2.c_str()));
  ASSERT_TRUE(kmz_file_);
  kml_data.clear();
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  ASSERT_TRUE(std::string::npos != kml_data.find("c.kml"));
  // Assert we handle a NULL output string.
  ASSERT_FALSE(kmz_file_->ReadKml(NULL));
}

// Verify the AndGetPath() part of ReadKmlAndGetPath().  Basic operation
// of ReadKml() is verified in TestReadKml() above.
TEST_F(KmzTest, TestReadKmlAndGetPath) {
  // doc.kmz has two KML files at the root level, a.kml and doc.kml, which were
  // added to the archive in that order. Assert that doc.kml is read instead
  // of a.kml.
  const std::string kDoc = kDataDir + "/kmz/doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kDoc.c_str()));
  std::string kml_data;
  std::string kml_path;
  ASSERT_TRUE(kmz_file_->ReadKmlAndGetPath(&kml_data, &kml_path));
  ASSERT_EQ(std::string("doc.kml"), kml_path);
  // Verify that a NULL output path arg is well behaved.
  ASSERT_TRUE(kmz_file_->ReadKmlAndGetPath(&kml_data, NULL));
  // multikml-doc.kmz has four kml files added in the following order:
  // - z/c.kml
  // - b.kml
  // - a/a.kml
  // - doc/doc.kml
  // Assert that z/c.kml is read because the default file (doc.kml at root
  // level) cannot be found.
  const std::string kMulti2 = kDataDir + "/kmz/multikml-doc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kMulti2.c_str()));
  ASSERT_TRUE(kmz_file_->ReadKmlAndGetPath(&kml_data, &kml_path));
  ASSERT_EQ(std::string("z/c.kml"), kml_path);
  // nokml.kmz is a valid zip archive, but does not contain any KML files
  const std::string kBadKmz = kDataDir + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kBadKmz.c_str()));
  kml_path.clear();
  ASSERT_FALSE(kmz_file_->ReadKmlAndGetPath(&kml_data, &kml_path));
  ASSERT_TRUE(kml_path.empty());
  // Verify that a NULL output path arg is well behaved in this circumstance.
  ASSERT_FALSE(kmz_file_->ReadKmlAndGetPath(&kml_data, NULL));
}

TEST_F(KmzTest, TestReadFile) {
  // nokml.kmz has a file called foo.txt in a folder called foo.
  const std::string kNokml = kDataDir + "/kmz/nokml.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kNokml.c_str()));
  ASSERT_TRUE(kmz_file_);
  std::string file_data;
  ASSERT_TRUE(kmz_file_->ReadFile("foo/foo.txt", &file_data));
  ASSERT_FALSE(file_data.empty());
  std::string tmp = file_data;
  // But does not have a file called bar.txt in that folder
  ASSERT_FALSE(kmz_file_->ReadFile("foo/bar.txt", &file_data));
  // The original data was untouched by this failure.
  ASSERT_FALSE(file_data.empty());
  ASSERT_EQ(tmp, file_data);
  // Assert we handle a NULL output string.
  ASSERT_FALSE(kmz_file_->ReadFile("bar", NULL));
}

TEST_F(KmzTest, TestIsKmz) {
  // Verify that a valid KMZ archive passes IsKmz().
  const std::string kGoodKmz= kDataDir + "/kmz/doc.kmz";
  std::string kmz_data;
  File::ReadFileToString(kGoodKmz, &kmz_data);
  ASSERT_FALSE(kmz_data.empty());
  ASSERT_TRUE(KmzFile::IsKmz(kmz_data));

  // Verify that an invalid KMZ archive fails IsKmz().
  const std::string kBadKmz= kDataDir + "/kmz/bad.kmz";
  kmz_data.clear();
  File::ReadFileToString(kBadKmz, &kmz_data);
  ASSERT_FALSE(kmz_data.empty());
  ASSERT_FALSE(KmzFile::IsKmz(kmz_data));
}

TEST_F(KmzTest, TestList) {
  // multikml-nodoc.kmz has three kml files added in the following order:
  // - z/c.kml
  // - b.kml
  // - a/a.kml
  const std::string kMulti1 = kDataDir + "/kmz/multikml-nodoc.kmz";
  kmz_file_.reset(KmzFile::OpenFromFile(kMulti1.c_str()));
  ASSERT_TRUE(kmz_file_);
  std::vector<std::string> list;
  kmz_file_->List(&list);
  // 3 files were read into the vector.
  ASSERT_TRUE(3 == list.size());
  // They appear in the same order in which they were added.
  ASSERT_EQ(std::string("z/c.kml"), list[0]);
  ASSERT_EQ(std::string("b.kml"), list[1]);
  ASSERT_EQ(std::string("a/a.kml"), list[2]);
}

TEST_F(KmzTest, TestWriteKmz) {
  // Create a temp file into which we'll write our KMZ data.
  kmlbase::TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  ASSERT_TRUE(tempfile != NULL);
  // Create a KMZ file containing a KML file that is a simple placemark called
  // 'tmp kml'.
  const std::string kKml("<Placemark><name>tmp kml</name></Placemark>");
  ASSERT_TRUE(KmzFile::WriteKmz(tempfile->name().c_str(), kKml));
  // Now read the file, ensuring it was properly written.
  ASSERT_TRUE(File::Exists(tempfile->name()));
  std::string foo;
  ASSERT_TRUE(File::ReadFileToString(tempfile->name(), &foo));
  ASSERT_FALSE(foo.empty());
  // Open the file into our KmzFile instance and assert the KML was written
  // correctly.
  kmz_file_.reset(KmzFile::OpenFromFile(tempfile->name().c_str()));
  ASSERT_TRUE(kmz_file_);
  std::string kml_data;
  ASSERT_TRUE(kmz_file_->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  ASSERT_EQ(kKml, kml_data);
}

// This verifies that a const KmzFile compiles and runs with read methods.
TEST_F(KmzTest, TestConstKmzFile) {
  const std::string kDoc = kDataDir + "/kmz/doc.kmz";
  const KmzFile* kmz_file = KmzFile::OpenFromFile(kDoc.c_str());
  std::string kml_data;
  ASSERT_TRUE(kmz_file->ReadKml(&kml_data));
  ASSERT_FALSE(kml_data.empty());
  delete kmz_file;
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
