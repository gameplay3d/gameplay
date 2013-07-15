cmake_minimum_required(VERSION 2.6)

project(GamePlay)

set(GAMEPLAY_VERSION 1.7.0)
set(CMAKE_C_COMPILER_INIT g++)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# debug
message( "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}" )
if ( "${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG" )
    add_definitions(-D_DEBUG)
endif()

# Check if this is a raspberry pi.
# If it is then it has BCMHOST
find_path(BCMHOST_INCLUDE_DIR bcm_host.h HINTS /opt/vc/include)
if ( BCMHOST_INCLUDE_DIR )
  set(RPI 1)
  add_definitions(-D__RPI__)
  message("Detected Raspberry pi")
  set(RPI_INCLUDE_DIR /opt/vc/include /opt/vc/include/interface/vcos/pthreads /opt/vc/include/interface/vmcs_host/linux)
  set(RPI_LIBRARIES /opt/vc/lib/libbcm_host.so)
endif()

find_package(Bullet)
find_package(Vorbis)
find_package(PNG)
find_package(Lua52)
find_package(OpenGLES2)

INCLUDE_DIRECTORIES(
  ${Bullet_INCLUDE_DIR}
  ${VORBIS_INCLUDE_DIR}
  ${PNG_INCLUDE_DIR}
  ${LUA_INCLUDE_DIR}
  ${OPENGLES2_INCLUDE_DIR}
  ${RPI_INCLUDE_DIR}
)

# gameplay library
add_subdirectory(gameplay)

# gameplay samples
add_subdirectory(samples)

# gameplay encoder
# A pre-compiled executable can be found in 'gameplay/bin'
# Uncomment out this line if you want to build the encoder instead of using the pre-compiled gameplay-encoder.
#add_subdirectory(tools/encoder)
