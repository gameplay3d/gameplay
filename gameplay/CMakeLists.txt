cmake_minimum_required(VERSION 3.5)

set(GAMEPLAY_SRC
    src/Activator.cpp
    src/Activator.h
    src/Animation.cpp
    src/Animation.h
    src/Asset.cpp
    src/Asset.h
    src/AssetLoader.cpp
    src/AssetLoader.h
    src/AssetManager.cpp
    src/AssetManager.h
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
    src/Buffer.cpp
    src/Buffer.h
    src/Camera.cpp
    src/Camera.h
    src/ColorBlendState.h
    src/CommandBuffer.cpp
    src/CommandBuffer.h
    src/Component.cpp
    src/Component.h
    src/Curve.cpp
    src/Curve.h
    src/DepthStencilState.h
    src/DescriptorSet.cpp
    src/DescriptorSet.h
    src/FileSystem.cpp
    src/FileSystem.h
    src/Format.h
    src/Frustum.cpp
    src/Frustum.h
    src/Game.cpp
    src/Game.h
    src/Graphics.cpp
    src/Graphics.h
    src/GraphicsUtil.h
    src/Heightfield.cpp
    src/Heightfield.h
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
    src/MeshInstance.cpp
    src/MeshInstance.h
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
    src/RasterizerState.h
    src/Ray.cpp
    src/Ray.h
    src/Rectangle.cpp
    src/Rectangle.h
    src/Renderer.cpp
    src/Renderer.h
    src/RenderPass.cpp
    src/RenderPass.h
    src/RenderPipeline.cpp
    src/RenderPipeline.h
    src/Sampler.cpp
    src/Sampler.h
    src/Scene.cpp
    src/Scene.h
    src/SceneLoader.cpp
    src/SceneLoader.h
    src/SceneObject.cpp
    src/SceneObject.h
    src/Script.cpp
    src/Script.h
    src/Semaphore.cpp
    src/Semaphore.h
    src/Serializable.h
    src/Serializer.cpp
    src/Serializer.h
    src/SerializerBinary.cpp
    src/SerializerBinary.h
    src/SerializerJson.cpp
    src/SerializerJson.h
    src/Shader.cpp
    src/Shader.h
    src/Sprite.cpp
    src/Sprite.h
    src/Text.cpp
    src/Text.h
    src/Texture.cpp
    src/Texture.h
    src/TextureLoader.cpp
    src/TextureLoader.h
    src/Tileset.cpp
    src/Tileset.h
    src/Vector2.cpp
    src/Vector2.h
    src/Vector3.cpp
    src/Vector3.h
    src/Vector4.cpp
    src/Vector4.h
    src/VertexLayout.cpp
    src/VertexLayout.h
    src/gameplay.h
)

include_directories(
    src
    ../external-deps/include
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
    include_directories( 
        $ENV{VULKAN_SDK}/Include
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-DSDL_VIDEO_DRIVER_X11)
    add_definitions(-DVK_USE_PLATFORM_XLIB_KHR)
    include_directories( 
        $ENV{VULKAN_SDK}/include
    )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
    include_directories( 
        $ENV{HOME}/vulkansdk-macos-1.0.69.0/macOS/include
    )
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

add_library(gameplay STATIC
    ${GAMEPLAY_SRC}
)

set_target_properties(gameplay PROPERTIES
    OUTPUT_NAME "gameplay"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(src FILES ${GAMEPLAY_SRC})
#source_group(lua FILES ${GAMEPLAY_LUA})
#source_group(res FILES ${GAMEPLAY_RES} ${GAMEPLAY_RES_SHADERS} ${GAMEPLAY_RES_FONTS} ${GAMEPLAY_RES_IMAGES})
