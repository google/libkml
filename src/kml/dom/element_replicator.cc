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

// This file contains the implementation of the Element Clone function and
// the ElementReplicator class.

#include "kml/dom/element_replicator.h"
#include <stack>
#include <string>
#include "kml/dom/element.h"
#include "kml/dom/kml_factory.h"

namespace kmldom {

class Attributes;

// Clone operates by "Serializing" the given element.  The ElementReplicator
// operates akin to the parser in that it maintains a stack of complex
// elements created from the factory and sets fields and child elements
// to the parent as they are completed.
ElementPtr Clone(const ElementPtr& element) {
  if (!element) {
    return NULL;
  }
  ElementReplicator serializer;
  element->Serialize(serializer);
  return serializer.root();
}

// Create a new Element of the given type and set its attributes from the
// source element.  Push it onto the stack to pull off as each element
// End() is called.
void ElementReplicator::BeginById(int type_id, const Attributes& attributes) {
  KmlDomType id = static_cast<KmlDomType>(type_id);
  ElementPtr clone = KmlFactory::GetFactory()->CreateElementById(id);
  clone->ParseAttributes(attributes);
  clone_stack_.push(clone);
}

// At the end of an element (</Placmark>, for example) we pop it off the stack
// and add it to the parent which is now the top item on the stack.
void ElementReplicator::End() {
  // Two or more items on the stack implies the top is a child to be added
  // to the parent above it on the stack.
  if (clone_stack_.size() > 1) {
    // Pop off the child.
    ElementPtr child = clone_stack_.top();
    clone_stack_.pop();
    // Parent is now the top item.
    clone_stack_.top()->AddElement(child);
  }
}

// Every field appears here.  As with elements create a field of this type,
// set its value and hand it to the current element put on the stack in
// BeginById().
void ElementReplicator::SaveStringFieldById(int type_id, std::string value) {
  KmlDomType id = static_cast<KmlDomType>(type_id);
  ElementPtr clone = KmlFactory::GetFactory()->CreateFieldById(id);
  clone->set_char_data(value);
  clone_stack_.top()->AddElement(clone);
}

// This is a special case for Snippet/linkSnippet which has both
// character data and attributes.
void ElementReplicator::SaveComplexStringFieldByName(std::string tagName,
                                              const Attributes& attributes,
                                              std::string value) {
  // TODO...
}

// This is used to emit raw character data content.
void ElementReplicator::SaveContent(std::string content) {
  // TODO... used only by coordinates... unify with above somehow...
}

}  // end namespace kmldom
