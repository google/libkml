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
#include "kml/engine.h"
#include "curlfetch.h"

using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::LinkPtr;
using kmldom::NetworkLinkPtr;
using kmlengine::KmlFile;
using std::cout;
using std::endl;

static void ComputeChild(const std::string& parent_url,
                         const std::string& child_url,
                         std::string* absolute_child);
static void CountFeature(int type_id);
static ElementPtr FetchAndParse(const std::string& url);
static bool GetNetworkLinkHref(const NetworkLinkPtr& networklink,
                               std::string* href);
static void PrintFeatureCounts();
static void PrintFileCount();
static void WalkFile(const std::string& url);
static void WalkNetworkLink(const std::string& parent_url,
                            const NetworkLinkPtr& networklink);
static void WalkUrl(const std::string& parent_url,
                    const std::string& child_url);

static int file_count;

static ElementPtr FetchAndParse(const std::string& url) {
  std::string kml;
  if (!CurlToString(url.c_str(), &kml)) {
    cout << "fetch failed " << url << endl;
    return NULL;
  }

  // Parse it.
  std::string errors;
  boost::scoped_ptr<KmlFile> kml_file(KmlFile::CreateFromParse(kml, &errors));
  if (!kml_file.get()) {
    cout << "parse failed " << url << endl;
    cout << errors;
    return NULL;
  }
  ++file_count;
  return kml_file->root();
}

static void PrintFileCount() {
  cout << "files " << file_count << endl;
}

static void ComputeChild(const std::string& parent_url,
                         const std::string& child_url,
                         std::string* absolute_child) {
  // See if child is already absolute.
  if (child_url.compare(0, 7, "http://", 7) == 0) {
    *absolute_child = child_url;
    return;
  }
  // NOTE: This does not detect local files (c:\foo\foo.kml, etc).
  // NOTE: And, this assumes at least one / in the parent_url.
  size_t last_component = parent_url.rfind("/");
  if (last_component != std::string::npos) {
    *absolute_child = parent_url.substr(0,last_component+1);  // keep '/'
    absolute_child->append(child_url);
  }
}

// Handles only href's that are network urls. No kmz, no local file, etc.
// Parent URL is required to compute URL of relative child URL.
static void WalkUrl(const std::string& parent_url,
                    const std::string& child_url) {
  std::string absolute_child;
  ComputeChild(parent_url, child_url, &absolute_child);
  WalkFile(absolute_child);
}

static bool GetNetworkLinkHref(const NetworkLinkPtr& networklink,
                               std::string* href) {
  if (networklink->has_link()) {
    const LinkPtr link = networklink->get_link();
    if (link->has_href()) {
      *href = link->get_href();
      return true;
    }
  }
  return false;
}

static void WalkNetworkLink(const std::string& parent_url,
                            const NetworkLinkPtr& networklink) {
  std::string url;
  GetNetworkLinkHref(networklink, &url);
  if (!url.empty()) {
    WalkUrl(parent_url, url);
  }
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
    switch (iter->first) {
      case kmldom::Type_Document:
        element_name = "Document";
        break;
      case kmldom::Type_Folder:
        element_name = "Folder";
        break;
      case kmldom::Type_GroundOverlay:
        element_name = "GroundOverlay";
        break;
      case kmldom::Type_NetworkLink:
        element_name = "NetworkLink";
        break;
      case kmldom::Type_Placemark:
        element_name = "Placemark";
        break;
      case kmldom::Type_PhotoOverlay:
        element_name = "PhotoOverlay";
        break;
      case kmldom::Type_ScreenOverlay:
        element_name = "ScreenOverlay";
        break;
      default:
        element_name = "unknown feature";
        break;
    }
    cout << element_name << " " << iter->second << endl;
  }
}

class NetworkFeatureVisitor : public kmlengine::FeatureVisitor {
 public:
  NetworkFeatureVisitor(const std::string& url) : url_(url) {}

  virtual ~NetworkFeatureVisitor() {}

  // FeatureVisitor::VisitFeature()
  virtual void VisitFeature(const kmldom::FeaturePtr& feature) {
    CountFeature(feature->Type());
    if (NetworkLinkPtr networklink = AsNetworkLink(feature)) {
      WalkNetworkLink(url_, networklink);
    }
  }

 private:
  std::string url_;
};

static void WalkFile(const std::string& url) {
  cout << url << endl;
  ElementPtr root = FetchAndParse(url);
  if (root) {
    NetworkFeatureVisitor network_feature_visitor(url);
    VisitFeatureHierarchy(kmlengine::GetRootFeature(root),
                          network_feature_visitor);
  }
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " url" << endl;
    return 1;
  }
  const char* url = argv[1];
  WalkFile(url);
  PrintFileCount();
  PrintFeatureCounts();
}
