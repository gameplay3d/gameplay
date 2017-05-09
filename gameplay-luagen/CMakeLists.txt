
include_directories( 
    ${CMAKE_SOURCE_DIR}/external-deps/include
)

add_definitions(-D__linux__)

IF(ARCH_DIR STREQUAL "x64")
    set(ARCH_DEPS_DIR "x86_64")
ELSE()
    set(ARCH_DEPS_DIR "x86")
ENDIF(ARCH_DIR STREQUAL "x64")

link_directories(
    ${CMAKE_SOURCE_DIR}/external-deps/lib/linux/${ARCH_DEPS_DIR}
)

set(APP_LIBRARIES
    gameplay-deps
) 

add_definitions(-std=c++11 -lstdc++ -lgameplay-deps)

set( APP_NAME gameplay-luagen )

set(APP_SRC
    src/main.cpp
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
)

add_executable(${APP_NAME}
    ${APP_SRC}
)

target_link_libraries(${APP_NAME} ${APP_LIBRARIES})

set_target_properties(${APP_NAME} PROPERTIES
    OUTPUT_NAME "${APP_NAME}"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(src FILES ${APP_SRC})

