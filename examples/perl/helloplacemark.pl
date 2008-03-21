#!/usr/bin/env perl

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

# This program demonstrates use of the KML DOM Perl SWIG bindings for
# creating and accessing simple elements and attributes such as
# Feature's <name> and <visibility> and Object's id= as in <Placemark>.

use lib "../../build/perl";
use kmldom;

my $factory = kmldom::KmlFactory::GetFactory;

print "Creating a Placemark...\n";
my $placemark = $factory->CreatePlacemark();

print "Get the Type()...\n";
printf("placemark->Type(): %s\n", $placemark->Type());

print "has_xxx() all return false...\n";
printf("placemark->has_id(): %d\n", $placemark->has_id());
printf("placemark->has_name(): %d\n", $placemark->has_name());
printf("placemark->has_visibility(): %d\n", $placemark->has_visibility());
printf("placemark->has_geometry(): %d\n", $placemark->has_geometry());

print "Set some fields...\n";
# Object std::string
$placemark->set_id("pm123");
# Feature std::string
$placemark->set_name("hi");
# Feature bool
$placemark->set_visibility(false);  # True by default.

print "has_xxx() now return true...\n";
printf("placemark->has_id(): %d\n", $placemark->has_id());
printf("placemark->has_name(): %d\n", $placemark->has_name());
printf("placemark->has_id(): %d\n", $placemark->has_id());

print "Get some fields...\n";
printf("placemark->id(): %s\n", $placemark->id());
printf("placemark->name(): %s\n", $placemark->name());
printf("placemark->visibility(): %d\n", $placemark->visibility());

print "Clear fields...\n";
$placemark->clear_name();
$placemark->clear_id();
$placemark->clear_visibility();
printf("Placemark->has_id(): %d\n", $placemark->has_id());
printf("Placemark->has_name(): %d\n", $placemark->has_name());
printf("placemark->has_visibility(): %d\n", $placemark->has_visibility());

print "visibility is true by default...\n";
printf("placemark->visibility(): %d\n", $placemark->visibility());

# Perl automatically cleans up placemark.

