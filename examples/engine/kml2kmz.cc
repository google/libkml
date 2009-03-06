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

// This program uses KmzFile::CreateFromKmlFilepath to create a KMZ archive
// from a KML file. The file's resources are bundled into the archive if their
// location is relative to the directory in which the KML file is located.
// A listing of the generated archive is printed at the end.

#include <iostream>
#include <string>
#include "kml/engine.h"

using kmlbase::StringVector;
using kmlengine::KmzFile;
using kmlengine::KmzFilePtr;

static void ListFiles(const std::string& kmz_filepath) {
  KmzFilePtr kmz(KmzFile::OpenFromFile(kmz_filepath.c_str()));
  if (!kmz) {
    return;
  }
  StringVector list;
  kmz->List(&list);
  StringVector::const_iterator itr;
  std::cout << "contents:" << std::endl;
  for (itr = list.begin(); itr != list.end(); ++itr) {
    std::cout << "  " << *itr << std::endl;
  }
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "usage: " << argv[0] << " input.kml output.kmz" << std::endl;
    return -1;
  }

  const std::string kml_filepath = argv[1];
  const std::string kmz_filepath = argv[2];

  if (!KmzFile::CreateFromKmlFilepath(kml_filepath, kmz_filepath)) {
    std::cerr << "could not create KMZ file from " << kml_filepath << std::endl;
    return -1;
  }

  std::cout << "wrote " << kmz_filepath << std::endl;
  ListFiles(kmz_filepath);
  return 0;
}

