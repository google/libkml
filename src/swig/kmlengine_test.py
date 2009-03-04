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
This file contains unit tests for the KML Engine Python SWIG bindings.
"""

import unittest
import kmldom
import kmlengine

# These VerySimple tests really only verify that the function/class exists.

class VerySimpleKmlDomTestCase(unittest.TestCase):
  """ Verify very simple KML DOM usage """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    placemark = factory.CreatePlacemark()
    name = 'hi'
    placemark.set_name(name)
    assert name == placemark.get_name()

class VerySimpleBboxTestCase(unittest.TestCase):
  """ Verify very simple usage of class Bbox """
  def runTest(self):
    bbox = kmlengine.Bbox(38, 36, -120, -122)
    assert bbox.Contains(37, -121)

class AllMethodsBboxTestCase(unittest.TestCase):
  """ Verify basic usage of each method of kmlengine.Bbox """
  def runTest(self):
    bbox = kmlengine.Bbox()
    assert -180.0 == bbox.get_north()
    assert 180.0 == bbox.get_south()
    assert -180.0 == bbox.get_east()
    assert 180.0 == bbox.get_west()

    bbox_a = kmlengine.Bbox(10.1, -11.2, 99.99, -88.88)
    bbox.ExpandFromBbox(bbox_a)
    assert 10.1 == bbox.get_north()
    assert -11.2 == bbox.get_south()
    assert 99.99 == bbox.get_east()
    assert -88.88 == bbox.get_west()

    bbox.ExpandLatitude(20.2)
    assert 20.2 == bbox.get_north()
    assert -11.2 == bbox.get_south()
    bbox.ExpandLatitude(-20.2)
    assert 20.2 == bbox.get_north()
    assert -20.2 == bbox.get_south()
    bbox.ExpandLongitude(101.101)
    assert 101.101 == bbox.get_east()
    assert -88.88 == bbox.get_west()
    bbox.ExpandLongitude(-101.101)
    assert 101.101 == bbox.get_east()
    assert -101.101 == bbox.get_west()

    bbox.ExpandLatLon(30.3, -111.222);
    assert 30.3 == bbox.get_north()
    assert -20.2 == bbox.get_south()
    assert 101.101 == bbox.get_east()
    assert -111.222 == bbox.get_west()

class VerySimpleCloneTestCase(unittest.TestCase):
  """ Verify very simple usage of Clone() """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    placemark = factory.CreatePlacemark()
    name = 'hi'
    placemark.set_name(name)
    clone_element = kmlengine.Clone(placemark)
    assert clone_element
    clone_placemark = kmldom.AsPlacemark(clone_element)
    assert clone_placemark
    assert name == clone_placemark.get_name()
    # Change the name in the original
    new_name = 'new name'
    placemark.set_name(new_name)
    # Verify that the clone still has the old name
    assert name == clone_placemark.get_name()

class VerySimpleGetFeatureLatLonTestCase(unittest.TestCase):
  def runTest(self):
    kml = '<Placemark>' \
            '<Point><coordinates>1,2,3</coordinates></Point>' \
          '</Placemark>'
    element = kmldom.ParseKml(kml)
    assert element
    feature = kmldom.AsFeature(element)
    assert feature
    status,lat,lon = kmlengine.GetFeatureLatLon(feature)
    assert True == status
    assert 1 == lon
    assert 2 == lat

class VerySimpleGetFeatureBoundsTestCase(unittest.TestCase):
  def runTest(self):
    kml = '<Placemark>' \
            '<LineString><coordinates>1,2 -1,-2</coordinates></LineString>' \
          '</Placemark>'
    element = kmldom.ParseKml(kml)
    assert element
    feature = kmldom.AsFeature(element)
    assert feature
    bbox = kmlengine.Bbox()
    status = kmlengine.GetFeatureBounds(feature, bbox)
    assert status
    assert 2 == bbox.get_north()
    assert -2 == bbox.get_south()
    assert 1 == bbox.get_east()
    assert -1 == bbox.get_west()

class VerySimpleGetRootFeatureTestCase(unittest.TestCase):
  """ Verify very simple usage of GetRootFeature() """
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    placemark = factory.CreatePlacemark()
    feature = kmlengine.GetRootFeature(placemark)
    assert feature
    assert kmldom.AsPlacemark(feature)

class VerySimpleKmzSplitTestCase(unittest.TestCase):
  def runTest(self):
    kml_url = 'http://foo.com/goo.kmz/bar.jpg'
    status,kmz_url,kmz_path = kmlengine.KmzSplit(kml_url)
    assert True == status
    assert 'http://foo.com/goo.kmz' == kmz_url
    assert 'bar.jpg' == kmz_path

class VerySimpleSplitUriTestCase(unittest.TestCase):
  def runTest(self):
    status,scheme,host,port,path,query,fragment = \
       kmlengine.SplitUri('http://host.com:82/a/b/c.kml?p=q#id')
    assert True == status
    assert 'http' == scheme
    assert 'host.com' == host
    assert 'a/b/c.kml' == path
    assert 'p=q' == query
    assert 'id' == fragment

class BasicKmlFileCreateFromParseTestCase(unittest.TestCase):
  """ Verify very simple usage of the KmlFile::CreateFromParse() method """
  def runTest(self):
    id = 'pm123'
    kml = '<Placemark id="%s"><name>hello</name></Placemark>' % id
    kmlfile,errors = kmlengine.KmlFile.CreateFromParse(kml)
    assert kmlfile
    object = kmlfile.GetObjectById(id)
    assert object
    placemark = kmldom.AsPlacemark(object)
    assert placemark
    assert placemark.has_id()
    assert id == placemark.get_id()

class KmlFileCreateFromParseOfBasicElementTestCase(unittest.TestCase):
  def runTest(self):
    kmlfile,errors = kmlengine.KmlFile.CreateFromParse('<kml/>')
    assert kmlfile
    root = kmlfile.get_root()
    assert kmldom.Type_kml == root.Type()

class BasicKmlFileCreateFromImportTestCase(unittest.TestCase):
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    placemark = factory.CreatePlacemark()
    id = 'placemark123'
    name = 'some name'
    placemark.set_id(id)
    placemark.set_name(name)
    folder = factory.CreateFolder()
    folder.add_feature(placemark)
    kmlfile = kmlengine.KmlFile.CreateFromImport(folder)
    assert kmlfile
    object = kmlfile.GetObjectById(id)
    assert object
    placemark = kmldom.AsPlacemark(object)
    assert placemark
    assert placemark.has_id()
    assert id == placemark.get_id()
    assert placemark.has_name()
    assert name == placemark.get_name()

class KmlFileCreateFromImportOfBasicElementTestCase(unittest.TestCase):
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    # TODO: This crashes CreateFromImport as do all non-Object complex elements
    # kml = factory.CreateKml()
    # This returns an ElementPtr for the given element and works fine in
    # CreateFromImport:
    kml_as_element = factory.CreateElementById(kmldom.Type_kml)
    kml = kmldom.AsKml(kml_as_element)
    kml.set_feature(factory.CreatePlacemark())
    kmlfile = kmlengine.KmlFile.CreateFromImport(kml_as_element)
    assert kmlfile
    root = kmlfile.get_root()
    assert root
    kml = kmldom.AsKml(root)
    assert kml
    assert kml.has_feature()
    placemark = kmldom.AsPlacemark(kml.get_feature())
    assert placemark

class BasicKmlFileSerializeToStringTestCase(unittest.TestCase):
  def runTest(self):
    factory = kmldom.KmlFactory_GetFactory()
    kml = factory.CreateElementById(kmldom.Type_kml)
    assert kml
    kmlfile = kmlengine.KmlFile.CreateFromImport(kml)
    assert kmlfile
    (ok, xml) = kmlfile.SerializeToString()
    assert ok
    kExpectedXml = '<?xml version="1.0" encoding="utf-8"?>\n' \
        '<kml xmlns="http://www.opengis.net/kml/2.2"/>\n'
    assert kExpectedXml == xml

class BasicKmzFileTestCase(unittest.TestCase):
  def runTest(self):
    kmz_filepath = '../../testdata/kmz/model-macky.kmz'
    kmzfile = kmlengine.KmzFile.OpenFromFile(kmz_filepath)
    assert kmzfile
    (ok, kml) = kmzfile.ReadKml()
    assert ok
    (kmlfile,errors) = kmlengine.KmlFile.CreateFromParse(kml)
    assert kmlfile
    root = kmldom.AsKml(kmlfile.get_root())
    assert root
    placemark = kmldom.AsPlacemark(root.get_feature())
    assert placemark
    assert 'SketchUp Model of Macky Auditorium' == placemark.get_name()
    assert placemark.has_geometry()
    model = kmldom.AsModel(placemark.get_geometry())
    assert model
    assert 'model_4' == model.get_id()
    (ok, dae) = kmzfile.ReadFile('geometry/CU-Macky.dae')
    assert ok
    assert 268477 == len(dae)

def suite():
  suite = unittest.TestSuite()
  suite.addTest(VerySimpleKmlDomTestCase())
  suite.addTest(VerySimpleBboxTestCase())
  suite.addTest(AllMethodsBboxTestCase())
  suite.addTest(VerySimpleCloneTestCase())
  suite.addTest(VerySimpleGetRootFeatureTestCase())
  suite.addTest(VerySimpleGetFeatureLatLonTestCase())
  suite.addTest(VerySimpleGetFeatureBoundsTestCase())
  suite.addTest(VerySimpleKmzSplitTestCase())
  suite.addTest(VerySimpleSplitUriTestCase())
  suite.addTest(KmlFileCreateFromParseOfBasicElementTestCase())
  suite.addTest(BasicKmlFileCreateFromParseTestCase())
  suite.addTest(BasicKmlFileCreateFromImportTestCase())
  suite.addTest(KmlFileCreateFromImportOfBasicElementTestCase())
  suite.addTest(BasicKmlFileSerializeToStringTestCase())
  suite.addTest(BasicKmzFileTestCase())
  return suite


runner = unittest.TextTestRunner()
runner.run(suite())
