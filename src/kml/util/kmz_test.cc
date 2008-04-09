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

#include "kml/util/kmz.h"
#include <string>
#include "kml/util/fileio.h"
#include "kml/util/unit_test.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#else
static const std::string kDataDir = DATADIR;
#endif

class KmzTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmzTest);
  CPPUNIT_TEST(TestDataIsKmz);
  CPPUNIT_TEST(TestReadFileFromKmz);
  CPPUNIT_TEST(TestReadKmlFromKmz);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestDataIsKmz();
  void TestReadFileFromKmz();
  void TestReadKmlFromKmz();
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmzTest);

void KmzTest::TestDataIsKmz() {
  std::string kmz_data;
  // doc.kmz contains a simple doc.kml and is a valid zip archive.
  const std::string doc = kDataDir + "/kmz/doc.kmz";
  bool b = ReadFileToString(doc.c_str(), &kmz_data);
  CPPUNIT_ASSERT(b);
  CPPUNIT_ASSERT(!kmz_data.empty());
  CPPUNIT_ASSERT(DataIsKmz(kmz_data));
  
  // bad.kmz is not a valid zip archive.
  const std::string bad = kDataDir + "/kmz/bad.kmz";
  b = ReadFileToString(bad.c_str(), &kmz_data);
  CPPUNIT_ASSERT(b);
  CPPUNIT_ASSERT(!kmz_data.empty());
  CPPUNIT_ASSERT(!DataIsKmz(kmz_data));
}

void KmzTest::TestReadFileFromKmz() {
  // nokml.kmz has a file called foo.txt in a folder called foo.
  const std::string nokml = kDataDir + "/kmz/nokml.kmz";
  std::string file_data;
  CPPUNIT_ASSERT(ReadFileFromKmz(nokml.c_str(),
                                 "foo/foo.txt", &file_data));
  CPPUNIT_ASSERT(!file_data.empty());
  // but does not have a file called bar.txt in that folder
  CPPUNIT_ASSERT(!ReadFileFromKmz(nokml.c_str(),
                                  "foo/bar.txt", &file_data));
  CPPUNIT_ASSERT(file_data.empty());
  // Assert we handle a NULL output string.
  CPPUNIT_ASSERT(!ReadFileFromKmz(nokml.c_str(),
                                  "bar", NULL));
}

void KmzTest::TestReadKmlFromKmz() {
  // doc.kmz has two KML files at the root level, a.kml and doc.kml, which were
  // added to the archive in that order. Assert that doc.kml is read instead
  // of foo.kml.
  const std::string doc = kDataDir + "/kmz/doc.kmz";
  std::string kml_data;
  CPPUNIT_ASSERT(ReadKmlFromKmz(doc.c_str(), &kml_data));
  CPPUNIT_ASSERT(false == kml_data.empty());
  CPPUNIT_ASSERT(std::string::npos != kml_data.find("doc.kml"));
  // nokml.kmz is a valid zip archive, but does not contain any KML files
  const std::string nokml = kDataDir + "/kmz/nokml.kmz";
  kml_data.clear();
  CPPUNIT_ASSERT(false == ReadKmlFromKmz(nokml.c_str(), &kml_data));
  CPPUNIT_ASSERT(kml_data.empty());
  // multikml-nodoc.kmz has three kml files added in the following order:
  // - z/c.kml
  // - b.kml
  // - a/a.kml
  // Each file has a placemark whose <name> is the archived filename.
  // Assert that z/c.kml is read first.
  const std::string multi1 = kDataDir + "/kmz/multikml-nodoc.kmz";
  kml_data.clear();
  CPPUNIT_ASSERT(ReadKmlFromKmz(multi1.c_str(), &kml_data));
  CPPUNIT_ASSERT(false == kml_data.empty());
  CPPUNIT_ASSERT(std::string::npos != kml_data.find("c.kml"));
  // multikml-doc.kmz has four kml files added in the following order:
  // - z/c.kml
  // - b.kml
  // - a/a.kml
  // - doc/doc.kml
  // Assert that z/c.kml is read because the default file (doc.kml at root
  // level) cannot be found.
  const std::string multi2 = kDataDir + "/kmz/multikml-doc.kmz";
  kml_data.clear();
  CPPUNIT_ASSERT(ReadKmlFromKmz(multi2.c_str(), &kml_data));
  CPPUNIT_ASSERT(false == kml_data.empty());
  CPPUNIT_ASSERT(std::string::npos != kml_data.find("c.kml"));
  // Assert we handle a NULL output string.
  CPPUNIT_ASSERT(false == ReadKmlFromKmz(multi2.c_str(), NULL));
}

TEST_MAIN

