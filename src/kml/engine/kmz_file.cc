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

// The file contains the implementation of the KmzFile class methods.

#include "kml/engine/kmz_file.h"
#include <cstring>
#include "kml/util/file.h"

namespace kmlengine {

const char kDefaultKmlFile[] = "doc.kml";

// Private constructor. The unzfile is guaranteed to be sane.
KmzFile::KmzFile(unzFile unzfile, kmlutil::TempFile* tempfile)
    : unzfile_(unzfile), tempfile_(tempfile) {}

KmzFile::~KmzFile() {
  delete tempfile_;
  if (unzfile_) {
    unzClose(unzfile_);
  }
}

// Static.
KmzFile* KmzFile::OpenFromFile(const char* kmz_filename) {
  if (!kmlutil::File::Exists(kmz_filename)) {
    return NULL;
  }
  std::string data;
  if (!kmlutil::File::ReadFileToString(kmz_filename, &data)) {
    return NULL;
  }
  return OpenFromString(data);
}

// Static.
KmzFile* KmzFile::OpenFromString(const std::string& kmz_data) {
  if (!IsKmz(kmz_data)) {
    return NULL;
  }
  // Minizip is file-based. The TempFile util class manages the creation and
  // deletion of the temporary files we use. This class owns the destruction
  // of the TempFile object.
  kmlutil::TempFile* tempfile = kmlutil::TempFile::CreateTempFile();
  if (!tempfile) {
    return NULL;
  }
  if (!kmlutil::File::WriteStringToFile(kmz_data, tempfile->name())) {
    delete tempfile;  // TODO: scoped_ptr.
    return NULL;
  }
  unzFile unzfile = unzOpen(tempfile->name().c_str());
  if (!unzfile) {
    delete tempfile;  // TODO: scoped_ptr.
    return NULL;
  }
  // unzfile_ is now a minizip file handle to the tempfile we created. It is
  // passed to the constructor of the KmzFile class.
  if (unzGoToFirstFile(unzfile) != UNZ_OK) {
    unzClose(unzfile);
    delete tempfile;  // TODO: scoped_ptr.
    return NULL;
  }
  // If we got this far, it's safe to construct the KmzFile object.
  KmzFile* kmzfile = new KmzFile(unzfile, tempfile);
  return kmzfile;
}

// Static.
bool KmzFile::IsKmz(const std::string& kmz_data) {
  return kmz_data.substr(0, 4) == "PK\003\004" ? true : false;
}

bool KmzFile::ReadKml(std::string* output) {
  if (!output) {
    return false;
  }
  // A doc.kml at the root level is the default KML file.
  if (ReadOne(kDefaultKmlFile, output)) {
    return true;
  }
  // If no doc.kml, then we read the first KML file.
  unz_file_info file_info;
  do {
    static char buf[1024];
    if (unzGetCurrentFileInfo(unzfile_, &file_info, buf, sizeof(buf),
                              0, 0, 0, 0) == UNZ_OK) {
      if (strlen(buf) >= 4 && strcmp(buf + strlen(buf)-4, ".kml") == 0 &&
          ReadCurrentFile(output)) {
        return true;
      }
    }
  } while (unzGoToNextFile(unzfile_) == UNZ_OK);
  return false;
}

bool KmzFile::ReadFile(const char* subfile, std::string* output) {
  return ReadOne(subfile, output);  // ReadOne performs NULL checks.
}

bool KmzFile::List(std::vector<std::string>* subfiles) {
  if (!subfiles || !unzfile_) {
    return false;
  }
  unz_file_info finfo;
  do {
    static char buf[1024];
    if (unzGetCurrentFileInfo(unzfile_, &finfo, buf, sizeof(buf),
                              0, 0, 0, 0) == UNZ_OK) {
      subfiles->push_back(buf);
    }
  } while (unzGoToNextFile(unzfile_) == UNZ_OK);
  return true;
}

// Static.
bool KmzFile::WriteKmz(const char* kmz_filepath, const std::string& kml) {
  zipFile zipfile_ = zipOpen(kmz_filepath, 0);
  if (!zipfile_) {
    return false;
  }
  zipOpenNewFileInZip(zipfile_, kDefaultKmlFile, 0, 0, 0, 0, 0, 0, Z_DEFLATED,
                      Z_DEFAULT_COMPRESSION);
  zipWriteInFileInZip(zipfile_, (void* const)kml.data(), kml.size());
  zipCloseFileInZip(zipfile_);
  zipClose(zipfile_, 0);
  return kmlutil::File::Exists(kmz_filepath);
}

// Private.
bool KmzFile::ReadCurrentFile(std::string* output) {
  if (unzfile_ == NULL) {
    return false;
  }
  if (unzOpenCurrentFile(unzfile_) != UNZ_OK) {
    return false;
  }
  unz_file_info finfo;
  if (unzGetCurrentFileInfo(unzfile_, &finfo, 0, 0, 0, 0, 0, 0) != UNZ_OK) {
    unzCloseCurrentFile(unzfile_);
    return false;
  }
  int nbytes = finfo.uncompressed_size;
  char* filedata = new char[nbytes];
  if (unzReadCurrentFile(unzfile_, filedata, nbytes) == nbytes) {
    output->assign(filedata, nbytes);
    unzCloseCurrentFile(unzfile_);
    delete [] filedata;
    return true;
  }
  unzCloseCurrentFile(unzfile_);
  delete [] filedata;
  return false;
}

// Private.
bool KmzFile::ReadOne(const char* subfile, std::string* output) {
  if (output == NULL || unzfile_ == NULL) {
    return false;
  }
  if (unzLocateFile(unzfile_, subfile, 0) == UNZ_OK
      && ReadCurrentFile(output)) {
    unzCloseCurrentFile(unzfile_);
    return true;
  }
  return false;
}

}  // end namespace kmlengine
