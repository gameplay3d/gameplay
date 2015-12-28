set(GAME_NAME sample-racer)

set(GAME_SRC
        src/RacerGame.cpp
        src/RacerGame.h
        )

if(!${APPLE})
    set_target_properties(${GAME_NAME} PROPERTIES
            OUTPUT_NAME "${GAME_NAME}"
            CLEAN_DIRECT_OUTPUT 1
            )
endif(!${APPLE})

source_group(res FILES ${GAME_RES} ${GAMEPLAY_RES} ${GAMEPLAY_RES_SHADERS} ${GAMEPLAY_RES_UI})
source_group(src FILES ${GAME_SRC})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    MAKE_ABSOLUTE(GAMEPLAY_RES_SHADERS  ${CMAKE_SOURCE_DIR}/gameplay
            res/shaders/*)
    MAKE_ABSOLUTE(GAMEPLAY_RES_UI  ${CMAKE_SOURCE_DIR}/gameplay
            res/ui/*)
    MESSAGE("GP Shaders ${GAMEPLAY_RES_SHADERS}")
#    include(BundleUtilities)
#    SET(GAME_RES res/png/car.png)
    COPY_RES_MAC(GAME_RES ${CMAKE_SOURCE_DIR}/samples/racer res/*)
    message("Resources are ${GAME_RES} !!!")
    set(Apple_Resources
            ../../gameplay/res/logo_powered_white.png
            ${GAME_RES}
            ${GAMEPLAY_RES}
            ${GAMEPLAY_RES_SHADERS}
            ${GAMEPLAY_RES_UI}
            game.config)
    SET(EXEC_TYPE MACOSX_BUNDLE)
    SET_SOURCE_FILES_PROPERTIES(
            ../../gameplay/res/logo_powered_white.png
            PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/res
    )
    SET_SOURCE_FILES_PROPERTIES(
            ${GAMEPLAY_RES_SHADERS}
            PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/res/shaders
    )
    SET_SOURCE_FILES_PROPERTIES(
            ${GAMEPLAY_RES_UI}
            PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources/res/ui
    )
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
    configure_file( game.png.config game.config COPYONLY )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_executable(${GAME_NAME} ${EXEC_TYPE}
        ${GAME_SRC}
        ${Apple_Resources}
        )
target_link_libraries(${GAME_NAME} ${GAMEPLAY_LIBRARIES})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #    SET_TARGET_PROPERTIES( ${GAME_NAME}
    #            PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/samples/racer/${PLIST_TEMPLATE}" )
    #    install(TARGETS ${GAME_NAME}
    #            RUNTIME DESTINATION bin
    #            BUNDLE DESTINATION bundle
    #            )
#    GET_BUNDLE_AND_EXECUTABLE(${GAME_NAME} BUNDLE_NAME EXEC_NAME VALID_TAG)
#    message("Bundle name is ${BUNDLE_NAME} valid ${VALID_TAG}")
#    GET_DOTAPP_DIR(${GAME_NAME} DIR_NAME)
#    message(".app dirr name is ${DIR_NAME}")
    configure_file( game.png.config game.config COPYONLY )
else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


    COPY_RES( ${GAME_NAME} )
    COPY_RES_EXTRA( ${GAME_NAME} ${CMAKE_SOURCE_DIR}/gameplay
            res/logo_powered_white.png
            res/shaders/*
            res/ui/*
            )

    # Just use the PNG config file (most compatible)
    configure_file( game.png.config game.config COPYONLY )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")