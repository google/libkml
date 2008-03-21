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

#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kmldom.h"
#include "kml/util/unit_test.h"

namespace kmldom {

class KmlCastTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(KmlCastTest);
  CPPUNIT_TEST(TestCasts);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestCasts();
};

CPPUNIT_TEST_SUITE_REGISTRATION(KmlCastTest);

void KmlCastTest::TestCasts() {
  KmlFactory* factory = KmlFactory::GetFactory();
  Element* e;

  e = factory->CreateElementById(Type_LookAt);
  CPPUNIT_ASSERT(AsAbstractView(e));
  delete e;

  e = factory->CreateElementById(Type_IconStyle);
  CPPUNIT_ASSERT(AsColorStyle(e));
  delete e;

  e = factory->CreateElementById(Type_Folder);
  CPPUNIT_ASSERT(AsContainer(e));
  delete e;

  e = factory->CreateElementById(Type_Placemark);
  CPPUNIT_ASSERT(AsFeature(e));
  delete e;

  e = factory->CreateElementById(Type_Point);
  CPPUNIT_ASSERT(AsGeometry(e));
  delete e;

  e = factory->CreateElementById(Type_Placemark);
  CPPUNIT_ASSERT(AsObject(e));
  delete e;

  e = factory->CreateElementById(Type_GroundOverlay);
  CPPUNIT_ASSERT(AsOverlay(e));
  delete e;

  e = factory->CreateElementById(Type_Style);
  CPPUNIT_ASSERT(AsStyleSelector(e));
  delete e;

  e = factory->CreateElementById(Type_BalloonStyle);
  CPPUNIT_ASSERT(AsSubStyle(e));
  delete e;

  e = factory->CreateElementById(Type_TimeSpan);
  CPPUNIT_ASSERT(AsTimePrimitive(e));
  delete e;

  e = factory->CreateElementById(Type_Alias);
  CPPUNIT_ASSERT(AsAlias(e));
  delete e;

  e = factory->CreateElementById(Type_BalloonStyle);
  CPPUNIT_ASSERT(AsBalloonStyle(e));
  delete e;

  e = factory->CreateElementById(Type_Camera);
  CPPUNIT_ASSERT(AsCamera(e));
  delete e;

  e = factory->CreateElementById(Type_Change);
  CPPUNIT_ASSERT(AsChange(e));
  delete e;

  e = factory->CreateElementById(Type_Create);
  CPPUNIT_ASSERT(AsCreate(e));
  delete e;

  e = factory->CreateElementById(Type_Data);
  CPPUNIT_ASSERT(AsData(e));
  delete e;

  e = factory->CreateElementById(Type_Delete);
  CPPUNIT_ASSERT(AsDelete(e));
  delete e;

  e = factory->CreateElementById(Type_Document);
  CPPUNIT_ASSERT(AsDocument(e));
  delete e;

  e = factory->CreateElementById(Type_ExtendedData);
  CPPUNIT_ASSERT(AsExtendedData(e));
  delete e;

  e = factory->CreateElementById(Type_Folder);
  CPPUNIT_ASSERT(AsFolder(e));
  delete e;

  e = factory->CreateElementById(Type_GroundOverlay);
  CPPUNIT_ASSERT(AsGroundOverlay(e));
  delete e;

  e = factory->CreateElementById(Type_hotSpot);
  CPPUNIT_ASSERT(AsHotSpot(e));
  delete e;

  e = factory->CreateElementById(Type_Icon);
  CPPUNIT_ASSERT(AsIcon(e));
  delete e;

  e = factory->CreateElementById(Type_IconStyle);
  CPPUNIT_ASSERT(AsIconStyle(e));
  delete e;

  e = factory->CreateElementById(Type_ImagePyramid);
  CPPUNIT_ASSERT(AsImagePyramid(e));
  delete e;

  e = factory->CreateElementById(Type_innerBoundaryIs);
  CPPUNIT_ASSERT(AsInnerBoundaryIs(e));
  delete e;

  e = factory->CreateElementById(Type_ItemIcon);
  CPPUNIT_ASSERT(AsItemIcon(e));
  delete e;

  e = factory->CreateElementById(Type_LabelStyle);
  CPPUNIT_ASSERT(AsLabelStyle(e));
  delete e;

  e = factory->CreateElementById(Type_LatLonAltBox);
  CPPUNIT_ASSERT(AsLatLonAltBox(e));
  delete e;

  e = factory->CreateElementById(Type_LatLonBox);
  CPPUNIT_ASSERT(AsLatLonBox(e));
  delete e;

  e = factory->CreateElementById(Type_LineString);
  CPPUNIT_ASSERT(AsLineString(e));
  delete e;

  e = factory->CreateElementById(Type_LineStyle);
  CPPUNIT_ASSERT(AsLineStyle(e));
  delete e;

  e = factory->CreateElementById(Type_LinearRing);
  CPPUNIT_ASSERT(AsLinearRing(e));
  delete e;

  e = factory->CreateElementById(Type_Link);
  CPPUNIT_ASSERT(AsLink(e));
  delete e;

  e = factory->CreateElementById(Type_linkSnippet);
  CPPUNIT_ASSERT(AsLinkSnippet(e));
  delete e;

  e = factory->CreateElementById(Type_ListStyle);
  CPPUNIT_ASSERT(AsListStyle(e));
  delete e;

  e = factory->CreateElementById(Type_Location);
  CPPUNIT_ASSERT(AsLocation(e));
  delete e;

  e = factory->CreateElementById(Type_Lod);
  CPPUNIT_ASSERT(AsLod(e));
  delete e;

  e = factory->CreateElementById(Type_LookAt);
  CPPUNIT_ASSERT(AsLookAt(e));
  delete e;

  e = factory->CreateElementById(Type_kml);
  CPPUNIT_ASSERT(AsKml(e));
  delete e;

  e = factory->CreateElementById(Type_Model);
  CPPUNIT_ASSERT(AsModel(e));
  delete e;

  e = factory->CreateElementById(Type_MultiGeometry);
  CPPUNIT_ASSERT(AsMultiGeometry(e));
  delete e;

  e = factory->CreateElementById(Type_NetworkLink);
  CPPUNIT_ASSERT(AsNetworkLink(e));
  delete e;

  e = factory->CreateElementById(Type_NetworkLinkControl);
  CPPUNIT_ASSERT(AsNetworkLinkControl(e));
  delete e;

  e = factory->CreateElementById(Type_Orientation);
  CPPUNIT_ASSERT(AsOrientation(e));
  delete e;

  e = factory->CreateElementById(Type_outerBoundaryIs);
  CPPUNIT_ASSERT(AsOuterBoundaryIs(e));
  delete e;

  e = factory->CreateElementById(Type_overlayXY);
  CPPUNIT_ASSERT(AsOverlayXY(e));
  delete e;

  e = factory->CreateElementById(Type_Pair);
  CPPUNIT_ASSERT(AsPair(e));
  delete e;

  e = factory->CreateElementById(Type_PhotoOverlay);
  CPPUNIT_ASSERT(AsPhotoOverlay(e));
  delete e;

  e = factory->CreateElementById(Type_Placemark);
  CPPUNIT_ASSERT(AsPlacemark(e));
  delete e;

  e = factory->CreateElementById(Type_Point);
  CPPUNIT_ASSERT(AsPoint(e));
  delete e;

  e = factory->CreateElementById(Type_PolyStyle);
  CPPUNIT_ASSERT(AsPolyStyle(e));
  delete e;

  e = factory->CreateElementById(Type_Polygon);
  CPPUNIT_ASSERT(AsPolygon(e));
  delete e;

  e = factory->CreateElementById(Type_Region);
  CPPUNIT_ASSERT(AsRegion(e));
  delete e;

  e = factory->CreateElementById(Type_ResourceMap);
  CPPUNIT_ASSERT(AsResourceMap(e));
  delete e;

  e = factory->CreateElementById(Type_rotationXY);
  CPPUNIT_ASSERT(AsRotationXY(e));
  delete e;

  e = factory->CreateElementById(Type_Scale);
  CPPUNIT_ASSERT(AsScale(e));
  delete e;

  e = factory->CreateElementById(Type_Schema);
  CPPUNIT_ASSERT(AsSchema(e));
  delete e;

  e = factory->CreateElementById(Type_SchemaData);
  CPPUNIT_ASSERT(AsSchemaData(e));
  delete e;

  e = factory->CreateElementById(Type_ScreenOverlay);
  CPPUNIT_ASSERT(AsScreenOverlay(e));
  delete e;

  e = factory->CreateElementById(Type_screenXY);
  CPPUNIT_ASSERT(AsScreenXY(e));
  delete e;

  e = factory->CreateElementById(Type_SimpleData);
  CPPUNIT_ASSERT(AsSimpleData(e));
  delete e;

  e = factory->CreateElementById(Type_SimpleField);
  CPPUNIT_ASSERT(AsSimpleField(e));
  delete e;

  e = factory->CreateElementById(Type_size);
  CPPUNIT_ASSERT(AsSize(e));
  delete e;

  e = factory->CreateElementById(Type_Snippet);
  CPPUNIT_ASSERT(AsSnippet(e));
  delete e;

  e = factory->CreateElementById(Type_Style);
  CPPUNIT_ASSERT(AsStyle(e));
  delete e;

  e = factory->CreateElementById(Type_StyleMap);
  CPPUNIT_ASSERT(AsStyleMap(e));
  delete e;

  e = factory->CreateElementById(Type_TimeSpan);
  CPPUNIT_ASSERT(AsTimeSpan(e));
  delete e;

  e = factory->CreateElementById(Type_TimeStamp);
  CPPUNIT_ASSERT(AsTimeStamp(e));
  delete e;

  e = factory->CreateElementById(Type_Update);
  CPPUNIT_ASSERT(AsUpdate(e));
  delete e;

  e = factory->CreateElementById(Type_ViewVolume);
  CPPUNIT_ASSERT(AsViewVolume(e));
  delete e;
}

}  // end namespace kmldom

TEST_MAIN
