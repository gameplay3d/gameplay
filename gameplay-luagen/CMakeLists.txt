cmake_minimum_required(VERSION 3.9)

set(LUAGEN_APP_SRC
    src/Base.h
    src/ClassBinding.cpp
    src/ClassBinding.h
    src/DebugNew.cpp
    src/DebugNew.h
    src/EnumBinding.h
    src/FunctionBinding.cpp
    src/FunctionBinding.h
    src/Generator.cpp
    src/Generator.h
    src/TypedefBinding.h
    src/main.cpp
)

include_directories( 
    ../external-deps/include
)

add_executable(gameplay-luagen ${LUAGEN_APP_SRC})

# platform specifics
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(gameplay-luagen
        imm32.lib
        version.lib
        imagehlp.lib
        winmm.lib
        xinput.lib
        $<$<CONFIG:Debug>:${PROJECT_SOURCE_DIR}/external-deps/lib/windows/x86_64/Debug/gameplay-deps.lib>
        $<$<CONFIG:Release>:${PROJECT_SOURCE_DIR}/external-deps/lib/windows/x86_64/Release/gameplay-deps.lib>
    )
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(gameplay-luagen
        ${PROJECT_SOURCE_DIR}/external-deps/lib/linux/x86_64/libgameplay-deps.a
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(gameplay-luagen
        ${PROJECT_SOURCE_DIR}/external-deps/lib/macos/x86_64/libgameplay-deps.a
    )
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

set_target_properties(gameplay-luagen PROPERTIES
    OUTPUT_NAME "gameplay-luagen"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(src FILES ${LUAGEN_APP_SRC})
