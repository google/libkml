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

// This program shows basic usage of the buffered parsing method of the
// kmlbase::ExpatParser class. By default it parses 1000000 placemarks. You
// can override this by passing in a different number:
// ./examples/helloworld/parsebuf 100
//
// For reference, on a 2.4 GHz Core2Duo MacBook Pro it takes around 2 seconds
// to parse one million placemark elements:
//
// $ time ./examples/helloworld/parsebuf 1000000
// real   0m2.046s
// user   0m1.825s
// sys    0m0.205s
//
// Five million placemarks takes roughly 10.5 seconds:
// $ time ./examples/helloworld/parsebuf 5000000
// real   0m10.416s
// user   0m9.334s
// sys    0m1.005s

#include <iostream>
#include <string>
#include "kml/base/expat_parser.h"
#include "kml/dom/kml_handler.h"
#include "kml/dom/parser_observer.h"
#include "kml/dom.h"

int main(int argc, char** argv) {

  // The creation of an empty ParserObserver vector is necessary to
  // satisfy the current API.
  kmldom::parser_observer_vector_t observers;
  kmldom::KmlHandler kml_handler(observers);

  kmlbase::ExpatParser parser(&kml_handler, false);

  bool status = false;
  std::string errors;

  // The content need not be split on element boundaries:
  parser.ParseBuffer("<k", NULL, false);
  parser.ParseBuffer("ml><Docume", NULL, false);
  parser.ParseBuffer("nt>", NULL, false);

  // Parse one million placemarks, or the number specified on the cmd line.
  int count = 1000000;
  if (argc == 2) {
    count = atoi(argv[1]);
  }
  while (--count) {
    status = parser.ParseBuffer("<Placemark/>", &errors, false);
    if (!status) {
      std::cerr << "Parse failed: " << errors << std::endl;
      return -1;
    }
  }
  status = parser.ParseBuffer("</Document></kml>", NULL, true);
  // A real client program would check the status here. In this contrived
  // example we know it succeeds.
  std::cout << "done" << std::endl;
  return 0;
}

