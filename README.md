![Awesome logo](bactrian128.png)

[![Travis Build Status](https://travis-ci.org/libkml/libkml.png?branch=master)](https://travis-ci.org/libkml/libkml)
[![Coverity Scan Build Status](https://scan.coverity.com/projects/4788/badge.svg)](https://scan.coverity.com/projects/4788)

This is Google's reference implementation of [OGC KML 2.2](http://www.opengeospatial.org/standards/kml). It also includes implementations of Google's `gx:` extensions used by Google Earth, as well as several utility libraries for working with other formats.

All of original documentation written by google is imported into "wiki" branch:
https://github.com/libkml/libkml/tree/wiki

The wiki contains documents that describe:
- An overview of the most recent release
- Building and running the code
- Running the unit tests
- A general API reference guide
- More details guides to each of the libkml modules

This libkml project was imported from code.google.com using their "Export to Github" tool and then all commits from google/libkml and rashadkm/libkml were merged.

All issues and wiki from code.google.com have also been imported, thanks to GoogleCodeExporter tool.

The primary motivation for the fork is the lack of alternatives to the libkml library. Most of the open source GIS projects uses this library for processing kml/kmz filesnotably GDAL, OSSIM, OTB and osgEarth. The development of the libkml project has been shelved by Google without any publicly stated reason so far.

Contacting the libkml maintainers on code.google.com failed to get a response. Google copied the project from code.google.com to GitHub and the GitHub repository was a little more active in terms of commits for a short while. Via the GitHub issue https://github.com/google/libkml/issues/4 another attempt to get feedback from Google was made, but other than the discussion between Bas Couwenberg and Rashad and a comment by Simon there was no response from anybody else. Because of that the decision was made to attempt to revitalize the libkml project with a fork on GitHub. At this point, the github way or forking from google/libkml and sending back a Pull Requests would also result in no effect.
