include(BuildHelpers.CMakeLists.txt)

include_directories( 
    ${CMAKE_SOURCE_DIR}/gameplay/src
    ${CMAKE_SOURCE_DIR}/external-deps/include
)

add_definitions(-D__linux__)

IF(ARCH_DIR STREQUAL "x64")
    link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib/linux/x86_64)
ELSE()
    link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib/linux/x86)
ENDIF(ARCH_DIR STREQUAL "x64")


set(GAMEPLAY_LIBRARIES
    stdc++
    gameplay
    gameplay-deps
    m
    GL
    rt
    dl
    X11
    pthread
    gtk-x11-2.0
    glib-2.0
    gobject-2.0
) 

add_definitions(-std=c++11)

add_subdirectory(browser)
add_subdirectory(character)
add_subdirectory(racer)
add_subdirectory(spaceship)
