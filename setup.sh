#!/bin/bash
#
# ./setup.sh
#
if [ "$(uname)" == "Darwin" ]; then
    platform=macos
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    platform=linux
else
    platform=windows
fi
mkdir -p _deps
# download + install: premake-<platform>.zip -> _deps
package_url=https://github.com/gameplay3d/gameplay-deps/releases/download/v4.0.0
filename=premake-5.0.0-$platform.zip
echo Downloading $filename from $package_url...
curl -# -LO $package_url/$filename
echo Extracting $filename... please standby...
unzip -q $filename -d _deps
echo Cleaning up...
rm $filename
# download + install: gameplay-deps-<platform>.zip -> _deps
package_url=https://github.com/gameplay3d/gameplay/releases/download/v4.0.0
filename=gameplay-deps-$platform.zip
echo Downloading $filename from $prefix...
curl -# -LO $package_url/$filename
echo Extracting $filename... please standby...
unzip -q $filename -d _deps
echo Cleaning up...
rm $filename
echo Done.
