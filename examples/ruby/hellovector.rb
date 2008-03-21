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
# for creating and accessing array values such as <coordinates>
# and <Folder>.

require 'setpath'

factory = Kmldom::KmlFactory.GetFactory()

puts "Coordinates is a vector of Vec3..."

coordinates = factory.CreateCoordinates()
coordinates.add_point2(1,1)
coordinates.add_point2(2,2)
coordinates.add_point2(3,3)

for i in 0..coordinates.coordinates_array_size()-1
  puts "longitude: #{coordinates.coordinates_array_at(i).longitude()}"
  puts "latitude: #{coordinates.coordinates_array_at(i).latitude()}"
  puts "altitude: #{coordinates.coordinates_array_at(i).altitude()}"
end

puts Kmldom::SerializePretty(coordinates)

# Ruby deletes coordinates

puts "Create a Folder with some Features..."

folder = factory.CreateFolder()
folder.add_feature(factory.CreatePlacemark())
folder.add_feature(factory.CreateFolder())

for i in 0..folder.feature_array_size()-1
  puts "feature Type: #{folder.feature_array_at(i).Type()}"
end

puts Kmldom::SerializePretty(folder)

# Ruby deletes folder which internally deletes its feature list.

