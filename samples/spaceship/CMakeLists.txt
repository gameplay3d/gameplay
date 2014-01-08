set(GAME_NAME sample-spaceship)

set(GAME_SRC
    src/SpaceshipGame.cpp
    src/SpaceshipGame.h
)

add_executable(${GAME_NAME}
    ${GAME_SRC}
)

target_link_libraries(${GAME_NAME} ${GAMEPLAY_LIBRARIES})

set_target_properties(${GAME_NAME} PROPERTIES
    OUTPUT_NAME "${GAME_NAME}"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(res FILES ${GAME_RES} ${GAMEPLAY_RES} ${GAMEPLAY_RES_SHADERS} ${GAMEPLAY_RES_UI})
source_group(src FILES ${GAME_SRC})

COPY_RES( ${GAME_NAME} )
COPY_RES_EXTRA( ${GAME_NAME} ${CMAKE_SOURCE_DIR}/gameplay
    res/logo_powered_white.png 
    res/shaders/*
    res/ui/*
)
