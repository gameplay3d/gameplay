cmake_minimum_required(VERSION 3.5)

set(GAMEPLAY_SRC
    Behaviour.hpp
    Exporter.cpp)


if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

add_library(gameplayuser SHARED
    ${GAMEPLAY_SRC}
)

set_target_properties(gameplayuser PROPERTIES
    LINKER_LANGUAGE CXX
)

add_executable (example_main main.cpp)

target_link_libraries (example_main LINK_PUBLIC dl)