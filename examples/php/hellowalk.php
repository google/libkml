#!/usr/bin/env php
<?php

// Copyright 2008, Google Inc. All rights reserved.
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

// This program demonstrates use of the KML DOM PHP SWIG bindings for
// walking the feature hierarchy of a KML file.

// This example uses the SWIG-generated PHP5 wrapper. This has the benefit
// of allowing access to the class hierarchy reflected from the source C++
// classes. For example, using the wrapper you can query a placemark's
// visibility by calling $placemark->has_visibility(). If you're accessing
// the raw extension module, SWIG's flattening of the object structure will
// require you to know that Placemark inherits from Feature and thus you have
// to call Feature_has_visibility($placemark). To see an example of this
// alternate use, see helloplacemark_module.php

  if (!file_exists("kmldom.php")) {
    echo "Before you run the examples you first have to run the\n";
    echo "tweak_generated_kmldom.php script. See README for more details\n";
    exit(1);
  }

  require 'load_kmldom.php';  // Loads the extension module.
  require 'kmldom.php';  // Loads our tweaked SWIG PHP wrapper.

  function ReadKmlFile($filename) {
    $f = fopen($filename, "r");
    $data = fread($f, filesize($filename));
    fclose($f);
    return $data;
  }

  function Indent($depth) {
    while ($depth > 0) {
      print '  ';
      $depth -= 1;
    }
  }

  function PrFeatureType($type) {
    if ($type == kmldom::Type_Placemark) {
      print("Placemark ");
    } else {
      print("some other Feature ");
    }
  }

  // This visits a feature.  The type of feature is printed.  If the feature
  // is a container such is visited recursively.
  function VisitFeature($feature, $depth) {
    Indent($depth);
    PrFeatureType($feature->Type());
    printf("id = %d\n", $feature->id());
    $container = kmldom::AsContainer($feature);
    if ($container) {
      WalkContainer($container, $depth+1);
    }
  }

  // This visits each feature in the given container (<Document> or <Folder>).
  function WalkContainer($container, $depth) {
    for ($i = 0; $i < $container->feature_array_size(); $i++) {
      VisitFeature($container->feature_array_at($i), $depth);
    }
  }

  // The root feature of a KML file is the child feature of the <kml>
  // element or the root xml element if that is a Feature.
  // If element is neither <kml> nor a feature None is returned.
  function GetRootFeature($element) {
    $kml = kmldom::AsKml($element);
    if ($kml) {
      if ($kml->has_feature()) {
        return $kml->feature();
      } else {
        return NULL;
      }
    }
    $feature = kmldom::AsFeature($element);
    if ($feature) {
      return $feature;
    }
    return NULL;
  }

  function main($inputkml) {
    $feature = GetRootFeature(kmldom::ParseKml(ReadKmlFile($inputkml)));
    if ($feature) {
      VisitFeature($feature, 0);
      KmlFactory::GetFactory()->DeleteElement($feature);
      # PHP deletes the feature returned from ParseKml which in turn deletes
      # all child elements.
    } else {
      // The file is a KML fragment.
      echo "No root feature in $inputkml\n";
    }
  }

  if ($argc != 2) {
    echo "usage: $argv[0] input.kml\n";
    exit(1);
  }

  main($argv[1]);

?>
