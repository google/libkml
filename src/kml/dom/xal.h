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

// This file contains the declaration of the <xal:AddressDetails> elements.
// Note, only a subset of XAL using these elements is implemented here.
// However, note that the normal unknown/misplaced element handling of libkml
// is employed thus all of XAL is preserved on parse and emitted on
// serialization.  The portion implemented here pertains to programmatic
// dom access.
// xAL complex elements
// <xal:AddressDetails>
// <xal:AdministrativeArea>
// <xal:Country>
// <xal:Locality>
// <xal:PostalCode>
// <xal:SubAdministrativeArea>
// <xal:Thoroughfare>
// xAL simple elements
// <xal:AdministrativeAreaName>
// <xal:CountryNameCode>
// <xal:LocalityName>
// <xal:PostalCodeNumber>
// <xal:SubAdministrativeAreaName>
// <xal:ThoroughfareName>

#ifndef KML_DOM_XAL_H__
#define KML_DOM_XAL_H__

#include "kml/dom/element.h"

namespace kmldom {

// <xal:AddressDetails>
class XalAddressDetails : public Element {
 public:
  virtual ~XalAddressDetails();
  virtual KmlDomType Type() const { return Type_XalAddressDetails; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_XalAddressDetails;
  }

  // <xal:Country>
  const XalCountryPtr get_country() const {
    return country_;
  }
  bool has_country() const { return country_ != NULL; }
  void set_country(const XalCountryPtr& country) {
    SetComplexChild(country, &country_);
  }
  void clear_country() {
    set_country(NULL);
  }

 private:
  XalCountryPtr country_;
  friend class KmlFactory;
  XalAddressDetails();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(XalAddressDetails);
};

// <xal:Country>
class XalCountry : public Element {
 public:
  virtual ~XalCountry();
  virtual KmlDomType Type() const { return Type_XalCountry; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_XalCountry;
  }

  // <xal:CountryNameCode>, ISO 3166-1
  const std::string& get_countrynamecode() const { return countrynamecode_; }
  bool has_countrynamecode() const { return has_countrynamecode_; }
  void set_countrynamecode(const std::string& value) {
    countrynamecode_ = value;
    has_countrynamecode_ = true;
  }
  void clear_countrynamecode() {
    countrynamecode_.clear();
    has_countrynamecode_ = false;
  }

  // <xal:AdministrativeArea>
  const XalAdministrativeAreaPtr get_administrativearea() const {
    return administrativearea_;
  }
  bool has_administrativearea() const {
    return administrativearea_ != NULL;
  }
  void set_administrativearea(
      const XalAdministrativeAreaPtr& administrativearea) {
    SetComplexChild(administrativearea, &administrativearea_);
  }
  void clear_administrativearea() {
    set_administrativearea(NULL);
  }

 private:
  bool has_countrynamecode_;
  std::string countrynamecode_;
  XalAdministrativeAreaPtr administrativearea_;
  friend class KmlFactory;
  XalCountry();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(XalCountry);
};

// <xal:AdministrativeArea>
class XalAdministrativeArea : public Element {
 public:
  virtual ~XalAdministrativeArea();
  virtual KmlDomType Type() const { return Type_XalAdministrativeArea; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_XalAdministrativeArea;
  }

  // <xal:SubAdministrativeArea>
  const XalSubAdministrativeAreaPtr get_subadministrativearea() const {
    return subadministrativearea_;
  }
  bool has_subadministrativearea() const {
    return subadministrativearea_ != NULL;
  }
  void set_subadministrativearea(
      const XalSubAdministrativeAreaPtr& subadministrativearea) {
    SetComplexChild(subadministrativearea, &subadministrativearea_);
  }
  void clear_subadministrativearea() {
    set_subadministrativearea(NULL);
  }

 private:
  XalSubAdministrativeAreaPtr subadministrativearea_;
  friend class KmlFactory;
  XalAdministrativeArea();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
};

// <xal:SubAdministrativeArea>
class XalSubAdministrativeArea : public Element {
 public:
  virtual ~XalSubAdministrativeArea();
  virtual KmlDomType Type() const { return Type_XalSubAdministrativeArea; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_XalSubAdministrativeArea;
  }

  // TODO: implement children

 private:
  friend class KmlFactory;
  XalSubAdministrativeArea();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
};

// <xal:Locality>
class XalLocality : public Element {
 public:
  virtual ~XalLocality();
  virtual KmlDomType Type() const { return Type_XalLocality; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_XalLocality;
  }

  // TODO: implement children
  // TODO: <xal:LocalityName>
  // TODO: <xal:Thoroughfare>
  // TODO: <xal:PostalCode>
};

// <xal:Thoroughfare>
class XalThoroughfare : public Element {
 public:
  virtual ~XalThoroughfare();
  virtual KmlDomType Type() const { return Type_XalThoroughfare; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_XalThoroughfare;
  }

  // TODO: <xal:ThoroughfareName>
};

// <xal:PostalCode>
class XalPostalCode : public Element {
 public:
  virtual ~XalPostalCode();
  virtual KmlDomType Type() const { return Type_XalPostalCode; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_XalPostalCode;
  }

  // TODO: <xal:PostalCodeNumber>
};

}  // end namespace kmldom

#endif  // KML_DOM_XAL_H__
