set(GAMEPLAY_SRC
    src/Animation.cpp
    src/Animation.h
    src/Audio.cpp
    src/Audio.h    
    src/AudioClip.cpp
    src/AudioClip.h
    src/AudioListener.cpp
    src/AudioListener.h
    src/AudioSource.cpp
    src/AudioSource.h
    src/Base.h
    src/BoundingBox.cpp
    src/BoundingBox.h
    src/BoundingSphere.cpp
    src/BoundingSphere.h
    src/Camera.cpp
    src/Camera.h
    src/Command.cpp
    src/Command.h
    src/Component.cpp
    src/Component.h
    src/Curve.cpp
    src/Curve.h
    src/FileSystem.cpp
    src/FileSystem.h
    src/Frustum.cpp
    src/Frustum.h
    src/Game.cpp
    src/Game.h
    src/Graphics.cpp
    src/Graphics.h
    src/Heightfield.cpp
    src/Heightfield.h
    src/Image.cpp
    src/Image.h
    src/Input.cpp
    src/Input.h
    src/Light.cpp
    src/Light.h
    src/Logger.cpp
    src/Logger.h
    src/Material.cpp
    src/Material.h
    src/MathUtil.cpp
    src/MathUtil.h
    src/Matrix.cpp
    src/Matrix.h
    src/Mesh.cpp
    src/Mesh.h
    src/Path.cpp
    src/Path.h
    src/Physics.cpp
    src/Physics.h
    src/PhysicsCharacter.cpp
    src/PhysicsCharacter.h
    src/PhysicsCloth.cpp
    src/PhysicsCloth.h
    src/PhysicsCollider.cpp
    src/PhysicsCollider.h
    src/PhysicsJoint.cpp
    src/PhysicsJoint.h
    src/PhysicsJointFixed.cpp
    src/PhysicsJointFixed.h
    src/PhysicsJointGeneric.cpp
    src/PhysicsJointGeneric.h
    src/PhysicsJointHinge.cpp
    src/PhysicsJointHinge.h
    src/PhysicsJointSocket.cpp
    src/PhysicsJointSocket.h
    src/PhysicsJointSpring.cpp
    src/PhysicsJointSpring.h
    src/PhysicsMaterial.cpp
    src/PhysicsMaterial.h
    src/PhysicsRigidBody.cpp
    src/PhysicsRigidBody.h
    src/PhysicsVehicle.cpp
    src/PhysicsVehicle.h
    src/Plane.cpp
    src/Plane.h
    src/Platform.cpp
    src/Platform.h
    src/Quaternion.cpp
    src/Quaternion.h
    src/Ray.cpp
    src/Ray.h
    src/Rectangle.cpp
    src/Rectangle.h
    src/Renderer.cpp
    src/Renderer.h
    src/SceneObject.cpp
    src/SceneObject.h
    src/Script.cpp
    src/Script.h
    src/Serializable.h
    src/Serializer.cpp
    src/Serializer.h
    src/SerializerBinary.cpp
    src/SerializerBinary.h
    src/SerializerJson.cpp
    src/SerializerJson.h
    src/Sprite.cpp
    src/Sprite.h
    src/Text.cpp
    src/Text.h
    src/Tileset.cpp
    src/Tileset.h
    src/Vector2.cpp
    src/Vector2.h
    src/Vector3.cpp
    src/Vector3.h
    src/Vector4.cpp
    src/Vector4.h
)

include_directories(
    src
    ../external-deps/include
)


if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(GAMEPLAY_PLATFORM_SRC
        src/PlatformWindows.cpp
    )
    include_directories(
        ../external-deps/include/compat/msvc
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(GAMEPLAY_PLATFORM_SRC 
        src/PlatformMacOS.mm
    )
    include_directories(
        ../external-deps/include/compat/osx 
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(GAMEPLAY_PLATFORM_SRC
        src/PlatformLinux.cpp
    )
    include_directories(
        ../external-deps/include/compat/freebsd
    )
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

add_library(gameplay STATIC
    ${GAMEPLAY_SRC}
    ${GAMEPLAY_PLATFORM_SRC}
)

set_target_properties(gameplay PROPERTIES
    OUTPUT_NAME "gameplay"
    CLEAN_DIRECT_OUTPUT 1
    VERSION ${GAMEPLAY_VERSION}
)

source_group(src FILES ${GAMEPLAY_SRC} ${GAMEPLAY_PLATFORM_SRC})
#source_group(lua FILES ${GAMEPLAY_LUA})
#source_group(res FILES ${GAMEPLAY_RES} ${GAMEPLAY_RES_SHADERS} ${GAMEPLAY_RES_FONTS} ${GAMEPLAY_RES_IMAGES})
