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

// This file contains the implementation of the GetElementsById() function and
// the ElementFinder class.

#include "kml/dom/element_finder.h"

namespace kmldom {

// Append all elements of the given type id in the hierarchy
// root at element.
void GetElementsById(const ElementPtr& element, KmlDomType type_id,
                     element_vector_t* element_vector) {
  if (!element || !element_vector) {
    return;
  }
  // The ElementFinder derivation of Serializer does all the work.
  ElementFinder element_finder(type_id, *element_vector);
  element->Serialize(element_finder);
}

// The constructor for ElementFinder takes a reference to
// the vector to which to apppend the elements of the give it.
ElementFinder::ElementFinder(KmlDomType type_id,
                             element_vector_t& element_vector)
    : type_id_(type_id), element_vector_(element_vector) {
}

// This is called for every complex element.
void ElementFinder::SaveElement(const ElementPtr& element) {
  // If this element is of the desired type save a pointer.
  if (type_id_ == element->Type()) {
    element_vector_.push_back(element);
  }
  // Call Serializer to recurse.
  Serializer::SaveElement(element);
}

}  // end namespace kmldom
