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

// This file implements the KML Parse() function.  The parser uses expat.
// The overall process is as follows:
// 1) map instance file element name to id
// 2) create Element for each element and push onto stack at StartElement
// 2a) call NewElement() for each ParserObserver.
// 3) gather character data for simple elements
// 4) in EndElement pop off the child to add to the parent on the stack
// 4a) call AddChild() for each ParserObserver.

#include "kml/dom/kml_handler.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/attributes.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/parser_observer.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;
using kmlbase::StringVector;

// The maximum nesting depth we permit. Depths beyond this are treated as
// errors. Override it with a -DLIBKML_MAX_NESTING_DEPTH preprocessor
// instruction.
// TODO: some flags-like solution would be preferable.
static const unsigned int kMaxNestingDepth = 100;
#ifdef LIBKML_MAX_NESTING_DEPTH
kMaxNestingDepth = LIBKML_MAX_NESTING_DEPTH;
#endif

namespace kmldom {

KmlHandler::KmlHandler(parser_observer_vector_t& observers)
  : kml_factory_(*KmlFactory::GetFactory()),
    skip_depth_(0),
    in_description_(0),
    nesting_depth_(0),
    in_old_schema_placemark_(false),
    observers_(observers) {
}

KmlHandler::~KmlHandler() {
  // stack_'s destructor calls the destructor of each ElementPtr releasing
  // the reference and potentially freeing the associated storage.
}

void KmlHandler::StartElement(const string& name,
                              const StringVector& attrs) {
  // Check that we're not nested beyond the max permissible depth.
  if (++nesting_depth_ > kMaxNestingDepth) {
    XML_StopParser(get_parser(), XML_TRUE);
    return;
  }
  // 3 possibilities:
  // 1) complex element: create an Element.
  // 2) simple element: create a Field
  // 3) unknown element: save XML as a string inside the parent element.
  // No matter what an Element is pushed onto the stack and we always gather
  // character data.

  // See the comment towards the end of this function about permitting "raw"
  // HTML inside <description> elements. This check will catch an instance
  // of a <description> inside a <description> and permit us to handle it
  // correctly as unknown text.
  if (in_description_ > 0 && name.length() == 11 && name == "description") {
    in_description_++;
  }

  if (skip_depth_ > 0) {
    // We're already inside an unknown element. Stringify the next element and
    // its attributes, increment the skip counter again, and return
    // immediately.
    InsertUnknownStartElement(name, attrs);
    skip_depth_++;
    return;
  }

  // If we see <Schema parent=""> then we attempt to parse the old Schema
  // usage outlined in the header. The name of the schema is stored in the
  // old_schema_name_ string.
  // Yes, this means that we'll only do this kind of parse if the Schema
  // defines its children before they appear. But, as mentioned in the header,
  // this is exactly Google Earth's behavior. Likewise, only one <Schema>
  // element is used to define a subclass of Placemark. In the case of
  // multiple Schema elements appearing at the top of the file, the last
  // one wins.
  if (name.length() == 6 && name == "Schema") {
    FindOldSchemaParentName(attrs, &old_schema_name_);
  }

  // Push a string onto the stack we'll use to manage the gathering of
  // character data.
  string element_char_data;
  char_data_.push(element_char_data);

  ElementPtr element;

  KmlDomType type_id =
    static_cast<KmlDomType>(Xsd::GetSchema()->ElementId(name));

  // If we're parsing old Schema usage, we force the creation of a Placemark.
  if (!old_schema_name_.empty() && name == old_schema_name_) {
    // Treat this as a Placemark.
    type_id = Type_Placemark;
  }

  XsdType xsd_type = Xsd::GetSchema()->ElementType(type_id);
  if ((xsd_type == XSD_COMPLEX_TYPE) &&
      (element = kml_factory_.CreateElementById(type_id))) {

    // Icon as a child of IconStyle is really IconStyleIcon
    if (element->Type() == Type_Icon) {
      // If there is a parent and it is IconStyle...
      if (!stack_.empty() && stack_.top()->Type() == Type_IconStyle) {
        // ... delete the Icon and create an IconStyleIcon instead.
        element = kml_factory_.CreateElementById(Type_IconStyleIcon);
      }
    }

    // We parse attributes only if StartElement received any.
    if (!attrs.empty()) {
      // Element::ParseAttributes takes ownership of the created Attributes.
      element->ParseAttributes(Attributes::Create(attrs));
    }
  } else if (xsd_type == XSD_SIMPLE_TYPE) {
    element = kml_factory_.CreateFieldById(type_id);
  } else if (xsd_type == XSD_UNKNOWN && !old_schema_name_.empty()) {
    // We might be parsing one of the children of the old schema usage.
    in_old_schema_placemark_ = ParseOldSchemaChild(name, simplefield_name_vec_,
                                                   &simpledata_vec_);
    if (in_old_schema_placemark_) {
      return;
    }
  }

  if (!element) {
    if (stack_.empty()) {
      // Root element is not known.  XML_TRUE causes XML_Parse() to return
      // XML_STATUS_SUSPENDED.  Returning XML_FALSE _can_ result in
      // XML_Parse() returning XML_STATUS_OK.
      XML_StopParser(get_parser(), XML_TRUE);
      return;
    }
    // The transition point from known to unknown KML. We treat everything
    // from this point as a string until EndElement has decremented the
    // skip_depth_ counter to 0.
    InsertUnknownStartElement(name, attrs);
    skip_depth_++;
    return;
  }
  // This is a known element.  Push onto parse stack and gather content.
  stack_.push(element);

  // We need to permit parsing of un-CDATA'd markup inside <description>
  // elements. We bump the skip counter here as if we'd encountered an unknown
  // element, but only after we've allowed the description ElementPtr to be
  // pushed onto the stack. In EndElement we'll check for the closing of
  // description and decrement the skip counter before anything else happens.
  if (element->Type() == Type_description) {
    skip_depth_++;
    in_description_++;
  }

  // Call the NewElement() method of each ParserObserver.  The whole parse
  // terminates if and when any observer's NewElement() returns false.
  if (!CallNewElementObservers(observers_, element)) {
    XML_StopParser(get_parser(), XML_TRUE);
  }
}

// private
bool KmlHandler::CallNewElementObservers(
    const parser_observer_vector_t& observers, const ElementPtr& element) {
  for (size_t i = 0; i < observers_.size(); ++i) {
    if (!observers_[i]->NewElement(element)) {
      return false;
    }
  }
  return true;
}

void KmlHandler::EndElement(const string& name) {
  --nesting_depth_;
  // See the comment towards the end of StartElement about handling "raw" HTML
  // inside <description> elements. Here we are checking to see if (1) we're
  // inside a closing </description> element and (2) if we're at the end of any
  // possible series of nested description elements.
  if (name.length() == 11 && name == "description" && --in_description_ == 0) {
    skip_depth_--;
  }

  if (skip_depth_ > 0) {
    // We're inside an unknown element. Build the closing tag, decrement
    // the skip counter and then check if we're back to known KML.
    InsertUnknownEndElement(name);
    if (--skip_depth_ == 0) {
      // The next element will be known KML. Push the gathered char_data_ up
      // to Element as a string for serializiation later on.
      char_data_.top().append("\n");
      stack_.top()->AddUnknownElement(char_data_.top());
      char_data_.pop();
    }
    return;
  }

  // This is to cover the special case of "<unknown/>" in expat where
  // endElement is still called even if XML_StopParser() was called in the
  // beginElement handler. See the discussion on this matter in
  // XML_StopParser comment in expat.h.
  if (stack_.empty()) {
    return;
  }

  // If we're parsing an old Schema placemark child, store the character data
  // gathered into the SimpleData element we put on a stack in StartElement.
  if (in_old_schema_placemark_ && simpledata_vec_.size() > 0) {
    // TODO: the pretty serialization of SimpleData will produce some
    // ugly (but harmless) whitespace and unnecessary line breaks. Fix this in
    // the serializer.
    simpledata_vec_.back()->set_text(char_data_.top());
    char_data_.pop();
    in_old_schema_placemark_ = false;
    return;
  }

  // The top of the stack is the begin of the element ending here.
  ElementPtr child = stack_.top();

  string child_char_data_ = char_data_.top();
  char_data_.pop();

  child->set_char_data(child_char_data_);

  if (child->Type() == Type_coordinates ||
      child->Type() == Type_Snippet ||
      child->Type() == Type_linkSnippet ||
      child->Type() == Type_SimpleData) {
    // These are effectively complex elements, but with character data.
    child->AddElement(child);  // "Parse yourself"
  }

  // Check if we're parsing old-style Schema KML. If we are, and if this
  // EndElement is the closing </Schema>, give the schema an id (by appending
  // "_id" to its name) and walk through its <SimpleField> children to
  // discover what element name we should special-case in StartElement.

  // Handle the case of reaching the closing of an old-style </Schema>.
  if (!old_schema_name_.empty()) {
    if (name.length() == 6 && name == "Schema") {
      HandleOldSchemaEndElement(AsSchema(child), old_schema_name_,
                                &simplefield_name_vec_);
    } else if (name == old_schema_name_) {
      // Or that of its Placemark substitute.
      HandleOldSchemaParentEndElement(AsPlacemark(child), old_schema_name_,
                                      kml_factory_, simpledata_vec_);
    }
  }

  // If stack_.size() == 1 this is the root element: leave it alone.
  if (stack_.size() >= 2) {
    // We have a parent.  Pop ourselves off and hand to parent.  Parent element
    // always takes ownsership: 1) a known complex child, 2) a known field,
    // or 3) unknown is passed onwards to its parent and possibly ultimately
    // to the unknown element list in Element.
    stack_.pop();
    if (CallEndElementObservers(observers_, stack_.top(), child)) {
      stack_.top()->AddElement(child);
    }
    if (!CallAddChildObservers(observers_, stack_.top(), child)) {
      XML_StopParser(get_parser(), XML_TRUE);
    }
  }
}

bool KmlHandler::CallEndElementObservers(
    const parser_observer_vector_t& observers, const ElementPtr& parent,
    const ElementPtr& child) {
  for (size_t i = 0; i < observers_.size(); ++i) {
    if (!observers_[i]->EndElement(parent, child)) {
      return false;
    }
  }
  return true;
}

// private
bool KmlHandler::CallAddChildObservers(
    const parser_observer_vector_t& observers, const ElementPtr& parent,
    const ElementPtr& child) {
  for (size_t i = 0; i < observers_.size(); ++i) {
    if (!observers_[i]->AddChild(parent, child)) {
      return false;
    }
  }
  return true;
}

// Note the handling of char data w.r.t. unknown elements. If we are within
// a known element that cannot contain char data, setting it in EndElement is
// a no-op. For known elements within unknown elements, everything is treated
// as a string and _all_ data is saved.
// <Placemark><Point>foo<coordinates/>bar</Point></Placemark> becomes:
// <Placemark><Point><coordinates/></Point></Placemark>
// <X><Point>foo<coordinates/>bar</Point></P> remains as-is.
void KmlHandler::CharData(const string& s) {
  char_data_.top().append(s);
}

// As with STL pop() methods this is (potentially) destructive.  If the
// parse succeeded the root element will be the only item on the stack and
// this method will detach it.  Either way the destructor will delete all
// elements on the stack.  This should only be called after XML_Parse()
// has completed.
ElementPtr KmlHandler::PopRoot() {
  if (!stack_.empty() && stack_.size() == 1) {
    ElementPtr root = stack_.top();
    stack_.pop();
    return root;
  }
  return NULL;
}

// Private.
void KmlHandler::InsertUnknownStartElement(const string& name,
                                           const StringVector& atts) {
  string& top = char_data_.top();
  top.append("<");
  top.append(name);
  for (size_t i = 0; i < atts.size(); i += 2)  {
    top.append(" ");
    top.append(atts.at(i));
    top.append("=\"");
    top.append(atts.at(i+1));
    top.append("\"");
  }
  top.append(">");
}

// Private.
void KmlHandler::InsertUnknownEndElement(const string& name) {
  string& top = char_data_.top();
  top.append("</");
  top.append(name);
  top.append(">");
}

// Static, private.
void KmlHandler::FindOldSchemaParentName(const StringVector& attrs,
                                         string* old_schema_name) {
  boost::scoped_ptr<Attributes> schema_attrs(Attributes::Create(attrs));
  if (schema_attrs.get() && old_schema_name &&
      schema_attrs->FindValue("parent", NULL)) {
    schema_attrs->FindValue("name", old_schema_name);
  }
}

// Static, private.
bool KmlHandler::ParseOldSchemaChild(
    const string& name,
    const StringVector& simplefield_name_vec,
    std::vector<SimpleDataPtr>* simpledata_vec) {
  // We'll iterate through a vector of possible names (created in
  // EndElement) and check to see if we have a match. If we do, we'll make
  // a SimpleData element and put it on a stack for later re-parenting to
  // an ExtendedData element (again in EndElement).
  if (!simpledata_vec) {
    return false;
  }
  StringVector::const_iterator itr = simplefield_name_vec.begin();
  for (; itr != simplefield_name_vec.end(); itr++) {
    if (name == *itr) {
      // Treat this as a SimpleData element.
      SimpleDataPtr simpledata = KmlFactory::GetFactory()->CreateSimpleData();
      simpledata->set_name(name);
      simpledata_vec->push_back(simpledata);
      return true;
    }
  }
  return false;
}

// Static, private.
void KmlHandler::HandleOldSchemaEndElement(
    const SchemaPtr& schema,
    const string& old_schema_name,
    StringVector* simplefield_name_vec) {
  if (!simplefield_name_vec) {
    return;
  }
  schema->set_id(old_schema_name + "_id");
  // TODO: nuke the parent="Placemark" attr.
  for (size_t i = 0; i < schema->get_simplefield_array_size(); i++) {
    if (const SimpleFieldPtr& simplefield =
        AsSimpleField(schema->get_simplefield_array_at(i))) {
      if (simplefield->has_name()) {
        simplefield_name_vec->push_back(simplefield->get_name());
      }
    }
  }
}

// Static, private.
void KmlHandler::HandleOldSchemaParentEndElement(
    const PlacemarkPtr& placemark,
    const string& old_schema_name,
    const KmlFactory& kml_factory,
    const std::vector<SimpleDataPtr> simpledata_vec) {
  // We've reached the closing tag of the old placemark substitute
  // element. Take the SimpleData elements we've been creating from its
  // children and hand them to an ExtendedData, then give that to the
  // parent placemark.
  ExtendedDataPtr extendeddata = kml_factory.CreateExtendedData();
  SchemaDataPtr schemadata = kml_factory.CreateSchemaData();
  schemadata->set_schemaurl(old_schema_name + "_id");
  std::vector<SimpleDataPtr>::const_iterator itr =
    simpledata_vec.begin();
  for (; itr != simpledata_vec.end(); itr++) {
    schemadata->add_simpledata(*itr);
  }
  extendeddata->add_schemadata(schemadata);
  placemark->set_extendeddata(extendeddata);
}

}  // end namespace kmldom

