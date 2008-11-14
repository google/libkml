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

// This file contains the implementation of the Attributes class.

#include "kml/base/attributes.h"
#include <stdlib.h>
#include <map>
#include <sstream>
#include <string>
#include "kml/base/util.h"

namespace kmlbase {

// static
Attributes* Attributes::Create(const char** attrs) {
  Attributes* attributes = new Attributes;
  if (attributes->Parse(attrs)) {
    return attributes;
  }
  delete attributes;
  return NULL;
}

// private
bool Attributes::Parse(const char** attrs) {
  while (*attrs && *(attrs+1)) {  // Quietly ignore unpaired last item.
    const char* attr_name = *attrs++;
    const char* attr_val = *attrs++;
    attributes_map_->Set(attr_name, attr_val);
  }
  return true;
}

// Get the value of the given attribute as a string.  Returns true if an
// attribute with this name exits.  If no attribute by this name exists
// false is returned and the string is untouched.  If no result string
// pointer is supplied false is returned.
bool Attributes::GetString(const std::string attr_name,
                           std::string* attr_val) const {
  return attributes_map_->Find(attr_name, attr_val);
}

// Set the value of the given attribute.  Any previous value for this
// attribute is overwritten.
void Attributes::SetString(const std::string attr_name,
                           const std::string attr_val) {
  attributes_map_->Set(attr_name, attr_val);
}

// Get the value of the given attribute as a double.  If the attribute value
// is not a valid double the output is 0.  This returns true if an attribute
// with this name exists.  If no attribute by this name exists false is
// returned and the result pointer is untouched.  If no result pointer is
// supplied false is returned.
bool Attributes::GetDouble(const std::string attr_name,
                           double* attr_val) const {
  if (!attr_val) {
    return false;
  }
  std::string string_value;
  if (GetString(attr_name, &string_value)) {
    *attr_val = strtod(string_value.c_str(), NULL);
    return true;
  }
  return false;
}

// Set the value of the given attribute from a double.  Any previous value
// for this attribute is overwritten.
void Attributes::SetDouble(const std::string attr_name, double attr_val) {
  std::stringstream ss;
  ss.precision(15);
  ss << attr_val;
  SetString(attr_name, ss.str());
}

bool Attributes::GetInt(const std::string attr_name, int* attr_val) const {
  if (!attr_val) {
    return false;
  }
  std::string string_value;
  if (GetString(attr_name, &string_value)) {
    *attr_val = atoi(string_value.c_str());
    return true;
  }
  return false;
}

void Attributes::SetInt(const std::string attr_name, int attr_val) {
  std::stringstream ss;
  ss << attr_val;
  SetString(attr_name, ss.str());
}

// Get the value of the given boolean attribute.  If attr_val is supplied
// it is set to true if the attribute value is "true", else false.  This
// method returns true if the given attribute exists, else false.
bool Attributes::GetBool(const std::string& attr_name, bool* attr_val) const {
  std::string string_value;
  if (GetString(attr_name, &string_value)) {
    if (attr_val) {
      *attr_val = string_value == "true";
    }
    return true;
  }
  return false;
}

// Serialize the current state of the Attributes instance into the
// passed string.  This appends to any content previously in the string.
// If no string pointer is supplied this method does nothing.
void Attributes::Serialize(std::string* output) const {
  attributes_map_->SerializeToXml(output);
}

// Creates an exact copy of the Attributes object. Called by
// Element::ParseAttributes().
Attributes* Attributes::Clone() const {
  return new Attributes(attributes_map_->Clone());
}

// This sets each attribute from the passed Attributes instance.
// Any conflicting attributes are overridden from the input.
void Attributes::MergeAttributes(const Attributes& attrs) {
  attributes_map_->Merge(*attrs.attributes_map_);
}

void Attributes::MatchSplitKey(const std::string& match,
                               StringStringMap* out) const {
  attributes_map_->MatchSplitKey(match, out);
}

void Attributes::GetAttrNames(std::vector<std::string>* attr_names) const {
  attributes_map_->GetKeys(attr_names);
}

}  // end namespace kmlbase

