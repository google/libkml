// Copyright 2009, Google Inc. All rights reserved.
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

// This program shows use of the kmlengine::SplitStyles function which parses
// a KML file splitting inlined style selectors from features to the root
// <Document>.   See kml/engine/style_splitter.h for details.

#include <iostream>
#include "kml/base/file.h"
#include "kml/dom.h"
#include "kml/engine.h"

bool SplitStyles(const char* input_filename, const char* output_filename) {
  std::string kml_input;
  if (!kmlbase::File::ReadFileToString(input_filename, &kml_input)) {
    std::cerr << "read failed: " << input_filename << std::cerr;
    return false;
  }
  std::string errors;
  kmldom::ElementPtr root = kmlengine::SplitStyles(kml_input, &errors);
  if (!root) {
    std::cerr << "parse failed: " << input_filename << std::cerr;
    std::cerr << "parse failed: " << errors << std::cerr;
    return false;
  }

  std::string kml_output = kmldom::SerializePretty(root);
  if (!kmlbase::File::WriteStringToFile(kml_output, output_filename)) {
    std::cerr << "write failed: " << output_filename << std::cerr;
    return false;
  }
  return true;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " input.kml output.kml" << std::endl;
    return 1;
  }
  return SplitStyles(argv[1], argv[2]) ? 0 : 1;
}
