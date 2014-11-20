set(GAME_NAME sample-browser)

set(GAME_SRC
    src/Audio3DSample.cpp
    src/Audio3DSample.h
    src/BillboardSample.cpp
    src/BillboardSample.h
    src/CreateSceneSample.cpp
    src/CreateSceneSample.h
    src/FirstPersonCamera.cpp
    src/FirstPersonCamera.h
    src/FormsSample.cpp
    src/FormsSample.h
    src/GamepadSample.cpp
    src/GamepadSample.h
    src/GestureSample.cpp
    src/GestureSample.h
    src/Grid.cpp
    src/Grid.h
    src/InputSample.cpp
    src/InputSample.h
    src/LightSample.cpp
    src/LightSample.h
    src/LoadSceneSample.cpp
    src/LoadSceneSample.h
    src/MeshBatchSample.cpp
    src/MeshBatchSample.h
    src/MeshPrimitiveSample.cpp
    src/MeshPrimitiveSample.h
    src/PhysicsCollisionObjectSample.cpp
    src/PhysicsCollisionObjectSample.h
    src/PostProcessSample.cpp
    src/PostProcessSample.h
    src/SpriteBatchSample.cpp
    src/SpriteBatchSample.h
    src/TerrainSample.cpp
    src/TerrainSample.h
    src/Sample.cpp
    src/Sample.h
    src/SamplesGame.cpp
    src/SamplesGame.h
    src/TextSample.cpp
    src/TextSample.h
    src/TextureSample.cpp
    src/TextureSample.h
    src/TriangleSample.cpp
    src/TriangleSample.h
    src/WaterSample.cpp
    src/WaterSample.h
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

