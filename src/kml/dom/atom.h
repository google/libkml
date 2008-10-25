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

// This file contains the declaration of the <atom:author> and <atom:link>
// elements.  See: http://atompub.org/rfc4287.html.

#ifndef KML_DOM_ATOM_H__
#define KML_DOM_ATOM_H__

#include "kml/dom/element.h"

namespace kmldom {

// <atom:author>, RFC 4287 4.2.1, and 3.2 (atomPersonConstruct)
class AtomAuthor : public Element {
 public:
  virtual ~AtomAuthor();
  virtual KmlDomType Type() const { return Type_AtomAuthor; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_AtomAuthor;
  }

  // <atom:name>
  const std::string& get_name() const { return name_; }
  bool has_name() const { return has_name_; }
  void set_name(const std::string& value) {
    name_ = value;
    has_name_ = true;
  }
  void clear_name() {
    name_.clear();
    has_name_ = false;
  }

  // <atom:uri>, RFC 3987
  const std::string& get_uri() const { return uri_; }
  bool has_uri() const { return has_uri_; }
  void set_uri(const std::string& value) {
    uri_ = value;
    has_uri_ = true;
  }
  void clear_uri() {
    uri_.clear();
    has_uri_ = false;
  }

  // <atom:email>, RFC 2822
  const std::string& get_email() const { return email_; }
  bool has_email() const { return has_email_; }
  void set_email(const std::string& value) {
    email_ = value;
    has_email_ = true;
  }
  void clear_email() {
    email_.clear();
    has_email_ = false;
  }

 private:
  bool has_name_;
  std::string name_;
  bool has_uri_;
  std::string uri_;
  bool has_email_;
  std::string email_;
  friend class KmlFactory;
  AtomAuthor();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(AtomAuthor);
};

// <atom:link>, RFC 4287 4.2.7
class AtomLink : public Element {
 public:
  virtual ~AtomLink();
  virtual KmlDomType Type() const { return Type_AtomLink; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_AtomLink;
  }

  // href=, RFC 4287 4.2.7.1, RFC 3987
  const std::string& get_href() const { return href_; }
  bool has_href() const { return has_href_; }
  void set_href(const std::string& value) {
    href_ = value;
    has_href_ = true;
  }
  void clear_href() {
    href_.clear();
    has_href_ = false;
  }

  // rel=, RFC 4287 4.2.7.2, RFC 3987
  const std::string& get_rel() const { return rel_; }
  bool has_rel() const { return has_rel_; }
  void set_rel(const std::string& value) {
    rel_ = value;
    has_rel_ = true;
  }
  void clear_rel() {
    rel_.clear();
    has_rel_ = false;
  }

  // type=, RFC 4287 4.2.7.3, MIME
  const std::string& get_type() const { return type_; }
  bool has_type() const { return has_type_; }
  void set_type(const std::string& value) {
    type_ = value;
    has_type_ = true;
  }
  void clear_type() {
    type_.clear();
    has_type_ = false;
  }

  // hreflang=, RFC 4287 4.2.7.4, RFC 3066
  const std::string& get_hreflang() const { return hreflang_; }
  bool has_hreflang() const { return has_hreflang_; }
  void set_hreflang(const std::string& value) {
    hreflang_ = value;
    has_hreflang_ = true;
  }
  void clear_hreflang() {
    hreflang_.clear();
    has_hreflang_ = false;
  }

  // title=, RFC 4287 4.2.7.5
  const std::string& get_title() const { return title_; }
  bool has_title() const { return has_title_; }
  void set_title(const std::string& value) {
    title_ = value;
    has_title_ = true;
  }
  void clear_title() {
    title_.clear();
    has_title_ = false;
  }

  // length=, RFC 4287 4.2.7.6
  const int get_length() const { return length_; }
  bool has_length() const { return has_length_; }
  void set_length(const int value) {
    length_ = value;
    has_length_ = true;
  }
  void clear_length() {
    length_ = 0;
    has_length_ = false;
  }

 private:
  bool has_href_;
  std::string href_;
  bool has_rel_;
  std::string rel_;
  bool has_type_;
  std::string type_;
  bool has_hreflang_;
  std::string hreflang_;
  bool has_title_;
  std::string title_;
  bool has_length_;
  int length_;
  friend class KmlFactory;
  AtomLink();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  void ParseAttributes(const kmlbase::Attributes& attributes);
  void GetAttributes(kmlbase::Attributes* attributes) const;
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(AtomLink);
};

}  // end namespace kmldom

#endif  // KML_DOM_ATOM_H__
