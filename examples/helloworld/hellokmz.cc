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

// An example of the KMZ API, showing DataIsKmz() and ReadKmlFromKmz().
// Prints the default KML file in a KMZ archive, if found, or the first KML
// file.

#include <iostream>
#include <string>
#include "kml/dom.h"
#include "kml/util/fileio.h"
#include "kml/util/kmz.h"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " kmzfile" << endl;
    return 1;
  }
  const char* infile = argv[1];

  std::string data;
  ReadFileToString(infile, &data);
  if (data.empty()) {
    cout << "error: no data read from " << infile << endl;
    return 1;
  }

  bool is_kmz = DataIsKmz(data);
  if (!is_kmz) {
    cout << "error: " << infile << " is not a KMZ file" << endl;
    return 1;
  }

  std::string kml;
  ReadKmlFromKmz(infile, &kml);
  if (kml.empty()) {
    cout << "No data read from " << kml << endl;
    return 1;
  }

  std::cout << kml << endl;
  return 0;
}
