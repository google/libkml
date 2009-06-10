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

// This is the SWIG interface file for the KML Base module.
// This was developed with SWIG version 1.3.35.

%module kmlbase

// Include the headers needed to build kmlengine_wrap.cc.
%{
#include "kml/base/attributes.h"
#include "kml/base/color32.h"
#include "kml/base/date_time.h"
#include "kml/base/vec3.h"
#include "kml/base/version.h"
#include "kml/base/xml_file.h"
%}

// Tell SWIG about C99 integer types.
%include "inttypes.i"

// Tell SWIG about C++ Standard Library std::string.
%include "std_string.i"

%include "typemaps.i"
// NOTE: these typemaps are not fully supported in current SWIG. See this doc:
// http://www.swig.org/Doc1.3/Library.html#Library_nn14 (1.3.39)
// It happens to work in Python, but Java is stymied. Ideally SWIG evolves to
// have a std_string.i that can wrap writing to a pointer.
%apply std::string* OUTPUT { std::string* output_value };
%apply std::string* OUTPUT { std::string* output_key };

typedef long time_t;

namespace kmlbase {

class Attributes {
 public:
  bool FindValue(const std::string& key, std::string* output_value) const;
  bool FindKey(const std::string& value, std::string* output_key) const;
  size_t GetSize() const;
  void SetString(const std::string& attr_name, const std::string& attr_val);
};

class Color32 {
 public:
  Color32();
  Color32(uint32_t abgr);
  Color32(unsigned char a, unsigned char b, unsigned char g, unsigned char r);
  Color32(const std::string& value);
  uint32_t get_red() const;
  void set_red(unsigned char value);
  uint32_t get_green() const;
  void set_green(unsigned char value);
  uint32_t get_blue() const;
  void set_blue(unsigned char value);
  uint32_t get_alpha() const;
  void set_alpha(unsigned char value);
  uint32_t get_color_abgr() const;
  uint32_t get_color_argb() const;
  std::string to_string_abgr() const;
  std::string to_string_argb() const;
  void set_color_abgr(uint32_t color_abgr);
  void set_color_abgr(const std::string& color_abgr);
  void set_color_abgr(unsigned char a, unsigned char b,
                      unsigned char g, unsigned char r);
  void set_color_argb(uint32_t color_argb);
  bool operator==(const Color32& color) const;
  // TODO: other operator overrides.
};

%nodefaultctor DateTime;
class DateTime {
 public:
  static DateTime* Create(const std::string& str);
  static time_t ToTimeT(const std::string& str);
  time_t GetTimeT();
  std::string GetXsdTime() const;
  std::string GetXsdDate() const;
  std::string GetXsdDateTime() const;
};

class Vec3 {
 public:
  Vec3(double longitude, double latitude);
  Vec3(double longitude, double latitude, double altitude);
  double get_longitude() const;
  double get_latitude() const;
  bool has_altitude() const;
  double get_altitude() const;
};

%nodefaultctor Version;
class Version {
 public:
  static std::string GetString();
};

%nodefaultctor XmlFile;
class XmlFile {
 public:
  const std::string& get_url() const;
};

}  // end namespace kmlbase
