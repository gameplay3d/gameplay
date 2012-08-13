set(GAME_NAME sample06-cubes)

set(GAME_SRC
	src/CubesGame.cpp
	src/CubesGame.h
)

add_executable(${GAME_NAME}
    ${GAME_SRC}
)

target_link_libraries(${GAME_NAME} ${GAMEPLAY_LIBRARIES})

set_target_properties(${GAME_NAME} PROPERTIES
    OUTPUT_NAME "${GAME_NAME}"
    CLEAN_DIRECT_OUTPUT 1
)

COPY_RES( ${GAME_NAME} )
