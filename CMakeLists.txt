cmake_minimum_required(VERSION 2.6)

project(GamePlay)

set(GAMEPLAY_VERSION 1.5.0)
set(CMAKE_C_COMPILER_INIT g++)

# debug
message( "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}" )
if( "${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG" )
    add_definitions(-D_DEBUG)
endif()

# gameplay library
add_subdirectory(gameplay)

# gameplay samples
add_subdirectory(gameplay-samples)

# gameplay samples
add_subdirectory(gameplay-tests)

# gameplay encoder (See gameplay/bin)
#add_subdirectory(gameplay-encoder)


