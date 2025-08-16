include(BuildHelpers.CMakeLists.txt)

include_directories(
        ${CMAKE_SOURCE_DIR}/gameplay/src
)

# Add external dependencies include path if using external deps
if(USE_EXTERNAL_DEPS)
    include_directories(${CMAKE_SOURCE_DIR}/external-deps/include)
    message(STATUS "Using external dependencies from external-deps for samples")
endif()

# Add system dependencies for samples
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
include_directories(${GTK2_INCLUDE_DIRS})

# Find Bullet Physics
if(PKG_CONFIG_FOUND)
    pkg_check_modules(BULLET bullet)
    pkg_check_modules(LUA lua5.3)
endif()

if(BULLET_FOUND)
    include_directories(${BULLET_INCLUDE_DIRS})
else()
    # Fallback: look for Bullet in common system locations
    find_path(BULLET_INCLUDE_DIR btBulletDynamicsCommon.h
        PATHS /usr/include/bullet /usr/local/include/bullet
    )
    if(BULLET_INCLUDE_DIR)
        include_directories(${BULLET_INCLUDE_DIR})
        message(STATUS "Found Bullet Physics headers for samples at: ${BULLET_INCLUDE_DIR}")
    endif()
endif()

if(LUA_FOUND)
    include_directories(${LUA_INCLUDE_DIRS})
    # Create a lua directory in the build folder with a compatibility header
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lua")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/lua/lua.hpp" "#include <lua5.3/lua.hpp>\n")
    include_directories("${CMAKE_CURRENT_BINARY_DIR}")
else()
    # Fallback: look for Lua in common system locations
    find_path(LUA_INCLUDE_DIR lua.hpp
        PATHS /usr/include/lua5.3 /usr/include/lua5.2 /usr/include/lua5.1 /usr/local/include/lua5.3
    )
    if(LUA_INCLUDE_DIR)
        include_directories(${LUA_INCLUDE_DIR})
        # Create a lua directory in the build folder with a compatibility header
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lua")
        file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/lua/lua.hpp" "#include <lua5.3/lua.hpp>\n")
        include_directories("${CMAKE_CURRENT_BINARY_DIR}")
        message(STATUS "Found Lua headers for samples at: ${LUA_INCLUDE_DIR}")
    endif()
endif()

# Add Lua compatibility defines for all samples
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
add_definitions(
    -DluaL_checkint=luaL_checkinteger
    -DluaL_checklong=luaL_checkinteger
    -DluaL_checkunsigned=luaL_checkinteger
    -Dlua_pushunsigned=lua_pushinteger
)
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_package(OpenGL REQUIRED)
    FIND_LIBRARY(AGL_LIBRARY AGL)
    FIND_LIBRARY(APP_SERVICES_LIBRARY ApplicationServices )
    FIND_LIBRARY(ATBOX_LIBRARY AudioToolbox)
    FIND_LIBRARY(CARBON_LIBRARY Carbon)
    FIND_LIBRARY(CAUDIO_LIBRARY CoreAudio)
    FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo)
    FIND_LIBRARY(CFOUNDATION_LIBRARY CoreFoundation)
    FIND_LIBRARY(CSERVICES_LIBRARY CoreServices)
    FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
    FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime )
    FIND_LIBRARY(IOKIT_LIBRARY IOKit )
    FIND_LIBRARY(AVF_LIBRARY AVFoundation)
    FIND_LIBRARY(OAL_LIBRARY OpenAL)
    FIND_LIBRARY(GKIT_LIBRARY GameKit)
    SET(FRAMEWORK_LIBRS
            ${AGL_LIBRARY}
            ${APP_SERVICES_LIBRARY}
            ${ATBOX_LIBRARY}
            ${CARBON_LIBRARY}
            ${CAUDIO_LIBRARY}
            ${COREVIDEO_LIBRARY}
            ${CFOUNDATION_LIBRARY}
            ${CSERVICES_LIBRARY}
            ${OAL_LIBRARY}
            ${OPENGL_LIBRARIES}
            ${GKIT_LIBRARY}
            ${IOKIT_LIBRARY}
            "-framework Foundation"
            "-framework Cocoa")
    # System libraries will be found by CMake, no need for specific link directories
    set(GAMEPLAY_LIBRARIES
            stdc++
            gameplay
            BulletDynamics
            BulletCollision
            LinearMath
            lua5.3
            m
            dl
            pthread
            ${FRAMEWORK_LIBRS}
            )
ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-D__linux__)

    # Choose libraries based on whether external dependencies are available
    if(USE_EXTERNAL_DEPS AND EXISTS "${CMAKE_SOURCE_DIR}/external-deps/lib")
        message(STATUS "Using external dependencies libraries for samples")
        link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib)
        
        set(GAMEPLAY_LIBRARIES
                stdc++
                gameplay
                gameplay-deps
                m
                dl
                pthread
                X11
                ${GTK2_LIBRARIES}
                )
    else()
        message(STATUS "Using system libraries for samples")
        # System libraries will be found by CMake, no need for specific link directories

        set(GAMEPLAY_LIBRARIES
                stdc++
                gameplay
                BulletDynamics
                BulletCollision
                LinearMath
                lua5.3
                png
                z
                m
                GL
                GLEW
                openal
                vorbis
                vorbisfile
                rt
                dl
                X11
                pthread
                gtk-x11-2.0
                glib-2.0
                gobject-2.0
                )
    endif()
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_definitions(-std=c++11)

add_subdirectory(browser)
add_subdirectory(character)
add_subdirectory(racer)
add_subdirectory(spaceship)
