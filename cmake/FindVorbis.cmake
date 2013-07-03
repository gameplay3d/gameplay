# - Try to find the ogg vorbis library
#
#  This module defines the following variables
#
#  VORBIS_FOUND - Was vorbis found
#  VORBIS_INCLUDE_DIRS - the vorbis include directories
#  VORBIS_LIBRARIES - Link to this, by default it includes
#                     all vorbis components
#
#  This module accepts the following variables
#
#  VORBIS_ROOT - Can be set to vorbis install path or Windows build path
#

macro(_FIND_VORBIS_LIBRARY _var)
  find_library(${_var}
     NAMES 
        ${ARGN}
     HINTS
        ${VORBIS_ROOT}
        ${VORBIS_ROOT}/out/release8/libs
        ${VORBIS_ROOT}/out/debug8/libs
     PATH_SUFFIXES lib
  )
  mark_as_advanced(${_var})
endmacro()

macro(_VORBIS_APPEND_LIBRARIES _list _release)
   set(_debug ${_release}_DEBUG)
   if(${_debug})
      set(${_list} ${${_list}} optimized ${${_release}} debug ${${_debug}})
   else()
      set(${_list} ${${_list}} ${${_release}})
   endif()
endmacro()

find_path(VORBIS_INCLUDE_DIR NAMES vorbisenc.h
  HINTS
    ${VORBIS_ROOT}/include
  PATH_SUFFIXES vorbis
)

# Find the libraries

_FIND_VORBIS_LIBRARY(VORBIS_LIBRARY        vorbis)
_FIND_VORBIS_LIBRARY(VORBIS_LIBRARY_DEBUG  vorbis_Debug vorbis_d)


# handle the QUIETLY and REQUIRED arguments and set VORBIS_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vorbis DEFAULT_MSG
    VORBIS_LIBRARY 
    VORBIS_INCLUDE_DIR)

set(VORBIS_INCLUDE_DIRS ${VORBIS_INCLUDE_DIR})
if(VORBIS_FOUND)
   _VORBIS_APPEND_LIBRARIES(VORBIS_LIBRARIES VORBIS_LIBRARY)
endif()
