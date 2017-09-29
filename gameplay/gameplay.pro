QT -= core gui
TARGET = gameplay
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
CONFIG -= qt
CONFIG(debug, debug|release): DEFINES += _DEBUG

SOURCES += \
    src/Animation.cpp \
    src/Audio.cpp \
    src/AudioClip.cpp \
    src/AudioListener.cpp \
    src/AudioSource.cpp \
    src/BoundingBox.cpp \
    src/BoundingSphere.cpp \
    src/Buffer.cpp \
    src/Camera.cpp \
    src/CommandList.cpp \
    src/CommandPool.cpp \
    src/Component.cpp \
    src/Curve.cpp \
    src/FileSystem.cpp \
    src/Frustum.cpp \
    src/Game.cpp \
    src/Graphics.cpp \
    src/Heightfield.cpp \
    src/Image.cpp \
    src/Input.cpp \
    src/Light.cpp \
    src/Logger.cpp \
    src/Material.cpp \
    src/MathUtil.cpp \
    src/Matrix.cpp \
    src/Mesh.cpp \
    src/Physics.cpp \
    src/PhysicsCharacter.cpp \
    src/PhysicsCloth.cpp \
    src/PhysicsCollisionShape.cpp \
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
    src/SceneObject.cpp \
    src/Script.cpp \
    src/Serializer.cpp \
    src/SerializerBinary.cpp \
    src/SerializerJson.cpp \
    src/Sprite.cpp \
    src/Text.cpp \
    src/Texture.cpp \
    src/Tileset.cpp \
    src/Vector2.cpp \
    src/Vector3.cpp \
    src/Vector4.cpp \
    src/VertexFormat.cpp

HEADERS += \
    src/Animation.h \
    src/Audio.h \
    src/AudioClip.h \
    src/AudioListener.h \
    src/AudioSource.h \
    src/Base.h \
    src/BoundingBox.h \
    src/BoundingSphere.h \
    src/Buffer.h \
    src/Camera.h \
    src/CommandList.h \
    src/CommandPool.h \
    src/Component.h \
    src/Curve.h \
    src/FileSystem.h \
    src/Frustum.h \
    src/Game.h \
    src/Graphics.h \
    src/Heightfield.h \
    src/Image.h \
    src/Light.h \
    src/Logger.h \
    src/Material.h \
    src/MathUtil.h \
    src/Matrix.h \
    src/Mesh.h \
    src/Path.h \
    src/Physics.h \
    src/PhysicsCharacter.h \
    src/PhysicsCloth.h \
    src/PhysicsCollisionShape.h \
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
    src/Ray.h \
    src/Rectangle.h \
    src/Renderer.h \
    src/Resource.h \
    src/SceneObject.h \
    src/Script.h \
    src/Serializable.h \
    src/Serializer.h \
    src/SerializerBinary.h \
    src/SerializerJson.h \
    src/Sprite.h \
    src/Stream.h \
    src/Text.h \
    src/Texture.h \
    src/Tileset.h \
    src/Vector2.h \
    src/Vector3.h \
    src/Vector4.h \
    src/VertexFormat.h \
    src/Tileset.h

INCLUDEPATH += $$PWD/../gameplay/src
INCLUDEPATH += $$PWD/../external-deps/include

win32 {
    DEFINES += _WINDOWS WIN32 _UNICODE UNICODE
    DEFINES += VK_USE_PLATFORM_WIN32_KHR
    SOURCES += src/PlatformWindows.cpp
    SOURCES += src/vk/GraphicsVK.cpp
    SOURCES += src/vk/BufferVK.cpp
    SOURCES += src/vk/CommandListVK.cpp
    SOURCES += src/vk/CommandPoolVK.cpp
    SOURCES += src/d3d12/GraphicsD3D12.cpp
    SOURCES += src/d3d12/BufferD3D12.cpp
    SOURCES += src/d3d12/CommandListD3D12.cpp
    SOURCES += src/d3d12/CommandPoolD3D12.cpp
    HEADERS += src/PlatformWindows.h
    HEADERS += src/vk/GraphicsVK.h
    HEADERS += src/vk/BufferVK.h
    HEADERS += src/vk/CommandListVK.h
    HEADERS += src/vk/CommandPoolVK.h
    HEADERS += src/d3d12/GraphicsD3D12.h
    HEADERS += src/d3d12/GraphicsD3D12.h
    HEADERS += src/d3d12/BufferD3D12.h
    HEADERS += src/d3d12/CommandListD3D12.h
    HEADERS += src/d3d12/CommandPoolD3D12.h
    INCLUDEPATH += src/vk
    INCLUDEPATH += src/d3d12
    INCLUDEPATH += $$(VULKAN_SDK)/Include
    QMAKE_CXXFLAGS_WARN_ON -= -w34100
    QMAKE_CXXFLAGS_WARN_ON -= -w34189
    QMAKE_CXXFLAGS_WARN_ON -= -w4302
    QMAKE_CXXFLAGS_WARN_ON -= -w4311
    QMAKE_CXXFLAGS_WARN_ON -= -w4244
}

linux {
    DEFINES += __linux__
    DEFINES += VK_USE_PLATFORM_XCB_KHR
    SOURCES += src/PlatformLinux.cpp
    SOURCES += src/vk/GraphicsVK.cpp
    SOURCES += src/vk/BufferVK.cpp
    SOURCES += src/vk/CommandListVK.cpp
    SOURCES += src/vk/CommandPoolVK.cpp
    HEADERS += src/PlatformLinux.h
    HEADERS += src/vk/GraphicsVK.h
    HEADERS += src/vk/BufferVK.h
    HEADERS += src/vk/CommandListVK.h
    HEADERS += src/vk/CommandPoolVK.h
    INCLUDEPATH += src/vk
    INCLUDEPATH += $$(VULKAN_SDK)/include
    INCLUDEPATH += /usr/include/gtk-2.0
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
    INCLUDEPATH += /usr/include/atk-1.0
    INCLUDEPATH += /usr/include/cairo
    INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
    INCLUDEPATH += /usr/include/pango-1.0
    INCLUDEPATH += /usr/include/gio-unix-2.0
    INCLUDEPATH += /usr/include/freetype2
    INCLUDEPATH += /usr/include/pixman-1
    INCLUDEPATH += /usr/include/libpng12
    INCLUDEPATH += /usr/include/harfbuzz
    QMAKE_CXXFLAGS += -lstdc++ -pthread -w
}

macx {
    OBJECTIVE_SOURCES += src/PlatformMacOS.mm
    HEADERS += src/PlatformMacOS.h
    QMAKE_CXXFLAGS += -x c++ -x objective-c++ -stdlib=libc++ -w -arch x86_64
    LIBS += -F/System/Library/Frameworks -framework GameKit
    LIBS += -F/System/Library/Frameworks -framework IOKit
    LIBS += -F/System/Library/Frameworks -framework QuartzCore
    LIBS += -F/System/Library/Frameworks -framework OpenAL
    LIBS += -F/System/Library/Frameworks -framework Cocoa
    LIBS += -F/System/Library/Frameworks -framework Foundation
}
