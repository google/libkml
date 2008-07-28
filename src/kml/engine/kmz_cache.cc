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

// This file contains the implementation of the KmzCache class.

#include "kml/engine/kmz_cache.h"
#include "kml/engine/kml_uri.h"
#include "kml/base/time_util.h"

namespace kmlengine {

bool KmzCache::FetchUrl(const std::string& kml_url, std::string* content) {
  std::string fetchable_url;
  std::string path_in_kmz;
  bool is_kmz = KmzSplit(kml_url, &fetchable_url, &path_in_kmz);
  if (!is_kmz) {
    // This is a KMZ file cache only.  Anything else we just fetch directly.
    return network_fetch_func_(kml_url.c_str(), content);
  }
  // See if this in in the cache.
  if (FetchFromCache(fetchable_url, path_in_kmz, content)) {
    return true;
  }
  // If we get here the URL isn't into a KMZ in the cache, so fetch the KMZ.
  // NOTE: We're borrowing the users content buffer for a bit.
  if (!network_fetch_func_(fetchable_url.c_str(),  content)) {
    return false;  // Network fetch failed, return the bad news.
  }
  // Fetch of this URL succeeded.  Let's see what we got.
  if (!KmzFile::IsKmz(*content)) {
    return false;  // Fail out if this is not actually KMZ content.
  }
  // Create a KmzFile for this KMZ data.
  KmzFilePtr kmz_file = KmzFile::OpenFromString(*content);
  if (!kmz_file) {
    return false;  // Fail out on KmlFile creation failure.
  }
  // Yes, it opened fine as KMZ so save this KmzFile in the cache using the
  // network fetchahble URL as the cache key.
  Save(fetchable_url, kmz_file);
  // Now proceed to try to dig out the content of the given file within the KMZ.
  return FetchFromCache(fetchable_url, path_in_kmz, content);
}

bool KmzCache::FetchFromCache(const std::string& fetchable_url,
                              const std::string& path_in_kmz,
                              std::string* content) const {
  // Yes, this is a URL to/into a KMZ file.  See if it's in the cache.
  if (const KmzFilePtr kmz_file = LookUp(fetchable_url)) {
    // Yes, it's in the cache.
    // No explicit path within the KMZ means "the KML file".
    if (path_in_kmz.empty()) {
      return kmz_file->ReadKml(content);
    }
    // Otherwise we look up the named file.
    return kmz_file->ReadFile(path_in_kmz.c_str(), content);
  }
  return false;
}

// TODO: generalize KmzCacheEntry and make these templates on the val type.
const kmlengine::KmzFilePtr KmzCache::LookUp(const std::string& url) const {
  KmzFileMap::const_iterator iter = kmz_file_map_.find(url);
  if (iter == kmz_file_map_.end()) {
    return NULL;
  }
  // iter->first is key, second is val and val is KmzCacheEntry pair whose
  // first is KmlFilePtr (second is creation time of cache entry).
  return iter->second.first;
}

bool KmzCache::Save(const std::string& url,
                    const kmlengine::KmzFilePtr& kmz_file) {
  const kmlengine::KmzFilePtr exists = LookUp(url);
  if (exists) {
    return false;
  }
  if (kmz_file_map_.size() == max_entries_) {
    RemoveOldest();
  }
  KmzCacheEntry kmz_cache_entry = std::make_pair(kmz_file,
                                                 kmlbase::GetMicroTime());
  kmz_file_map_[url] = kmz_cache_entry;
  return true;
}

bool KmzCache::Delete(const std::string& url) {
  const kmlengine::KmzFilePtr kmzfile = LookUp(url);
  if (kmzfile) {
    kmz_file_map_.erase(url);
    return true;
  }
  return false;
}

bool KmzCache::RemoveOldest() {
  if (kmz_file_map_.empty()) {
    return false;
  }
  // Find the entry with the smallest time.
  KmzFileMap::iterator iter = kmz_file_map_.begin();
  KmzFileMap::iterator oldest = iter;
  for (;iter != kmz_file_map_.end(); ++iter) {
    // STL map iter is a pair<key,val> with val KmzCacheEntry which is a pair
    if (iter->second.second < oldest->second.second) {
      oldest = iter;
    }
  }
  kmz_file_map_.erase(oldest);
  return true;
}

}  // end namespace kmlengine
