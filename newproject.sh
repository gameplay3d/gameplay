#!/bin/bash
# ********************************************************************
#
# newproject.sh
#
# This script generates a set of gameplay project files.
# The new project will be based of the template project and 
# it will be generated with the name and location that is specified
# as input parameters.
#
# IMPORTANT: This script must be run from the root of the gameplay
# source tree.
#
# ********************************************************************

#Find out which OS we're on. 
unamestr=$(uname)

# Switch-on alias expansion within the script 
shopt -s expand_aliases

#Alias the sed in-place command for OSX and Linux - incompatibilities between BSD and Linux sed args
if [[ "$unamestr" == "Darwin" ]]; then
	alias aliassedinplace='sed -i ""'
else
	#For Linux, notice no space after the '-i' 
	alias aliassedinplace='sed -i""'
fi


echo
echo "1. Enter a name for the new project."
echo
echo "   This name will be given to the project"
echo "   executable and a folder with this name"
echo "   will be created to store all project files."
echo
read -p "Project Name: " projName 
if [[ "$projName" == "" ]]; then
	echo
	echo "ERROR: No project name specified."
	echo
	exit -1;
fi
echo 
echo
echo "2. Enter a game title."
echo
echo "   On some platforms, this title is used to"
echo "   identify the game during installation and"
echo "   on shortcuts/icons."
echo
read -p "Title: " title 
if [[ "$title" == "" ]]; then
	echo
	echo "ERROR: No game title specified."
	echo
	exit -1;
fi
echo 

echo
echo "3. Enter a short game description."
echo
read -p "Description: " desc
if [[ "$desc" == "" ]]; then
	desc=$title
fi
echo 

echo
echo "4. Enter a unique identifier for your project."
echo
echo "   This should be a human readable package name,"
echo "   containing at least two words separated by a"
echo "   period (eg. com.surname.gamename)."
echo
read -p "Unique ID: " uuid
if [[ "$uuid" == "" ]]; then
	echo
	echo "ERROR: No uuid specified."
	echo
	exit -1;
fi
echo 

echo
echo "5. Enter author name."
echo
echo "   On BlackBerry targets, this is used for"
echo "   signing and must match the developer name"
echo "   of your development certificate."
echo
read -p "Author: " author
if [[ "$author" == "" ]]; then
	echo
	echo "ERROR: No author specified."
	echo
	exit -1;
fi
echo 

echo
echo "6. Enter your game's main class name."
echo
echo "   Your initial game header and source file"
echo "   will be given this name and a class with"
echo "   this name will be created in these files."
echo
read -p "Class name: " className
if [[ "$className" == "" ]]; then
	echo
	echo "ERROR: No class name specified."
	echo
	exit -1;
fi
echo 

echo
echo "7. Enter the project path."
echo
echo "   This can be a relative path, absolute path,"
echo "   or empty for the current folder. Note that"
echo "   a project folder named $projName will also"
echo "   be created inside this folder."
echo
read -p "Path: " location
if [[ "$location" == "" ]]; then
	projPath=$projName
else
	projPath="$location/$projName"
fi
echo

# Verify Path and eliminate double '//'
projPath=`echo "$projPath" | sed 's_//_/_g'`
if [ -e $projPath ]; then
	echo
	echo "ERROR: Path '$projPath' already exists, aborting."
	echo
	exit -2
fi

# Make required source folder directories
mkdir -p "$projPath"
mkdir -p "$projPath/src"
mkdir -p "$projPath/res"

if [[ ${projPath:0:1} != "/" ]]; then
	currPwd=`pwd`
	projPath=`cd $projPath; pwd`
	`cd $currPwd`	
fi

# Generate relative path from project folder to gameplay folder
gpPathAbs=`pwd`
common_path=$projPath
back=
while [ "${gpPathAbs#$common_path}" = "${gpPathAbs}" ]; do
	common_path=$(dirname "$common_path")
	if [ -z "$back" ]; then
		back=".."
	else
		back="../${back}"
	fi
done
gpPath=${back}/${gpPathAbs#$common_path/}
if [[ ${gpPathAbs} == ${common_path} ]]; then
	gpPath=${back}
fi

#############################################
# Copy Microsoft Visual Studio project files
#############################################
cp "template/template.vcxproj" "$projPath/$projName.vcxproj"
aliassedinplace "s*TEMPLATE_PROJECT*$projName*g" "$projPath/$projName.vcxproj"
aliassedinplace "s*TemplateGame*$className*g" "$projPath/$projName.vcxproj"
aliassedinplace "s*GAMEPLAY_PATH*$gpPath*g" "$projPath/$projName.vcxproj"

cp "template/template.vcxproj.filters" "$projPath/$projName.vcxproj.filters"
aliassedinplace "s*TemplateGame*$className*g" "$projPath/$projName.vcxproj.filters"

cp "template/template.vcxproj.user" "$projPath/$projName.vcxproj.user"
aliassedinplace "s*GAMEPLAY_PATH*$gpPath*g" "$projPath/$projName.vcxproj.user"


#############################################
# Copy Apple Xcode project files
#############################################
mkdir -p "$projPath/$projName.xcodeproj"
cp "template/template.xcodeproj/project.pbxproj" "$projPath/$projName.xcodeproj/project.pbxproj"
aliassedinplace "s*TEMPLATE_PROJECT*$projName*g" "$projPath/$projName.xcodeproj/project.pbxproj"
aliassedinplace "s*TemplateGame*$className*g" "$projPath/$projName.xcodeproj/project.pbxproj"
aliassedinplace "s*GAMEPLAY_PATH*$gpPath*g" "$projPath/$projName.xcodeproj/project.pbxproj"

cp "template/TEMPLATE_PROJECT-macosx.plist" "$projPath/$projName-macosx.plist"
aliassedinplace "s*TEMPLATE_UUID*$uuid*g" "$projPath/$projName-macosx.plist"
aliassedinplace "s*TEMPLATE_AUTHOR*$author*g" "$projPath/$projName-macosx.plist"

cp "template/TEMPLATE_PROJECT-ios.plist" "$projPath/$projName-ios.plist"
cp "template/Default-568h@2x.png" "$projPath/Default-568h@2x.png"
aliassedinplace "s*TEMPLATE_TITLE*$title*g" "$projPath/$projName-ios.plist"
aliassedinplace "s*TEMPLATE_UUID*$uuid*g" "$projPath/$projName-ios.plist"
aliassedinplace "s*TEMPLATE_AUTHOR*$author*g" "$projPath/$projName-ios.plist"

#############################################
# Copy BlackBerry NDK project files
#############################################
cp "template/template.cproject" "$projPath/.cproject"
aliassedinplace "s*TEMPLATE_PROJECT*$projName*g" "$projPath/.cproject"
aliassedinplace "s*TEMPLATE_UUID*$uuid*g" "$projPath/.cproject"
aliassedinplace "s*GAMEPLAY_PATH*$gpPath*g" "$projPath/.cproject"

cp "template/template.project" "$projPath/.project"
aliassedinplace "s*TEMPLATE_PROJECT*$projName*g" "$projPath/.project"

cp "template/template.bar-descriptor.xml" "$projPath/bar-descriptor.xml"
aliassedinplace "s*TEMPLATE_PROJECT*$projName*g" "$projPath/bar-descriptor.xml"
aliassedinplace "s*TEMPLATE_TITLE*$title*g" "$projPath/bar-descriptor.xml"
aliassedinplace "s*TEMPLATE_UUID*$uuid*g" "$projPath/bar-descriptor.xml"
aliassedinplace "s*TEMPLATE_AUTHOR*$author*g" "$projPath/bar-descriptor.xml"
aliassedinplace "s*TEMPLATE_DESCRIPTION*$desc*g" "$projPath/bar-descriptor.xml"

#############################################
# Copy Android NDK project files
#############################################
mkdir -p "$projPath/android"
mkdir -p "$projPath/android/jni"
mkdir -p "$projPath/android/res/values"
mkdir -p "$projPath/android/res/drawable"

cp "template/android/template.AndroidManifest.xml" "$projPath/android/AndroidManifest.xml"
aliassedinplace "s*TEMPLATE_PROJECT*$projName*g" "$projPath/android/AndroidManifest.xml"
aliassedinplace "s*TEMPLATE_UUID*$uuid*g" "$projPath/android/AndroidManifest.xml"

cp "template/android/template.build.xml" "$projPath/android/build.xml"
aliassedinplace "s*TEMPLATE_PROJECT*$projName*g" "$projPath/android/build.xml"

cp "template/android/jni/Application.mk" "$projPath/android/jni/Application.mk"
cp "template/android/jni/template.Android.mk" "$projPath/android/jni/Android.mk"
aliassedinplace "s*TEMPLATE_PROJECT*$projName*g" "$projPath/android/jni/Android.mk"
aliassedinplace "s*TemplateGame*$className*g" "$projPath/android/jni/Android.mk"
aliassedinplace "s*GAMEPLAY_PATH*$gpPath*g" "$projPath/android/jni/Android.mk"

cp "template/icon.png" "$projPath/android/res/drawable/icon.png"
cp "template/android/res/values/template.strings.xml" "$projPath/android/res/values/strings.xml"
aliassedinplace "s*TEMPLATE_TITLE*$title*g" "$projPath/android/res/values/strings.xml"

#############################################
# Copy CMake files
#############################################
mkdir -p "$projPath/build"
cp "template/template-CMakeLists.txt" "$projPath/CMakeLists.txt"
aliassedinplace "s*TEMPLATE_PROJECT*$projName*g" "$projPath/CMakeLists.txt"
aliassedinplace "s*TemplateGame*$className*g" "$projPath/CMakeLists.txt"
aliassedinplace "s*GAMEPLAY_PATH*$gpPath*g" "$projPath/CMakeLists.txt"

#############################################
# Copy source files
#############################################
cp "template/src/TemplateGame.h" "$projPath/src/$className.h"
cp "template/src/TemplateGame.cpp" "$projPath/src/$className.cpp"
aliassedinplace "s*TemplateGame*$className*g" "$projPath/src/$className.h"
aliassedinplace "s*TemplateGame*$className*g" "$projPath/src/$className.cpp"

# Copy resource files
cp "template/res/"* "$projPath/res/"

# Copy icon
cp "template/icon.png" "$projPath/icon.png"

# Copy config
cp "template/game.config" "$projPath/game.config"
aliassedinplace "s*TEMPLATE_TITLE*$title*g" "$projPath/game.config"

# Open the new project folder, use xdg-open on Linux
if [[ "$unamestr" == "Linux" ]]; then
	xdg-open "$projPath"
else
	open "$projPath"
fi

exit 0
