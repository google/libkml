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

#ifndef KML_BASE_UTIL_H__
#define KML_BASE_UTIL_H__

#ifndef _MSC_VER
#include <stdint.h>  // For fixed-size interger typedefs in this file.
#endif

// A macro to disallow the evil copy constructor and assignment operator.
// Should be used in the private: declarations for a class.
#define LIBKML_DISALLOW_EVIL_CONSTRUCTORS(TypeName) \
  TypeName(const TypeName&);\
  void operator=(const TypeName&)

typedef unsigned int uint;
// MSVC has no header for C99 typedefs.
#ifdef _MSC_VER
typedef __int8  int_8;
typedef __int16 int_16;
typedef __int32 int_32;
typedef __int64 int_64;
typedef unsigned __int8  uint_8;
typedef unsigned __int16 uint_16;
typedef unsigned __int32 uint_32;
typedef unsigned __int64 uint_64;
#endif  // _MSC_VER

#endif  // KML_BASE_UTIL_H__
