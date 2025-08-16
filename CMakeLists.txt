cmake_minimum_required(VERSION 3.11)  # Required for FetchContent

project(GamePlay)

set(GAMEPLAY_VERSION 3.0.0)

# Include FetchContent module for downloading dependencies
include(FetchContent)

# =============================================================================
# DEPENDENCY MANAGEMENT OPTIONS
# =============================================================================

# Dependency management options
set(DEPENDENCY_MODE "AUTO" CACHE STRING "Choose dependency management mode: AUTO, SOURCE, SYSTEM")
set_property(CACHE DEPENDENCY_MODE PROPERTY STRINGS AUTO SOURCE SYSTEM)

option(USE_EXTERNAL_DEPS "Use external dependencies (AUTO mode only)" ON)

message(STATUS "Dependency mode: ${DEPENDENCY_MODE}")

# =============================================================================
# DEPENDENCY HANDLING
# =============================================================================

set(DEPS_SUCCESS FALSE)

if(DEPENDENCY_MODE STREQUAL "SYSTEM")
    message(STATUS "Using system libraries...")
    set(USE_EXTERNAL_DEPS OFF)
    set(DEPS_SUCCESS TRUE)
    
elseif(DEPENDENCY_MODE STREQUAL "SOURCE")
    message(STATUS "Building dependencies from source...")
    include(cmake/BuildDependenciesFromSource.cmake)
    
elseif(DEPENDENCY_MODE STREQUAL "AUTO")
    message(STATUS "AUTO mode: Trying source build -> system libraries...")
    
    # First try: Build from source
    message(STATUS "Attempting to build from source...")
    include(cmake/BuildDependenciesFromSource.cmake)
    
    if(NOT DEPS_SUCCESS)
        message(STATUS "Source build failed, falling back to system libraries...")
        set(USE_EXTERNAL_DEPS OFF)
        set(DEPS_SUCCESS TRUE)
    endif()
    
else()
    message(FATAL_ERROR "Invalid DEPENDENCY_MODE: ${DEPENDENCY_MODE}. Valid options are: AUTO, SOURCE, SYSTEM")
endif()

message(STATUS "Using external dependencies: ${USE_EXTERNAL_DEPS}")

# =============================================================================
# COMMON PROJECT SETTINGS
# =============================================================================

# debug
message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    add_definitions(-D_DEBUG)
endif()

# architecture
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_DIR "x64")
else()
    set(ARCH_DIR "x86")
endif()

# =============================================================================
# PROJECT COMPONENTS
# =============================================================================

# gameplay library
add_subdirectory(gameplay)

# gameplay samples
add_subdirectory(samples)

# gameplay encoder
# A pre-compiled executable can be found in 'gameplay/bin'. Uncomment to build yourself.
#add_subdirectory(tools/encoder)
#add_subdirectory(tools/luagen)
