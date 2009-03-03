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

// This file contains the declaration of the KML URI resolution and parsing
// functions.

#ifndef KML_ENGINE_KML_URI_H__
#define KML_ENGINE_KML_URI_H__

#include <string>
#include "boost/scoped_ptr.hpp"

// Forward declare to avoid including uri_parser.h in app code.
namespace kmlbase {
class UriParser;
}

namespace kmlengine {

// This function implements standard RFC 3986 URI resolution (Section 5).
// In the context of KML the base URI is typically that of the KML file
// (see KmlFile::get_url()), and "relative" is typically the contents of an
// <href> (such as in <NetworkLink>'s <Link> or any Overlay's <Icon>),
// <styleURl> or schemaUrl=.  The result string is the resolved URI to fetch.
// No network fetching is performed with this function.  This merely
// computes the URI resolution.

// For example,
// if the file overlay.kml is fetched from http://host.com/dir/overlay.kml
//   <GroundOverlay>
//     <Icon>
//       <href>../images/pretty.jpg</href>
//     </Icon>
//   </GroundOverlay>
// ...then the result resolving relative="../images/pretty.jpg" against
// the base of "http://host.com/kml/overlay.kml" will be:
// "http://host.com/images/pretty.jpg".

// Also, note that these same standard rules apply for KMZ URIs.
// Here is an example of the two levels of resolution required to resolve
// each <targetHref> of a given <Model>'s <ResourceMap>.
// First presume the base URI of a KMZ is "http://host.com/dir/model.kmz".
// Within the default KML file of the KMZ is the following:
//   <Model>
//     <Link>
//       <href>geometry/bldg.dae</href>
//     </Link>
//     <ResourceMap>
//       <Alias>
//         <targetHref>../textures/brick.jpg</targetHref>
//         <sourceHref>../files/CU-Macky-4sideturretnoCulling.jpg</sourceHref>
//       </Alias>
//     </ResourceMap>
//   </Model>
// The relative "geometry/bldg.dae" results in
// "http://host.com/dir/model.kmz/geometry/bldg.dae".  (A KMZ-aware fetching
// system is required to split the fetchable URL from the path reference into
// the KMZ, but this result is still a valid standard URI).
// With the resolved base of "http://host.com/dir/model.kmz/geometry/bldg.dae"
// and relative of "../textures/brick.jpg" the resolved result URI is
// "http://host.com/dir/model.kmz/textures/brick.jpg" which is to say a
// path of "textures/brick.jpg" at the KMZ whose URI is
// "http://host.com/dir/model.kmz".  Note that it is perfectly valid for a
// relative reference with a KMZ to refer "up and out" of the KMZ to
// either a file within another KMZ or a single file.
bool ResolveUri(const std::string& base, const std::string& relative,
                std::string* result);

// This function splits out the various components of a URI:
// uri = scheme://host:port/path?query#fragment
// An output string NULL pointer simply ignores splitting out that component.
// The return value reflects the validity of the uri.  Each desired output
// string should be inspected using empty() to discover if the uri has
// the particular component.
bool SplitUri(const std::string& uri, std::string* scheme, std::string* host,
              std::string* port, std::string* path, std::string* query,
              std::string* fragment);

// This function returns true if the given uri is valid and has a fragment.
// If it has a fragment and a string pointer is supplied it is saved there
// (without the #).
bool SplitUriFragment(const std::string& uri, std::string* fragment);

// This function returns true if the given uri is valid and has a path.
// If is has a path and the string pointer is supplied it is saved there.
bool SplitUriPath(const std::string& uri, std::string* path);

// This function returns true if the given uri is valid.  If the fetchable_uri
// output string is supplied a uri w/o the fragment is stored there.
bool GetFetchableUri(const std::string& uri, std::string* fetchable_uri);

bool KmzSplit(const std::string& kml_url, std::string* kmz_url,
              std::string* kmz_path);


// TODO: split query name-value pairs

// Resolve the URL to the Model's targetHref.  The base is the URL
// of the KML file holding the Model.  The geometry_href is the value
// of the Model's Link/href.  The target_href is the value of one of the
// Model's ResourceMap/Alias/targetHref's.  Note that the result URL may
// be into a KMZ and hence might be used with KmzSplit.
bool ResolveModelTargetHref(const std::string& base,
                            const std::string& geometry_href, 
                            const std::string& target,
                            std::string* result);

// The main purpose of the KmlUri class is to hold the URI state for a given
// fetch.  This state is principally a base url and a relative target to fetch.
// Ideally any URI stands alone, however, the two-level fetch system used for
// relative KMZ references requires the target reference to be retained
// to be resolved against either the full URI of the base (typically that of
// the KmlFile in the KML Engine) or the URI of the KMZ archive containing
// the KML file, in that order.  For more details and examples see kml_uri.cc
// NOTE: This is an internal class.  Do not use in application code.
// Applications should use KmlFile (where the API provides the means to pass
// a base URI and target URI for relative fetches).
class KmlUri {
 public:
  // The base is a full absolute URI including scheme.  The base is typically
  // the URI of a KML file as maintained in KmlFile::get_url().  For example,
  // http://host.com/dir/path.kml, or http://host.com/dir/path.kmz/doc.kml.
  // (Note that a "bare" KMZ reference here does _not_ automatically imply
  // "the KML file" within the KMZ.  See the note above about this being and
  // internal class).  The target is a relative or abstolue URI typically the
  // raw content of any <href>, <styleUrl>, schemaUrl=, <targetHref>,
  // <a href="...">, or <img href="..."> within the KmlFile.  However, there
  // is no specific knowlege of any KML or HTML element within this class.
  static KmlUri* CreateRelative(const std::string& base,
                                const std::string& target);

  ~KmlUri();

  bool is_kmz() const {
    return is_kmz_;
  }

  const std::string& get_target() const {
    return target_;
  }

  const std::string& get_url() const {
    return url_;
  }

  const std::string& get_kmz_url() const {
    return kmz_url_;
  }

  const std::string& get_path_in_kmz() const {
    return path_in_kmz_;
  }

  // TODO Ideally this class has no non-const methods.  No module should alter
  // a KmlUri.  Instead a new one should be created as needed.
  void set_path_in_kmz(const std::string path_in_kmz) {
    path_in_kmz_ = path_in_kmz;
    url_ = kmz_url_ + "/" + path_in_kmz;
  }

 private:
  // Private constructor.  Use static Create() method.
  // TODO streamline this with the Create method.
  KmlUri(const std::string& base, const std::string& target);

  bool is_kmz_;  // TODO should this be is_relative_kmz_?
  const std::string base_;
  const std::string target_;
  // TODO use UriParser's throughout _or_ std::string, not both.
  boost::scoped_ptr<kmlbase::UriParser> target_uri_;

  std::string url_;

  // TODO this is too complex.  Better might be to create a new KmlUri for
  // a new fetch.
  std::string kmz_url_;
  std::string path_in_kmz_;
};

}  // end namespace kmlengine

#endif  // KML_ENGINE_KML_URI_H__
