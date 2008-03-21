#!/usr/bin/env ruby

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

# This program demonstrates use of the KML DOM Ruby SWIG bindings 
# for walking the feature hierarchy of a KML file.

require 'setpath'

def ReadFile(filename)
  data = ''
  file = File.new(filename, "r")
  file.each { |line| data += line }
  return data
end

def Indent(depth)
  while depth > 0
    print "  "
    depth -= 1
  end
end
 
def PrFeatureType(type)
  if type == Kmldom::Type_Placemark
    print "Placemark "
  else
    print "some other Feature "
  end
end

# This visits a feature.  The type of feature is printed.  If the feature
# is a container such is visited recursively.
def VisitFeature(feature, depth)
  Indent(depth)
  PrFeatureType(feature.Type())
  puts "id = #{feature.id()}"
  container = Kmldom::AsContainer(feature)
  if container:
    WalkContainer(container, depth+1)
  end
end

# This visits each feature in the given container (<Document> or <Folder>).
def WalkContainer(container, depth)
  for i in 0..container.feature_array_size()-1
    VisitFeature(container.feature_array_at(i), depth)
  end
end

# The root feature of a KML file is the child feature of the <kml>
# element or the root xml element if that is a Feature.
# If element is neither <kml> nor a feature nil is returned.
def GetRootFeature(element)
  kml = Kmldom::AsKml(element)
  if kml
    if kml.has_feature():
      return kml.feature()
    else
      return nil
    end
  end
  feature = Kmldom::AsFeature(element)
  if feature
    return feature
  end
  return nil
end

def main(inputkml)
  feature = GetRootFeature(Kmldom::ParseKml(ReadFile(inputkml)))
  if feature
    VisitFeature(feature, 0)
    # Ruby deletes the feature returned from ParseKml which in turn
    # deletes all child elements.
  else
    # The file is a KML fragment.
    puts "No root feature in #{inputkml}"
  end
end

if __FILE__ == $0
  if ARGV.length != 1
    puts "usage: #{$0} input.kml"
    exit 0
  end
  main(ARGV[0])
end

