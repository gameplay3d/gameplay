#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       -= core gui

TARGET = gameplay-encoder
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += src/Mesh.cpp \
    src/AnimationChannel.cpp \
    src/Animation.cpp \
    src/Animations.cpp \
    src/Base.cpp \
    src/BoundingVolume.cpp \
    src/Camera.cpp \
    src/Constants.cpp \
    src/Curve.cpp \
    src/edtaa3func.c \
    src/Effect.cpp \
    src/EncoderArguments.cpp \
    src/FBXSceneEncoder.cpp \
    src/FBXUtil.cpp \
    src/FileIO.cpp \
    src/Font.cpp \
    src/Glyph.cpp \
    src/GPBDecoder.cpp \
    src/GPBFile.cpp \
    src/Heightmap.cpp \
    src/Image.cpp \
    src/Light.cpp \
    src/main.cpp \
    src/Material.cpp \
    src/MaterialParameter.cpp \
    src/Matrix.cpp \
    src/MeshPart.cpp \
    src/MeshSkin.cpp \
    src/MeshSubSet.cpp \
    src/Model.cpp \
    src/Node.cpp \
    src/NormalMapGenerator.cpp \
    src/Object.cpp \
    src/Quaternion.cpp \
    src/Reference.cpp \
    src/ReferenceTable.cpp \
    src/Sampler.cpp \
    src/Scene.cpp \
    src/StringUtil.cpp \
    src/Transform.cpp \
    src/TTFFontEncoder.cpp \
    src/Vector2.cpp \
    src/Vector3.cpp \
    src/Vector4.cpp \
    src/Vertex.cpp \
    src/VertexElement.cpp

HEADERS += src/AnimationChannel.h \
    src/AnimationChannel.h \
    src/Animation.h \
    src/Animations.h \
    src/Base.h \
    src/BoundingVolume.h \
    src/Camera.h \
    src/Constants.h \
    src/Curve.h \
    src/Curve.inl \
    src/edtaa3func.h \
    src/Effect.h \
    src/EncoderArguments.h \
    src/FBXSceneEncoder.h \
    src/FBXUtil.h \
    src/FileIO.h \
    src/Font.h \
    src/Glyph.h \
    src/GPBDecoder.h \
    src/GPBFile.h \
    src/Heightmap.h \
    src/Image.h \
    src/Light.h \
    src/Material.h \
    src/MaterialParameter.h \
    src/Matrix.h \
    src/Mesh.h \
    src/MeshPart.h \
    src/MeshSkin.h \
    src/MeshSubSet.h \
    src/Model.h \
    src/Node.h \
    src/NormalMapGenerator.h \
    src/Object.h \
    src/Quaternion.h \
    src/Quaternion.inl \
    src/Reference.h \
    src/ReferenceTable.h \
    src/Sampler.h \
    src/Scene.h \
    src/StringUtil.h \
    src/Thread.h \
    src/Transform.h \
    src/TTFFontEncoder.h \
    src/Vector2.h \
    src/Vector2.inl \
    src/Vector3.h \
    src/Vector3.inl \
    src/Vector4.h \
    src/Vector4.inl \
    src/VertexElement.h \
    src/Vertex.h

INCLUDEPATH += $$PWD/../../external-deps/include
linux:!android: INCLUDEPATH += /usr/include/fbxsdk
linux:!android: INCLUDEPATH += /usr/include

DEPENDPATH += INCLUDEPATH

linux:!android: DEFINES += USE_FBX
linux:!android: DEFINES += __linux__

linux:!android: QMAKE_CXXFLAGS += -std=c++11 -lstdc++ -pthread -w

linux:!android: LIBS += -L$$PWD/../../external-deps/lib/linux/x86_64/ -lgameplay-deps -lfreetype
linux:!android: LIBS += -L/usr/lib/gcc4/x64/release -lfbxsdk
linux:!android: LIBS += -lstdc++
linux:!android: LIBS += -ldl
linux:!android: LIBS += -lpthread

linux:!android: PRE_TARGETDEPS += $$PWD/../../external-deps/lib/linux/x86_64/libgameplay-deps.a
linux:!android: PRE_TARGETDEPS += $$PWD/../../external-deps/lib/linux/x86_64/libfreetype.a
linux:!android: PRE_TARGETDEPS += /usr/lib/gcc4/x64/release/libfbxsdk.a
