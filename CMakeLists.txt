cmake_minimum_required(VERSION 2.6)

project(GamePlay)

set(GAMEPLAY_VERSION 1.5.0)

message( "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}" )
if( ${CMAKE_BUILD_TYPE} STREQUAL "DEBUG" )
    add_definitions(-D_DEBUG)
endif()

# gameplay library
add_subdirectory(gameplay)

# gameplay samples
add_subdirectory(gameplay-samples/sample00-mesh)
#add_subdirectory(gameplay-samples/sample01-longboard)
#add_subdirectory(gameplay-samples/sample02-spaceship)
#add_subdirectory(gameplay-samples/sample03-character)
#add_subdirectory(gameplay-samples/sample04-particles)
#add_subdirectory(gameplay-samples/sample05-lua)

