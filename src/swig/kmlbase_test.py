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
This file contains unit tests for the KML Base Python SWIG bindings.
"""

import unittest
import kmlbase

# These VerySimple tests really only verify that the function/class exists.
# Based on kml/base/color32_test.cc.

class BasicAttributesTestCase(unittest.TestCase):
  def runTest(self):
    attributes = kmlbase.Attributes()
    assert 0 == attributes.GetSize()
    found,value = attributes.FindValue("key")
    assert not found
    found,key = attributes.FindKey("value")
    assert not found

class BasicColor32TestCase(unittest.TestCase):
  """Verify very simple Color32 usage """
  def runTest(self):
    # Default construction.
    color = kmlbase.Color32()
    assert 0xffffffff == color.get_color_abgr()

    # Construction from specific color.
    red = 0xff0000ff
    color = kmlbase.Color32(red)
    assert red == color.get_color_abgr()

    # Construction from RGBA chars.
    A = 0xff
    B = 0x00
    G = 0x00
    R = 0xff
    color = kmlbase.Color32(A, B, G, R)
    assert red == color.get_color_abgr()

    # Construction from string.
    red_str = "ff0000ff"
    color = kmlbase.Color32(red_str)
    assert red == color.get_color_abgr()

    # Bad construction.
    nonsense = "This isn't even close to a color value"
    color = kmlbase.Color32(nonsense)
    assert "00000000" == color.to_string_abgr()

    # Getters in default state.
    color = kmlbase.Color32()
    kFF = 0xff

    assert kFF == color.get_alpha()
    assert kFF == color.get_blue()
    assert kFF == color.get_green()
    assert kFF == color.get_red()

    # Verify getters of newly set state.
    kAB = 0xab
    color.set_alpha(kAB)
    color.set_blue(kAB)
    color.set_green(kAB)
    color.set_red(kAB)
    assert kAB == color.get_alpha()
    assert kAB == color.get_blue()
    assert kAB == color.get_green()
    assert kAB == color.get_red()

    # Verify get_color_abgr and get_color_rgba.
    kOpaqueGreen = 0xff00ff00
    color = kmlbase.Color32(kOpaqueGreen)
    assert kOpaqueGreen == color.get_color_abgr()
    kOpaqueGreenARGB = 0xff00ff00
    assert kOpaqueGreenARGB == color.get_color_argb()

    # Verify reconstruction to AABBGGRR string.
    kOpaqueRedStr = "ff0000ff"
    color = kmlbase.Color32(kOpaqueRedStr);
    assert kOpaqueRedStr == color.to_string_abgr()

    # Verify to_argb_string.
    assert kOpaqueRedStr == color.to_string_abgr()
    kOpaqueRedStr_argb = "ffff0000"
    assert kOpaqueRedStr_argb == color.to_string_argb()

    # Equality operator.
    red = kmlbase.Color32(kOpaqueRedStr)
    green = kmlbase.Color32(kOpaqueGreen)
    assert red == red
    assert green != red

class BasicDateTimeTestCase(unittest.TestCase):
  def runTest(self):
    kWhen = "2008-10-03T09:25:42Z"
    datetime = kmlbase.DateTime_Create(kWhen)
    assert datetime
    assert 1223025942 == datetime.GetTimeT()
    assert "09:25:42" == datetime.GetXsdTime()
    assert "2008-10-03" == datetime.GetXsdDate()
    assert kWhen == datetime.GetXsdDateTime()

class BasicVec3TestCase(unittest.TestCase):
  def runTest(self):
    kLatitude = 38.12345
    kLongitude = -121.123123
    kAltitude = 9876.5432
    vec3 = kmlbase.Vec3(kLongitude, kLatitude, kAltitude)
    assert vec3
    assert kLatitude == vec3.get_latitude()
    assert kLongitude == vec3.get_longitude()
    assert vec3.has_altitude()
    assert kAltitude == vec3.get_altitude()
    vec3 = kmlbase.Vec3(kLongitude, kLatitude)
    assert vec3
    assert kLatitude == vec3.get_latitude()
    assert kLongitude == vec3.get_longitude()
    assert not vec3.has_altitude()

class BasicVersionTestCase(unittest.TestCase):
  def runTest(self):
    assert "1.2.0" == kmlbase.Version_GetString()

def suite():
  suite = unittest.TestSuite()
  suite.addTest(BasicAttributesTestCase())
  suite.addTest(BasicColor32TestCase())
  suite.addTest(BasicDateTimeTestCase())
  suite.addTest(BasicVec3TestCase())
  suite.addTest(BasicVersionTestCase())
  return suite

runner = unittest.TextTestRunner()
runner.run(suite())
