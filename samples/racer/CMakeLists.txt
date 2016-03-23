set(GAME_NAME sample-racer)

set(GAME_SRC
        src/RacerGame.cpp
        src/RacerGame.h
        )

if(NOT ${APPLE})
    set_target_properties(${GAME_NAME} PROPERTIES
            OUTPUT_NAME "${GAME_NAME}"
            CLEAN_DIRECT_OUTPUT 1
            )
endif(NOT ${APPLE})

source_group(res FILES ${GAME_RES} ${GAMEPLAY_RES} ${GAMEPLAY_RES_SHADERS} ${GAMEPLAY_RES_UI})
source_group(src FILES ${GAME_SRC})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    COPY_RES_MAC(GAME_RES ${CMAKE_SOURCE_DIR}/samples/racer res/*)
    COPY_RES_MAC(GAMEPLAY_RES ${CMAKE_SOURCE_DIR}/gameplay
            res/shaders/* res/ui/* res/logo_powered_white.png)
    set(Apple_Resources
            ${GAME_RES}
            ${GAMEPLAY_RES}
            game.config)
    SET(EXEC_TYPE MACOSX_BUNDLE)

    SET_SOURCE_FILES_PROPERTIES(
            game.config
            PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources
    )
    set( MACOSX_BUNDLE_INFO_STRING "\"${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}\",\nCopyright 2016 gameplay3d contributors" )
    set( MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}" )
    set( MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
    set( MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
    set( MACOSX_BUNDLE_COPYRIGHT "(C) gameplay3d contributors" )
    set( PLIST_TEMPLATE sample-racer-macosx.plist )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_executable(${GAME_NAME} ${EXEC_TYPE}
        ${GAME_SRC}
        ${Apple_Resources}
        )
target_link_libraries(${GAME_NAME} ${GAMEPLAY_LIBRARIES})

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


    COPY_RES( ${GAME_NAME} )
    COPY_RES_EXTRA( ${GAME_NAME} ${CMAKE_SOURCE_DIR}/gameplay
            res/logo_powered_white.png
            res/shaders/*
            res/ui/*
            )

endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
# Just use the PNG config file (most compatible)
configure_file( game.png.config game.config COPYONLY )