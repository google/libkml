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

// This file contains the implementation of the KmlFile class methods.

#include "kml/engine/kml_file.h"
#include "kml/engine/id_mapper.h"
#include "kml/dom.h"

namespace kmlengine {

// static
KmlFile* KmlFile::CreateFromParse(const std::string& kml_or_kmz_data,
                                  std::string* errors) {
  // Here our focus is on managing the KmlFile storage.  If _CreateFromParse()
  // fails we release the storage else we return a pointer to it.
  KmlFile* kml_file = new KmlFile;
  if (kml_file->_CreateFromParse(kml_or_kmz_data, errors)) {
    return kml_file;
  }
  delete kml_file;
  return NULL;
}

// static
KmlFile* KmlFile::CreateFromStringWithUrl(const std::string& kml_data,
                                          const std::string& url,
                                          KmlCache* kml_cache) {
  if (KmlFile* kml_file = CreateFromString(kml_data)) {
    kml_file->set_url(url);
    kml_file->set_kml_cache(kml_cache);
    return kml_file;
  }
  return NULL;
}

// private
// This is an internal helper function used in CreateFromParse().
bool KmlFile::_CreateFromParse(const std::string& kml_or_kmz_data,
                               std::string* errors) {
  // Here our focus is on deciding KML vs KMZ.
  if (kmlengine::KmzFile::IsKmz(kml_or_kmz_data)) {
    return OpenAndParseKmz(kml_or_kmz_data, errors);
  }
  return ParseFromString(kml_or_kmz_data, errors);
}

// private
// The caller is expected to have called KmzFile::IsKmz on this, thus the return
// status represents file handling errors.
bool KmlFile::OpenAndParseKmz(const std::string& kmz_data,
                              std::string* errors) {
  std::string kml_data;
  kmz_file_ = kmlengine::KmzFile::OpenFromString(kmz_data);
  if (!kmz_file_ || !kmz_file_->ReadKml(&kml_data)) {
      return false;
  }
  return ParseFromString(kml_data, errors);
}

// private
// TODO: push strict parsing out as a Create() method arg
KmlFile::KmlFile()
  : encoding_(kDefaultEncoding),
    default_xmlns_(kDefaultXmlns),
    kml_cache_(NULL),
    strict_parse_(false) {
}

// private
bool KmlFile::ParseFromString(const std::string& kml, std::string* errors) {
  // Create a parser object.
  kmldom::Parser parser;

  // Create a ParserObserver both to save the id's of all Objects as well as
  // check for duplicates if strict parsing has been enabled. If set, this
  // ParserObserver fails the parse immediately on the first duplicate id.
  ObjectIdParserObserver object_id_parser_observer(&object_id_map_,
                                                   strict_parse_);
  parser.AddObserver(&object_id_parser_observer);

  // Create a ParserObserver to map and save the id's of all shared
  // StyleSelectors.
  SharedStyleParserObserver shared_style_parser_observer(&shared_style_map_,
                                                         strict_parse_);
  parser.AddObserver(&shared_style_parser_observer);

  // Create a ParserObserver to save the parent of all <Link> and <Icon>
  // elements found in the KML file.  See get_link_parents.h for more info.
  GetLinkParentsParserObserver get_link_parents(&link_parent_vector_);
  parser.AddObserver(&get_link_parents);

  // Actually perform the parse.
  if (kmldom::ElementPtr root = parser.Parse(kml, errors)) {
    // TODO: set encoding, xmlns, etc from parse
    set_root(root);
    return true;
  }
  return false;
}

// static
// TODO: add a bool strict arg and decide on dup handling strategy in MapIds
// in the case of lax import.  At present the import is strict and fails
// completely if there are any dups.
KmlFile* KmlFile::CreateFromImport(kmldom::ElementPtr element) {
  if (!element) {
    return NULL;
  }
  KmlFile* kml_file = new KmlFile;
  ElementVector dup_id_elements;
  MapIds(element, &kml_file->object_id_map_, &dup_id_elements);
  if (dup_id_elements.empty()) {
    // TODO: look for shared styles in object_id_map_ and add to
    // shared_style_map_
    // TODO check/set all elements under elements to be in this file.
    kml_file->set_root(element);
    return kml_file;
  }
  delete kml_file;
  return NULL;
}

const std::string KmlFile::CreateXmlHeader() const {
  return std::string("<?xml version=\"1.0\" encoding=\"" + encoding_ + "\"?>\n");
}

bool KmlFile::SerializeToString(std::string* xml_output) const {
  if (!xml_output || !get_root()) {
    return false;
  }
  xml_output->append(CreateXmlHeader());
  get_root()->set_default_xmlns(default_xmlns_);
  xml_output->append(kmldom::SerializePretty(get_root()));
  return true;
}

kmldom::ObjectPtr KmlFile::GetObjectById(std::string id) const {
  ObjectIdMap::const_iterator find = object_id_map_.find(id);
  return find != object_id_map_.end() ? kmldom::AsObject(find->second) : NULL;
}

kmldom::StyleSelectorPtr KmlFile::GetSharedStyleById(std::string id) const {
  SharedStyleMap::const_iterator find = shared_style_map_.find(id);
  return find != shared_style_map_.end() ? find->second : NULL;
}


}  // end namespace kmlengine
