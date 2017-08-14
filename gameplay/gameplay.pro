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
    src/AudioOpenAL.cpp \
    src/BoundingBox.cpp \
    src/BoundingSphere.cpp \
    src/Camera.cpp \
    src/Component.cpp \
    src/Curve.cpp \
    src/FileSystem.cpp \
    src/Frustum.cpp \
    src/Game.cpp \
    src/Graphics.cpp \
    src/GraphicsDirect3D.cpp \
    src/GraphicsVulkan.cpp \
    src/Light.cpp \
    src/Logger.cpp \
    src/MathUtil.cpp \
    src/Matrix.cpp \
    src/Physics.cpp \
    src/PhysicsPhysX.cpp \
    src/Plane.cpp \
    src/Quaternion.cpp \
    src/Ray.cpp \
    src/Rectangle.cpp \
    src/SceneObject.cpp \
    src/Script.cpp \
    src/Serializer.cpp \
    src/SerializerBinary.cpp \
    src/SerializerJson.cpp \
    src/Vector2.cpp \
    src/Vector3.cpp \
    src/Vector4.cpp

HEADERS += \
    src/Animation.h \
    src/Audio.h \
    src/AudioOpenAL.h \
    src/Base.h \
    src/BoundingBox.h \
    src/BoundingSphere.h \
    src/Camera.h \
    src/Component.h \
    src/Curve.h \
    src/FileSystem.h \
    src/Frustum.h \
    src/Game.h \
    src/Graphics.h \
    src/GraphicsDirect3D.h \
    src/GraphicsVulkan.h \
    src/Light.h \
    src/Logger.h \
    src/MathUtil.h \
    src/Matrix.h \
    src/Physics.h \
    src/PhysicsPhysX.h \
    src/Plane.h \
    src/Platform.h \
    src/Quaternion.h \
    src/Ray.h \
    src/Rectangle.h \
    src/SceneObject.h \
    src/Script.h \
    src/Serializer.h \
    src/SerializerBinary.h \
    src/SerializerJson.h \
    src/Stream.h \
    src/Vector2.h \
    src/Vector3.h \
    src/Vector4.h

INCLUDEPATH += $$PWD/../gameplay/src
INCLUDEPATH += $$PWD/../external-deps/include

win32 {
    DEFINES += _WINDOWS WIN32 _UNICODE UNICODE
    DEFINES += VK_USE_PLATFORM_WIN32_KHR
    INCLUDEPATH += $$(VULKAN_SDK)/Include
    SOURCES += src/PlatformWindows.cpp
    HEADERS += src/PlatformWindows.h
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
    HEADERS += src/PlatformLinux.h
    INCLUDEPATH += /usr/include/gtk-2.0
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/gtk-2.0/include
    INCLUDEPATH += /usr/include/atk-1.0
    INCLUDEPATH += /usr/include/cairo
    INCLUDEPATH += /usr/include/gdk-pixbuf-2.0
    INCLUDEPATH += /usr/include/pango-1.0
    INCLUDEPATH += /usr/include/gio-unix-2.0
    INCLUDEPATH += /usr/include/freetype2
    INCLUDEPATH += /usr/include/glib-2.0
    INCLUDEPATH += /usr/lib/x86_64-linux-gnu/glib-2.0/include
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
