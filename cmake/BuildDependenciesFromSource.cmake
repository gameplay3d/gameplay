# =============================================================================
# BUILD DEPENDENCIES FROM SOURCE
# =============================================================================

message(STATUS "Setting up external dependencies from source...")

# Download gameplay-deps repository using git clone
FetchContent_Declare(
    gameplay_deps
    GIT_REPOSITORY https://github.com/gameplay3d/gameplay-deps.git
    GIT_TAG        master
    SOURCE_DIR     ${CMAKE_SOURCE_DIR}/external-deps
)

FetchContent_GetProperties(gameplay_deps)
if(NOT gameplay_deps_POPULATED)
    message(STATUS "Downloading gameplay-deps repository...")
    FetchContent_Populate(gameplay_deps)
endif()

# Check if external dependencies need to be built
set(DEPS_LIB_PATH "${CMAKE_SOURCE_DIR}/external-deps/out/external-deps/lib/linux/x86_64/libgameplay-deps.a")

if(NOT EXISTS "${DEPS_LIB_PATH}")
    message(STATUS "Building external dependencies from source...")
    
    # Configure external dependencies with C++11 and disabled FluidSynth
    execute_process(
        COMMAND ${CMAKE_COMMAND} -S . -B build 
            -DCMAKE_CXX_STANDARD=11 
            -DALSOFT_MIDI_FLUIDSYNTH=OFF
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external-deps
        RESULT_VARIABLE deps_config_result
        OUTPUT_QUIET
        ERROR_QUIET
    )
    
    if(deps_config_result EQUAL 0)
        # Build external dependencies
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build build
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/external-deps
            RESULT_VARIABLE deps_build_result
            OUTPUT_QUIET
            ERROR_QUIET
        )
        
        if(deps_build_result EQUAL 0)
            message(STATUS "✓ External dependencies built successfully from source")
            set(USE_EXTERNAL_DEPS ON CACHE BOOL "" FORCE)
            set(DEPS_SUCCESS TRUE CACHE BOOL "" FORCE)
        else()
            message(STATUS "✗ Failed to build external dependencies from source")
            set(DEPS_SUCCESS FALSE CACHE BOOL "" FORCE)
        endif()
    else()
        message(STATUS "✗ Failed to configure external dependencies from source")
        set(DEPS_SUCCESS FALSE CACHE BOOL "" FORCE)
    endif()
else()
    message(STATUS "✓ Using existing external dependencies from source")
    set(USE_EXTERNAL_DEPS ON CACHE BOOL "" FORCE)
    set(DEPS_SUCCESS TRUE CACHE BOOL "" FORCE)
endif()
