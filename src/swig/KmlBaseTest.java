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

// This is a small subset of the python tests to ensure sanity of the basic
// Java bindings.

import org.junit.*;
import static org.junit.Assert.*;

import com.googlecode.libkml.*;

public class KmlBaseTest {

  static {
    try {
      System.loadLibrary("kmlbase_swig_java");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Failed to load kmlbase. " + e);
      System.err.println("Make sure you have your classpath set correctly " +
                         "and that LD_LIBRARY_PATH can see " +
                         "libkmldom_swig_java and friends");
      System.exit(1);
    }
  }

  @Test public void testBasicAttributes() {
    Attributes attributes = new Attributes();
    assertEquals(0, attributes.GetSize());
  }

  @Test public void testBasicColor32() {
    Color32 color = new Color32();
    long expected = 0xffffffffL;
    assertEquals(expected, color.get_color_abgr());

    long red = 0xff0000ffL;
    color = new Color32(red);
    assertEquals(red, color.get_color_abgr());

    String red_str = "ff0000ff";
    color = new Color32(red_str);
    assertEquals(red, color.get_color_abgr());
  }

  @Test public void testBasicDateTime() {
    String when = "2008-10-03T09:25:42Z";
    DateTime datetime = DateTime.Create(when);
    assertEquals(1223025942, datetime.GetTimeT());
    assertEquals("09:25:42", datetime.GetXsdTime());
    assertEquals("2008-10-03", datetime.GetXsdDate());
    assertEquals(when, datetime.GetXsdDateTime());
  }

  @Test public void testBasicVec3() {
    double latitude = 38.12345;
    double longitude = -121.123123;
    double altitude = 9876.5432;
    Vec3 vec3 = new Vec3(longitude, latitude, altitude);
    assertEquals(latitude, vec3.get_latitude(), 0.000001);
    assertEquals(longitude, vec3.get_longitude(), 0.000001);
    assertTrue(vec3.has_altitude());
    assertEquals(altitude, vec3.get_altitude(), 0.000001);
    vec3 = new Vec3(longitude, latitude);
    assertEquals(latitude, vec3.get_latitude(), 0.000001);
    assertEquals(longitude, vec3.get_longitude(), 0.000001);
    assertFalse(vec3.has_altitude());
  }

}

