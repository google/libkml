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

// This file contains the declaration of the internal ElementReplicator class.

#ifndef KML_DOM_ELEMENT_REPLICATOR_H__
#define KML_DOM_ELEMENT_REPLICATOR_H__

#include <stack>
#include <string>
#include "kml/dom.h"
#include "kml/dom/serializer.h"

namespace kmldom {

class Attributes;
class Element;
class Xsd;

// This returns a "deep" clone of the given element.  All child elements and
// fields are copied.
ElementPtr Clone(const ElementPtr& element);

// The ElementReplicator is used by the Clone() function to walk the entire
// "XML" hierarchy of the target element.
class ElementReplicator : public Serializer {
 public:
  virtual ~ElementReplicator() {}

  // Serializer::BeginById() is called at the start of a complex element.
  virtual void BeginById(int type_id, const Attributes& attributes);

  // Serializer::End() is called at the end of a complex element.
  virtual void End();

  // Serializer::SaveStringFieldById() is called for each field.
  virtual void SaveStringFieldById(int type_id, std::string value);

  // Handles the case of a complex element with character data. Used by
  // <Snippet> and <SimpleData>.
  virtual void SaveComplexStringFieldByName(std::string tag_name,
                                            const Attributes& attributes,
                                            std::string value);

  // Serializer::SaveContent() is called for arbitrary character data.
  virtual void SaveContent(std::string content);


  // Return the top of the stack which holds the root element.
  ElementPtr root() {
    if (clone_stack_.empty()) {
      return NULL;
    }
    return clone_stack_.top();
  }

 private:
  // This stack operates akin to the stack in the parser.
  std::stack<ElementPtr> clone_stack_;
};

}  // end namespace kmldom

#endif  // KML_DOM_ELEMENT_REPLICATOR_H__
