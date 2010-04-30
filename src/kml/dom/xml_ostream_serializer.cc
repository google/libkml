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

// This file contains the implementation of the public XML serializing
// functions and the implementation of the XmlOstreamSerialize class.  This is
// exactly compatible with XmlSerialize which this someday will replace.
// TODO: this is a work in progress, expect changes

#include "kml/dom/xml_ostream_serializer.h"
#include "kml/dom/kml_funcs.h"
#include <cstring>
#include <stack>
#include <sstream>
#include "kml/base/attributes.h"
#include "kml/base/string_util.h"
#include "kml/dom/element.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;

namespace kmldom {

void SerializeToOstream(const ElementPtr& root, bool pretty,
                        std::ostream* oss) {
  if (!root || !oss) {
    return;
  }
  boost::scoped_ptr<XmlOstreamSerializer> xml_serializer;
  if (pretty) {
    xml_serializer.reset(new XmlOstreamSerializer("\n", "  ", oss));
  } else {
    xml_serializer.reset(new XmlOstreamSerializer("", "", oss));
  }
  root->Serialize(*xml_serializer);
}

// static
void XmlOstreamSerializer::SerializePrettyToOstream(const ElementPtr& root,
                                                    std::ostream* output) {
  if (!root || !output) {
    return;
  }
  boost::scoped_ptr<XmlOstreamSerializer> xml_ostream_serializer(
      new XmlOstreamSerializer("\n", "  ", output));
  root->Serialize(*xml_ostream_serializer);
}

// private
XmlOstreamSerializer::XmlOstreamSerializer(const char* newline,
                                           const char* indent,
                                           std::ostream* output)
  : newline_(newline),
    indent_(indent),
    output_(output),
    start_pending_(false) {
}

void XmlOstreamSerializer::BeginById(int type_id,
                              const kmlbase::Attributes& attributes) {
  // Here we just record the element we're starting and its attributes if
  // it has any.  The "<TAGNAME [name="VAL" ...]..." are not emmited until
  // it is known if this is a nil element or not.
  EmitStart(false);
  Indent();
  tag_stack_.push(type_id);  // So we know what tag to use in End().
  if (attributes.GetSize() > 0) {
    // TODO: Attributes::SerializeToOstream would be handy.
    attributes.Serialize(&serialized_attributes_);
  }
  start_pending_ = true;
}

bool XmlOstreamSerializer::EmitStart(bool nil) {
  if (!start_pending_) {
    return false;
  }
  output_->put('<');
  const string& tag_name = xsd_.ElementName(tag_stack_.top());
  output_->write(tag_name.data(), tag_name.size());
  if (!serialized_attributes_.empty()) {
    output_->write(serialized_attributes_.data(),
                   serialized_attributes_.size());
    serialized_attributes_.clear();
  }
  if (nil) {
    output_->write("/>", 2);
  } else {
    output_->put('>');
  }
  Newline();
  start_pending_ = false;
  return true;
}

void XmlOstreamSerializer::End() {
  int type_id = tag_stack_.top();
  // TODO: make this less fiddly
  if (EmitStart(true)) {
    tag_stack_.pop();
  } else {
    tag_stack_.pop();
    Indent();
    output_->write("</", 2);
    const string& tag_name = xsd_.ElementName(type_id);
    output_->write(tag_name.data(), tag_name.size());
    output_->put('>');
    Newline();
  }
}

void XmlOstreamSerializer::WriteQuoted(const string& value) {
  string quoted = MaybeQuoteString(value);
  output_->write(quoted.data(), quoted.size());
}

void XmlOstreamSerializer::SaveStringFieldById(int type_id, string value) {
  EmitStart(false);
  Indent();
  const string& tag_name = xsd_.ElementName(type_id);
  output_->put('<');
  output_->write(tag_name.data(), tag_name.size());
  if (value.empty()) {  // Special case to emit <TAGNAME/>
    output_->put('/');
  } else {  // <TAGNAME>VALUE</TAGNAME>
    output_->put('>');
    WriteQuoted(value);
    output_->write("</", 2);
    output_->write(tag_name.data(), tag_name.size());
  }
  output_->put('>');
  Newline();
}

void XmlOstreamSerializer::SaveContent(const string& content,
                                       bool maybe_quote) {
  EmitStart(false);
  if (maybe_quote) {
    WriteQuoted(content);
  } else {
    output_->write(content.data(), content.size());
  }
}

void XmlOstreamSerializer::SaveVec3(const kmlbase::Vec3& vec3) {
  EmitStart(false);
  Indent();
  string val = kmlbase::ToString(vec3.get_longitude());
  output_->write(val.data(), val.size());
  output_->put(',');
  val = kmlbase::ToString(vec3.get_latitude());
  output_->write(val.data(), val.size());
  // Ideally, we'd only emit if vec3.has_altitude(), but lots of test cases
  // expect lon,lat,0
  output_->put(',');
  val = kmlbase::ToString(vec3.get_altitude());
  output_->write(val.data(), val.size());
  Newline();
}

void XmlOstreamSerializer::SaveColor(int type_id,
                                     const kmlbase::Color32& color) {
  EmitStart(false);
  SaveFieldById(type_id, color.to_string_abgr());
}

// This callback can be used to know that we're starting a single-value
// <coordinates> such that we can avoid indent and line break:
//
// void XmlOstreamSerializer::BeginElementArray(int group_id,
//                                             size_t element_count) {
//  if (group_id == Type_coordinates) {
//    coordinates_size_ = element_count;
//  }
// }

void XmlOstreamSerializer::Newline() {
  if (!newline_.empty()) {
    output_->write(newline_.data(), newline_.size());
  }
}

void XmlOstreamSerializer::Indent() {
  if (!indent_.empty()) {
    size_t depth = tag_stack_.size();
    while (depth--) {
      output_->write(indent_.data(), indent_.size());
    }
  }
}

}  // namespace kmldom
