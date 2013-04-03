include(BuildHelpers.CMakeLists.txt)

include_directories( 
    ${CMAKE_SOURCE_DIR}/gameplay/src
    ${CMAKE_SOURCE_DIR}/external-deps/lua/include
    ${CMAKE_SOURCE_DIR}/external-deps/bullet/include
    ${CMAKE_SOURCE_DIR}/external-deps/libpng/include
    ${CMAKE_SOURCE_DIR}/external-deps/oggvorbis/include
    ${CMAKE_SOURCE_DIR}/external-deps/zlib/include
    ${CMAKE_SOURCE_DIR}/external-deps/openal/include
    ${CMAKE_SOURCE_DIR}/external-deps/glew/include
)

add_definitions(-D__linux__)

link_directories(
    ${CMAKE_SOURCE_DIR}/external-deps/lua/lib/linux/${ARCH_DIR}
    ${CMAKE_SOURCE_DIR}/external-deps/zlib/lib/linux/${ARCH_DIR}
    ${CMAKE_SOURCE_DIR}/external-deps/libpng/lib/linux/${ARCH_DIR}
    ${CMAKE_SOURCE_DIR}/external-deps/bullet/lib/linux/${ARCH_DIR}
    ${CMAKE_SOURCE_DIR}/external-deps/oggvorbis/lib/linux/${ARCH_DIR}
    ${CMAKE_SOURCE_DIR}/external-deps/openal/lib/linux/${ARCH_DIR}
    ${CMAKE_SOURCE_DIR}/external-deps/glew/lib/linux/${ARCH_DIR}
)


set(GAMEPLAY_LIBRARIES
    gameplay
    m
    lua
    png
    z
    vorbis
    ogg
    BulletDynamics
    BulletCollision
    LinearMath
    openal
    GLEW
    GL
    rt
    dl
    X11
    pthread
) 

add_definitions(-lstdc++ -lgameplay -lm -llua -lz -lpng -lvorbis -logg -lBulletCollision -lBulletDynamics -lLinearMath -lopenal -LGLEW -lGL -lrt -ldl -lX11 -lpthread)

add_subdirectory(browser)
add_subdirectory(character)
add_subdirectory(longboard)
add_subdirectory(lua)
add_subdirectory(mesh)
add_subdirectory(particles)
add_subdirectory(racer)
add_subdirectory(spaceship)


