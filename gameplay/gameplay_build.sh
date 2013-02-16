#!/bin/sh
rm -f ../libgameplay/armv7/*.*
mkdir ../libgameplay/armv7
xcodebuild -project gameplay.xcodeproj -target gameplay-ios -configuration Release -arch armv7
cp build/Release-iphoneos/libgameplay.a ../libgameplay/armv7/libgameplay.a
#rm -rf build

rm -f ../libgameplay/armv7s/*.*
mkdir ../libgameplay/armv7s
xcodebuild -project gameplay.xcodeproj -target gameplay-ios -configuration Release -arch armv7s
cp build/Release-iphoneos/libgameplay.a ../libgameplay/armv7s/libgameplay.a
#rm -rf build

