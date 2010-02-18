#!/usr/bin/env python

# Copyright 2008, Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#  3. Neither the name of Google Inc. nor the names of its contributors may be
#     used to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""
This file contains unit tests for the KML DOM Python SWIG bindings.
"""

import unittest
import kmlbase
import kmldom


class VerySimpleParseTestCase(unittest.TestCase):
  """ This simply verifies the existence of the binding to ParseKml() """

  def runTest(self):
    assert kmldom.ParseKml('<kml/>')


class VerySimpleSerializeTestCase(unittest.TestCase):
  """ This verifies the existence of the binding to SerializeRaw() """

  def runTest(self):
    kml = '<kml/>'
    assert kml == kmldom.SerializeRaw(kmldom.ParseKml(kml))


class SimpleFactoryTestCase(unittest.TestCase):
  """
  This verifies the existence of the binding to KmlFactory::GetFactory
  and the exitence of bindings for all factory methods.
  """

  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    assert factory.CreateElementById(kmldom.Type_Placemark)
    assert factory.CreateAlias()
    assert factory.CreateAtomAuthor()
    assert factory.CreateAtomLink()
    assert factory.CreateBalloonStyle()
    assert factory.CreateCamera()
    assert factory.CreateChange()
    assert factory.CreateCreate()
    assert factory.CreateData()
    assert factory.CreateDelete()
    assert factory.CreateDocument()
    assert factory.CreateExtendedData()
    assert factory.CreateFolder()
    assert factory.CreateGroundOverlay()
    assert factory.CreateIcon()
    assert factory.CreateIconStyle()
    assert factory.CreateImagePyramid()
    assert factory.CreateItemIcon()
    assert factory.CreateLabelStyle()
    assert factory.CreateLatLonAltBox()
    assert factory.CreateLatLonBox()
    assert factory.CreateLineString()
    assert factory.CreateLineStyle()
    assert factory.CreateLinearRing()
    assert factory.CreateLink()
    assert factory.CreateListStyle()
    assert factory.CreateLocation()
    assert factory.CreateLod()
    assert factory.CreateLookAt()
    assert factory.CreateModel()
    assert factory.CreateMultiGeometry()
    assert factory.CreateNetworkLink()
    assert factory.CreateNetworkLinkControl()
    assert factory.CreateOrientation()
    assert factory.CreatePair()
    assert factory.CreatePhotoOverlay()
    assert factory.CreatePlacemark()
    assert factory.CreatePoint()
    assert factory.CreatePolyStyle()
    assert factory.CreatePolygon()
    assert factory.CreateRegion()
    assert factory.CreateResourceMap()
    assert factory.CreateScale()
    assert factory.CreateSchema()
    assert factory.CreateSchemaData()
    assert factory.CreateScreenOverlay()
    assert factory.CreateSimpleData()
    assert factory.CreateSimpleField()
    assert factory.CreateSnippet()
    assert factory.CreateStyle()
    assert factory.CreateStyleMap()
    assert factory.CreateTimeSpan()
    assert factory.CreateTimeStamp()
    assert factory.CreateUpdate()
    assert factory.CreateViewVolume()
    assert factory.CreateHotSpot()
    assert factory.CreateInnerBoundaryIs()
    assert factory.CreateKml()
    assert factory.CreateLinkSnippet()
    assert factory.CreateOuterBoundaryIs()
    assert factory.CreateOverlayXY()
    assert factory.CreateRotationXY()
    assert factory.CreateScreenXY()
    assert factory.CreateSize()
    assert factory.CreateGxAnimatedUpdate()
    assert factory.CreateGxFlyTo()
    assert factory.CreateGxLatLonQuad()
    assert factory.CreateGxPlaylist()
    assert factory.CreateGxSoundCue()
    assert factory.CreateGxTimeSpan()
    assert factory.CreateGxTimeStamp()
    assert factory.CreateGxTour()
    assert factory.CreateGxTourControl()
    assert factory.CreateGxWait()

class FactoryCreateElementByIdTestCase(unittest.TestCase):
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    assert factory.CreateElementById(kmldom.Type_Alias)
    assert factory.CreateElementById(kmldom.Type_AtomAuthor)
    assert factory.CreateElementById(kmldom.Type_AtomLink)
    assert factory.CreateElementById(kmldom.Type_BalloonStyle)
    assert factory.CreateElementById(kmldom.Type_Camera)
    assert factory.CreateElementById(kmldom.Type_Change)
    assert factory.CreateElementById(kmldom.Type_coordinates)
    assert factory.CreateElementById(kmldom.Type_Create)
    assert factory.CreateElementById(kmldom.Type_Data)
    assert factory.CreateElementById(kmldom.Type_Delete)
    assert factory.CreateElementById(kmldom.Type_Document)
    assert factory.CreateElementById(kmldom.Type_ExtendedData)
    assert factory.CreateElementById(kmldom.Type_Folder)
    assert factory.CreateElementById(kmldom.Type_GroundOverlay)
    assert factory.CreateElementById(kmldom.Type_Icon)
    assert factory.CreateElementById(kmldom.Type_IconStyle)
    assert factory.CreateElementById(kmldom.Type_ImagePyramid)
    assert factory.CreateElementById(kmldom.Type_ItemIcon)
    assert factory.CreateElementById(kmldom.Type_LabelStyle)
    assert factory.CreateElementById(kmldom.Type_LatLonAltBox)
    assert factory.CreateElementById(kmldom.Type_LatLonBox)
    assert factory.CreateElementById(kmldom.Type_LineString)
    assert factory.CreateElementById(kmldom.Type_LineStyle)
    assert factory.CreateElementById(kmldom.Type_LinearRing)
    assert factory.CreateElementById(kmldom.Type_Link)
    assert factory.CreateElementById(kmldom.Type_ListStyle)
    assert factory.CreateElementById(kmldom.Type_Location)
    assert factory.CreateElementById(kmldom.Type_Lod)
    assert factory.CreateElementById(kmldom.Type_LookAt)
    assert factory.CreateElementById(kmldom.Type_Model)
    assert factory.CreateElementById(kmldom.Type_MultiGeometry)
    assert factory.CreateElementById(kmldom.Type_NetworkLink)
    assert factory.CreateElementById(kmldom.Type_NetworkLinkControl)
    assert factory.CreateElementById(kmldom.Type_Orientation)
    assert factory.CreateElementById(kmldom.Type_Pair)
    assert factory.CreateElementById(kmldom.Type_PhotoOverlay)
    assert factory.CreateElementById(kmldom.Type_Placemark)
    assert factory.CreateElementById(kmldom.Type_Point)
    assert factory.CreateElementById(kmldom.Type_PolyStyle)
    assert factory.CreateElementById(kmldom.Type_Polygon)
    assert factory.CreateElementById(kmldom.Type_Region)
    assert factory.CreateElementById(kmldom.Type_ResourceMap)
    assert factory.CreateElementById(kmldom.Type_Scale)
    assert factory.CreateElementById(kmldom.Type_Schema)
    assert factory.CreateElementById(kmldom.Type_SchemaData)
    assert factory.CreateElementById(kmldom.Type_ScreenOverlay)
    assert factory.CreateElementById(kmldom.Type_SimpleData)
    assert factory.CreateElementById(kmldom.Type_SimpleField)
    assert factory.CreateElementById(kmldom.Type_Snippet)
    assert factory.CreateElementById(kmldom.Type_Style)
    assert factory.CreateElementById(kmldom.Type_StyleMap)
    assert factory.CreateElementById(kmldom.Type_TimeSpan)
    assert factory.CreateElementById(kmldom.Type_TimeStamp)
    assert factory.CreateElementById(kmldom.Type_Update)
    assert factory.CreateElementById(kmldom.Type_ViewVolume)
    assert factory.CreateElementById(kmldom.Type_hotSpot)
    assert factory.CreateElementById(kmldom.Type_innerBoundaryIs)
    assert factory.CreateElementById(kmldom.Type_kml)
    assert factory.CreateElementById(kmldom.Type_linkSnippet)
    assert factory.CreateElementById(kmldom.Type_outerBoundaryIs)
    assert factory.CreateElementById(kmldom.Type_overlayXY)
    assert factory.CreateElementById(kmldom.Type_rotationXY)
    assert factory.CreateElementById(kmldom.Type_screenXY)
    assert factory.CreateElementById(kmldom.Type_size)
    assert factory.CreateElementById(kmldom.Type_GxAnimatedUpdate)
    assert factory.CreateElementById(kmldom.Type_GxFlyTo)
    assert factory.CreateElementById(kmldom.Type_GxLatLonQuad)
    assert factory.CreateElementById(kmldom.Type_GxPlaylist)
    assert factory.CreateElementById(kmldom.Type_GxSoundCue)
    assert factory.CreateElementById(kmldom.Type_GxTimeSpan)
    assert factory.CreateElementById(kmldom.Type_GxTimeStamp)
    assert factory.CreateElementById(kmldom.Type_GxTour)
    assert factory.CreateElementById(kmldom.Type_GxTourControl)
    assert factory.CreateElementById(kmldom.Type_GxWait)

class VerySimpleCastTestCase(unittest.TestCase):
  """ This verifies the existence of the bindings to a few of the casts """

  def runTest(self):
    """ kmldom.ParseKml() returns an ElementPtr """
    assert kmldom.AsCoordinates(kmldom.ParseKml('<coordinates/>'))
    assert not kmldom.AsCoordinates(kmldom.ParseKml('<Document/>'))
    assert kmldom.AsFolder(kmldom.ParseKml('<Folder/>'))
    assert not kmldom.AsFolder(kmldom.ParseKml('<Document/>'))
    assert kmldom.AsKml(kmldom.ParseKml('<kml/>'))
    assert not kmldom.AsKml(kmldom.ParseKml('<Placemark/>'))
    assert kmldom.AsPlacemark(kmldom.ParseKml('<Placemark/>'))
    assert not kmldom.AsPlacemark(kmldom.ParseKml('<NetworkLink/>'))
    assert kmldom.AsPoint(kmldom.ParseKml('<Point/>'))
    assert not kmldom.AsPoint(kmldom.ParseKml('<LineString/>'))
    assert kmldom.AsGxAnimatedUpdate(kmldom.ParseKml('<gx:AnimatedUpdate/>'))
    assert kmldom.AsGxFlyTo(kmldom.ParseKml('<gx:FlyTo/>'))
    assert kmldom.AsGxLatLonQuad(kmldom.ParseKml('<gx:LatLonQuad/>'))
    assert kmldom.AsGxPlaylist(kmldom.ParseKml('<gx:Playlist/>'))
    assert kmldom.AsGxSoundCue(kmldom.ParseKml('<gx:SoundCue/>'))
    assert kmldom.AsGxTimeSpan(kmldom.ParseKml('<gx:TimeSpan/>'))
    assert kmldom.AsGxTimeStamp(kmldom.ParseKml('<gx:TimeStamp/>'))
    assert kmldom.AsGxTour(kmldom.ParseKml('<gx:Tour/>'))
    assert kmldom.AsGxTourControl(kmldom.ParseKml('<gx:TourControl/>'))
    assert kmldom.AsGxWait(kmldom.ParseKml('<gx:Wait/>'))


class VerySimpleSimpleChildTestCase(unittest.TestCase):
  """
  This tests that an element derived from Feature can properly set/get/clear
  the simple value <name> element.
  """

  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    folder = factory.CreateFolder()
    assert not folder.has_name()
    name = 'my name'
    folder.set_name(name)
    assert folder.has_name()
    assert name == folder.get_name()
    folder.clear_name()
    assert not folder.has_name()


class VerySimpleComplexChildTestCase(unittest.TestCase):
  """
  This tests that a complex child of a specific type can be set and
  cleared as a child of another element.
  """

  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    region = factory.CreateRegion()
    assert not region.has_lod()
    assert not region.has_latlonaltbox()
    region.set_lod(factory.CreateLod())
    region.set_latlonaltbox(factory.CreateLatLonAltBox())
    assert region.has_lod()
    assert region.has_latlonaltbox()
    region.clear_lod()
    region.clear_latlonaltbox()
    assert not region.has_lod()
    assert not region.has_latlonaltbox()


class SimpleAtomAuthorTestCase(unittest.TestCase):
  """ This tests the AtomAuthor element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.atomauthor = self.factory.CreateAtomAuthor()

  def testDefault(self):
    assert kmldom.Type_AtomAuthor == self.atomauthor.Type()
    assert self.atomauthor.IsA(kmldom.Type_AtomAuthor)
    # default state
    assert not self.atomauthor.has_name()
    assert "" == self.atomauthor.get_name()
    assert not self.atomauthor.has_uri()
    assert "" == self.atomauthor.get_uri()
    assert not self.atomauthor.has_email()
    assert "" == self.atomauthor.get_email()

  def testSetClear(self):
    # name
    name = 'Scott Turow'
    self.atomauthor.set_name(name)
    assert self.atomauthor.has_name()
    assert name == self.atomauthor.get_name()
    self.atomauthor.clear_name()

    # uri
    uri = 'http://www.authorsguild.org/'
    self.atomauthor.set_uri(uri)
    assert self.atomauthor.has_uri()
    assert uri == self.atomauthor.get_uri()
    self.atomauthor.clear_uri()

    # email
    email = 'jsmith@example.com'
    self.atomauthor.set_email(email)
    assert self.atomauthor.has_email()
    assert email == self.atomauthor.get_email()
    self.atomauthor.clear_email()

    self.testDefault()


class SimpleAtomLinkTestCase(unittest.TestCase):
  """ This tests the AtomLink element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.atomlink = self.factory.CreateAtomLink()

  def testDefault(self):
    assert kmldom.Type_AtomLink == self.atomlink.Type()
    assert self.atomlink.IsA(kmldom.Type_AtomLink)
    assert not self.atomlink.has_href()
    assert "" == self.atomlink.get_href()
    assert not self.atomlink.has_rel()
    assert "" == self.atomlink.get_rel()
    assert not self.atomlink.has_type()
    assert "" == self.atomlink.get_type()
    assert not self.atomlink.has_hreflang()
    assert "" == self.atomlink.get_hreflang()
    assert not self.atomlink.has_title()
    assert "" == self.atomlink.get_title()
    assert not self.atomlink.has_length()
    assert 0 == self.atomlink.get_length()

  def testSetClear(self):
    # href
    href = 'http://example.com'
    self.atomlink.set_href(href)
    assert self.atomlink.has_href()
    assert href == self.atomlink.get_href()
    self.atomlink.clear_href()

    rel = 'alternate'
    self.atomlink.set_rel(rel)
    assert self.atomlink.has_rel()
    assert rel == self.atomlink.get_rel()
    self.atomlink.clear_rel()

    type = 'text/html'
    self.atomlink.set_type(type)
    assert self.atomlink.has_type()
    assert type == self.atomlink.get_type()
    self.atomlink.clear_type()

    hreflang = 'en'
    self.atomlink.set_hreflang(hreflang)
    assert self.atomlink.has_hreflang()
    assert hreflang == self.atomlink.get_hreflang()
    self.atomlink.clear_hreflang()

    title = 'dive into mark'
    self.atomlink.set_title(title)
    assert self.atomlink.has_title()
    assert title == self.atomlink.get_title()
    self.atomlink.clear_title()

    length = 42
    self.atomlink.set_length(length)
    assert self.atomlink.has_length()
    assert length == self.atomlink.get_length()
    self.atomlink.clear_length()

    self.testDefault()

  def testParse(self):
    # Straight out of RFC 4287
    link_xml = '<link rel=\"alternate\" type=\"text/html\" '
    link_xml += 'hreflang=\"en\" href=\"http://example.org/\"/>'
    self.atomlink = kmldom.ParseKml(link_xml)
    assert self.atomlink.has_rel()
    assert self.atomlink.has_type()
    assert self.atomlink.has_hreflang()
    assert self.atomlink.has_href()
    assert not self.atomlink.has_title()
    assert not self.atomlink.has_length()

    assert self.atomlink.clear_rel()
    assert self.atomlink.clear_type()
    assert self.atomlink.clear_hreflang()
    assert self.atomlink.clear_href()

    self.testDefault()

class SimpleCoordinatesTestCase(unittest.TestCase):
  """ This tests the methods on Coordinates and Vec3 """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    coordinates = factory.CreateCoordinates()
    # Test the Element methods
    assert kmldom.Type_coordinates == coordinates.Type()
    assert coordinates.IsA(kmldom.Type_coordinates)
    # Test the Coordinates methods
    lon = -123.456
    lat = 38.765
    alt = 1001.909
    assert 0 == coordinates.get_coordinates_array_size()
    coordinates.add_latlng(lat, lon)
    assert 1 == coordinates.get_coordinates_array_size()
    coordinates.add_latlngalt(lat, lon, alt)
    assert 2 == coordinates.get_coordinates_array_size()
    vec0 = coordinates.get_coordinates_array_at(0)
    vec1 = coordinates.get_coordinates_array_at(1)
    # Test the Vec3 methods
    assert lon == vec0.get_longitude()
    assert lat == vec0.get_latitude()
    assert 0 == vec0.get_altitude()
    assert lon == vec1.get_longitude()
    assert lat == vec1.get_latitude()
    assert alt == vec1.get_altitude()


class SimpleVec2TestCase(unittest.TestCase):
  """ This tests the methods on Vec2 (using HotSpot) """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    hotspot = factory.CreateHotSpot()
    # Test the Element methods
    assert kmldom.Type_hotSpot == hotspot.Type()
    assert hotspot.IsA(kmldom.Type_hotSpot)
    # Test the Vec2 methods
    assert not hotspot.has_x()
    assert not hotspot.has_xunits()
    assert not hotspot.has_y()
    assert not hotspot.has_yunits()
    x = 128
    xunits = kmldom.UNITS_PIXELS
    y = .7
    yunits = kmldom.UNITS_FRACTION
    # Test the setters
    hotspot.set_x(x)
    hotspot.set_xunits(xunits)
    hotspot.set_y(y)
    hotspot.set_yunits(yunits)
    # Test the has'ers
    assert hotspot.has_x()
    assert hotspot.has_xunits()
    assert hotspot.has_y()
    assert hotspot.has_yunits()
    # Test the getters
    assert x == hotspot.get_x()
    assert xunits == hotspot.get_xunits()
    assert y == hotspot.get_y()
    assert yunits == hotspot.get_yunits()
    # Test the clear'ers
    hotspot.clear_x()
    hotspot.clear_xunits()
    hotspot.clear_y()
    hotspot.clear_yunits()
    assert not hotspot.has_x()
    assert not hotspot.has_xunits()
    assert not hotspot.has_y()
    assert not hotspot.has_yunits()


class SimpleObjectTestCase(unittest.TestCase):
  """ This tests the Object methods (using Placemark) """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    placemark = factory.CreatePlacemark()
    # Element methods
    assert kmldom.Type_Placemark == placemark.Type()
    assert placemark.IsA(kmldom.Type_Object)
    assert placemark.IsA(kmldom.Type_Placemark)
    # Object methods
    # has_xxx()...
    assert not placemark.has_id()
    assert not placemark.has_targetid()
    id = 'placemark123'
    targetid ='placemark456'
    # set_xxx()...
    placemark.set_id(id)
    placemark.set_targetid(targetid)
    assert placemark.has_id()
    assert placemark.has_targetid()
    # xxx()...
    assert id == placemark.get_id()
    assert targetid == placemark.get_targetid()
    # clear_xxx()...
    placemark.clear_id()
    placemark.clear_targetid()
    assert not placemark.has_id()
    assert not placemark.has_targetid()


class SimpleFeatureTestCase(unittest.TestCase):
  """ This tests the Feature methods (using Folder) """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    folder = factory.CreateFolder()

    # Element methods
    assert kmldom.Type_Folder == folder.Type()
    assert folder.IsA(kmldom.Type_Object)
    assert folder.IsA(kmldom.Type_Feature)
    assert folder.IsA(kmldom.Type_Folder)

    # TODO: Object methods

    # Feature methods
    # TODO everything else

    assert not folder.has_phonenumber()
    phonenumber = '867-5309'
    folder.set_phonenumber(phonenumber)
    assert folder.has_phonenumber()
    assert phonenumber == folder.get_phonenumber()

    assert not folder.has_atomauthor()
    atomauthor = factory.CreateAtomAuthor()
    assert atomauthor
    atom_name = 'D.F. Wallace'
    atomauthor.set_name(atom_name)
    folder.set_atomauthor(atomauthor)
    got_atomauthor = folder.get_atomauthor()
    assert atom_name == got_atomauthor.get_name()

    assert not folder.has_atomlink()
    atomlink = factory.CreateAtomLink()
    assert atomlink
    atom_href = 'http://infinitesummer.org'
    atomlink.set_href(atom_href)
    folder.set_atomlink(atomlink)
    got_atomlink = folder.get_atomlink()
    assert atom_href == got_atomlink.get_href()

    assert not folder.has_snippet()
    folder.set_snippet(factory.CreateSnippet())
    assert folder.has_snippet()
    snippet = folder.get_snippet()
    # Test that the snippet() returns a SnippetPtr
    assert not snippet.has_maxlines()
    folder.clear_snippet()
    assert not folder.has_snippet()

  def testParseAtom(self):
    kml = ('<Placemark>'
           '<atom:author>'
           '<atom:name>D.F. Wallace</atom:name>'
           '</atom:author>'
           '<atom:link href="http://infinitesummer.org"></atom:link>'
           '</Placemark>')
    root = kmldom.ParseKml(kml)
    assert root
    placemark = kmldom.AsPlacemark(root)
    assert placemark
    assert placemark.has_atomauthor()
    author = kmldom.AsAtomAuthor(placemark.get_atomauthor())
    assert author
    assert author.has_name();
    assert "D.F. Wallace" == author.get_name()
    assert placemark.has_atomlink()
    link = kmldom.AsAtomLink(placemark.get_atomlink())
    assert link
    assert link.has_href()
    assert "http://infinitesummer.org" == link.get_href()


class SimpleRegionTestCase(unittest.TestCase):
  """ This tests the methods on Region """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    region = factory.CreateRegion()

    # Element methods
    assert kmldom.Type_Region == region.Type()
    assert region.IsA(kmldom.Type_Object)
    assert region.IsA(kmldom.Type_Region)

    # Object methods
    # has_xxx()...
    assert not region.has_id()
    assert not region.has_targetid()
    id = 'region123'
    targetid ='region456'
    # set_xxx()...
    region.set_id(id)
    region.set_targetid(targetid)
    assert region.has_id()
    assert region.has_targetid()
    # xxx()...
    assert id == region.get_id()
    assert targetid == region.get_targetid()
    # clear_xxx()...
    region.clear_id()
    region.clear_targetid()
    assert not region.has_id()
    assert not region.has_targetid()

    # Region methods
    assert not region.has_lod()
    assert not region.has_latlonaltbox()
    region.set_lod(factory.CreateLod())
    region.set_latlonaltbox(factory.CreateLatLonAltBox())
    assert region.has_lod()
    assert region.has_latlonaltbox()
    region.clear_lod()
    region.clear_latlonaltbox()
    assert not region.has_lod()
    assert not region.has_latlonaltbox()


class SimpleParseTestCase(unittest.TestCase):
  """ This tests the Parse() function."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.root = kmldom.ParseKml('<kml>'
                                '<Placemark id=\"pm123\">'
                                '<name>hi</name>'
                                '</Placemark>'
                                '</kml>')

  def tearDown(self):
    """ KML DOM memory is managed with smart pointers no tear down is needed """

  def testBasic(self):
    assert self.root
    kml = kmldom.AsKml(self.root)
    assert kml
    assert not kml.has_hint()
    assert not kml.has_networklinkcontrol()
    assert kml.has_feature()
    placemark = kmldom.AsPlacemark(kml.get_feature())
    assert placemark
    assert placemark.has_id()
    assert 'pm123' == placemark.get_id()
    assert placemark.has_name()
    assert 'hi' == placemark.get_name()
    assert not placemark.has_geometry()


class SimpleSerializePrettyTestCase(unittest.TestCase):
  """ This tests the SerializePretty() function."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.kml = self.factory.CreateKml()
    self.folder = self.factory.CreateFolder()
    self.folder.set_name('folder')
    self.placemark = self.factory.CreatePlacemark()
    self.placemark.set_name('placemark')
    self.folder.add_feature(self.placemark)
    self.kml.set_feature(self.folder)
    self.xml = kmldom.SerializePretty(self.kml)

  def testBasic(self):
    assert self.xml
    assert '<kml>\n'\
           '  <Folder>\n'\
           '    <name>folder</name>\n'\
           '    <Placemark>\n'\
           '      <name>placemark</name>\n'\
           '    </Placemark>\n'\
           '  </Folder>\n'\
           '</kml>\n' == self.xml


class SimpleSerializeRawTestCase(unittest.TestCase):
  """ This tests the SerializeRaw() function."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.kml = self.factory.CreateKml()
    self.folder = self.factory.CreateFolder()
    self.folder.set_name('folder')
    self.placemark = self.factory.CreatePlacemark()
    self.placemark.set_name('placemark')
    self.folder.add_feature(self.placemark)
    self.kml.set_feature(self.folder)
    self.xml = kmldom.SerializeRaw(self.kml)

  def testBasic(self):
    assert self.xml
    assert '<kml>'\
           '<Folder>'\
           '<name>folder</name>'\
           '<Placemark>'\
           '<name>placemark</name>'\
           '</Placemark>'\
           '</Folder>'\
           '</kml>' == self.xml


class SimpleExtendedDataTestCase(unittest.TestCase):
  """ This tests the ExtendedData element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.extendeddata = self.factory.CreateExtendedData()

  def testDefault(self):
    assert kmldom.AsExtendedData(self.extendeddata)
    assert 0 == self.extendeddata.get_data_array_size()
    assert 0 == self.extendeddata.get_schemadata_array_size()

class DataTestCase(unittest.TestCase):
  """ This tests the Data element """

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.data = self.factory.CreateData()

  def testDefault(self):
    assert kmldom.AsData(self.data)
    assert not self.data.has_name()
    assert "" == self.data.get_name()
    assert not self.data.has_displayname()
    assert "" == self.data.get_displayname()
    assert not self.data.has_value()
    assert "" == self.data.get_value()

class SimpleDataTestCase(unittest.TestCase):
  """ This tests the SimpleData element """

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.simpledata = self.factory.CreateSimpleData()

  def testDefault(self):
    assert kmldom.AsSimpleData(self.simpledata)
    assert not self.simpledata.has_name()
    assert "" == self.simpledata.get_name()
    assert not self.simpledata.has_text()
    assert "" == self.simpledata.get_text()

def TestColorStyle(colorstyle):
  """ This tests the ColorStyle elements of the given ColorStyle-derived
  element"""
  # ColorStyle fields
  # Verify default state of <color>
  assert not colorstyle.has_color()
  # Set <color> to a given value and verify get_ and has_
  color = kmlbase.Color32(0xf0f0c80f)
  colorstyle.set_color(color)
  assert colorstyle.has_color()
  assert color == colorstyle.get_color()
  # Clear <color> and verify has_
  colorstyle.clear_color()
  assert not colorstyle.has_color()

  assert not colorstyle.has_colormode()
  colormode = kmldom.COLORMODE_RANDOM
  colorstyle.set_colormode(colormode)
  assert colorstyle.has_colormode()
  assert colormode == colorstyle.get_colormode()
  colorstyle.clear_colormode()
  assert not colorstyle.has_colormode()

class SimpleIconStyleTestCase(unittest.TestCase):
  """ This tests the IconStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.iconstyle = self.factory.CreateIconStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.iconstyle)
    assert kmldom.AsSubStyle(self.iconstyle)
    assert kmldom.AsColorStyle(self.iconstyle)

    TestColorStyle(self.iconstyle)

class SimpleKmlTestCase(unittest.TestCase):
  """ This tests the Kml element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.kml = self.factory.CreateKml()

  def testDefault(self):
    assert kmldom.Type_kml == self.kml.Type()
    assert self.kml.IsA(kmldom.Type_kml)
    assert not self.kml.has_hint()
    assert "" == self.kml.get_hint()
    assert not self.kml.has_networklinkcontrol()
    assert None == self.kml.get_networklinkcontrol()
    assert not self.kml.has_feature()
    assert None == self.kml.get_feature()

  def testSetClear(self):
    hint = 'target=sky'
    self.kml.set_hint(hint)
    assert self.kml.has_hint()
    assert hint == self.kml.get_hint()
    self.kml.clear_hint()
    self.testDefault()

class SimpleLabelStyleTestCase(unittest.TestCase):
  """ This tests the LabelStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.labelstyle = self.factory.CreateLabelStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.labelstyle)
    assert kmldom.AsSubStyle(self.labelstyle)
    assert kmldom.AsColorStyle(self.labelstyle)

    TestColorStyle(self.labelstyle)

class SimpleLineStyleTestCase(unittest.TestCase):
  """ This tests the LineStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.linestyle = self.factory.CreateLineStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.linestyle)
    assert kmldom.AsSubStyle(self.linestyle)
    assert kmldom.AsColorStyle(self.linestyle)

    TestColorStyle(self.linestyle)

    # LineStyle fields
    # Verify default state of <width>
    assert not self.linestyle.has_width()
    # Set <width> and verify get_ and has_
    width = 4
    self.linestyle.set_width(width)
    assert self.linestyle.has_width()
    assert width == self.linestyle.get_width()
    # Clear <width> and verify has_
    self.linestyle.clear_width()
    assert not self.linestyle.has_width()

class SimpleListStyleTestCase(unittest.TestCase):
  """ This tests the ListStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.liststyle = self.factory.CreateListStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.liststyle)
    assert kmldom.AsSubStyle(self.liststyle)
    assert kmldom.AsListStyle(self.liststyle)
    assert not self.liststyle.has_id()
    assert 0 == self.liststyle.get_itemicon_array_size()

class SimpleNetworkLinkControlTestCase(unittest.TestCase):
  """ This tests the NetworkLinkControl element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.nlc = self.factory.CreateNetworkLinkControl()

  def testDefault(self):
    assert kmldom.Type_NetworkLinkControl == self.nlc.Type()
    assert self.nlc.IsA(kmldom.Type_NetworkLinkControl)
    assert not self.nlc.has_minrefreshperiod()
    assert not self.nlc.has_maxsessionlength()
    assert not self.nlc.has_cookie()
    assert not self.nlc.has_message()
    assert not self.nlc.has_linkname()
    assert not self.nlc.has_linkdescription()
    assert not self.nlc.has_linksnippet()
    assert not self.nlc.has_expires()
    assert not self.nlc.has_update()
    assert not self.nlc.has_abstractview()

  def testSetClear(self):
    minrefreshperiod = 42.123
    self.nlc.set_minrefreshperiod(minrefreshperiod)
    assert self.nlc.has_minrefreshperiod()
    assert minrefreshperiod == self.nlc.get_minrefreshperiod()
    self.nlc.clear_minrefreshperiod()
    assert not self.nlc.has_minrefreshperiod()

class SimplePhotoOverlayTestCase(unittest.TestCase):
  """ This tests the PhotoOverlay element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.photooverlay = self.factory.CreatePhotoOverlay()

  def testDefault(self):
    assert kmldom.AsObject(self.photooverlay)
    assert kmldom.AsFeature(self.photooverlay)
    assert kmldom.AsOverlay(self.photooverlay)
    assert kmldom.AsPhotoOverlay(self.photooverlay)
    assert not self.photooverlay.has_id()  # Object-ness
    assert not self.photooverlay.has_name()  # Feature-ness
    assert not self.photooverlay.has_icon()  # Overlay-ness
    assert not self.photooverlay.has_rotation()  # PhotoOverlay-ness...
    assert not self.photooverlay.has_viewvolume()
    assert not self.photooverlay.has_imagepyramid()
    assert not self.photooverlay.has_point()
    assert not self.photooverlay.has_shape()


class SimplePlacemarkTestCase(unittest.TestCase):
  """ This tests the Placemark element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.placemark = self.factory.CreatePlacemark()

  def testDefault(self):
    assert kmldom.AsObject(self.placemark)
    assert kmldom.AsFeature(self.placemark)
    assert kmldom.AsPlacemark(self.placemark)
    assert not self.placemark.has_id()  # Object-ness
    assert not self.placemark.has_name()  # Feature-ness
    assert not self.placemark.has_geometry()  # Placemark-ness

  def testName(self):
    name = 'This is my name'
    self.placemark.set_name(name)
    assert self.placemark.has_name()
    assert name == self.placemark.get_name()
    self.placemark.clear_name()
    assert not self.placemark.has_name()


class SimpleLatLonBoxTestCase(unittest.TestCase):
  """ This tests the LatLonBox element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.latlonbox = self.factory.CreateLatLonBox()

  def testDefault(self):
    assert kmldom.AsObject(self.latlonbox)
    assert kmldom.AsLatLonBox(self.latlonbox)
    assert not self.latlonbox.has_id()  # Object-ness
    assert not self.latlonbox.has_north()
    assert 180.0 == self.latlonbox.get_north()
    assert not self.latlonbox.has_south()
    assert -180.0 == self.latlonbox.get_south()
    assert not self.latlonbox.has_east()
    assert 180.0 == self.latlonbox.get_east()
    assert not self.latlonbox.has_west()
    assert -180.0 == self.latlonbox.get_west()

  def testSetClear(self):
    north = 42.42
    south = -11.12
    east = -80.80
    west = -123.321
    self.latlonbox.set_north(north)
    self.latlonbox.set_south(south)
    self.latlonbox.set_east(east)
    self.latlonbox.set_west(west)
    assert self.latlonbox.has_north()
    assert north == self.latlonbox.get_north()
    assert self.latlonbox.has_south()
    assert south == self.latlonbox.get_south()
    assert self.latlonbox.has_east()
    assert east == self.latlonbox.get_east()
    assert self.latlonbox.has_west()
    assert west == self.latlonbox.get_west()
    self.latlonbox.clear_north()
    self.latlonbox.clear_south()
    self.latlonbox.clear_east()
    self.latlonbox.clear_west()
    # Clearing everything returns to default state
    self.testDefault()


class SimpleLinkTestCase(unittest.TestCase):
  """ This tests the Link element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.link = self.factory.CreateLink()

  def testDefault(self):
    assert kmldom.AsObject(self.link)
    assert kmldom.AsLink(self.link)
    assert not self.link.has_id()  # Object-ness
    assert not self.link.has_href()  # Link-ness

  def testSetClear(self):
    href = 'http://example.com/cool.kml'
    self.link.set_href(href)
    assert self.link.has_href()
    assert href == self.link.get_href()
    self.link.clear_href()
    # Clearing everything returns to default state
    self.testDefault()


class SimpleChangeTestCase(unittest.TestCase):
  """ This tests the Change element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.change = self.factory.CreateChange()

  def testDefault(self):
    assert kmldom.AsChange(self.change)
    assert 0 == self.change.get_object_array_size()

  def testAddOneObject(self):
    placemark = self.factory.CreatePlacemark()
    name = 'hi'
    placemark.set_name(name)
    self.change.add_object(placemark)
    assert 1 == self.change.get_object_array_size()
    placemark = kmldom.AsPlacemark(self.change.get_object_array_at(0))
    assert name == placemark.get_name()

  def testAddNObjects(self):
    placemark = self.factory.CreatePlacemark()
    placemark_id = 'pm123'
    placemark.set_id(placemark_id)
    self.change.add_object(placemark)

    latlonbox = self.factory.CreateLatLonBox()
    latlonbox_id = 'llb123'
    latlonbox.set_id(latlonbox_id)
    self.change.add_object(latlonbox)

    location = self.factory.CreateLocation()
    location_id = 'location123'
    location.set_id(location_id)
    self.change.add_object(location)

    """ Verify there are 3 items in the array in the proper order."""
    assert 3 == self.change.get_object_array_size()
    assert placemark_id == self.change.get_object_array_at(0).get_id()
    assert latlonbox_id == self.change.get_object_array_at(1).get_id()
    assert location_id == self.change.get_object_array_at(2).get_id()


class SimpleCreateTestCase(unittest.TestCase):
  """ This tests the Create element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.create = self.factory.CreateCreate()

  def testDefault(self):
    assert kmldom.AsCreate(self.create)
    assert 0 == self.create.get_container_array_size()

  def testAddOneContainer(self):
    folder = self.factory.CreateFolder()
    target_folder_id = 'target-folder'
    folder.set_targetid(target_folder_id)
    self.create.add_container(folder)
    assert 1 == self.create.get_container_array_size()
    folder = kmldom.AsFolder(self.create.get_container_array_at(0))
    assert target_folder_id == folder.get_targetid()

  def testAddNContainers(self):
    folder = self.factory.CreateFolder()
    folder_a_id = 'folder-a'
    folder.set_id(folder_a_id)
    self.create.add_container(folder)

    document = self.factory.CreateFolder()
    document_a_id = 'container-a'
    document.set_id(document_a_id)
    self.create.add_container(document)

    document = self.factory.CreateFolder()
    document_b_id = 'container-b'
    document.set_id(document_b_id)
    self.create.add_container(document)

    folder = self.factory.CreateFolder()
    folder_b_id = 'folder-b'
    folder.set_id(folder_b_id)
    self.create.add_container(folder)

    # Verify there are 4 items in the array in the proper order.
    assert 4 == self.create.get_container_array_size()
    assert folder_a_id == self.create.get_container_array_at(0).get_id()
    assert document_a_id == self.create.get_container_array_at(1).get_id()
    assert document_b_id == self.create.get_container_array_at(2).get_id()
    assert folder_b_id == self.create.get_container_array_at(3).get_id()


class SimpleDeleteTestCase(unittest.TestCase):
  """ This tests the Delete element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.delete = self.factory.CreateDelete()

  def testDefault(self):
    assert kmldom.AsDelete(self.delete)
    assert 0 == self.delete.get_feature_array_size()

  def testAddOneFeature(self):
    groundoverlay = self.factory.CreateGroundOverlay()
    target_groundoverlay_id = 'target-groundoverlay'
    groundoverlay.set_targetid(target_groundoverlay_id)
    self.delete.add_feature(groundoverlay)
    assert 1 == self.delete.get_feature_array_size()
    groundoverlay = kmldom.AsGroundOverlay(self.delete.get_feature_array_at(0))
    assert target_groundoverlay_id == groundoverlay.get_targetid()

  def testAddNFeatures(self):
    document = self.factory.CreateDocument()
    document_id = 'document-id'
    document.set_id(document_id)
    self.delete.add_feature(document)

    folder = self.factory.CreateFolder()
    folder_id = 'folder-id'
    folder.set_id(folder_id)
    self.delete.add_feature(folder)

    groundoverlay = self.factory.CreateGroundOverlay()
    groundoverlay_id = 'groundoverlay-id'
    groundoverlay.set_id(groundoverlay_id)
    self.delete.add_feature(groundoverlay)

    networklink = self.factory.CreateNetworkLink()
    networklink_id = 'networklink-id'
    networklink.set_id(networklink_id)
    self.delete.add_feature(networklink)

    placemark = self.factory.CreatePlacemark()
    placemark_id = 'placemark-id'
    placemark.set_id(placemark_id)
    self.delete.add_feature(placemark)

    photooverlay = self.factory.CreatePhotoOverlay()
    photooverlay_id = 'photooverlay-id'
    photooverlay.set_id(photooverlay_id)
    self.delete.add_feature(photooverlay)

    screenoverlay = self.factory.CreateScreenOverlay()
    screenoverlay_id = 'screenoverlay-id'
    screenoverlay.set_id(screenoverlay_id)
    self.delete.add_feature(screenoverlay)

    # Verify there are 7 items in the array in the proper order.
    assert 7 == self.delete.get_feature_array_size()
    assert document_id == self.delete.get_feature_array_at(0).get_id()
    assert folder_id == self.delete.get_feature_array_at(1).get_id()
    assert groundoverlay_id == self.delete.get_feature_array_at(2).get_id()
    assert networklink_id == self.delete.get_feature_array_at(3).get_id()
    assert placemark_id == self.delete.get_feature_array_at(4).get_id()
    assert photooverlay_id == self.delete.get_feature_array_at(5).get_id()
    assert screenoverlay_id == self.delete.get_feature_array_at(6).get_id()


class SimpleDocumentTestCase(unittest.TestCase):
  """ This tests the Document element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.document = self.factory.CreateDocument()

  def testDefault(self):
    assert kmldom.AsObject(self.document)
    assert kmldom.AsFeature(self.document)
    assert kmldom.AsContainer(self.document)
    assert kmldom.AsDocument(self.document)
    assert not self.document.has_id()  # Object-ness
    assert not self.document.has_name()  # Feature-ness
    assert 0 == self.document.get_feature_array_size()  # Container-ness
    assert 0 == self.document.get_schema_array_size()  # Document-ness
    assert 0 == self.document.get_styleselector_array_size()  # Document-ness


class SimpleMultiGeometryTestCase(unittest.TestCase):
  """  This tests the MultiGeometry element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.multigeometry = self.factory.CreateMultiGeometry()

  def testDefault(self):
    assert kmldom.AsObject(self.multigeometry)
    assert kmldom.AsGeometry(self.multigeometry)
    assert kmldom.AsMultiGeometry(self.multigeometry)
    assert not self.multigeometry.has_id()  # Object-ness
    # MultiGeometry-ness
    assert 0 == self.multigeometry.get_geometry_array_size()


class SimpleOuterBoundaryIsTestCase(unittest.TestCase):
  """  TODO: This tests the OuterBoundaryIs element."""


class SimpleInnerBoundaryIsTestCase(unittest.TestCase):
  """  TODO: This tests the InnerBoundaryIs element."""


class SimplePolygonTestCase(unittest.TestCase):
  """  TODO: This tests the Polygon element."""

class SimplePolyStyleTestCase(unittest.TestCase):
  """ This tests the PolyStyle element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.polystyle = self.factory.CreatePolyStyle()

  def testDefault(self):
    assert kmldom.AsObject(self.polystyle)
    assert kmldom.AsSubStyle(self.polystyle)
    assert kmldom.AsColorStyle(self.polystyle)

    TestColorStyle(self.polystyle)

    # PolyStyle fields
    # Verify default state of <fill>
    assert not self.polystyle.has_fill()
    # Set <fill> and verify get_ and has_
    fill = True
    self.polystyle.set_fill(fill)
    assert self.polystyle.has_fill()
    assert fill == self.polystyle.get_fill()
    # Clear <fill> and verify has_
    self.polystyle.clear_fill()
    assert not self.polystyle.has_fill()

    # Verify default state of <outline>
    assert not self.polystyle.has_outline()
    # Set <outline> and verify get_ and has_
    outline = True
    self.polystyle.set_outline(outline)
    assert self.polystyle.has_outline()
    assert outline == self.polystyle.get_outline()
    # Clear <outline> and verify has_
    self.polystyle.clear_outline()
    assert not self.polystyle.has_outline()

class SimpleResourceMapTestCase(unittest.TestCase):
  """ This tests the ResourceMap element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.resourcemap = self.factory.CreateResourceMap()

  def testDefault(self):
    assert kmldom.AsObject(self.resourcemap)
    assert kmldom.AsResourceMap(self.resourcemap)
    assert not self.resourcemap.has_id()
    assert 0 == self.resourcemap.get_alias_array_size()


class SimpleSchemaTestCase(unittest.TestCase):
  """ This tests the Schema element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.schema = self.factory.CreateSchema()

  def testDefault(self):
    assert kmldom.AsSchema(self.schema)
    assert not self.schema.has_name()
    assert not self.schema.has_id()
    assert 0 == self.schema.get_simplefield_array_size()


class SimpleSchemaDataTestCase(unittest.TestCase):
  """ This tests the SchemaData element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.schemadata = self.factory.CreateSchemaData()

  def testDefault(self):
    assert kmldom.AsObject(self.schemadata)
    assert kmldom.AsSchemaData(self.schemadata)
    assert not self.schemadata.has_schemaurl()
    assert "" == self.schemadata.get_schemaurl()
    assert not self.schemadata.has_id()
    assert 0 == self.schemadata.get_simpledata_array_size()


class SimpleSimpleFieldTestCase(unittest.TestCase):
  """ This tests the SimpleField element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.simplefield = self.factory.CreateSimpleField()

  def testDefault(self):
    assert kmldom.AsSimpleField(self.simplefield)
    assert not self.simplefield.has_type()
    assert not self.simplefield.has_name()
    assert not self.simplefield.has_displayname()


class SimpleStyleMapTestCase(unittest.TestCase):
  """  TODO: This tests the StyleMap element."""


class SimpleUpdateTestCase(unittest.TestCase):
  """ This tests the Update element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.update = self.factory.CreateUpdate()

  def testDefault(self):
    assert kmldom.AsUpdate(self.update)
    assert 0 == self.update.get_updateoperation_array_size()

  def testTargetHref(self):
    assert not self.update.has_targethref()
    assert '' == self.update.get_targethref()
    href = 'mycoolhref'
    self.update.set_targethref(href)
    assert self.update.has_targethref()
    assert href == self.update.get_targethref()

class SimpleStyleMapTestCase(unittest.TestCase):
  """  TODO: This tests the StyleMap element."""

class SimpleIconStyleIconTestCase(unittest.TestCase):
  """ This tests the IconStyleIcon element."""

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.iconstyleicon= self.factory.CreateIconStyleIcon()

  def testBasic(self):
    assert not self.iconstyleicon.has_href() # default state
    assert '' == self.iconstyleicon.get_href()
    href = 'http://blah.blah'
    self.iconstyleicon.set_href(href)
    assert self.iconstyleicon.has_href()
    assert href == self.iconstyleicon.get_href()
    self.iconstyleicon.clear_href()
    assert not self.iconstyleicon.has_href() # back in default state
    assert '' == self.iconstyleicon.get_href()

class GxExtensionsTestCase(unittest.TestCase):
  """ This tests the gx: elements extended by Google Earth 5.0
      These are basic subsets of the full suite of tests in the main
      C++ code and verify the basic operation of the API """

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()

  # Feature's gx:balloonVisibility.
  def testGxBalloonVisibility(self):
    placemark = self.factory.CreatePlacemark()
    assert not placemark.has_gx_balloonvisibility()
    assert False == placemark.get_gx_balloonvisibility()
    placemark.set_gx_balloonvisibility(True)
    assert placemark.has_gx_balloonvisibility()
    assert True == placemark.get_gx_balloonvisibility()
    expected = (
        '<Placemark>\n'
        '  <gx:balloonVisibility>1</gx:balloonVisibility>\n'
        '</Placemark>\n')
    assert expected == kmldom.SerializePretty(placemark)

  # <gx:TimeStamp> and <gx:TimeSpan> in AbstractView.
  def testGxTimePrimitives(self):
    lookat = self.factory.CreateLookAt()
    assert not lookat.has_gx_timeprimitive()
    assert None == lookat.get_gx_timeprimitive()
    timespan = self.factory.CreateGxTimeSpan()
    lookat.set_gx_timeprimitive(timespan)
    assert lookat.has_gx_timeprimitive()
    expected_lookat = (
        '<LookAt>\n'
        '  <gx:TimeSpan/>\n'
        '</LookAt>\n')
    assert expected_lookat == kmldom.SerializePretty(lookat)

    camera = self.factory.CreateCamera()
    assert not camera.has_gx_timeprimitive()
    assert None == camera.get_gx_timeprimitive()
    timestamp = self.factory.CreateGxTimeStamp()
    camera.set_gx_timeprimitive(timestamp)
    assert camera.has_gx_timeprimitive()
    expected_camera = (
        '<Camera>\n'
        '  <gx:TimeStamp/>\n'
        '</Camera>\n')
    assert expected_camera == kmldom.SerializePretty(camera)

  # <gx:altitudeMode>clampToSeaFloor|relativeToSeaFloor.
  def testGxAltitudeModes(self):
    llab = self.factory.CreateLatLonAltBox()
    assert not llab.has_gx_altitudemode()
    assert kmldom.GX_ALTITUDEMODE_CLAMPTOSEAFLOOR == llab.get_gx_altitudemode()
    altmode_rel = kmldom.GX_ALTITUDEMODE_RELATIVETOSEAFLOOR
    llab.set_gx_altitudemode(altmode_rel)
    assert llab.has_gx_altitudemode()
    assert altmode_rel == llab.get_gx_altitudemode()
    expected = (
        '<LatLonAltBox>\n'
        '  <gx:altitudeMode>relativeToSeaFloor</gx:altitudeMode>\n'
        '</LatLonAltBox>\n')
    assert expected == kmldom.SerializePretty(llab)
    # TODO: Camera, GroundOverlay, LineString, LineStyle, LinearRing, LookAt,
    #       Model, Point, Polygon.

  # <gx:LatLonQuad>
  def testGxLatLonQuad(self):
    groundoverlay = self.factory.CreateGroundOverlay()
    assert False == groundoverlay.has_gx_latlonquad()
    assert None == groundoverlay.get_gx_latlonquad()
    llq = self.factory.CreateGxLatLonQuad()
    coords = self.factory.CreateCoordinates()
    llq.set_coordinates(coords)
    groundoverlay.set_gx_latlonquad(llq)
    assert True == groundoverlay.has_gx_latlonquad()
    expected = (
        '<GroundOverlay>\n'
        '  <gx:LatLonQuad>\n'
        '    <coordinates/>\n'
        '  </gx:LatLonQuad>\n'
        '</GroundOverlay>\n')
    assert expected == kmldom.SerializePretty(groundoverlay)

  # <gx:Tour>
  def testGxTour(self):
    tour = self.factory.CreateGxTour()
    assert not tour.has_gx_playlist()
    assert None == tour.get_gx_playlist()
    tour.set_gx_playlist(self.factory.CreateGxPlaylist())
    assert tour.has_gx_playlist()

  # <gx:Playlist>
  def testGxPlaylist(self):
    playlist = self.factory.CreateGxPlaylist()
    assert 0 == playlist.get_gx_tourprimitive_array_size()
    playlist.add_gx_tourprimitive(self.factory.CreateGxFlyTo())
    assert 1 == playlist.get_gx_tourprimitive_array_size()

  # <gx:AnimatedUpdate>
  def testGxAnimatedUpdate(self):
    animatedupdate = self.factory.CreateGxAnimatedUpdate()
    assert False == animatedupdate.has_gx_duration()
    assert 0.0 == animatedupdate.get_gx_duration()
    animatedupdate.set_gx_duration(1.0)
    assert animatedupdate.has_gx_duration()
    assert 1.0 == animatedupdate.get_gx_duration()

    assert False == animatedupdate.has_update()
    assert None == animatedupdate.get_update()
    animatedupdate.set_update(self.factory.CreateUpdate())
    assert animatedupdate.has_update()

  # <gx:FlyTo>
  def testGxFlyTo(self):
    flyto = self.factory.CreateGxFlyTo()
    assert False == flyto.has_gx_duration()
    assert 0.0 == flyto.get_gx_duration()
    flyto.set_gx_duration(1.0)
    assert flyto.has_gx_duration()
    assert 1.0 == flyto.get_gx_duration()

    assert False == flyto.has_gx_flytomode()
    assert kmldom.GX_FLYTOMODE_BOUNCE == flyto.get_gx_flytomode()
    flyto.set_gx_flytomode(kmldom.GX_FLYTOMODE_SMOOTH)
    assert flyto.has_gx_flytomode()
    assert kmldom.GX_FLYTOMODE_SMOOTH == flyto.get_gx_flytomode()

  # <gx:Wait>
  def testGxWait(self):
    wait = self.factory.CreateGxWait()
    assert False == wait.has_gx_duration()
    assert 0.0 == wait.get_gx_duration()
    wait.set_gx_duration(1.0)
    assert wait.has_gx_duration()
    assert 1.0 == wait.get_gx_duration()

  # <gx:SoundCue>
  def testGxSoundCue(self):
    soundcue = self.factory.CreateGxSoundCue()
    assert False == soundcue.has_href()
    assert '' == soundcue.get_href()
    href = 'some href'
    soundcue.set_href(href)
    assert soundcue.has_href()
    assert href == soundcue.get_href()

  # <gx:TourControl>
  def testGxTourControl(self):
    tourcontrol = self.factory.CreateGxTourControl()
    assert False == tourcontrol.has_gx_playmode()
    assert kmldom.GX_PLAYMODE_PAUSE == tourcontrol.get_gx_playmode()
    tourcontrol.set_gx_playmode(kmldom.GX_PLAYMODE_PAUSE)
    assert tourcontrol.has_gx_playmode()
    assert kmldom.GX_PLAYMODE_PAUSE == tourcontrol.get_gx_playmode()

  # Parse and serialization of gx:Tour and friends.
  def testGxTourParseSerialize(self):
    kml = (
        '<gx:Tour>'
        '<gx:Playlist>'
        '<gx:SoundCue><href>x</href></gx:SoundCue>'
        '<gx:FlyTo>'
        '<gx:duration>1.0</gx:duration>'
        '</gx:FlyTo>'
        '<gx:AnimatedUpdate>'
        '<gx:duration>2.0</gx:duration>'
        '<Update/>'
        '</gx:AnimatedUpdate>'
        '<gx:TourControl><gx:playMode>pause</gx:playMode></gx:TourControl>'
        '<gx:Wait><gx:duration>3.0</gx:duration></gx:Wait>'
        '</gx:Playlist>'
        '</gx:Tour>')
    assert kmldom.ParseKml(kml)
    assert kmldom.AsGxTour(kmldom.ParseKml(kml))
    tour = kmldom.AsGxTour(kmldom.ParseKml(kml))
    assert tour.has_gx_playlist()
    playlist = tour.get_gx_playlist()
    assert 5 == playlist.get_gx_tourprimitive_array_size()
    soundcue = kmldom.AsGxSoundCue(playlist.get_gx_tourprimitive_array_at(0))
    assert soundcue
    assert soundcue.has_href()
    assert 'x' == soundcue.get_href()
    flyto = kmldom.AsGxFlyTo(playlist.get_gx_tourprimitive_array_at(1))
    assert flyto
    assert flyto.has_gx_duration()
    assert 1.0 == flyto.get_gx_duration()
    au = kmldom.AsGxAnimatedUpdate(playlist.get_gx_tourprimitive_array_at(2))
    assert au
    assert au.has_gx_duration()
    assert 2.0 == au.get_gx_duration()
    assert au.has_update()
    assert kmldom.AsUpdate(au.get_update())
    tc = kmldom.AsGxTourControl(playlist.get_gx_tourprimitive_array_at(3))
    assert tc
    assert tc.has_gx_playmode()
    assert kmldom.GX_PLAYMODE_PAUSE == tc.get_gx_playmode()
    wait = kmldom.AsGxWait(playlist.get_gx_tourprimitive_array_at(4))
    assert wait
    assert wait.has_gx_duration()
    assert 3.0 == wait.get_gx_duration()

class ItemIconTestCase(unittest.TestCase):
  """ This verifies the behavior of ItemIcon and its children """

  def setUp(self):
    self.factory = kmldom.KmlFactory_GetFactory()
    self.itemicon = self.factory.CreateItemIcon()

  def testDefaults(self):
    assert not self.itemicon.has_state()
    assert 1 == self.itemicon.get_state_array_size()
    assert kmldom.ITEMICONSTATE_OPEN == self.itemicon.get_state_array_at(0)
    assert not self.itemicon.has_href()
    assert "" == self.itemicon.get_href()

  def testState(self):
    state_open = kmldom.ITEMICONSTATE_OPEN
    state_error = kmldom.ITEMICONSTATE_ERROR
    state_fetching0 = kmldom.ITEMICONSTATE_FETCHING0

    self.itemicon.clear_state()
    assert 0 == self.itemicon.get_state_array_size()
    self.itemicon.add_state(state_error)
    assert 1 == self.itemicon.get_state_array_size()
    assert state_error == self.itemicon.get_state_array_at(0)
    self.itemicon.add_state(state_fetching0)
    assert 2 == self.itemicon.get_state_array_size()
    assert state_fetching0 == self.itemicon.get_state_array_at(1)


def suite():
  suite = unittest.TestSuite()
  suite.addTest(VerySimpleParseTestCase())
  suite.addTest(VerySimpleSerializeTestCase())
  suite.addTest(SimpleFactoryTestCase())
  suite.addTest(FactoryCreateElementByIdTestCase())
  suite.addTest(VerySimpleCastTestCase())
  suite.addTest(VerySimpleSimpleChildTestCase())
  suite.addTest(VerySimpleComplexChildTestCase())
  suite.addTest(SimpleAtomAuthorTestCase('testDefault'))
  suite.addTest(SimpleAtomAuthorTestCase('testSetClear'))
  suite.addTest(SimpleAtomLinkTestCase('testDefault'))
  suite.addTest(SimpleAtomLinkTestCase('testSetClear'))
  suite.addTest(SimpleCoordinatesTestCase())
  suite.addTest(SimpleVec2TestCase())
  suite.addTest(SimpleObjectTestCase())
  suite.addTest(SimpleFeatureTestCase())
  suite.addTest(SimpleFeatureTestCase('testParseAtom'))
  suite.addTest(SimpleRegionTestCase())
  suite.addTest(SimpleParseTestCase('testBasic'))
  suite.addTest(SimpleSerializePrettyTestCase('testBasic'))
  suite.addTest(SimpleSerializeRawTestCase('testBasic'))
  suite.addTest(SimpleExtendedDataTestCase('testDefault'))
  suite.addTest(DataTestCase('testDefault'))
  suite.addTest(SimpleDataTestCase('testDefault'))
  suite.addTest(SimpleIconStyleTestCase('testDefault'))
  suite.addTest(SimpleKmlTestCase('testDefault'))
  suite.addTest(SimpleKmlTestCase('testSetClear'))
  suite.addTest(SimpleNetworkLinkControlTestCase('testDefault'))
  suite.addTest(SimpleNetworkLinkControlTestCase('testSetClear'))
  suite.addTest(SimpleLabelStyleTestCase('testDefault'))
  suite.addTest(SimpleLineStyleTestCase('testDefault'))
  suite.addTest(SimpleListStyleTestCase('testDefault'))
  suite.addTest(SimplePhotoOverlayTestCase('testDefault'))
  suite.addTest(SimplePlacemarkTestCase('testDefault'))
  suite.addTest(SimplePlacemarkTestCase('testName'))
  suite.addTest(SimplePolyStyleTestCase('testDefault'))
  suite.addTest(SimpleLatLonBoxTestCase('testDefault'))
  suite.addTest(SimpleLatLonBoxTestCase('testSetClear'))
  suite.addTest(SimpleLinkTestCase('testDefault'))
  suite.addTest(SimpleLinkTestCase('testSetClear'))
  suite.addTest(SimpleChangeTestCase('testDefault'))
  suite.addTest(SimpleChangeTestCase('testAddOneObject'))
  suite.addTest(SimpleChangeTestCase('testAddNObjects'))
  suite.addTest(SimpleCreateTestCase('testDefault'))
  suite.addTest(SimpleCreateTestCase('testAddOneContainer'))
  suite.addTest(SimpleCreateTestCase('testAddNContainers'))
  suite.addTest(SimpleDeleteTestCase('testAddOneFeature'))
  suite.addTest(SimpleDeleteTestCase('testAddNFeatures'))
  suite.addTest(SimpleDocumentTestCase('testDefault'))
  suite.addTest(SimpleResourceMapTestCase('testDefault'))
  suite.addTest(SimpleMultiGeometryTestCase('testDefault'))
  suite.addTest(SimpleSchemaTestCase('testDefault'))
  suite.addTest(SimpleSchemaDataTestCase('testDefault'))
  suite.addTest(SimpleSimpleFieldTestCase('testDefault'))
  suite.addTest(SimpleUpdateTestCase('testDefault'))
  suite.addTest(SimpleUpdateTestCase('testTargetHref'))
  suite.addTest(SimpleIconStyleIconTestCase('testBasic'))
  suite.addTest(GxExtensionsTestCase('testGxBalloonVisibility'))
  suite.addTest(GxExtensionsTestCase('testGxTimePrimitives'))
  suite.addTest(GxExtensionsTestCase('testGxAltitudeModes'))
  suite.addTest(GxExtensionsTestCase('testGxLatLonQuad'))
  suite.addTest(GxExtensionsTestCase('testGxTour'))
  suite.addTest(GxExtensionsTestCase('testGxPlaylist'))
  suite.addTest(GxExtensionsTestCase('testGxAnimatedUpdate'))
  suite.addTest(GxExtensionsTestCase('testGxFlyTo'))
  suite.addTest(GxExtensionsTestCase('testGxWait'))
  suite.addTest(GxExtensionsTestCase('testGxSoundCue'))
  suite.addTest(GxExtensionsTestCase('testGxTourControl'))
  suite.addTest(GxExtensionsTestCase('testGxTourParseSerialize'))
  suite.addTest(ItemIconTestCase('testDefaults'))
  suite.addTest(ItemIconTestCase('testState'))
  return suite


runner = unittest.TextTestRunner()
runner.run(suite())
