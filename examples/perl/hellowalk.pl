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
# walking the feature hierarchy of a KML file.

use strict;
use warnings;

use lib "../../build/perl";
use kmldom;

sub ReadKmlFile {
  my $filename = $_[0];
  local $/;
  open(FILE, $filename) or die("error: can't open $filename");
  my $data = <FILE>;
  close(FILE);
  return $data;
}

sub Indent {
  my $depth = $_[0];
  while ($depth > 0) {
    print '  ';
    $depth -= 1;
  }
}

sub PrFeatureType {
  my $type = $_[0];
  if ($type == $kmldom::Type_Placemark) {
    print("Placemark ");
  } else {
    print("some other Feature ");
  }
}

# This visits a feature.  The type of feature is printed.  If the feature
# is a container such is visited recursively.
sub VisitFeature {
  my $feature = $_[0];
  my $depth = $_[1];
  Indent($depth);
  PrFeatureType($feature->Type());
  printf("id = %s\n", $feature->id());
  my $container = kmldom::AsContainer($feature);
  if (defined $container) {
    WalkContainer($container, $depth+1);
  }
}

# This visits each feature in the given container (<Document> or <Folder>).
sub WalkContainer {
  my $container = $_[0];
  my $depth = $_[1];
  for (my $i = 0; $i < $container->feature_array_size(); $i++) {
    VisitFeature($container->feature_array_at($i), $depth);
  }
}

# The root feature of a KML file is the child feature of the <kml>
# element or the root xml element if that is a Feature.
# If element is neither <kml> nor a feature None is returned.
sub GetRootFeature {
  my $kml = kmldom::AsKml($_[0]);
  if (defined $kml) {
    if ($kml->has_feature()) {
      return $kml->feature();
    } else {
      return undef;
    }
  }
  my $feature = kmldom::AsFeature($_[0]);
  if (defined $feature) {
    return $feature;
  }
  return undef;
}

sub main {
  my $inputkml = $_[0];
  my $kml = ReadKmlFile($inputkml);
  my $feature = GetRootFeature(kmldom::ParseKml(ReadKmlFile($inputkml)));
  if (defined $feature) {
    VisitFeature($feature, 0);
    # Perl deletes the returned feature from ParseKml which in turn deletes all
    # child elements.
  } else {
    # The file is a KML fragment.
    print "No root feature in $inputkml\n";
  }
}

if ($#ARGV != 0) {
  printf("usage: input.kml\n");
  exit;
}

main($ARGV[0]);

