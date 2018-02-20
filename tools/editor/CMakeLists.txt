
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
    include_directories(
        ../../external-deps/include/compat/osx 
    )
    target_link_libraries(gameplay-editor
        ${PROJECT_SOURCE_DIR}/external-deps/lib/macosx/x86_64/libgameplay-deps.a
    )

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include_directories(
        ../../external-deps/include/compat/freebsd
    )
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

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

set_target_properties(gameplay-editor PROPERTIES
    OUTPUT_NAME "gameplay-editor"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(src FILES ${GAMEPLAY_EDITOR_SRC})
