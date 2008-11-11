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

// This file contains the implementation of the <xal:AddressDetails> elements.
// TODO: handle <xs:choice> better.

#include "kml/dom/xal.h"
#include "kml/base/attributes.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;

namespace kmldom {

XalAddressDetails::XalAddressDetails() {}

XalAddressDetails::~XalAddressDetails() {}

void XalAddressDetails::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }

  switch(element->Type()) {
    case Type_XalCountry:
      set_country(AsXalCountry(element));
      break;
    default:
      Element::AddElement(element);
  }
}

void XalAddressDetails::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  // <xal:AddressDetails> is a <xs:sequence>
  // TODO: <xal:PostalServiceElements>
  // After <xal:PostalServiceElements> is a <xs:choice>:
  // TODO: <xal:Address>
  // TODO: <xal:AddressLines>
  // <xal:Country>
  if (has_country()) {
    serializer.SaveElement(get_country());
  } 
  // TODO: <xal:AdministrativeArea>
  // TODO: <xal:Locality>
  // TODO: <xal:Thoroughfare>
  // TODO <xs:any>
  SerializeUnknown(serializer);
  serializer.End();
} 

XalCountry::XalCountry()
  : has_countrynamecode_(false) {
}

XalCountry::~XalCountry() {}

void XalCountry::AddElement(const ElementPtr& element) {
  switch(element->Type()) {
    case Type_xalCountryNameCode:
      has_countrynamecode_ = element->SetString(&countrynamecode_);
      break;
    case Type_XalAdministrativeArea:
      set_administrativearea(AsXalAdministrativeArea(element));
      break;
    default:
      Element::AddElement(element);
  }
}

void XalCountry::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  // xal:Country is a <xs:sequence> of these children in this order:
  // TODO: <xal:AddressLine>
  // <xal:CountryNameCode>
  if (has_countrynamecode()) {
    serializer.SaveFieldById(Type_xalCountryNameCode, get_countrynamecode());
  } 
  // TODO: <xal:CountryName>
  // After <xal:CountryName> is an <xs:choice>
  // <xal:AdministrativeArea>
  if (has_administrativearea()) {
    serializer.SaveElement(get_administrativearea());
  }
  // TODO: <xal:Locality>
  // TODO: <xal:Thoroughfare>
  //SerializeUnknown(serializer);
  serializer.End();
}

XalAdministrativeArea::XalAdministrativeArea() {}

XalAdministrativeArea::~XalAdministrativeArea() {}

void XalAdministrativeArea::AddElement(const ElementPtr& element) {
  // TODO: implement parsing
  Element::AddElement(element);
}

void XalAdministrativeArea::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  SerializeUnknown(serializer);
  serializer.End();
}

XalSubAdministrativeArea::XalSubAdministrativeArea() {}

XalSubAdministrativeArea::~XalSubAdministrativeArea() {}

void XalSubAdministrativeArea::AddElement(const ElementPtr& element) {
  // TODO: implement parsing
  Element::AddElement(element);
}

void XalSubAdministrativeArea::Serialize(Serializer& serializer) const {
  Attributes attributes;
  GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
