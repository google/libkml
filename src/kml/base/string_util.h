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

// This file contains the declarations of various string utility functions.

#ifndef KML_BASE_STRING_UTIL_H__
#define KML_BASE_STRING_UTIL_H__

#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace kmlbase {

typedef std::map<std::string, std::string> StringMap;
typedef std::map<std::string, std::string>::value_type StringPair;

// This permits a class containing a StringMap to export a way to iterate the
// internal container without exposing it directly.  In the future the STL
// map presently used internally may be replaced with a hash map, for example.
class StringMapIterator {
 public:
  StringMapIterator(const StringMap& map)
    : map_(map), iter_(map.begin()) {
  }

  StringPair Data() const {
    return *iter_;
  }

  bool AtEnd() const {
    return iter_ == map_.end();
  }

  void Advance() {
    ++iter_;
  }

 private:
  const StringMap& map_;
  StringMap::const_iterator iter_;
};

// Walks through the input string, replacing all keys in StringMap
// with the corresponding key value. The key strings are wrapped with the
// start and end strings before searching the input string. Returns a new
// string with the replaced entities. For example:
// input: "this is $[my] $[cool] string"
// string_map: { "my" : "your", "cool", "cooler" }
// start: "$["
// end: "]"
// replaced string: "this is your cooler string"
std::string CreateExpandedStrings(const std::string& in,
                                  const StringMap& string_map,
                                  const std::string& start,
                                  const std::string& end);

// This converts from std::string to any T of int, bool or double.
template<typename T>
void FromString(const std::string& in, T* out);

// This converts to std::string from any T of int, bool or double.
template<typename T>
inline std::string ToString(T value) {
  std::stringstream ss;
  ss.precision(15);
  ss << value;
  return ss.str();
}

// Split the input string on the split_string saving each string into the
// output vector.
void SplitStringUsing(const std::string& input, const std::string& split_string,
                      std::vector<std::string>* output);

}  // end namespace kmlbase

#endif  // KML_BASE_STRING_UTIL_H__
