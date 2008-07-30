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

// Walk a KML NetworkLink hierarchy.
// Fetches and parses the NetworkLinks in a given file recursively.
// Prints a count of the number of KML files fetched and the total number
// of each kind of Feature in the hierachy.

#include <iostream>
#include <map>
#include <string>
#include "boost/scoped_ptr.hpp"
#include "kml/dom.h"
#include "kml/dom/xsd.h"  // TODO: expose Xsd::ElementName() properly
#include "kml/engine.h"
#include "curlfetch.h"

using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::NetworkLinkPtr;
using kmldom::OverlayPtr;
using kmlengine::KmlFile;
using kmlengine::KmzCache;
using std::cout;
using std::endl;

static void CountFeature(int type_id);
static void PrintFeatureCounts();
static void PrintFileCount();
static void WalkFile(const std::string& url, KmzCache* kmz_cache);

static int file_count;

static void PrintFileCount() {
  cout << "files " << file_count << endl;
}

typedef std::map<int,int> feature_counter_t;
feature_counter_t feature_counter;

static void CountFeature(int type_id) {
  feature_counter_t::const_iterator entry = feature_counter.find(type_id);
  if (entry == feature_counter.end()) {
    feature_counter[type_id] = 1;
  } else {
    ++feature_counter[type_id];
  }
}

static void PrintFeatureCounts() {
  for (feature_counter_t::const_iterator iter = feature_counter.begin();
       iter != feature_counter.end(); ++iter) {
    std::string element_name;
    cout << kmldom::Xsd::GetSchema()->ElementName(iter->first) << " "
      << iter->second << endl;
  }
}

const std::string ComputeRelativeUrl(const std::string& parent_url,
                                     const std::string& child_url) {
  kmlengine::Href child(child_url);    
  // See if child is already absolute.
  if (!child.IsRelative()) {
    return child_url;
  } 

  std::string new_child;
  // NOTE: This does not detect local files (c:\foo\foo.kml, etc).
  // NOTE: And, this assumes at least one / in the parent_url.
  size_t last_component = parent_url.rfind("/");
  if (last_component != std::string::npos) {
    new_child = parent_url.substr(0,last_component+1);  // keep '/'
    new_child.append(child_url);
  } 
  return new_child;
}

class FeatureCounter : public kmlengine::FeatureVisitor {
 public:
  FeatureCounter(const KmlFile& kml_file, KmzCache* kmz_cache)
      : kml_file_(kml_file), kmz_cache_(kmz_cache) {}

  virtual void VisitFeature(const kmldom::FeaturePtr& feature) {
    CountFeature(feature->Type());
    if (OverlayPtr overlay = AsOverlay(feature)) {
      std::string href;
      if (kmlengine::GetIconParentHref(overlay, &href)) {
        std::string url = ComputeRelativeUrl(kml_file_.get_url(), href);
        std::string data;
        if (!kmz_cache_->FetchUrl(url.c_str(), &data)) {
          cout << "fetch failed " << url << endl;
          return;
        }
        cout << href << " bytes " << data.size() << endl;
      }
    }
  }

 private:
  const KmlFile& kml_file_;
  KmzCache* kmz_cache_;
};

static void HandleFile(const KmlFile& kml_file, KmzCache* kmz_cache) {
  FeatureCounter feature_counter(kml_file, kmz_cache);
  kmlengine::VisitFeatureHierarchy(kmlengine::GetRootFeature(kml_file.root()),
                                   feature_counter);
}

static void WalkNetworkLinks(const KmlFile& kml_file, KmzCache* kmz_cache) {
  const kmlengine::ElementVector& link_vector =
      kml_file.get_link_parent_vector();
  for (size_t i = 0; i < link_vector.size(); ++i) {
    if (NetworkLinkPtr networklink = AsNetworkLink(link_vector[i])) {
      std::string href;
      if (kmlengine::GetLinkParentHref(networklink, &href)) {
        WalkFile(ComputeRelativeUrl(kml_file.get_url(), href), kmz_cache);
      }
    }
  }
}

static void WalkFile(const std::string& url, KmzCache* kmz_cache) {
  cout << url << endl;

  std::string kml;
  if (!kmz_cache->FetchUrl(url.c_str(), &kml)) {
    cout << "fetch failed " << url << endl;
    return;
  }

  // Parse it.
  std::string errors;
  boost::scoped_ptr<KmlFile> kml_file(KmlFile::CreateFromParse(kml, &errors));
  if (!kml_file.get()) {
    cout << "parse failed " << url << endl;
    cout << errors;
    return;
  }
  kml_file->set_url(url);
  ++file_count;

  HandleFile(*kml_file.get(), kmz_cache);

  WalkNetworkLinks(*kml_file.get(), kmz_cache);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " url" << endl;
    return 1;
  }
  const char* url = argv[1];
  KmzCache kmz_cache(CurlToString, 30);
  WalkFile(url, &kmz_cache);
  PrintFileCount();
  PrintFeatureCounts();
}
