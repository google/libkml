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

// This sample program uses a ParserObserver which counts element usage
// in the given KML or KMZ file.

#include <iostream>
#include <map>
#include <string>
#include "kml/dom.h"
// TODO: move these into kml/dom.h
#include "kml/dom/parser_observer.h"
#include "kml/dom/parser.h"
#include "kml/dom/xsd.h"  // TODO: consider the Xsd class public?
#include "kml/util/fileio.h"
#include "kml/util/kmz.h"

// This map is used to hold the occurrence count for each element.
typedef std::map<kmldom::KmlDomType, int> element_count_map_t;

// This ParserObserver uses the NewElement() method to count the number of
// ocurrences of each type of element.
class ElementCounter : public kmldom::ParserObserver {
 public:
  // ParserObserver::NewElement()
  virtual bool NewElement(const kmldom::ElementPtr& element) {
    if (element_count_map_.find(element->Type()) == element_count_map_.end()) {
      element_count_map_[element->Type()] = 1;
    } else {
      element_count_map_[element->Type()] += 1;
    }
    return true;  // Always return true to keep parsing.
  }

  // This method prints a summary of the internal element counting map.
  void PrintElementCounts() const {
    int total_element_count = 0;
    element_count_map_t::const_iterator map_iter;
    for (map_iter = element_count_map_.begin();
         map_iter != element_count_map_.end();
         ++map_iter) {
      std::cout << kmldom::Xsd::GetSchema()->ElementName((*map_iter).first)
        << " " << (*map_iter).second << std::endl;
      total_element_count += (*map_iter).second;
    }
    std::cout << "Element types " << element_count_map_.size() << std::endl;
    std::cout << "Total elements " << total_element_count << std::endl;
  }

 private:
  element_count_map_t element_count_map_;
};

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " kmlfile" << std::endl;
    return 1;
  }

  // Read the file.
  std::string file_data;
  if (!ReadFileToString(argv[1], &file_data)) {
    std::cout << argv[1] << " read failed" << std::endl;
    return 1;
  }

  // If the file was KMZ, extract the KML file.
  std::string kml;
  if (DataIsKmz(file_data)) {
    if (!ReadKmlFromKmz(argv[1], &kml)) {
      std::cout << "Failed reading KMZ file" << std::endl;
      return 1;
    }
  } else {
    kml = file_data;
  }

  // Parse it with the ElementCounter installed as a ParseObserver.
  kmldom::Parser parser;
  ElementCounter element_counter;
  parser.AddObserver(&element_counter);
  std::string errors;
  kmldom::ElementPtr root = parser.Parse(kml, &errors);
  if (!root) {
    std::cout << errors << std::endl;
    return 1;
  }

  // Ask the ElementCounter to print out what it observed.
  element_counter.PrintElementCounts();
  return 0;
}
