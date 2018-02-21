
set(GAMEPLAY_EDITOR_SRC
    src/Editor.cpp
    src/Editor.h
)

include_directories( 
    ../../gameplay/src
    ../../external-deps/include
)

add_executable(gameplay-editor ${GAMEPLAY_EDITOR_SRC})

# platform specifics
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    include_directories(
        ../../external-deps/include/compat/msvc
    )
    target_link_libraries(gameplay-editor
        gameplay
        $<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/external-deps/lib/windows/x86_64/Debug/gameplay-deps.lib>
        $<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/external-deps/lib/windows/x86_64/Release/gameplay-deps.lib>
        $ENV{VULKAN_SDK}/Lib/vulkan-1.lib
        version.lib
        winmm.lib
        xinput.lib
        d3d12.lib
        dxgi.lib
        d3dcompiler.lib
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(CARBON_LIBRARY Carbon)
    find_library(QUARTZ_LIBRARY Quartz)
    find_library(METAL_LIBRARY Metal)
    find_library(METALKIT_LIBRARY MetalKit)
    find_library(OPENAL_LIBRARY OpenAL)
    find_library(IOKIT_LIBRARY IOKit )
    find_library(GAMEKIT_LIBRARY GameKit)

    SET(FRAMEWORK_LIBRARIES
            ${FOUNDATION_LIBRARY}
            ${CARBON_LIBRARY}
            ${QUARTZ_LIBRARY}
            ${METAL_LIBRARY}
            ${METALKIT_LIBRARY}
            ${OPENAL_LIBRARY}
            ${IOKIT_LIBRARY}
            ${GAMEKIT_LIBRARY}
      )
    target_link_libraries(gameplay-editor
        gameplay
        ${PROJECT_SOURCE_DIR}/external-deps/lib/macos/x86_64/libgameplay-deps.a
        m
        dl
        pthread
        ${FRAMEWORK_LIBRARIES}
    )

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(gameplay-editor
        gameplay
        ${PROJECT_SOURCE_DIR}/external-deps/lib/linux/x86_64/libgameplay-deps.a
        m
        GL
        rt
        dl
        X11
        pthread
    )
    add_definitions(-D__linux__)
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

set_target_properties(gameplay-editor PROPERTIES
    OUTPUT_NAME "gameplay-editor"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(src FILES ${GAMEPLAY_EDITOR_SRC})
