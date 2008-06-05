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

// This program makes some basic use of the KML Engine CreateResolvedStyle()
// function.

#include <iostream>
#include "kml/dom.h"
#include "kml/engine/feature_visitor.h"
#include "kml/engine/kml_file.h"

class ResolvedStylePrinter : public kmlengine::FeatureVisitor {
  public:
   static ResolvedStylePrinter* CreateResolvedStylePrinter(
       const std::string& kml_content) {
     std::string errors;
     if (!kml_file_.ParseFromString(kml_content, &errors)) {
       return NULL;
     }
     return new ResolvedStylePrinter(kml_file_);
   }

   // FeatureVisitor::VisitFeature()
   virtual void VisitFeature(const kmldom::FeaturePtr& feature) {
     kmldom::StylePtr style = kmlengine::CreateResolvedStyle(
         feature, kml_file_, kmldom::STYLESTATE_NORMAL);
   }

 private:
   ResolvedStylePrinter(kmlengine::KmlFile& kml_file)
       : kml_file_(kml_file) {}
   kmlengine::KmlFile& kml_file_;
};

void PrintResolvedStyles(const std::string& kml_filename) {
  ResolveStylePrinter resolved_style_printer(kml_file);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << "kmlfile" << std::endl;
    return -1;
  }
  if (!PrintResolvedStyles(argv[1])) {
    return -1;
  }
  return 0;
}
