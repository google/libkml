![Awesome logo](bactrian128.png)

[![Build Status](https://travis-ci.org/google/libkml.png?branch=master)](https://travis-ci.org/google/libkml)

This is Google's reference implementation of [OGC KML 2.2](http://www.opengeospatial.org/standards/kml). It also includes implementations of Google's `gx:` extensions used by Google Earth, as well as several utility libraries for working with other formats.

All of our documentation is on the project wiki:
http://code.google.com/p/libkml/w/list

The wiki contains documents that describe:
- An overview of the most recent release
- Building and running the code
- Running the unit tests
- A general API reference guide
- More details guides to each of the libkml modules

The main project page has links to the discussion group and the mailing list that mirrors the commit and issue logs.

Note on strings in libkml:
As of r680, we changed all use of "std::string" to just "string". This change was motivated by a desire to permit use in codebases that provide their own string class. This change was transparent to you because of this change to src/kml/base/util.h:

```
#ifndef HAS_GLOBAL_STRING
using std::string;
#endif
```

All string-using code in libkml was modified to pull in that header.

[The above is the old README from libkml.googlecode.com. The wiki pages will be migrated across to github soon(-ish).]
