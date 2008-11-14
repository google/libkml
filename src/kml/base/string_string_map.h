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

#ifndef KML_BASE_STRING_STRING_MAP_H__
#define KML_BASE_STRING_STRING_MAP_H__

#include <map>
#include <string>
#include <vector>

namespace kmlbase {

// This class encapsulates the implementation of the string-to-string map
// and provides some common code for other classes using this map.  Users of
// this class should not presume any particular implementation of the map.
// (Future implementations of this class may use a hashmap of some sort).
class StringStringMap {
  typedef std::map<std::string, std::string> StrStrMap;
 public:
  StringStringMap* Clone() const {
    StringStringMap* string_string_map = new StringStringMap();
    for (StrStrMap::const_iterator itr = str_str_map_.begin();
         itr != str_str_map_.end(); ++itr) {
      string_string_map->Set(itr->first, itr->second);
    }
    return string_string_map;
  }

  bool Empty() const {
    return str_str_map_.empty();
  }

  // If there is a mapping for this key return true, else false.  If there is
  // a mapping and value is non-NULL assign it the value.
  bool Find(const std::string& key, std::string* value) const {
    StrStrMap::const_iterator entry = str_str_map_.find(key);
    if (entry != str_str_map_.end()) {
      if (value) {
        *value = entry->second;
      }
      return true;
    }
    return false;
  }

  // Get the value for the given key.  An empty string is returned if there
  // is no mapping.
  const std::string GetValue(const std::string& key) const {
    std::string value;
    Find(key, &value);
    return value;  // Empty if Find() has no such key.
  }

  const std::string GetKey(const std::string& value) const {
    StrStrMap::const_iterator iter = str_str_map_.begin();
    for (; iter != str_str_map_.end(); ++iter) {
      if (value == iter->second) {
        return iter->first;
      }
    }
    return "";  // Empty string.
  }

  void GetKeys(std::vector<std::string>* string_vector) const {
    StrStrMap::const_iterator iter = str_str_map_.begin();
    for (; iter != str_str_map_.end(); ++iter) {
      string_vector->push_back(iter->first);
    }
  }

  void Merge(const StringStringMap& string_string_map) {
    StrStrMap::const_iterator itr =
        string_string_map.str_str_map_.begin();
    for (; itr != string_string_map.str_str_map_.end(); ++itr) {
      str_str_map_[itr->first] = itr->second;
    }
  }

  void MatchSplitKey(const std::string& match, StringStringMap* out) const {
    if (out) {
      size_t match_size = match.size();
      for (StrStrMap::const_iterator itr = str_str_map_.begin();
           itr != str_str_map_.end(); ++itr) {
        const std::string& lhs = itr->first;
        if (lhs.size() > match_size && lhs.compare(0, match_size, match) == 0) {
          out->Set(lhs.substr(match_size), itr->second);
        }
      }
    }
  }

  // Serialize the key/value pairs to XML attributes.
  void SerializeToXml(std::string* output) const {
    if (output) {
      StrStrMap::const_iterator entry;
      for (entry = str_str_map_.begin(); entry != str_str_map_.end(); ++entry) {
        *output += " ";
        *output += entry->first;
        *output += "=\"";
        *output += entry->second;
        *output += "\"";
      }
    }
  }

  void Set(const std::string& key, const std::string& value) {
    str_str_map_[key] = value;
  }

  size_t GetSize() const {
    return str_str_map_.size();
  }

 private:
  StrStrMap str_str_map_;
};

}  // end namespace kmlbase

#endif // KML_BASE_STRING_STRING_MAP_H__
