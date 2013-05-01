#ifndef FBXUTIL_H_
#define FBXUTIL_H_

#define FBXSDK_NEW_API

#include <iostream>
#include <list>
#include <vector>
#include <ctime>
#ifdef WIN32
    #pragma warning( disable : 4100 )
    #pragma warning( disable : 4512 )
#endif
#include <fbxsdk.h>

#include "Base.h"
#include "Vertex.h"
#include "Animation.h"
#include "AnimationChannel.h"
#include "EncoderArguments.h"

using namespace gameplay;

/**
 * Returns the aspect ratio from the given camera.
 * 
 * @param fbxCamera The FBX camera to get the aspect ratio from.
 * 
 * @return The aspect ratio from the camera.
 */
float getAspectRatio(FbxCamera* fbxCamera);

/**
 * Returns the field of view Y from the given camera.
 * 
 * @param fbxCamera The camera to get the fiew of view from.
 * 
 * @return The field of view Y.
 */
float getFieldOfView(FbxCamera* fbxCamera);

/**
 * Loads the texture coordinates from given mesh's polygon part into the vertex.
 * 
 * @param fbxMesh The mesh to get the polygon from.
 * @param uvs The UV list to load tex coords from.
 * @param uvSetIndex The UV set index of the uvs.
 * @param polyIndex The index of the polygon in the mesh.
 * @param posInPoly The position of the vertex in the polygon.
 * @param meshVertexIndex The index of the vertex in the mesh.
 * @param vertex The vertex to copy the texture coordinates to.
 */
void loadTextureCoords(FbxMesh* fbxMesh, const FbxGeometryElementUV* uvs, int uvSetIndex, int polyIndex, int posInPoly, int meshVertexIndex, Vertex* vertex);

/**
 * Loads the normal from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to get the polygon from.
 * @param vertexIndex The vertex index in the mesh.
 * @param controlPointIndex The control point index.
 * @param vertex The vertex to copy to.
 */
void loadNormal(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex);

/**
 * Loads the tangent from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to load from.
 * @param vertexIndex The index of the vertex within fbxMesh.
 * @param controlPointIndex The control point index.
 * @param vertex The vertex to copy to.
 */
void loadTangent(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex);

/**
 * Loads the binormal from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to load from.
 * @param vertexIndex The index of the vertex within fbxMesh.
 * @param controlPointIndex The control point index.
 * @param vertex The vertex to copy to.
 */
void loadBinormal(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex);

/**
 * Loads the vertex diffuse color from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to load from.
 * @param vertexIndex The index of the vertex within fbxMesh.
 * @param controlPointIndex The control point index.
 * @param vertex The vertex to copy to.
 */
void loadVertexColor(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex);

/**
 * Loads the blend weight and blend indices data into the vertex.
 * 
 * @param vertexWeights List of vertex weights. The x member contains the blendIndices. The y member contains the blendWeights.
 * @param vertex The vertex to copy the blend data to.
 */
void loadBlendData(const std::vector<Vector2>& vertexWeights, Vertex* vertex);

/**
 * Loads the blend weights and blend indices from the given mesh.
 * 
 * Each element of weights is a list of Vector2s where "x" is the blend index and "y" is the blend weight.
 * 
 * @param fbxMesh The mesh to load from.
 * @param weights List of blend weights and blend indices for each vertex.
 * 
 * @return True if this mesh has a mesh skin, false otherwise.
 */
bool loadBlendWeights(FbxMesh* fbxMesh, std::vector<std::vector<Vector2> >& weights);

/**
 * Copies from an FBX matrix to a float[16] array.
 */
void copyMatrix(const FbxMatrix& fbxMatrix, float* matrix);

/**
 * Copies from an FBX matrix to a gameplay matrix.
 */
void copyMatrix(const FbxMatrix& fbxMatrix, Matrix& matrix);

/**
 * Finds the min and max start time and stop time of the given animation curve.
 * 
 * startTime is updated if the animation curve contains a start time that is less than startTime.
 * stopTime is updated if the animation curve contains a stop time that is greater than stopTime.
 * frameRate is updated if the animation curve contains a frame rate that is greater than frameRate.
 * 
 * @param animCurve The animation curve to read from.
 * @param startTime The min start time. (in/out)
 * @param stopTime The max stop time. (in/out)
 * @param frameRate The frame rate. (in/out)
 */
void findMinMaxTime(FbxAnimCurve* animCurve, float* startTime, float* stopTime, float* frameRate);

/**
 * Appends key frame data to the given node for the specified animation target attribute.
 * 
 * @param fbxNode The node to get the matrix transform from.
 * @param channel The aniamtion channel to write values into.
 * @param time The time of the keyframe.
 * @param scale The evaluated scale for the keyframe.
 * @param rotation The evalulated rotation for the keyframe.
 * @param translation The evalulated translation for the keyframe.

 */
void appendKeyFrame(FbxNode* fbxNode, AnimationChannel* channel, float time, const Vector3& scale, const Quaternion& rotation, const Vector3& translation);

/**
 * Decomposes the given node's matrix transform at the given time and copies to scale, rotation and translation.
 * 
 * @param fbxNode The node to get the matrix transform from.
 * @param time The time to get the matrix transform from.
 * @param scale The scale to copy to.
 * @param rotation The rotation to copy to.
 * @param translation The translation to copy to.
 */
void decompose(FbxNode* fbxNode, float time, Vector3* scale, Quaternion* rotation, Vector3* translation);

/**
 * Creates an animation channel that targets the given node and target attribute using the given key times and key values.
 * 
 * @param fbxNode The node to target.
 * @param targetAttrib The attribute type to target.
 * @param keyTimes The key times for the animation channel.
 * @param keyValues The key values for the animation channel.
 * 
 * @return The newly created animation channel.
 */
AnimationChannel* createAnimationChannel(FbxNode* fbxNode, unsigned int targetAttrib, const std::vector<float>& keyTimes, const std::vector<float>& keyValues);

void addScaleChannel(Animation* animation, FbxNode* fbxNode, float startTime, float stopTime);

void addTranslateChannel(Animation* animation, FbxNode* fbxNode, float startTime, float stopTime);

/**
 * Determines if it is possible to automatically group animations for mesh skins.
 * 
 * @param fbxScene The FBX scene to search.
 * 
 * @return True if there is at least one mesh skin that has animations that can be grouped.
 */
bool isGroupAnimationPossible(FbxScene* fbxScene);
bool isGroupAnimationPossible(FbxNode* fbxNode);
bool isGroupAnimationPossible(FbxMesh* fbxMesh);

bool isBlack(FbxDouble3& fbxDouble);

/**
 * Recursively generates the tangents and binormals for all nodes that were specified in the command line arguments.
 */
void generateTangentsAndBinormals(FbxNode* fbxNode, const EncoderArguments& arguments);

FbxAnimCurve* getCurve(FbxPropertyT<FbxDouble3>& prop, FbxAnimLayer* animLayer, const char* pChannel);

std::string toString(const FbxDouble3& fbxDouble);
std::string toString(const FbxDouble3& fbxDouble, double d);
std::string toString(double value);

#endif
