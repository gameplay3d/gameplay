# Locate FBX library
# This module defines
#  FBX_FOUND, if false, do not try to link to FBX 
#  FBX_LIBRARIES
#  FBX_INCLUDE_DIR, where to find FBX.h
#

#=============================================================================
# Copyright 2007-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(FBX_INCLUDE_DIR fbxsdk.h
  HINTS
  $ENV{FBX_DIR}
  /usr/include
)

FIND_LIBRARY(FBX_LIBRARY 
  NAMES fbxsdk
  HINTS
  $ENV{FBX_DIR}
  PATH_SUFFIXES gcc4/x86/release gcc4/x64/release
  PATHS
)


set(FBX_LIBRARIES ${FBX_LIBRARY})
set(FBX_INCLUDE_DIRS ${FBX_INCLUDE_DIR})

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set FBX_FOUND to TRUE if 
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FBX
                                  REQUIRED_VARS FBX_LIBRARIES FBX_INCLUDE_DIR
)

MARK_AS_ADVANCED(FBX_INCLUDE_DIR FBX_LIBRARIES FBX_LIBRARY)

