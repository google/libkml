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
#include "kml/base/color32.h"
%}

// Tell SWIG about boost::intrusive_ptr and the inheritance hierarchy
// of objects using intrusive_ptr.
#ifdef SWIGPYTHON
%include "python/boost_intrusive_ptr.i"
#elif SWIGJAVA
%include "java/boost_intrusive_ptr.i"
#endif

// Tell SWIG about C99 integer types.
%include "inttypes.i"

// Tell SWIG about C++ Standard Library std::string.
%include "std_string.i"

// Tell SWIG about the KML DOM intrusive_ptr typedefs.
%include "kml/dom/kml_ptr.h"

namespace kmlbase {

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

}  // end namespace kmlbase
