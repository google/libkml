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

// This file contains the declaration and definition of the internal
// Attributes class.

#ifndef KML_BASE_ATTRIBUTES_H__
#define KML_BASE_ATTRIBUTES_H__

#include <stdlib.h>
#include <map>
#include <sstream>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/base/string_string_map.h"
#include "kml/base/util.h"

namespace kmlbase {

class Attributes {
 public:
  // Construct the Attributes instance from a list of name-value pairs
  // as is used in expat's startElement.
  static Attributes* Create(const char** attrs);

  // Construct the Attributes instance with no initial name-value pairs.
  Attributes() {
    attributes_map_.reset(new StringStringMap);
  }

  // Get the value of the given attribute as a string.  Returns true if an
  // attribute with this name exits.  If no attribute by this name exists
  // false is returned and the string is untouched.  If no result string
  // pointer is supplied false is returned.
  bool GetString(const std::string attr_name, std::string* attr_val) const;

  // Set the value of the given attribute.  Any previous value for this
  // attribute is overwritten.
  void SetString(const std::string attr_name, const std::string attr_val);

  // Get the value of the given attribute as a double.  If the attribute value
  // is not a valid double the output is 0.  This returns true if an attribute
  // with this name exists.  If no attribute by this name exists false is
  // returned and the result pointer is untouched.  If no result pointer is
  // supplied false is returned.
  bool GetDouble(const std::string attr_name, double* attr_val) const;

  // Set the value of the given attribute from a double.  Any previous value
  // for this attribute is overwritten.  If no attribute by this name exists
  // false is returned and the result pointer is untouched.  If no result
  // pointer is supplied false is returned.
  void SetDouble(const std::string attr_name, double attr_val);

  // Get the value of the given attribute as an int.  If the attribute value
  bool GetInt(const std::string attr_name, int* attr_val) const;

  // Set the value of the given attribute from an int.  Any previous value
  // for this attribute is overwritten.
  void SetInt(const std::string attr_name, int attr_val);

  // Get the value of the given boolean attribute.  If attr_val is supplied
  // it is set to true if the attribute value is "true", else false.  This
  // method returns true if the given attribute exists, else false.
  bool GetBool(const std::string& attr_name, bool* attr_val) const;

  // Serialize the current state of the Attributes instance into the
  // passed string.  This appends to any content previously in the string.
  // If no string pointer is supplied this method does nothing.
  void Serialize(std::string* output) const;

  // Creates an exact copy of the Attributes object.
  Attributes* Clone() const;

  // This sets each attribute from the passed Attributes instance.
  // Any conflicting attributes are overridden from the input.
  void MergeAttributes(const Attributes& attrs);

  // For cracking xmlns:prefix="namespace" pairs.  If key_match matches the
  // first part of a key then the rest of the key is used to store the mapped
  // value into the out map.  For example, if key_match="xmlns:" then any key
  // matching "xmlns:foo" is matched and "foo" is used as the key in the out
  // map.
  void MatchSplitKey(const std::string& key_match, StringStringMap* out) const;

  // Returns all attribute names.
  void GetAttrNames(std::vector<std::string>* attr_names) const;

 private:
  Attributes(StringStringMap* string_string_map) {
    attributes_map_.reset(string_string_map);
  }
  bool Parse(const char** attrs);

  // XML attributes have no order and are unique.  The attribute name is
  // preserved to properly save unknown attributes.
  boost::scoped_ptr<StringStringMap> attributes_map_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Attributes);
};

}  // end namespace kmlbase

#endif  // KML_BASE_ATTRIBUTES_H__
