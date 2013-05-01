cmake_minimum_required(VERSION 2.6)

project(GamePlay)

set(GAMEPLAY_VERSION 1.6.0)
set(CMAKE_C_COMPILER_INIT g++)

# debug
message( "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}" )
if ( "${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG" )
    add_definitions(-D_DEBUG)
endif()

# architecture
if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
set(ARCH_DIR "x64")
else()
set(ARCH_DIR "x86")
endif()

# gameplay library
add_subdirectory(gameplay)

# gameplay samples
add_subdirectory(samples)

# gameplay encoder
# A pre-compiled executable can be found in 'gameplay/bin'
# Uncomment out this line if you want to build the encoder instead of using the pre-compiled gameplay-encoder.
#add_subdirectory(tools/encoder)
