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

#include "kml/base/string_util.h"
#include "gtest/gtest.h"

namespace kmlbase {

TEST(StringUtilTest, Testb2a_hex) {
  const uint32_t k00 = 0x00;
  const uint32_t kff = 0xff;

  const char* e = "00";
  static char a[2];

  b2a_hex(k00, a);
  ASSERT_EQ(0, strcmp(e, a));

  e = "ff";
  b2a_hex(kff, a);
  ASSERT_EQ(0, strcmp(e, a));
}

TEST(StringUtilTest, TestBasicReplacements) {
  StringMap sm;
  sm["old"] = "new";
  sm["one"] = "two";
  string in("old{old}old one{one}one");

  // Verify that supplying neither start nor end terminators produces a simple
  // key-value replacement.
  string expected("new{new}new two{two}two");
  ASSERT_EQ(expected, CreateExpandedStrings(in, sm, "", ""));

  // Verify that specifying either of start or end produces the expected
  // replacement.
  const string kStart("{");
  const string kEnd("}");
  expected = "oldnew}old onetwo}one";
  ASSERT_EQ(expected, CreateExpandedStrings(in, sm, kStart, ""));
  expected = "old{newold one{twoone";
  ASSERT_EQ(expected, CreateExpandedStrings(in, sm, "", kEnd));
  expected = "oldnewold onetwoone";
  ASSERT_EQ(expected, CreateExpandedStrings(in, sm, kStart, kEnd));

  // Verify that the replacement logic is well-behaved in the presence of
  // multiple start/end delimiters.
  in = "{{{old}}}}{}{one}";
  expected = "{{new}}}{}two";
  ASSERT_EQ(expected, CreateExpandedStrings(in, sm, kStart, kEnd));
}

TEST(StringUtilTest, TestSplitStringUsing) {
  const string kHi("hi");
  const string kHow("how");
  const string kAre("are");
  const string kYou("you");
  const string kSep("|");
  const string kCsvLine(kHi + kSep + kHow + kSep + kAre + kSep + kYou);
  std::vector<string> parts;

  // A line with no separator gets one thing.  Special case of the "last" item
  // on a line.
  SplitStringUsing(kHi, "|", &parts);
  ASSERT_EQ(static_cast<size_t>(1), parts.size());

  // A line with 3 separators gets you 4 things.
  parts.clear();
  SplitStringUsing(kCsvLine, "|", &parts);
  ASSERT_EQ(static_cast<size_t>(4), parts.size());
  ASSERT_EQ(kHi, parts[0]);
  ASSERT_EQ(kHow, parts[1]);
  ASSERT_EQ(kAre, parts[2]);
  ASSERT_EQ(kYou, parts[3]);
}

class ThingMappingStrings {
 public:
  void AddMapping(const string& key, const string& value) {
    map_[key] = value;
  }

  StringMapIterator CreateIterator () {
    return StringMapIterator(map_);
  }

 private:
  StringMap map_;
};

// Verity the StringMapIterator class.
TEST(StringUtiltest, TestStringMapIterator) {
  std::map<string, string> map;
  ThingMappingStrings thing;
  const string k0("a");
  const string v0("b");
  const string k1("c");
  const string v1("d");
  const string k2("z");
  const string v2("q");
  thing.AddMapping(k0, v0);
  map[k0] = v0;
  thing.AddMapping(k1, v1);
  map[k1] = v1;
  thing.AddMapping(k2, v2);
  map[k2] = v2;

  StringMapIterator iter = thing.CreateIterator();
  for (; !iter.AtEnd(); iter.Advance()) {
    ASSERT_EQ(map[iter.Data().first], iter.Data().second);
  }
}

// Verify the FromString function template.
TEST(StringUtilTest, TestFromString) {
  double pi;
  FromString("3.1415926535", &pi);
  ASSERT_EQ(3.1415926535, pi);

  bool val;
  FromString("true", &val);
  ASSERT_EQ(true, val);
  FromString("1", &val);
  ASSERT_EQ(true, val);
  FromString("false", &val);
  ASSERT_EQ(false, val);
  FromString("0", &val);
  ASSERT_EQ(false, val);

  int fsc;
  FromString("137", &fsc);
  ASSERT_EQ(137, fsc);
}

// Verify the ToString function template.
TEST(StringUtilTest, TestToString) {
  ASSERT_EQ(string("3.1415926535"), ToString(3.1415926535));
  ASSERT_EQ(string("1"), ToString(1));
  ASSERT_EQ(string("-42"), ToString(-42));
  ASSERT_EQ(string("1"), ToString(true));
  ASSERT_EQ(string("0"), ToString(false));
}

TEST(StringUtilTest, TestEndsWith) {
  ASSERT_TRUE(StringEndsWith("foo", "oo"));
  ASSERT_FALSE(StringEndsWith("foo", "x"));
  ASSERT_FALSE(StringEndsWith("foo", "ooooo"));
  string empty;
  ASSERT_FALSE(StringEndsWith(empty, empty));
  ASSERT_FALSE(StringEndsWith("foo", empty));
  ASSERT_FALSE(StringEndsWith(empty, "foo"));
  ASSERT_TRUE(StringEndsWith("foo", "foo"));
}

TEST(StringUtilTest, TestStringCaseEqual) {
  ASSERT_TRUE(StringCaseEqual("foo", "Foo"));
  ASSERT_TRUE(StringCaseEqual("foo", "fOO"));
  ASSERT_TRUE(StringCaseEqual("FOO", "Foo"));
  ASSERT_TRUE(StringCaseEqual("foo", "foo"));
  ASSERT_TRUE(StringCaseEqual("FOO", "FOO"));
  ASSERT_TRUE(StringCaseEqual("THIS SOFTWARE IS", "This software is"));
  ASSERT_FALSE(StringCaseEqual("this", "is not this"));
  ASSERT_FALSE(StringCaseEqual("this ", "this"));
  ASSERT_FALSE(StringCaseEqual("this ", " this"));
}

TEST(StringUtilTest, TestStringToDouble) {
  double val;

  ASSERT_TRUE(StringToDouble("3.1415", &val));
  ASSERT_TRUE(StringToDouble("3.1415", NULL));
  ASSERT_DOUBLE_EQ(3.1415, val);

  ASSERT_TRUE(StringToDouble("52.566571", &val));
  ASSERT_TRUE(StringToDouble("52.566571", NULL));
  ASSERT_DOUBLE_EQ(52.566571, val);

  ASSERT_TRUE(StringToDouble("-1.680006", &val));
  ASSERT_TRUE(StringToDouble("-1.680006", NULL));
  ASSERT_DOUBLE_EQ(-1.680006, val);

  ASSERT_TRUE(StringToDouble(".1", &val));
  ASSERT_TRUE(StringToDouble(".1", NULL));
  ASSERT_DOUBLE_EQ(.1, val);
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
