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
#include <set>
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/engine/get_links.h"
#include "kml/engine/href.h"
#include "kml/engine/kml_uri.h"
#include "minizip/unzip.h"
#include "minizip/zip.h"

using kmlbase::StringVector;
using kmlbase::TempFilePtr;
using kmlbase::File;

namespace kmlengine {

// We use this Pimpl-type idiom to keep the requirement for minizip headers
// confined to this file.
// TODO: push more zip-specific stuff from KmzFile into a broader ZipFile.
class ZlibImpl {
 public:
  // The underlying zlib/minizip library does not permit reading and writing to
  // the same file handle. This class is constructed with a handle to a unzFile
  // (for reading from an open KMZ file) and/or a handle to a zipFile
  // (for writing).
  ZlibImpl(unzFile unzfile, zipFile zipfile)
    : unzfile_(unzfile), zipfile_(zipfile) {
  }

  ~ZlibImpl() {
    unzClose(unzfile_);
    zipClose(zipfile_, 0);
  }

  unzFile get_unzfile() {
    return unzfile_;
  }

  zipFile get_zipfile() {
    return zipfile_;
  }

 private:
  unzFile unzfile_;
  zipFile zipfile_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(ZlibImpl);
};

const char kDefaultKmlFilename[] = "doc.kml";

KmzFile::KmzFile(ZlibImpl* zlibimpl, const TempFilePtr& tempfile)
    : zlibimpl_(zlibimpl), tempfile_(tempfile) {}

KmzFile::~KmzFile() {
  delete zlibimpl_;
}

// Static.
KmzFile* KmzFile::OpenFromFile(const char* kmz_filename) {
  if (!kmlbase::File::Exists(kmz_filename)) {
    return NULL;
  }
  std::string data;
  if (!kmlbase::File::ReadFileToString(kmz_filename, &data)) {
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
  // deletion of the temporary files we use. 
  TempFilePtr tempfile = kmlbase::TempFile::CreateTempFile();
  if (!tempfile) {
    return NULL;
  }
  if (!kmlbase::File::WriteStringToFile(kmz_data, tempfile->name())) {
    return NULL;
  }

  unzFile unzfile = unzOpen(tempfile->name().c_str());
  if (!unzfile) {
    return NULL;
  }
  // unzfile_ is now a minizip file handle to the tempfile we created. It is
  // passed to the constructor of the KmzFile class.
  if (unzGoToFirstFile(unzfile) != UNZ_OK) {
    unzClose(unzfile);
    return NULL;
  }
  // If we got this far, it's safe to construct the KmzFile object.
  return new KmzFile(new ZlibImpl(unzfile, NULL), tempfile);
}

// Static.
bool KmzFile::IsKmz(const std::string& kmz_data) {
  return kmz_data.substr(0, 4) == "PK\003\004" ? true : false;
}

bool KmzFile::ReadKmlAndGetPath(std::string* output,
                                std::string* kml_name) const {
  if (!output) {
    return false;
  }
  // A doc.kml at the root level is the default KML file.
  if (ReadOne(kDefaultKmlFilename, output)) {
    if (kml_name) {
      *kml_name = kDefaultKmlFilename;
    }
    return true;
  }
  // If no doc.kml, then we read the first KML file.
  unz_file_info file_info;
  do {
    static char buf[1024];
    if (unzGetCurrentFileInfo(zlibimpl_->get_unzfile(), &file_info, buf,
                              sizeof(buf), 0, 0, 0, 0) == UNZ_OK) {
      if (strlen(buf) >= 4 && strcmp(buf + strlen(buf)-4, ".kml") == 0 &&
          ReadCurrentFile(output)) {
        if (kml_name) {
          *kml_name = buf;
        }
        return true;
      }
    }
  } while (unzGoToNextFile(zlibimpl_->get_unzfile()) == UNZ_OK);
  return false;
}

bool KmzFile::ReadKml(std::string* output) const {
  return ReadKmlAndGetPath(output, NULL);
}

bool KmzFile::ReadFile(const char* subfile, std::string* output) const {
  return ReadOne(subfile, output);  // ReadOne performs NULL checks.
}

bool KmzFile::List(std::vector<std::string>* subfiles) {
  if (!subfiles || !zlibimpl_->get_unzfile()) {
    return false;
  }
  unz_file_info finfo;
  do {
    static char buf[1024];
    if (unzGetCurrentFileInfo(zlibimpl_->get_unzfile(), &finfo, buf,
                              sizeof(buf), 0, 0, 0, 0) == UNZ_OK) {
      subfiles->push_back(buf);
    }
  } while (unzGoToNextFile(zlibimpl_->get_unzfile()) == UNZ_OK);
  return true;
}

// Static.
KmzFile* KmzFile::Create(const char* kmz_filepath) {
  zipFile zipfile = zipOpen(kmz_filepath, 0);
  if (!zipfile) {
    return NULL;
  }
  return new KmzFile(new ZlibImpl(NULL, zipfile), NULL);
}

bool KmzFile::AddFile(const std::string& data, const std::string& path_in_kmz) {
  // The path must be relative to and below the archive.
  if (path_in_kmz.substr(0, 1).find_first_of("/\\") != std::string::npos ||
      path_in_kmz.substr(0, 2) == "..") {
    return false;
  }
  zipFile zipfile = zlibimpl_->get_zipfile();
  if (!zipfile) {
    return false;
  }
  zipOpenNewFileInZip(zipfile, path_in_kmz.c_str(), 0, 0, 0, 0, 0, 0,
                      Z_DEFLATED, Z_DEFAULT_COMPRESSION);
  zipWriteInFileInZip(zipfile, static_cast<const void*>(data.data()),
                      static_cast<unsigned int>(data.size()));
  return zipCloseFileInZip(zipfile) == ZIP_OK;
}

size_t KmzFile::AddFileList(const std::string& base_url,
                            const StringVector& file_paths) {
  size_t error_count = 0;
  // We remember all stored resources so we can eliminate duplicates.
  std::set<std::string> stored_hrefs;

  StringVector::const_iterator itr;
  for (itr = file_paths.begin(); itr != file_paths.end(); ++itr) {
    // Drop the fragment if any to get the stem of the filename.
    Href href(*itr);
    if (href.has_fragment()) {
      href.clear_fragment();
    }

    // Normalize the href.
    std::string normalized_href;
    if (!NormalizeHref(href.get_path(), &normalized_href)) {
      error_count++;
      continue;
    }

    // If the normalized_href points above the base_url, we consider it
    // invalid.
    if (normalized_href.substr(0, 2) == "..") {
      error_count++;
      continue;
    }

    // Detect duplicate resources and skip if found.
    if (stored_hrefs.find(normalized_href) != stored_hrefs.end()) {
      continue;  // Not an error.
    }
    stored_hrefs.insert(normalized_href);

    // Try to read the file pointed to by base_url and the normalized href.
    std::string relative_path = File::JoinPaths(base_url, normalized_href);
    std::string file_data;
    if (!kmlbase::File::ReadFileToString(relative_path, &file_data)) {
      error_count++;
      continue;
    }

    // Add the file to the KMZ archive.
    if (!AddFile(file_data, normalized_href)) {
      error_count++;
      continue;
    }
  }
  return error_count;
}

// Static.
bool KmzFile::WriteKmz(const char* kmz_filepath, const std::string& kml) {
  boost::scoped_ptr<KmzFile> kmz(KmzFile::Create(kmz_filepath));
  if (!kmz.get()) {
    return false;
  }
  if (!kmz->AddFile(kml, kDefaultKmlFilename)) {
    return false;
  }
  return kmlbase::File::Exists(kmz_filepath);
}

// Static.
bool KmzFile::CreateFromKmlFilepath(const std::string& kml_filepath,
                                    const std::string& kmz_filepath) {
  if (kmz_filepath.empty() || kml_filepath.empty()) {
    return false;
  }
  std::string kml_data;
  if (!kmlbase::File::ReadFileToString(kml_filepath, &kml_data)) {
    return false;
  }

  std::string base_dir;
  kmlbase::File::SplitFilePath(kml_filepath, &base_dir, NULL);

  KmlFilePtr kml_file =
    KmlFile::CreateFromStringWithUrl(kml_data, base_dir, NULL);

  return CreateFromKmlFile(kml_file, kmz_filepath);
}

// Static.
bool KmzFile::CreateFromElement(const kmldom::ElementPtr& element,
                                const std::string& base_url,
                                const std::string& kmz_filepath) {
  if (kmz_filepath.empty()) {
    return false;
  }
  KmzFilePtr kmz_file = Create(kmz_filepath.c_str());
  if (!kmz_file) {
    return false;
  }
  const std::string kml_data = kmldom::SerializePretty(element);
  // First add the KML file. This is the file opened by default by a client
  // from a KMZ archive.
  kmz_file->AddFile(kml_data, kDefaultKmlFilename);

  // Next gather the local references and add them.
  StringVector file_paths;
  if (GetRelativeLinks(kml_data, &file_paths)) {
    kmz_file->AddFileList(base_url, file_paths);
  }

  return kmlbase::File::Exists(kmz_filepath);
}

// Static.
bool KmzFile::CreateFromKmlFile(const KmlFilePtr& kml_file,
                                const std::string& kmz_filepath) {
  return KmzFile::CreateFromElement(
      kml_file->get_root(), kml_file->get_url(), kmz_filepath);
}

// Private.
bool KmzFile::ReadCurrentFile(std::string* output) const {
  if (zlibimpl_->get_unzfile() == NULL) {
    return false;
  }
  if (unzOpenCurrentFile(zlibimpl_->get_unzfile()) != UNZ_OK) {
    return false;
  }
  unz_file_info finfo;
  if (unzGetCurrentFileInfo(zlibimpl_->get_unzfile(),
                            &finfo, 0, 0, 0, 0, 0, 0) != UNZ_OK) {
    unzCloseCurrentFile(zlibimpl_->get_unzfile());
    return false;
  }
  int nbytes = finfo.uncompressed_size;
  char* filedata = new char[nbytes];
  if (unzReadCurrentFile(zlibimpl_->get_unzfile(), filedata, nbytes) == nbytes) {
    output->assign(filedata, nbytes);
    unzCloseCurrentFile(zlibimpl_->get_unzfile());
    delete [] filedata;
    return true;
  }
  unzCloseCurrentFile(zlibimpl_->get_unzfile());
  delete [] filedata;
  return false;
}

// Private.
bool KmzFile::ReadOne(const char* subfile, std::string* output) const {
  if (output == NULL || zlibimpl_->get_unzfile() == NULL) {
    return false;
  }
  if (unzLocateFile(zlibimpl_->get_unzfile(), subfile, 0) == UNZ_OK
      && ReadCurrentFile(output)) {
    unzCloseCurrentFile(zlibimpl_->get_unzfile());
    return true;
  }
  return false;
}

}  // end namespace kmlengine
