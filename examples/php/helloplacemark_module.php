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
// creating and accessing simple elements and attributes such as
// Feature's <name> and <visibility> and Object's id= as in <Placemark>.

// This example uses the SWIG'd kmldom extension module directly. Because
// SWIG flattens the C++ object structure, you are required to know the
// inheritance hierarchy. For example, to query a Placemark's visibility,
// you have to know that a Placemark is a Feature and the correct function
// call is Feature_has_visibility($placemark).
// SWIG generates a wrapper that does a reasonable job of reflecting the
// source C++ classes into a PHP5 class hierarchy. We recommend you use this
// instead. To see an example of this alternate usage, see
// helloplacemark_wrapper.php

  require 'load_kmldom.php';  // Loads the extension module.

  function main() {

    $factory = KmlFactory_GetFactory();

    echo "Creating a Placemark...\n";
    $placemark = KmlFactory_CreatePlacemark($factory);

    echo "Get the Type()...\n";
    printf("Element_Type(placemark): %s\n", Element_Type($placemark));

    echo "has_xxx() all return false...\n";
    // A Placemark's id comes from Object:
    printf("Object_has_id(placemark): %d\n",
           Object_has_id($placemark));
    // name and visibility are from Feature:
    printf("Feature_has_name(placemark): %d\n",
           Feature_has_name($placemark));
    printf("Feature_has_visibility(placemark): %d\n",
           Feature_has_visibility($placemark));
    // geometry is from Placemark:
    printf("Feature_has_geometry(): %d\n",
           Placemark_has_geometry($placemark));

    echo "Set some fields...\n";
    // Object std::string
    Object_set_id($placemark, "pm123");
    // Feature std::string
    Feature_set_name($placemark, "hi");
    // Feature bool
    Feature_set_visibility($placemark, false);  // True by default.

    echo "has_xxx() now return true...\n";
    printf("Object_has_id(placemark): %d\n",
           Object_has_id($placemark));
    printf("Feature_has_name(placemark): %d\n",
           Feature_has_name($placemark));
    printf("Feature_has_visibility(placemark): %d\n",
           Feature_has_visibility($placemark));

    echo "Get some fields...\n";
    printf("Object_id(placemark): %s\n",
           Object_id($placemark));
    printf("Feature_name(placemark): %s\n",
           Feature_name($placemark));
    printf("Feature_visibility(placemark): %d\n",
           Feature_visibility($placemark));

    echo "Clear fields...\n";
    Feature_clear_name($placemark);
    Object_clear_id($placemark);
    Feature_clear_visibility($placemark);
    printf("Object_has_id(placemark): %d\n",
           Object_has_id($placemark));
    printf("Feature_has_name(placemark): %d\n",
           Feature_has_name($placemark));
    printf("Feature_has_visibility(placemark): %d\n",
           Feature_has_visibility($placemark));

    echo "visibility is true by default...\n";
    printf("Feature_visibility(placemark): %d\n",
           Feature_visibility($placemark));

    // PHP automatically cleans up placemark.
  }

  main();

?>
