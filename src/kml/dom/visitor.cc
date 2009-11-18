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

// WARNING: THE VISITOR API IMPLEMENTED IN THIS CLASS IS EXPERIMENTAL AND
// SUBJECT TO CHANGE WITHOUT WARNING.

#include "kml/dom/visitor.h"

#include "kml/dom/kmldom.h"
#include "kml/dom/kml_cast.h"

namespace kmldom {

Visitor::Visitor() { }

Visitor::~Visitor() { }

void Visitor::VisitElement(const ElementPtr& element) {
  /* Top of element class hierarchy: Do nothing. */
}

// >> Visitor API Start >>
void Visitor::VisitContainer(
    const ContainerPtr& element) {
  VisitFeature(element);
}

void Visitor::VisitDocument(
    const DocumentPtr& element) {
  VisitContainer(element);
}

void Visitor::VisitFeature(
    const FeaturePtr& element) {
  VisitObject(element);
}

void Visitor::VisitFolder(
    const FolderPtr& element) {
  VisitContainer(element);
}

void Visitor::VisitGroundOverlay(
    const GroundOverlayPtr& element) {
  VisitOverlay(element);
}

void Visitor::VisitKml(
    const KmlPtr& element) {
  VisitElement(element);
}

void Visitor::VisitNetworkLink(
    const NetworkLinkPtr& element) {
  VisitFeature(element);
}

void Visitor::VisitObject(
    const ObjectPtr& element) {
  VisitElement(element);
}

void Visitor::VisitOverlay(
    const OverlayPtr& element) {
  VisitFeature(element);
}

void Visitor::VisitPhotoOverlay(
    const PhotoOverlayPtr& element) {
  VisitOverlay(element);
}

void Visitor::VisitPlacemark(
    const PlacemarkPtr& element) {
  VisitFeature(element);
}

void Visitor::VisitScreenOverlay(
    const ScreenOverlayPtr& element) {
  VisitOverlay(element);
}
// << Visitor API End <<

}  // end namespace kmldom
