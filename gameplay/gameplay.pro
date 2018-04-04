QT -= core gui
TARGET = gameplay
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
CONFIG -= qt
CONFIG(debug, debug|release): DEFINES += _DEBUG

SOURCES += \
    src/Activator.cpp \
    src/Animation.cpp \
    src/Asset.cpp \
    src/AssetLoader.cpp \
    src/AssetManager.cpp \
    src/Audio.cpp \
    src/AudioClip.cpp \
    src/AudioListener.cpp \
    src/AudioSource.cpp \
    src/BoundingBox.cpp \
    src/BoundingSphere.cpp \
    src/Buffer.cpp \
    src/Camera.cpp \
    src/CommandBuffer.cpp \
    src/Component.cpp \
    src/Curve.cpp \
    src/DescriptorSet.cpp \
    src/FileSystem.cpp \
    src/Frustum.cpp \
    src/Game.cpp \
    src/Graphics.cpp \
    src/Heightfield.cpp \
    src/Input.cpp \
    src/Light.cpp \
    src/Logger.cpp \
    src/Material.cpp \
    src/MathUtil.cpp \
    src/Matrix.cpp \
    src/Mesh.cpp \
    src/MeshInstance.cpp \
    src/Path.cpp \
    src/Physics.cpp \
    src/PhysicsCharacter.cpp \
    src/PhysicsCloth.cpp \
    src/PhysicsCollider.cpp \
    src/PhysicsJoint.cpp \
    src/PhysicsJointGeneric.cpp \
    src/PhysicsJointHinge.cpp \
    src/PhysicsJointSocket.cpp \
    src/PhysicsJointSpring.cpp \
    src/PhysicsMaterial.cpp \
    src/PhysicsRigidBody.cpp \
    src/PhysicsVehicle.cpp \
    src/Plane.cpp \
    src/Platform.cpp \
    src/Quaternion.cpp \
    src/Ray.cpp \
    src/Rectangle.cpp \
    src/Renderer.cpp \
    src/RenderPass.cpp \
    src/RenderPipeline.cpp \
    src/Sampler.cpp \
    src/Scene.cpp \
    src/SceneLoader.cpp \
    src/SceneObject.cpp \
    src/Script.cpp \
    src/Semaphore.cpp \
    src/Serializer.cpp \
    src/SerializerBinary.cpp \
    src/SerializerJson.cpp \
    src/Shader.cpp \
    src/Sprite.cpp \
    src/Text.cpp \
    src/Texture.cpp \
    src/TextureLoader.cpp \
    src/Tileset.cpp \
    src/Vector2.cpp \
    src/Vector3.cpp \
    src/Vector4.cpp \
    src/VertexLayout.cpp

HEADERS += \
    src/Activator.h \
    src/Animation.h \
    src/Asset.h \
    src/AssetLoader.h \
    src/AssetManager.h \
    src/Audio.h \
    src/AudioClip.h \
    src/AudioListener.h \
    src/AudioSource.h \
    src/Base.h \
    src/BoundingBox.h \
    src/BoundingSphere.h \
    src/Buffer.h \
    src/Camera.h \
    src/ColorBlendState.h \
    src/CommandBuffer.h \
    src/Component.h \
    src/Curve.h \
    src/DepthStencilState.h \
    src/DescriptorSet.h \
    src/FileSystem.h \
    src/Format.h \
    src/Frustum.h \
    src/Game.h \
    src/Graphics.h \
    src/GraphicsUtil.h \
    src/Heightfield.h \
    src/Light.h \
    src/Logger.h \
    src/Material.h \
    src/MathUtil.h \
    src/Matrix.h \
    src/Mesh.h \
    src/MeshInstance.h \
    src/Path.h \
    src/Physics.h \
    src/PhysicsCharacter.h \
    src/PhysicsCloth.h \
    src/PhysicsCollider.h \
    src/PhysicsJoint.h \
    src/PhysicsJointGeneric.h \
    src/PhysicsJointHinge.h \
    src/PhysicsJointSocket.h \
    src/PhysicsJointSpring.h \
    src/PhysicsMaterial.h \
    src/PhysicsRigidBody.h \
    src/PhysicsVehicle.h \
    src/Plane.h \
    src/Platform.h \
    src/Quaternion.h \
    src/RasterizerState.h \
    src/Ray.h \
    src/Rectangle.h \
    src/Renderer.h \
    src/RenderPass.h \
    src/RenderPipeline.h \
    src/Sampler.h \
    src/Scene.h \
    src/SceneLoader.h \
    src/SceneObject.h \
    src/Script.h \
    src/Semaphore.h \
    src/Serializable.h \
    src/Serializer.h \
    src/SerializerBinary.h \
    src/SerializerJson.h \
    src/Shader.h \
    src/Sprite.h \
    src/Stream.h \
    src/Text.h \
    src/Texture.h \
    src/TextureLoader.h \
    src/Tileset.h \
    src/Vector2.h \
    src/Vector3.h \
    src/Vector4.h \
    src/VertexLayout.h \
    src/gameplay.h 

INCLUDEPATH += $$PWD/../gameplay/src
INCLUDEPATH += $$PWD/../external-deps/include

win32 {
    DEFINES += _WINDOWS WIN32 _WIN32
    DEFINES += VK_USE_PLATFORM_WIN32_KHR
    INCLUDEPATH += $$(VULKAN_SDK)/Include
    QMAKE_CXXFLAGS_WARN_ON -= -w34100
    QMAKE_CXXFLAGS_WARN_ON -= -w34189
    QMAKE_CXXFLAGS_WARN_ON -= -w4302
    QMAKE_CXXFLAGS_WARN_ON -= -w4311
    QMAKE_CXXFLAGS_WARN_ON -= -w4244
}

linux {
    DEFINES += SDL_VIDEO_DRIVER_X11
    DEFINES += VK_USE_PLATFORM_XLIB_KHR
    INCLUDEPATH += $$(VULKAN_SDK)/include
    QMAKE_CXXFLAGS += -lstdc++ -pthread -w
}

macx {
    QMAKE_CXXFLAGS += -x c++ -x objective-c++ -stdlib=libc++ -w
    DEFINES += VK_USE_PLATFORM_MACOS_MVK
    INCLUDEPATH += "$$(HOME)/vulkansdk-macos-1.0.69.0/macOS/include"
}
