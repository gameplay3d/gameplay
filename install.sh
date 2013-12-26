#!/bin/bash
#
# ./install.sh
#
# Download GamePlay external dependencies and binaries and extract from ZIP
#
# Helps prevent repo bloat due to large binary files since they can
# be hosted separately.
#

prefix=https://github.com/blackberry/GamePlay/releases/download/v1.7.0

filename=gameplay-deps

echo Downloading $filename.zip from $prefix...
curl -# -O -L $prefix/$filename.zip
echo Extracting $filename.zip... please standby...
unzip -q $filename.zip
echo Cleaning up...
rm $filename.zip
echo Done.
