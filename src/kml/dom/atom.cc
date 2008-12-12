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

// This file contains the implementation of the <atom:author> and <atom:link>
// elements.

#include "kml/dom/atom.h"
#include "kml/dom/serializer.h"
#include "kml/base/attributes.h"

using kmlbase::Attributes;

namespace kmldom {

AtomAuthor::AtomAuthor()
  : has_name_(false),
    has_uri_(false),
    has_email_(false) {
}

AtomAuthor::~AtomAuthor() {}

void AtomAuthor::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }

  // Explicit child elements.
  switch(element->Type()) {
    case Type_atomEmail:
      has_email_ = element->SetString(&email_);
      break;
    case Type_atomName:
      has_name_ = element->SetString(&name_);
      break;
    case Type_atomUri:
      has_uri_ = element->SetString(&uri_);
      break;
    default:
      Element::AddElement(element);
  }
}

void AtomAuthor::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  // In order of http://schemas.opengis.net/kml/2.2.0/atom-author-link.xsd
  // although no order is specified (this is not an XSD sequence, for example).
  if (has_name()) {
    serializer.SaveFieldById(Type_atomName, get_name());
  } 
  if (has_uri()) {
    serializer.SaveFieldById(Type_atomUri, get_uri());
  }
  if (has_email()) {
    serializer.SaveFieldById(Type_atomEmail, get_email());
  }
} 

AtomLink::AtomLink()
  : has_href_(false),
    has_rel_(false),
    has_type_(false),
    has_hreflang_(false),
    has_title_(false),
    has_length_(false),
    length_(0) {
}

AtomLink::~AtomLink() {}

void AtomLink::AddElement(const ElementPtr& element) {
  // Any element passed in here is by definition unknown, or "undefinedContent"
  // in the atom standard.
  Element::AddElement(element);
}

static const char kHref[] = "href";
static const char kRel[] = "rel";
static const char kType[] = "type";
static const char kHrefLang[] = "hreflang";
static const char kTitle[] = "title";
static const char kLength[] = "length";

void AtomLink::ParseAttributes(Attributes* attributes) {
  if (!attributes) {
    return;
  }
  has_href_ = attributes->CutValue(kHref, &href_);
  has_rel_ = attributes->CutValue(kRel, &rel_);
  has_type_ = attributes->CutValue(kType, &type_);
  has_hreflang_ = attributes->CutValue(kHrefLang, &hreflang_);
  has_title_ = attributes->CutValue(kTitle, &title_);
  has_length_ = attributes->CutValue(kLength, &length_);
  AddUnknownAttributes(attributes);
}

void AtomLink::SerializeAttributes(Attributes* attributes) const {
  Element::SerializeAttributes(attributes);
  if (has_href()) {
    attributes->SetValue(kHref, get_href());
  }
  if (has_rel()) {
    attributes->SetValue(kRel, get_rel());
  }
  if (has_type()) {
    attributes->SetValue(kType, get_type());
  }
  if (has_hreflang()) {
    attributes->SetValue(kHrefLang, get_hreflang());
  }
  if (has_title()) {
    attributes->SetValue(kTitle, get_title());
  }
  if (has_length()) {
    attributes->SetValue(kLength, get_length());
  }
}

void AtomLink::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
}

}  // end namespace kmldom
