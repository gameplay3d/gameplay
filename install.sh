#!/bin/bash
#
# ./install.sh
#
# Download GamePlay dependencies and binaries from github releases and extracts from ZIP
#
# Helps prevent repo bloat due to large binary files
#

prefix=https://github.com/blackberry/GamePlay/releases/download/v2.0.0

filename=gameplay-deps

echo Downloading $filename.zip from $prefix...
curl -# -LO $prefix/$filename.zip
echo Extracting $filename.zip... please standby...
unzip -q $filename.zip
echo Cleaning up...
rm $filename.zip
echo Done.
