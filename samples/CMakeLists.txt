include(BuildHelpers.CMakeLists.txt)

include_directories( 
    ${CMAKE_SOURCE_DIR}/gameplay/src
)


link_directories(
  ${Bullet_LIB_DIR}
  ${VORBIS_LIB_DIR}
  ${PNG_LIB_DIR}
  ${LUA_LIB_DIR}
  ${OPENGLES2_LIB_DIR}
  ${RPI_LIB_DIR}
)

set(GAMEPLAY_LIBRARIES
    gameplay
    m
    ${LUA_LIBRARIES}
    ${PNG_LIBRARIES}
    z
    ${VORBIS_LIBRARIES}
    ${BULLET_LIBRARIES}
    ${OPENGLES2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${EGL_LIBRARIES}
    openal
    rt
    dl
    pthread
    ${RPI_LIBRARIES}
)


add_subdirectory(browser)
add_subdirectory(character)
add_subdirectory(longboard)
add_subdirectory(lua)
add_subdirectory(mesh)
add_subdirectory(particles)
add_subdirectory(racer)
add_subdirectory(spaceship)


