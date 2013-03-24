#!/bin/bash

libname=$1
osname=$2

mkdir -p "_Frameworks/$osname/$libname.framework/Versions/A/Headers"

# Link the "Current" version to "A"
/bin/ln -sfh A "_Frameworks/$osname/$libname.framework/Versions/Current"
/bin/ln -sfh Versions/Current/Headers "_Frameworks/$osname/$libname.framework/Headers"
/bin/ln -sfh "Versions/Current/$libname" "_Frameworks/$osname/$libname.framework/$libname"

# The -a ensures that the headers maintain the source modification date so that we don't constantly
# cause propagating rebuilds of files that import these headers.
/bin/cp -a "external-deps/$libname/include/" "_Frameworks/$osname/$libname.framework/Versions/A/Headers"

if [[ $osname = "iOS" ]]
then
lipo -create "external-deps/$libname/lib/ios/armv7/lib$libname.a" "external-deps/$libname/lib/ios/i386/lib$libname.a" -output "_Frameworks/$osname/$libname.framework/Versions/A/lib$libname.a"
else
lipo -create "external-deps/$libname/lib/macosx/lib$libname.a" -output "_Frameworks/$osname/$libname.framework/Versions/A/lib$libname.a"
fi
