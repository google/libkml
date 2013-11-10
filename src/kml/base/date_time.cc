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
#include "boost/scoped_ptr.hpp"
#include <stdlib.h>

// TODO: fix this for real.
#ifdef _WIN32
char* strptime(const char* buf, const char* format, struct tm* tm) {
  // Not yet implemented on this platform.
  return NULL;
}
#endif

namespace kmlbase {

// static
DateTime* DateTime::Create(const string& str) {
  DateTime* date_time = new DateTime;
  if (date_time->ParseXsdDateTime(str)) {
    return date_time;
  }
  delete date_time;
  return NULL;
}

// static
time_t DateTime::ToTimeT(const string& str) {
  boost::scoped_ptr<DateTime> date_time(DateTime::Create(str));
  return date_time.get() ? date_time->GetTimeT() : 0;
}

// Overkill since all time_t's on a 32-bit that are divisible by four are
// leap years, but this should handle the 2100 case (not a LY) on 64-bit
// systems.
static bool is_leap(int year) {
  year += 1900;
  return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}

time_t DateTime::GetTimeT() const {
  // Always return time as GMT; disregard local time, unlike  mktime(3).
  // Faster (and less "obviously correct") implementations are possible.
  static const unsigned ndays[2][12] ={
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
  time_t res = 0;

  // This and is_leap() base from 1970, the epoch of a time_t.
  for (int year = 70; year < tm_.tm_year; ++year)
    res += is_leap(year) ? 366 : 365;

  for (int month = 0; month < tm_.tm_mon; ++month)
    res += ndays[is_leap(tm_.tm_year)][month];

  res += tm_.tm_mday - 1;
  res *= 24;
  res += tm_.tm_hour;
  res *= 60;
  res += tm_.tm_min;
  res *= 60;
  res += tm_.tm_sec;
  return res;
}

template<int N>
string DateTime::DoStrftime(const char* format) const {
  char buf[N];
  strftime(buf, N, format, &tm_);
  return buf;
}

string DateTime::GetXsdTime() const {
  return DoStrftime<9>("%H:%M:%S");
}

string DateTime::GetXsdDate() const {
  return DoStrftime<11>("%Y-%m-%d");
}

string DateTime::GetXsdDateTime() const {
  return GetXsdDate() + "T" + GetXsdTime() + "Z";
}

// private
DateTime::DateTime() {
}

// private
bool DateTime::ParseXsdDateTime(const string& xsd_date_time) {
  // TODO: strptime on win32?
  return strptime(xsd_date_time.c_str(), "%Y-%m-%dT%H:%M:%SZ", &tm_) != NULL;
}

}  // end namespace kmlbase
