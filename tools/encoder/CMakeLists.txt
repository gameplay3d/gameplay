
include_directories( 
    ${CMAKE_SOURCE_DIR}/external-deps/zlib/include
    ${CMAKE_SOURCE_DIR}/external-deps/libpng/include
    ${CMAKE_SOURCE_DIR}/external-deps/freetype2/include
    /usr/include/fbxsdk
    /usr/include
)

add_definitions(-D_DEBUG)
add_definitions(-D__linux__ -DNO_BOOST -DNO_ZAE -DUSE_FBX)

link_directories(
    ${CMAKE_SOURCE_DIR}/external-deps/zlib/lib/linux/${ARCH_DIR}
    ${CMAKE_SOURCE_DIR}/external-deps/libpng/lib/linux/${ARCH_DIR}
    ${CMAKE_SOURCE_DIR}/external-deps/freetype2/lib/linux/${ARCH_DIR}
    /usr/lib/gcc4/${ARCH_DIR}
    /usr/lib
)

set(APP_LIBRARIES
    dl
    fbxsdk-2013.3-static
    png
    z   
    freetype
    pthread
) 

add_definitions(-lstdc++ -ldl -lfbxsdk-2013.3-static -lpng -lz -lfreetype -lpthread)

set( APP_NAME gameplay-encoder )

set(APP_SRC
    src/AnimationChannel.cpp
    src/AnimationChannel.h
    src/Animation.cpp
    src/Animation.h
    src/Animations.cpp
    src/Animations.h
    src/Base.cpp
    src/Base.h
    src/BoundingVolume.cpp
    src/BoundingVolume.h
    src/Camera.cpp
    src/Camera.h
    src/Constants.cpp
    src/Constants.h
    src/Curve.cpp
    src/Curve.h
    src/Curve.inl
    src/Effect.cpp
    src/Effect.h
    src/EncoderArguments.cpp
    src/EncoderArguments.h
    src/FBXSceneEncoder.cpp
    src/FBXSceneEncoder.h
    src/FBXUtil.cpp
    src/FBXUtil.h
    src/FileIO.cpp
    src/FileIO.h
    src/Font.cpp
    src/Font.h
    src/Glyph.cpp
    src/Glyph.h
    src/GPBDecoder.cpp
    src/GPBDecoder.h
    src/GPBFile.cpp
    src/GPBFile.h
    src/Heightmap.cpp
    src/Heightmap.h
    src/Image.cpp
    src/Image.h
    src/Light.cpp
    src/Light.h
    src/main.cpp
    src/Material.cpp
    src/Material.h
    src/MaterialParameter.cpp
    src/MaterialParameter.h
    src/Matrix.cpp
    src/Matrix.h
    src/Mesh.cpp
    src/Mesh.h
    src/MeshPart.cpp
    src/MeshPart.h
    src/MeshSkin.cpp
    src/MeshSkin.h
    src/MeshSubSet.cpp
    src/MeshSubSet.h
    src/Model.cpp
    src/Model.h
    src/Node.cpp
    src/Node.h
    src/NormalMapGenerator.cpp
    src/NormalMapGenerator.h
    src/Object.cpp
    src/Object.h
    src/Quaternion.cpp
    src/Quaternion.h
    src/Quaternion.inl
    src/Reference.cpp
    src/Reference.h
    src/ReferenceTable.cpp
    src/ReferenceTable.h
    src/Sampler.cpp
    src/Sampler.h
    src/Scene.cpp
    src/Scene.h
    src/StringUtil.cpp
    src/StringUtil.h
    src/Thread.h
    src/Transform.cpp
    src/Transform.h
    src/TTFFontEncoder.cpp
    src/TTFFontEncoder.h
    src/Vector2.cpp
    src/Vector2.h
    src/Vector2.inl
    src/Vector3.cpp
    src/Vector3.h
    src/Vector3.inl
    src/Vector4.cpp
    src/Vector4.h
    src/Vector4.inl
    src/Vertex.cpp
    src/VertexElement.cpp
    src/VertexElement.h
    src/Vertex.h
)

add_executable(${APP_NAME}
    ${APP_SRC}
)

target_link_libraries(
    ${APP_NAME}
    ${APP_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

set_target_properties(${APP_NAME} PROPERTIES
    OUTPUT_NAME "${APP_NAME}"
    CLEAN_DIRECT_OUTPUT 1
)

source_group(src FILES ${APP_SRC})

