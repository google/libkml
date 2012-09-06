#!/bin/sh

date=$( date +"%Y%m%d" )

rev=$( git rev-parse HEAD )
srev=$( git rev-parse --short HEAD )

echo "Making a new tarball of $rev..."

tarball="libkml-${date}git$srev.tar.gz"

git archive HEAD --prefix libkml/ | gzip > "$tarball"

echo "Available as $tarball"

md5="$( md5sum "$tarball" )"

echo "MD5SUM: $md5"
