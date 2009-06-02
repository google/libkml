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

// This program demonstrates use of the KML DOM Java SWIG bindings
// for creating a Placemark and serializing to XML.  This demonstrates
// use of the factory, setting complex child elements, setting
// simple elements, and serializing out to XML.

import com.googlecode.libkml.Coordinates;
import com.googlecode.libkml.Folder;
import com.googlecode.libkml.Kml;
import com.googlecode.libkml.KmlFactory;
import com.googlecode.libkml.Placemark;
import com.googlecode.libkml.Feature;
import com.googlecode.libkml.Point;
import com.googlecode.libkml.kmldom;

public class CreateFolder {

  static {
    try {
      System.loadLibrary("kmldom_swig_java");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Failed to load kmldom. " + e);
      System.err.println("Make sure you have your classpath set correctly " +
                         "and that LD_LIBRARY_PATH can see " +
                         "libkmldom_swig_java and friends");
      System.exit(1);
    }
  }

  public static void main(String[] args) throws InterruptedException {
    KmlFactory factory = KmlFactory.GetFactory();

    Folder folder = factory.CreateFolder();
    folder.set_name("foo");

    Placemark p0 = factory.CreatePlacemark();
    p0.set_name("placemark 0");
    p0.set_id("p0");

    folder.add_feature(p0);

    System.out.println("folder size: " + folder.get_feature_array_size());

    System.out.println("name: " + folder.get_name());

    Feature f = folder.get_feature_array_at(0);

    Placemark p = kmldom.AsPlacemark(f);

    System.out.println("placemark name: " + p.get_name());

    System.out.print(kmldom.SerializePretty(p));
  }
}
