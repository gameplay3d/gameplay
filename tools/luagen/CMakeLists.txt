
include_directories( 
    ${CMAKE_SOURCE_DIR}/external-deps/lua/include
    ${CMAKE_SOURCE_DIR}/external-deps/tinyxml2/include
    /usr/include
)

add_definitions(-D__linux__)

link_directories(
    ${CMAKE_SOURCE_DIR}/external-deps/lua/lib/linux/${ARCH_DIR}
    ${CMAKE_SOURCE_DIR}/external-deps/tinyxml2/lib/linux
    /usr/lib
)

set(APP_LIBRARIES
    lua
    tinyxml2
) 

add_definitions(-lstdc++ -llua -ltinyxml2)

set( APP_NAME gameplay-luagen )

set(APP_SRC
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
    src/main.cpp
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

