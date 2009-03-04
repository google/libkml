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

# This program demonstrates use of the KML DOM Python SWIG bindings 
# for determining the location and bounding box of a feature and/or feature
# hierarchy using kmlengine.GetFeatureLatLon(), kmlengine.GetFeatureBounds(),
# and kmlengine.Bbox() methods.

import sys
import kmldom
import kmlengine

def ReadFile(filename):
  f = open(filename, 'r')
  data = f.read()
  return data

# Program main: read the file to memory, parse it, get and visit
# the root feature if such exists.
def main(inputkml):
  feature = kmlengine.GetRootFeature(kmldom.ParseKml(ReadFile(inputkml)))
  (status, lat, lon) = kmlengine.GetFeatureLatLon(feature)
  if status:
    print 'center',lat,lon
  bbox = kmlengine.Bbox()
  status = kmlengine.GetFeatureBounds(feature, bbox)
  if status:
    print 'north',bbox.get_north()
    print 'south',bbox.get_south()
    print 'east',bbox.get_east()
    print 'west',bbox.get_west()

if __name__ == '__main__':
  if len(sys.argv) != 2:
    print 'usage: %s input.kml' % sys.argv[0]
    sys.exit(1)
  main(sys.argv[1])
