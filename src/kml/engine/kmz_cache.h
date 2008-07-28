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

#ifndef KML_ENGINE_KMZ_CACHE_H__
#define KML_ENGINE_KMZ_CACHE_H__

#include <map>
#include <string>
#include "kml/engine/kmz_file.h"

namespace kmlengine {

// This is the signature of the function used by KmzCache to fetch the content
// at the given URL.  It is the responsibility of the implementation to detect
// 404s and other such errors.  A true return status indicates to KmzCache that
// the expected data at the given URL was fetched properly.
typedef bool NetworkFetchFunc(const char* url, std::string* content);

// A KMZ cache entry is a mapping from the URL used to fetch the KMZ file
// to a pair of the KmzFile representing the local copy of the fetched and
// opened KMZ file and the absolute time the KMZ was fetched.
// This is a type used internally to KmzClass whose intended access is via
// the public methods described below.  As an implementation note this is
// one map of pairs as opposed to a pair of maps to better enforce consistency.
// Given that this caches a fairly heavyweight item correctness is placed
// at a premium in this code.
// TODO: expiration
typedef std::pair<kmlengine::KmzFilePtr, double> KmzCacheEntry;
typedef std::map<const std::string, KmzCacheEntry> KmzFileMap;

class KmzCache {
 public:
  // The constructor must be supplied with the NetworkFetchFunc and a given
  // limit on the number of open KmzFile's to cache.  Note that the number
  // of entries to cache is expected to be less than the maximum number of
  // open file descriptors for a given system and is thus expected to be
  // generally fairly small (<100 entries in typical usage).
  KmzCache(NetworkFetchFunc* network_fetch_func, size_t max_entries)
      : network_fetch_func_(network_fetch_func),
        max_entries_(max_entries) {}

  // This is the main public method to fetch a "KML URL" through the cache.
  // A "KML URL" is anything that may appear in a <href> especially including
  // file references into a KMZ such as: "http://foo.com/goo.kmz/images/z.jpg".
  // The engine::KmzSplit method is used to detect and save the two parts
  // of a "KML URL" if it is this variety of KMZ file access.  If the
  // KmzCache has no entry for the given KMZ file it is fetched using the
  // NetworkFetchFunc provided in the constructor.  If the KmzCache does have
  // this KMZ cached then this simply accesses the internal KmzFile for this
  // KMZ and reads the content.  This returns true if the KMZ was fetched
  // properly and/or the file within the KMZ was found and read properly.
  bool FetchUrl(const std::string& kml_url, std::string* content);

  // If the given "KML URL" is to a KMZ file within the cache then the content
  // for this file within the KMZ is saved to the supplied buffer if such is
  // supplied.  The return status indicates whether or not a KMZ file for
  // this URL is the cache and if that KMZ has the given file.  See the
  // the discussion in KmzSplit above.  In most circumstances application
  // code should use FetchUrl(), but this method is well behaved as described.
  bool FetchFromCache(const std::string& fetchable_url,
                      const std::string& path_in_kmz,
                      std::string* content) const;

  // This returns the KmlFile for the given "KML URL" or NULL if the cache
  // has no entry for this URL.  Note that this is the network fetchable URL
  // such as that returned by the KmzSplit() function.
  const kmlengine::KmzFilePtr LookUp(const std::string& fetchable_url) const;

  // This stores the given KmzFile into the cache for the given URL.
  // False is returned and no internal state is altered if an entry already
  // exists for this URL.  Use Delete() explicitly to remove an entry.
  // Saving a new item _may_ result in deleting the oldest item in the cache.
  bool Save(const std::string& fetchable_url,
            const kmlengine::KmzFilePtr& kmz_file);

  // This removes the KmzFile at the given URL from the cache.  This returns
  // if an entry was found and deleted and false otherwise.
  bool Delete(const std::string& fetchable_url);

  // Remove the oldest entry in the cache and return true.  False is returned
  // if the cache was empty.
  bool RemoveOldest();

  // This returns the number of entries (URL -> KMZ mappings) in the cache.
  size_t Size() const {
    return kmz_file_map_.size();
  }

 private:
  NetworkFetchFunc* network_fetch_func_;
  KmzFileMap kmz_file_map_;
  size_t max_entries_;
};

}  // end namespace kmlengine

#endif  // KML_ENGINE_KMZ_CACHE_H__
