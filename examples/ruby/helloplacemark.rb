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

require 'setpath'  # sets path to uninstalled kmldom module.

factory = Kmldom::KmlFactory.GetFactory()
placemark = factory.CreatePlacemark()

puts "has_xxx() all return false..."
puts "placemark.has_id(): #{placemark.has_id()}"
puts "placemark.has_name(): #{placemark.has_name()}"
puts "placemark.has_geometry(): #{placemark.has_geometry()}"

puts "set some fields"
placemark.set_id("id123")
placemark.set_name("we love Ruby")

puts "has_xxx() now return true"
puts "placemark.has_id(): #{placemark.has_id()}"
puts "placemark.has_name(): #{placemark.has_name()}"

puts "get the fields we set"
puts "placemark.id(): #{placemark.id()}"
puts "placemark.name(): #{placemark.name()}"

puts "print the KML for the placemark"
puts Kmldom::SerializePretty(placemark)

puts "clear the fields"
placemark.clear_id()
placemark.clear_name()
puts "placemark.id(): #{placemark.id()}"
puts "placemark.name(): #{placemark.name()}"

# Ruby automatically cleans up placemark.

