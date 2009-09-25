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

// This file contains the implementation of the Atom utility functions.

#include "kml/convenience/atom_util.h"

#include <string>
#include "kml/base/string_util.h"
#include "kml/dom.h"
#include "kml/engine/clone.h"

using kmldom::AtomContentPtr;
using kmldom::AtomEntryPtr;
using kmldom::AtomFeedPtr;
using kmldom::AtomLinkPtr;
using kmldom::ContainerPtr;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::KmlFactory;

namespace kmlconvenience {

// static
AtomEntryPtr AtomUtil::CreateBasicEntry(const std::string& title,
                                        const std::string& summary) {
  AtomEntryPtr entry = KmlFactory::GetFactory()->CreateAtomEntry();
  entry->set_title(title);
  entry->set_summary(summary);
  return entry;
}

// static
kmldom::AtomEntryPtr AtomUtil::CreateEntryForFeature(
    const FeaturePtr& feature) {
  if (!feature.get()) {
    return NULL;
  }
  AtomEntryPtr entry = CreateBasicEntry(feature->get_name(),
                                        feature->get_description());
  AtomContentPtr content = KmlFactory::GetFactory()->CreateAtomContent();
  ElementPtr element = content;
  // There is no public API for programmatically adding an unknown or
  // misplaced element to any other, so use the same API the parser uses
  // which is that all elements pass anything they don't know up to
  // Element::AddElement().
  element->AddElement(feature);
  entry->set_content(content);
  return entry;
}

// static
bool AtomUtil::GetContentSrc(const AtomEntryPtr& entry, std::string* src) {
  if (entry->has_content() && entry->get_content()->has_src()) {
    if (src) {
      *src = entry->get_content()->get_src();
    }
    return true;
  }
  return false;
}

// static
bool AtomUtil::LinkIsOfRel(const kmldom::AtomLinkPtr& link,
                           const std::string& rel_type) {
  return link.get() && !rel_type.empty() &&
      kmlbase::StringEndsWith(link->get_rel(), rel_type);
}

// static
bool AtomUtil::FindRelUrl(const kmldom::AtomCommon& atom_common,
                          const std::string& rel_type, std::string* href) {
  size_t link_size = atom_common.get_link_array_size();
  for (size_t i = 0; i < link_size; ++i) {
    const kmldom::AtomLinkPtr& link = atom_common.get_link_array_at(i);
    if (link->has_href() && LinkIsOfRel(link, rel_type)) {
      if (href) {
        *href = link->get_href();
      }
      return true;
    }
  }
  return false;
}

// static
kmldom::AtomLinkPtr AtomUtil::FindLink(const kmldom::AtomCommon& atom_common,
                                       const std::string& rel_type,
                                       const std::string& mime_type) {
  size_t link_size = atom_common.get_link_array_size();
  for (size_t i = 0; i < link_size; ++i) {
    const kmldom::AtomLinkPtr& link = atom_common.get_link_array_at(i);
    if (LinkIsOfRel(link, rel_type) && link->get_type() == mime_type) {
      return link;
    }
  }
  return NULL;
}

// static
FeaturePtr AtomUtil::GetEntryFeature(const AtomEntryPtr& entry) {
  // Any KML child of <content> will appear as a misplaced element.
  if (entry.get() && entry->has_content() &&
      entry->get_content()->get_misplaced_elements_array_size() > 0) {
    return kmldom::AsFeature(
             entry->get_content()->get_misplaced_elements_array_at(0));
  }
  return NULL;
}

// static
FeaturePtr AtomUtil::CloneEntryFeature(const AtomEntryPtr& entry) {
  if (FeaturePtr feature = GetEntryFeature(entry)) {
    std::string href;
    if (FindRelUrl(*entry.get(), "self", &href)) {
      AtomLinkPtr link = KmlFactory::GetFactory()->CreateAtomLink();
      link->set_href(href);
      feature->set_atomlink(link);
    }
    return feature;
  }
  return NULL;
}

// static
void AtomUtil::GetFeedFeatures(const AtomFeedPtr& feed,
                               ContainerPtr container) {
  // Need both an <atom:feed> and Container to do anything interesting.
  if (feed.get() && container.get()) {
    for (size_t i = 0; i < feed->get_entry_array_size(); ++i) {
      // We need to clone because libkml allows no more than one parent per
      // XmlElement (see kmlbase::XmlElement::SetParent).
      container->add_feature(CloneEntryFeature(feed->get_entry_array_at(i)));
    }
  }
}

}  // end namespace kmlconvenience
