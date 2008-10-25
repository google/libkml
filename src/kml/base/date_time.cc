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

// This file contains the implementation of the DateTime class.

#include "kml/base/date_time.h"
#include <stdlib.h>

namespace kmlbase {

// static
DateTime* DateTime::Create(const std::string& str) {
  DateTime* date_time = new DateTime;
  if (date_time->ParseXsdDateTime(str)) {
    return date_time;
  }
  delete date_time;
  return NULL;
}

time_t DateTime::GetTimeT() /* const */ {
  return timegm(&tm_);
}

template<int N>
std::string DateTime::DoStrftime(const char* format) const {
  char buf[N];
  strftime(buf, N, format, &tm_);
  return buf;
}

std::string DateTime::GetXsdTime() const {
  return DoStrftime<9>("%H:%M:%S");
}

std::string DateTime::GetXsdDate() const {
  return DoStrftime<11>("%Y-%m-%d");
}

std::string DateTime::GetXsdDateTime() const {
  return GetXsdDate() + "T" + GetXsdTime() + "Z";
}

// private
DateTime::DateTime() {
}

// private
bool DateTime::ParseXsdDateTime(const std::string& xsd_date_time) {
  // TODO: strptime on win32?
  return strptime(xsd_date_time.c_str(), "%Y-%m-%dT%H:%M:%SZ", &tm_) != NULL;
}

}  // end namespace kmlbase
