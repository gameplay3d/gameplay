cmake_minimum_required(VERSION 3.9)

project(GamePlay)

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

# projects
add_subdirectory(gameplay)
add_subdirectory(gameplay-app)
#add_subdirectory(gameplay-luagen)