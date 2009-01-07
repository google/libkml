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

// This program shows basic use of streamed parsing.

#include <fstream>
#include <iostream>
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/engine.h"

using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::PlacemarkPtr;
using kmlengine::KmlStream;
using std::cerr;
using std::endl;

static int HelloKmlStreamCreateFromIstream(const char* filename) {
  std::ifstream input_file(filename, std::ios_base::in|std::ios_base::binary);
  if (!input_file.is_open() || !input_file.good()) {
    cerr << "open failed: " << filename << endl;
    return 1;
  }

  std::string errors;
  boost::scoped_ptr<KmlStream> kml_stream(
      KmlStream::ParseFromIstream(&input_file, &errors, NULL));
  if (!kml_stream.get()) {
    cerr << "KmlStream::CreateFromIstream failed: " << filename << endl;
    if (!errors.empty()) {
      cerr << errors << endl;
    }
    return 1;
  }
  return 0;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "usage: " << argv[0] << "big.kml" << endl;
    return 1;
  }
  return HelloKmlStreamCreateFromIstream(argv[1]);
}
