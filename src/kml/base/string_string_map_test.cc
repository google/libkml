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

// This file contains the unit tests for the StringStringMap class.

#include "kml/base/string_string_map.h"
#include "boost/scoped_ptr.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include "gtest/gtest.h"

namespace kmlbase {

class StringStringMapTest : public testing::Test {
 protected:
  StringStringMap string_string_map_;
};

TEST_F(StringStringMapTest, TestClone) {
  // Clone an empty map and an empty map is created.
  boost::scoped_ptr<StringStringMap> clone(string_string_map_.Clone());
  ASSERT_TRUE(clone->Empty());
  // Clone a populated map.
  const std::string kA("aa");
  const std::string kB("bbb");
  const std::string kC("cccc");
  const std::string kD("ddddd");
  string_string_map_.Set(kA, kB);
  string_string_map_.Set(kC, kD);
  string_string_map_.Set(kD, kA);
  clone.reset(string_string_map_.Clone());
  ASSERT_EQ(static_cast<size_t>(3), clone->GetSize());
  ASSERT_EQ(kB, clone->GetValue(kA));
  ASSERT_EQ(kD, clone->GetValue(kC));
  ASSERT_EQ(kA, clone->GetValue(kD));
}

// Verify the Empty() method.
TEST_F(StringStringMapTest, TestEmpty) {
  ASSERT_TRUE(string_string_map_.Empty());
  string_string_map_.Set("name", "value");
  ASSERT_FALSE(string_string_map_.Empty());
}

// Verify the Find() method.
TEST_F(StringStringMapTest, TestFind) {
  const std::string kKey("key");
  const std::string kValue("value");
  ASSERT_FALSE(string_string_map_.Find(kKey, NULL));
  string_string_map_.Set(kKey, kValue);
  ASSERT_TRUE(string_string_map_.Find(kKey, NULL));
  std::string got_value;
  ASSERT_TRUE(string_string_map_.Find(kKey, &got_value));
  ASSERT_EQ(kValue, got_value);
}

// Verify the Set() and GetValue() methods.
TEST_F(StringStringMapTest, TestSetGet) {
  const std::string kKey("key");
  const std::string kValue("value");
  ASSERT_EQ("", string_string_map_.GetValue(kKey));
  string_string_map_.Set(kKey, kValue);
  ASSERT_EQ(kValue, string_string_map_.GetValue(kKey));
}

// Verify the GetKey() method.
TEST_F(StringStringMapTest, TestGetKey) {
  const std::string kKey("key");
  const std::string kValue("value");
  ASSERT_EQ("", string_string_map_.GetKey(kValue));
  string_string_map_.Set(kKey, kValue);
  ASSERT_EQ(kKey, string_string_map_.GetKey(kValue));
}

// Verify the GetKeys() method.
TEST_F(StringStringMapTest, TestGetKeys) {
  const std::string kKey0("key0");
  const std::string kKey1("key1");
  std::vector<std::string> keys;
  string_string_map_.GetKeys(&keys);
  ASSERT_TRUE(keys.empty());
  string_string_map_.Set(kKey0, "foo");
  string_string_map_.Set(kKey1, "foo");
  string_string_map_.GetKeys(&keys);
  ASSERT_EQ(static_cast<size_t>(2), keys.size());
  // ASSERT_EQ(k.end(), std::find(...)) nada work.
  ASSERT_FALSE(keys.end() == std::find(keys.begin(), keys.end(), kKey0));
}

// Verify the Merge() method.
TEST_F(StringStringMapTest, TestMerge) {
  StringStringMap source;
  // Merging one empty map to another empty map results in an empty map.
  string_string_map_.Merge(source);
  ASSERT_TRUE(string_string_map_.Empty());

  // Add one entry to the source map and merge that map in an empty target.
  const std::string kKey0("key0");
  const std::string kValue0("value0");
  source.Set(kKey0, kValue0);
  string_string_map_.Merge(source);
  ASSERT_FALSE(string_string_map_.Empty());
  ASSERT_EQ(static_cast<size_t>(1), string_string_map_.GetSize());
  ASSERT_EQ(kValue0, string_string_map_.GetValue(kKey0));

  // Merge again and verify that nothing has changed in the target.
  string_string_map_.Merge(source);
  ASSERT_FALSE(string_string_map_.Empty());
  ASSERT_EQ(static_cast<size_t>(1), string_string_map_.GetSize());
  ASSERT_EQ(kValue0, string_string_map_.GetValue(kKey0));

  // At a 2nd key/value to the source map and merge that in.
  const std::string kKey1("key1");
  const std::string kValue1("value1");
  source.Set(kKey1, kValue1);
  string_string_map_.Merge(source);
  ASSERT_EQ(static_cast<size_t>(2), string_string_map_.GetSize());
  ASSERT_EQ(kValue0, string_string_map_.GetValue(kKey0));
  ASSERT_EQ(kValue1, string_string_map_.GetValue(kKey1));
}

// Verify the MatchSplitKey() method.
TEST_F(StringStringMapTest, TestMatchSplitKey) {
  const std::string kAtom("atom");
  const std::string kAtomNs("http://www.w3.org/2005/Atom");
  const std::string kKmlNs("http://earth.google.com/kml/2.2");
  const std::string kXal("xal");
  const std::string kXalNs("urn:oasis:names:tc:ciq:xsdschema:xAL:2.0");
  const std::string kXmlns("xmlns");
  string_string_map_.Set(kXmlns + ":" + kAtom, kAtomNs);
  string_string_map_.Set(kXmlns + ":" + kXal, kXalNs);
  string_string_map_.Set(kXmlns, kKmlNs);
  StringStringMap split_key_map;
  string_string_map_.MatchSplitKey(kXmlns + ":", &split_key_map);
  ASSERT_EQ(static_cast<size_t>(2), split_key_map.GetSize());
  ASSERT_EQ(kXalNs, split_key_map.GetValue(kXal));
  ASSERT_EQ(kAtomNs, split_key_map.GetValue(kAtom));
  ASSERT_EQ("", split_key_map.GetValue("kml"));
}

// Verify the SerializeToXml() method.
TEST_F(StringStringMapTest, TestSerializeToXml) {
  const std::string kA("aa");
  const std::string kB("bbb");
  const std::string kC("cccc");
  const std::string kD("ddddd");
  string_string_map_.Set(kA, kB);
  string_string_map_.Set(kC, kD);
  string_string_map_.Set(kD, kA);
  std::string xml;
  string_string_map_.SerializeToXml(&xml);
  const std::string want(" " + kA + "=\"" + kB + "\" "
                             + kC + "=\"" + kD + "\" "
                             + kD + "=\"" + kA + "\"");
  ASSERT_EQ(want, xml);
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
