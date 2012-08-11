set(GAME_NAME sample00-mesh)

set(GAME_SRC
    src/MeshGame.cpp
    src/MeshGame.h
)

set(GAME_RES
    res/arial.ttf
    res/arial.gpb
    res/duck.dae
    res/duck.gpb
    res/duck.material
    res/duck.diffuse.png
    res/grid.material
)

set(GAME_RES_SHADERS
    res/shaders/colored-unlit.frag
    res/shaders/colored-unlit.vert
    res/shaders/colored.frag
    res/shaders/colored.vert
    res/shaders/textured-bumped.frag
    res/shaders/textured-bumped.vert
    res/shaders/textured-unlit.frag
    res/shaders/textured-unlit.vert
    res/shaders/textured.frag
    res/shaders/textured.vert
)

set(GAME_RES_SHADERS_LIB
    res/shaders/lib/attributes-skinning.vert
    res/shaders/lib/attributes.vert
    res/shaders/lib/lighting-directional.frag
    res/shaders/lib/lighting-directional.vert
    res/shaders/lib/lighting-point.frag
    res/shaders/lib/lighting-point.vert
    res/shaders/lib/lighting-spot.frag
    res/shaders/lib/lighting-spot.vert
    res/shaders/lib/lighting.frag
)

add_executable(${GAME_NAME}
    ${GAME_SRC}
)

target_link_libraries(${GAME_NAME} ${GAMEPLAY_LIBRARIES})

set_target_properties(${GAME_NAME} PROPERTIES
    OUTPUT_NAME "${GAME_NAME}"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(res FILES ${GAME_RES} ${GAMEPLAY_RES} ${GAME_RES_SHADERS} ${GAME_RES_SHADERS_LIB})
source_group(src FILES ${GAME_SRC})

#EM: I'll leave the above untouched for now, the below is to setup the "res" folder for linux testing
COPY_RES( ${GAME_NAME} )
COPY_RES_EXTRA( ${GAME_NAME} ${CMAKE_SOURCE_DIR}/gameplay
    res/logo_powered_white.png 
    res/shaders/*
    )
    