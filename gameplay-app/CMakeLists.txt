set(GAMEPLAY_APP_SRC
    src/App.cpp
    src/App.h
)

include_directories( 
    ../gameplay/src
    ../external-deps/include
)

# platform specifics
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
    include_directories( 
        $ENV{VULKAN_SDK}/Include
    )
    set(GAMEPLAY_APP_PLATFORM_SRC
        src/main-windows.cpp
    )
    add_executable(gameplay-app ${GAMEPLAY_APP_SRC} ${GAMEPLAY_APP_PLATFORM_SRC})
    target_link_libraries(gameplay-app
        imm32.lib
        version.lib
        imagehlp.lib
        winmm.lib
        xinput.lib
        gameplay
        $<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/external-deps/lib/windows/x86_64/Debug/gameplay-deps.lib>
        $<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/external-deps/lib/windows/x86_64/Release/gameplay-deps.lib>
        $ENV{VULKAN_SDK}/Lib/vulkan-1.lib
    )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DVK_USE_PLATFORM_XCB_KHR)
    include_directories( 
        $ENV{VULKAN_SDK}/include
    )
    set(GAMEPLAY_APP_PLATFORM_SRC
        src/main-linux.cpp
    )
    add_executable(gameplay-app ${GAMEPLAY_APP_SRC} ${GAMEPLAY_APP_PLATFORM_SRC})
    target_link_libraries(gameplay-app
        gameplay
        ${PROJECT_SOURCE_DIR}/external-deps/lib/linux/x86_64/libgameplay-deps.a
        m
        GL
        rt
        dl
        X11
        pthread
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
    include_directories( 
        $ENV{VULKAN_SDK}/include
    )
    set(GAMEPLAY_APP_PLATFORM_SRC
        src/main-macos.mm
    )
    add_executable(gameplay-app ${GAMEPLAY_APP_SRC} ${GAMEPLAY_APP_PLATFORM_SRC})
    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(CARBON_LIBRARY Carbon)
    find_library(QUARTZ_LIBRARY Quartz)
    find_library(IOSURFACe_LIBRARY IOSurface)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(METAL_LIBRARY Metal)
    find_library(METALKIT_LIBRARY MetalKit)
    find_library(GAMEKIT_LIBRARY GameKit)
    find_library(OPENAL_LIBRARY OpenAL)

    SET(FRAMEWORK_LIBRARIES
        ${FOUNDATION_LIBRARY}
        ${CARBON_LIBRARY}
        ${QUARTZ_LIBRARY}
        ${IOSURFACe_LIBRARY}
        ${IOKIT_LIBRARY}
        ${METAL_LIBRARY}
        ${METALKIT_LIBRARY}
        ${GAMEKIT_LIBRARY}
        ${OPENAL_LIBRARY}
      )
    add_executable(gameplay-app ${GAMEPLAY_APP_SRC} ${GAMEPLAY_APP_PLATFORM_SRC})
    target_link_libraries(gameplay-app
        gameplay
        ${PROJECT_SOURCE_DIR}/external-deps/lib/macos/x86_64/libgameplay-deps.a
        m
        dl
        pthread
        ${FRAMEWORK_LIBRARIES}
    )
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

set_target_properties(gameplay-app PROPERTIES
    OUTPUT_NAME "gameplay-app"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(src FILES ${GAMEPLAY_APP_SRC} ${GAMEPLAY_APP_PLATFORM_SRC})
