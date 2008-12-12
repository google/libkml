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

// This file contains the implementation of the MapIds function.

#include "kml/engine/id_mapper.h"
#include "kml/dom/serializer.h"
#include "kml/engine/engine_types.h"

using kmldom::ElementPtr;
using kmldom::ObjectPtr;
using kmldom::Serializer;

namespace kmlengine {

// SaveElement is the only virtual method required given that IdMapper is only
// concerned with Objects which are Elements (no fields).
void IdMapper::SaveElement(const ElementPtr& element) {
  if (ObjectPtr object = AsObject(element)) {
    if (object->has_id()) {
      const std::string& id = object->get_id();
      ObjectIdMap::const_iterator iter = object_id_map_->find(id);
      if (iter != object_id_map_->end()) {
        // Save this as a dupe if a vector was supplied.
        if (dup_id_vector_) {
          dup_id_vector_->push_back(iter->second);
        }
      }
      // This matches the semantics of ObjectIdParserObserver.
      (*object_id_map_)[id] = object;  // "Last one wins"
    }
  }
  // Call Serializer to recurse.
  Serializer::SaveElement(element);
}

// Append all elements of the given type id in the hierarchy rooted at element.
void MapIds(const ElementPtr& root, ObjectIdMap* object_id_map,
            ElementVector* dup_id_vector) {
  if (root && object_id_map) {  // NULL dup_id_vector ok.
    // The IdMapper derivation of Serializer does all the work.
    IdMapper id_mapper(object_id_map, dup_id_vector);
    id_mapper.SaveElement(root);
  }
}

}  // end namespace kmlengine
