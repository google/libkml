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

// TODO: actually implement RunExpat and get rid of kml/dom/parser.cc

#ifndef KML_BASE_EXPAT_PARSER_H__
#define KML_BASE_EXPAT_PARSER_H__

#include <map>
#include <string>

namespace kmlbase {

class ExpatHandler;

typedef std::map<std::string, ExpatHandler*> ExpatHandlerMap;

class ExpatHandlerSet {
 public:
  ExpatHandlerSet()
    : default_(NULL) {
  }

  void set_handler(const std::string& xml_namespace,
                  ExpatHandler* expat_handler) {
    if (!default_) {  // TODO: hack
      default_ = expat_handler;
    }
    expat_handler_map_[xml_namespace] = expat_handler;
  }

  // TODO: this is a hack.  only the instance document has a concept of a
  // default namespace (which may have no default namespace at all).
  ExpatHandler* get_default_handler() const {
    return default_;
  }

  // TODO: this is how the parser core really looks up the handler for
  // a given namespace.  This returns NULL if no handler is available for
  // the given namespace.
  ExpatHandler* get_handler(const std::string& xmlns) const {
    ExpatHandlerMap::const_iterator iter = expat_handler_map_.find(xmlns);
    return iter == expat_handler_map_.end() ? NULL : iter->second;
  }

 private:
  ExpatHandler* default_;
  ExpatHandlerMap expat_handler_map_;
};

// Run expat using the supplied handler over the supplied input.  Any parse
// errors are are saved to the supplied string.  This returns true if the
// parse succeeded, false otherwise.
bool RunExpat(const std::string& xml, ExpatHandlerSet* expat_handler_set,
              std::string* errors);

}  // end namespace kmldom

#endif  // KML_BASE_EXPAT_PARSER_H__
