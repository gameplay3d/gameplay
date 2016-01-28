#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------
QT       -= core gui
TARGET = gameplay-encoder
CONFIG   += console
CONFIG   -= app_bundle
CONFIG(debug, debug|release): DEFINES += _DEBUG

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
    src/TMXSceneEncoder.cpp \
    src/TMXTypes.cpp \
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
    src/TMXSceneEncoder.h \
    src/TMXTypes.h \
    src/Vector2.h \
    src/Vector2.inl \
    src/Vector3.h \
    src/Vector3.inl \
    src/Vector4.h \
    src/Vector4.inl \
    src/VertexElement.h \
    src/Vertex.h

DEFINES += USE_FBX
INCLUDEPATH += $$PWD/../../external-deps/include


linux: DEFINES += __linux__
linux: QMAKE_CXXFLAGS += -std=c++11 -lstdc++ -pthread -w
linux: INCLUDEPATH += /usr/include/fbxsdk
linux: INCLUDEPATH += /usr/include
linux: LIBS += -L$$PWD/../../external-deps/lib/linux/x86_64/ -lgameplay-deps -lfreetype
linux: LIBS += -L/usr/lib/gcc4/x64/release -lfbxsdk
linux: LIBS += -lstdc++ -ldl -lpthread

macx: QMAKE_CXXFLAGS += -x c++ -stdlib=libc++ -w -arch x86_64
macx: INCLUDEPATH += "/Applications/Autodesk/FBX SDK/2015.1/include"
macx: LIBS += -L$$PWD/../../external-deps/lib/macosx/x86_64/ -lgameplay-deps
macx: LIBS += -L$$PWD/../../external-deps/lib/macosx/x86_64/ -lfreetype
macx: LIBS += -L"/Applications/Autodesk/FBX SDK/2015.1/lib/clang/release/" -lfbxsdk-static
macx: LIBS += -F/System/Library/Frameworks -framework Cocoa
macx: LIBS += -F/System/Library/Frameworks -framework SystemConfiguration
macx: LIBS += -F/System/Library/Frameworks -framework Foundation
macx: LIBS += -lm -lbz2 -lxml2 -liconv

win32: DEFINES += WIN32 _WINDOWS _UNICODE UNICODE
win32: CONFIG(debug, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Debug/ -lgameplay-deps
win32: CONFIG(release, debug|release): LIBS += -L$$PWD/../../external-deps/lib/windows/x86_64/Release/ -lgameplay-deps
win32: LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34100
win32: QMAKE_CXXFLAGS_WARN_ON -= -w34189
win32: QMAKE_CXXFLAGS_WARN_ON -= -w4302
