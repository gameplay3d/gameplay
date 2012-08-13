#/bin/bash
# ********************************************************************
#
# generate-project.sh
#
# This script generates a set of gameplay project files.
# The new project will be based of the gameplay-template project and 
# it will be generated with the name and location that is specified
# as input parameters.
#
# IMPORTANT: This script must be run from the root of the gameplay
# source tree (on Linux can run from anywhere)
#
# ********************************************************************

function die()
{
    echo "$1"
    exit 1
}

if [[ "${OSTYPE}" == linux* ]]; then
    GAME_PLAY_PATH=$(readlink -e $(dirname $0))
    
    # the common sed syntax
    function replace()
    {
        sed -i "s*$1*$2*g" $3
    }
    
else
    GAME_PLAY_PATH=`pwd`
    
    # the orignal version (MacOSX probably)
    function replace()
    {
        sed -i "" 's*$1*$2*g' $3
    }
fi

# ensure run from correct directory
TEMPLATE_PATH=${GAME_PLAY_PATH}/gameplay-template
[ -d "$TEMPLATE_PATH" ] || die "Missing gameplay-template"

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
gpPathAbs=$GAME_PLAY_PATH
gpPath=$GAME_PLAY_PATH
common_path=$projPath
back=
while [ "${gpPathAbs#$common_path}" = "${gpPathAbs}" ]; do
	common_path=$(dirname $common_path)
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
cp "${TEMPLATE_PATH}/gameplay-template.vcxproj" "$projPath/$projName.vcxproj"
replace TEMPLATE_PROJECT "$projectName" "$projPath/$projName.vcxproj"
replace TemplateGame "$className" "$projPath/$projName.vcxproj"
replace GAMEPLAY_PATH "$gpPath" "$projPath/$projName.vcxproj"

cp "${TEMPLATE_PATH}/gameplay-template.vcxproj.filters" "$projPath/$projName.vcxproj.filters"
replace TemplateGame "$className" "$projPath/$projName.vcxproj.filters"

cp "${TEMPLATE_PATH}/gameplay-template.vcxproj.user" "$projPath/$projName.vcxproj.user"
replace GAMEPLAY_PATH "$gpPath" "$projPath/$projName.vcxproj.user"


#############################################
# Copy Apple Xcode project files
#############################################
mkdir -p "$projPath/$projName.xcodeproj"
cp "${TEMPLATE_PATH}/gameplay-template.xcodeproj/project.pbxproj" "$projPath/$projName.xcodeproj/project.pbxproj"
replace TEMPLATE_PROJECT "$projName" "$projPath/$projName.xcodeproj/project.pbxproj"
replace TemplateGame "$className" "$projPath/$projName.xcodeproj/project.pbxproj"
replace GAMEPLAY_PATH "$gpPath" "$projPath/$projName.xcodeproj/project.pbxproj"

cp "${TEMPLATE_PATH}/TEMPLATE_PROJECT-macosx.plist" "$projPath/$projName-macosx.plist"
replace TEMPLATE_UUID "$uuid" "$projPath/$projName-macosx.plist"
replace TEMPLATE_AUTHOR "$author" "$projPath/$projName-macosx.plist"

cp "${TEMPLATE_PATH}/TEMPLATE_PROJECT-ios.plist" "$projPath/$projName-ios.plist"
replace TEMPLATE_TITLE "$title" "$projPath/$projName-ios.plist"
replace TEMPLATE_UUID "$uuid" "$projPath/$projName-ios.plist"
replace TEMPLATE_AUTHOR "$author" "$projPath/$projName-ios.plist"

#############################################
# Copy BlackBerry NDK project files
#############################################
cp "${TEMPLATE_PATH}/template.cproject" "$projPath/.cproject"
replace TEMPLATE_PROJECT "$projName" "$projPath/.cproject"
replace TEMPLATE_UUID "$uuid" "$projPath/.cproject"
replace GAMEPLAY_PATH "$gpPath" "$projPath/.cproject"

cp "${TEMPLATE_PATH}/template.project" "$projPath/.project"
replace TEMPLATE_PROJECT "$projName" "$projPath/.project"

cp "${TEMPLATE_PATH}/template.bar-descriptor.xml" "$projPath/bar-descriptor.xml"
replace TEMPLATE_PROJECT "$projName" "$projPath/bar-descriptor.xml"
replace TEMPLATE_TITLE "$title" "$projPath/bar-descriptor.xml"
replace TEMPLATE_UUID "$uuid" "$projPath/bar-descriptor.xml"
replace TEMPLATE_AUTHOR "$author" "$projPath/bar-descriptor.xml"
replace TEMPLATE_DESCRIPTION "$desc" "$projPath/bar-descriptor.xml"

#############################################
# Copy Android NDK project files
#############################################
mkdir -p "$projPath/android"
mkdir -p "$projPath/android/jni"
mkdir -p "$projPath/android/res/values"
mkdir -p "$projPath/android/res/drawable"

cp "${TEMPLATE_PATH}/android/template.AndroidManifest.xml" "$projPath/android/AndroidManifest.xml"
replace TEMPLATE_PROJECT "$projName" "$projPath/android/AndroidManifest.xml"
replace TEMPLATE_UUID "$uuid" "$projPath/android/AndroidManifest.xml"

cp "${TEMPLATE_PATH}/android/template.build.xml" "$projPath/android/build.xml"
replace TEMPLATE_PROJECT "$projName" "$projPath/android/build.xml"

cp "${TEMPLATE_PATH}/android/jni/Application.mk" "$projPath/android/jni/Application.mk"

cp "${TEMPLATE_PATH}/android/jni/template.Android.mk" "$projPath/android/jni/Android.mk"
replace TEMPLATE_PROJECT "$projName" "$projPath/android/jni/Android.mk"
replace TemplateGame "$className" "$projPath/android/jni/Android.mk"
replace GAMEPLAY_PATH "$gpPath" "$projPath/android/jni/Android.mk"


cp "${TEMPLATE_PATH}/icon.png" "$projPath/android/res/drawable/icon.png"
cp "${TEMPLATE_PATH}/android/res/values/template.strings.xml" "$projPath/android/res/values/strings.xml"
replace TEMPLATE_TITLE "$title" "$projPath/android/res/values/strings.xml"


#############################################
# Copy source files
#############################################
cp "${TEMPLATE_PATH}/src/TemplateGame.h" "$projPath/src/$className.h"
cp "${TEMPLATE_PATH}/src/TemplateGame.cpp" "$projPath/src/$className.cpp"
replace TemplateGame "$className" "$projPath/src/$className.h"
replace TemplateGame "$className" "$projPath/src/$className.cpp"

# Copy resource files
cp "${TEMPLATE_PATH}/res/"* "$projPath/res/"

# Copy icon
cp "${TEMPLATE_PATH}/icon.png" "$projPath/icon.png"

# Copy config
cp "${TEMPLATE_PATH}/game.config" "$projPath/game.config"
replace TEMPLATE_TITLE "$title" "$projPath/game.config"


if [[ "${OSTYPE}" == darwin* ]]; then
    # Open the new project folder
    open $projPath
fi

exit 0
