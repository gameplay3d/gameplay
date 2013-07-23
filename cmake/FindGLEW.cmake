# Locate GLEW library
# This module defines
#  GLEW_FOUND, if false, do not try to link to GLEW 
#  GLEW_LIBRARIES
#  GLEW_INCLUDE_DIR, where to find glew.h
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

FIND_PATH(GLEW_INCLUDE_DIR glew.h
  HINTS
  $ENV{GLEW_DIR}
  PATH_SUFFIXES include/GL include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
)

FIND_LIBRARY(GLEW_LIBRARY 
  NAMES GLEW
  HINTS
  $ENV{GLEW_DIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /sw
  /opt/local
  /opt/csw
  /opt
)


set(GLEW_LIBRARIES ${GLEW_LIBRARY})
set(GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GLEW_FOUND to TRUE if 
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW
                                  REQUIRED_VARS GLEW_LIBRARIES GLEW_INCLUDE_DIR
)

MARK_AS_ADVANCED(GLEW_INCLUDE_DIR GLEW_LIBRARIES GLEW_LIBRARY)

