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

// This file contains the declaration of the GetElementsByType() and the
// internal ElementFinder class.

#ifndef KML_DOM_ELEMENT_FINDER_H__
#define KML_DOM_ELEMENT_FINDER_H__

#include <stack>
#include <string>
#include "kml/dom.h"
#include "kml/dom/serializer.h"

namespace kmldom {

class Attributes;

typedef std::vector<ElementPtr> element_vector_t;

// Starting at the hierarchy rooted at element this finds all complex
// elements of the given type and appends them to the given array.
void GetElementsById(const ElementPtr& element, KmlDomType type_id,
                     element_vector_t* element_vector);

// The ElementFinder adds every complex element of the given type to the
// given vector.
class ElementFinder : public Serializer {
 public:
  ElementFinder(KmlDomType type_id, element_vector_t& element_vector);

  // See serializer.h for details about these methods.  These are all
  // nop's in ElementFinder.
  virtual void BeginById(int type_id, const Attributes& attributes) {}
  virtual void End() {}
  virtual void SaveStringFieldById(int type_id, std::string value) {}
  virtual void SaveComplexStringFieldByName(std::string tag_name,
                                            const Attributes& attributes,
                                            std::string value) {}
  virtual void SaveContent(std::string content) {}

  // This is the only Serializer virtual method implemented
  // in ElementFinder.  As such only complex elements are found.
  virtual void SaveElement(const ElementPtr& element);

 private:
  KmlDomType type_id_;
  element_vector_t& element_vector_;
};

}  // end namespace kmldom

#endif  // KML_DOM_ELEMENT_FINDER_H__
