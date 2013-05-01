#include <algorithm>
#include <string>
#include <sstream>

#include "FBXUtil.h"
#include "Transform.h"
#include "Vector3.h"
#include "Vector2.h"

using namespace gameplay;
using std::string;
using std::vector;
using std::map;
using std::ostringstream;

float getAspectRatio(FbxCamera* fbxCamera)
{
    return (float)fbxCamera->FilmAspectRatio.Get();
    /*
    FbxCamera::ECameraAspectRatioMode camAspectRatioMode = fbxCamera->GetAspectRatioMode();
    double aspectX = fbxCamera->AspectWidth.Get();
    double aspectY = fbxCamera->AspectHeight.Get();
    double aspectRatio = 1.333333;
    switch ( camAspectRatioMode)
    {
    case FbxCamera::eWINDOW_SIZE:
        aspectRatio = aspectX / aspectY;
        break;
    case FbxCamera::eFIXED_RATIO:
        aspectRatio = aspectX;
        break;
    case FbxCamera::eFIXED_RESOLUTION:
        aspectRatio = aspectX / aspectY * fbxCamera->GetPixelRatio();
        break;
    case FbxCamera::eFIXED_WIDTH:
        aspectRatio = fbxCamera->GetPixelRatio() / aspectY;
        break;
    case FbxCamera::eFIXED_HEIGHT:
        aspectRatio = fbxCamera->GetPixelRatio() * aspectX;
        break;
    default:
        break;
    }
    return (float)aspectRatio;
    */
}

inline double vfov(double hfov, double aspect)
{
    static const double MATH_PI_180 = 0.01745329251994329576923690768489;
    static const double MATH_180_PI = 57.295779513082320876798154814105;
    return (2.0 * atan((aspect) * tan( (hfov * MATH_PI_180) * 0.5)) * MATH_180_PI);
}

float getFieldOfView(FbxCamera* fbxCamera)
{
    double fieldOfViewX = 0.0;
    double fieldOfViewY = 0.0;
    double filmHeight = fbxCamera->GetApertureHeight();
    double filmWidth = fbxCamera->GetApertureWidth() * fbxCamera->GetSqueezeRatio();
    double apertureRatio = filmHeight / filmWidth;
    if ( fbxCamera->GetApertureMode() == FbxCamera::eVertical)
    {
        fieldOfViewY = fbxCamera->FieldOfView.Get();
    }
    else if (fbxCamera->GetApertureMode() == FbxCamera::eHorizontal)
    {
        fieldOfViewX = fbxCamera->FieldOfView.Get();
        fieldOfViewY = vfov( fieldOfViewX, apertureRatio);
    }
    else if (fbxCamera->GetApertureMode() == FbxCamera::eFocalLength)
    {
        fieldOfViewX = fbxCamera->ComputeFieldOfView(fbxCamera->FocalLength.Get());
        fieldOfViewY = vfov( fieldOfViewX, apertureRatio);
    }
    else if (fbxCamera->GetApertureMode() == FbxCamera::eHorizAndVert)
    {
        fieldOfViewY = fbxCamera->FieldOfViewY.Get();
    }
    else
    {
        fieldOfViewY = 45.0;
    }
    return (float)fieldOfViewY;
}

void loadTextureCoords(FbxMesh* fbxMesh, const FbxGeometryElementUV* uvs, int uvSetIndex, int polyIndex, int posInPoly, int meshVertexIndex, Vertex* vertex)
{
    assert(fbxMesh && polyIndex >= 0 && posInPoly >= 0);

    const bool useIndex = uvs->GetReferenceMode() != FbxGeometryElement::eDirect;
    const int indexCount = useIndex ? uvs->GetIndexArray().GetCount() : 0;
    int uvIndex = -1;

    switch (uvs->GetMappingMode())
    {
    case FbxGeometryElement::eByControlPoint:
        {
            // Get the index of the current vertex in control points array
            int polyVertIndex = fbxMesh->GetPolygonVertex(polyIndex, posInPoly);

            // The UV index depends on the reference mode
            uvIndex = useIndex ? uvs->GetIndexArray().GetAt(polyVertIndex) : polyVertIndex;
        }
        break;

    case FbxGeometryElement::eByPolygonVertex:
        if (meshVertexIndex < indexCount)
        {
            uvIndex = useIndex ? uvs->GetIndexArray().GetAt(meshVertexIndex) : meshVertexIndex;
        }
        break;

    default:
        // Only support eByPolygonVertex and eByControlPoint mappings
        break;
    }

    vertex->hasTexCoord[uvSetIndex] = true;

    // Store UV information in vertex
    if (uvIndex != -1)
    {
        FbxVector2 uvValue = uvs->GetDirectArray().GetAt(uvIndex);
        vertex->texCoord[uvSetIndex].x = (float)uvValue[0];
        vertex->texCoord[uvSetIndex].y = (float)uvValue[1];
    }
}

void loadNormal(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex)
{
    if (fbxMesh->GetElementNormalCount() > 0)
    {
        // Get only the first
        FbxGeometryElementNormal* normal = fbxMesh->GetElementNormal(0);
        FbxGeometryElement::EMappingMode mappingMode = normal->GetMappingMode();
        if (mappingMode == FbxGeometryElement::eByControlPoint)
        {
            switch (normal->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                {
                    FbxVector4 vec4 = normal->GetDirectArray().GetAt(controlPointIndex);
                    vertex->hasNormal = true;
                    vertex->normal.x = (float)vec4[0];
                    vertex->normal.y = (float)vec4[1];
                    vertex->normal.z = (float)vec4[2];
                }
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = normal->GetIndexArray().GetAt(controlPointIndex);
                    FbxVector4 vec4 = normal->GetDirectArray().GetAt(id);
                    vertex->hasNormal = true;
                    vertex->normal.x = (float)vec4[0];
                    vertex->normal.y = (float)vec4[1];
                    vertex->normal.z = (float)vec4[2];
                }
                break;
            default:
                break;
            }
        }
        else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
        {
            switch (normal->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                {
                    FbxVector4 vec4 = normal->GetDirectArray().GetAt(vertexIndex);
                    vertex->hasNormal = true;
                    vertex->normal.x = (float)vec4[0];
                    vertex->normal.y = (float)vec4[1];
                    vertex->normal.z = (float)vec4[2];
                }
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = normal->GetIndexArray().GetAt(vertexIndex);
                    FbxVector4 vec4 = normal->GetDirectArray().GetAt(id);
                    vertex->hasNormal = true;
                    vertex->normal.x = (float)vec4[0];
                    vertex->normal.y = (float)vec4[1];
                    vertex->normal.z = (float)vec4[2];
                }
                break;
            default:
                break;
            }
        }
    }
}

void loadTangent(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex)
{
    if (fbxMesh->GetElementTangentCount() > 0)
    {
        // Get only the first tangent
        FbxGeometryElementTangent* tangent = fbxMesh->GetElementTangent(0);
        FbxGeometryElement::EMappingMode mappingMode = tangent->GetMappingMode();
        if (mappingMode == FbxGeometryElement::eByControlPoint)
        {
            switch (tangent->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                {
                    FbxVector4 vec4 = tangent->GetDirectArray().GetAt(controlPointIndex);
                    vertex->hasTangent = true;
                    vertex->tangent.x = (float)vec4[0];
                    vertex->tangent.y = (float)vec4[1];
                    vertex->tangent.z = (float)vec4[2];
                }
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = tangent->GetIndexArray().GetAt(controlPointIndex);
                    FbxVector4 vec4 = tangent->GetDirectArray().GetAt(id);
                    vertex->hasTangent = true;
                    vertex->tangent.x = (float)vec4[0];
                    vertex->tangent.y = (float)vec4[1];
                    vertex->tangent.z = (float)vec4[2];
                }
                break;
            default:
                break;
            }
        }
        else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
        {
            switch (tangent->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                {
                    FbxVector4 vec4 = tangent->GetDirectArray().GetAt(vertexIndex);
                    vertex->hasTangent = true;
                    vertex->tangent.x = (float)vec4[0];
                    vertex->tangent.y = (float)vec4[1];
                    vertex->tangent.z = (float)vec4[2];
                }
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = tangent->GetIndexArray().GetAt(vertexIndex);
                    FbxVector4 vec4 = tangent->GetDirectArray().GetAt(id);
                    vertex->hasTangent = true;
                    vertex->tangent.x = (float)vec4[0];
                    vertex->tangent.y = (float)vec4[1];
                    vertex->tangent.z = (float)vec4[2];
                }
                break;
            default:
                break;
            }
        }
    }
}

void loadBinormal(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex)
{
    if (fbxMesh->GetElementBinormalCount() > 0)
    {
        // Get only the first binormal.
        FbxGeometryElementBinormal* binormal = fbxMesh->GetElementBinormal(0);
        FbxGeometryElement::EMappingMode mappingMode = binormal->GetMappingMode();

        if (mappingMode == FbxGeometryElement::eByControlPoint)
        {
            switch (binormal->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                {
                    FbxVector4 vec4 = binormal->GetDirectArray().GetAt(controlPointIndex);
                    vertex->hasBinormal = true;
                    vertex->binormal.x = (float)vec4[0];
                    vertex->binormal.y = (float)vec4[1];
                    vertex->binormal.z = (float)vec4[2];
                }
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = binormal->GetIndexArray().GetAt(controlPointIndex);
                    FbxVector4 vec4 = binormal->GetDirectArray().GetAt(id);
                    vertex->hasBinormal = true;
                    vertex->binormal.x = (float)vec4[0];
                    vertex->binormal.y = (float)vec4[1];
                    vertex->binormal.z = (float)vec4[2];
                }
                break;
            default:
                break;
            }
        }
        else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
        {
            switch (binormal->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                {
                    FbxVector4 vec4 = binormal->GetDirectArray().GetAt(vertexIndex);
                    vertex->hasBinormal = true;
                    vertex->binormal.x = (float)vec4[0];
                    vertex->binormal.y = (float)vec4[1];
                    vertex->binormal.z = (float)vec4[2];
                }
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = binormal->GetIndexArray().GetAt(vertexIndex);
                    FbxVector4 vec4 = binormal->GetDirectArray().GetAt(id);
                    vertex->hasBinormal = true;
                    vertex->binormal.x = (float)vec4[0];
                    vertex->binormal.y = (float)vec4[1];
                    vertex->binormal.z = (float)vec4[2];
                }
                break;
            default:
                break;
            }
        }
    }
}

void loadVertexColor(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex)
{
    if (fbxMesh->GetElementVertexColorCount() > 0)
    {
        // Get only the first vertex color.
        FbxGeometryElementVertexColor* vertexColor = fbxMesh->GetElementVertexColor(0);
        FbxGeometryElement::EMappingMode mappingMode = vertexColor->GetMappingMode();
        if (mappingMode == FbxGeometryElement::eByControlPoint)
        {
            switch (vertexColor->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                {
                    FbxColor color = vertexColor->GetDirectArray().GetAt(controlPointIndex);

                    vertex->hasDiffuse = true;
                    vertex->diffuse.x = (float)color.mRed;
                    vertex->diffuse.y = (float)color.mGreen;
                    vertex->diffuse.z = (float)color.mBlue;
                    vertex->diffuse.w = (float)color.mAlpha;
                }
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = vertexColor->GetIndexArray().GetAt(controlPointIndex);
                    FbxColor color = vertexColor->GetDirectArray().GetAt(id);

                    vertex->hasDiffuse = true;
                    vertex->diffuse.x = (float)color.mRed;
                    vertex->diffuse.y = (float)color.mGreen;
                    vertex->diffuse.z = (float)color.mBlue;
                    vertex->diffuse.w = (float)color.mAlpha;
                }
                break;
            default:
                break;
            }
        }
        else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
        {
            switch (vertexColor->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                {
                    FbxColor color = vertexColor->GetDirectArray().GetAt(vertexIndex);

                    vertex->hasDiffuse = true;
                    vertex->diffuse.x = (float)color.mRed;
                    vertex->diffuse.y = (float)color.mGreen;
                    vertex->diffuse.z = (float)color.mBlue;
                    vertex->diffuse.w = (float)color.mAlpha;
                }
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = vertexColor->GetIndexArray().GetAt(vertexIndex);
                    FbxColor color = vertexColor->GetDirectArray().GetAt(id);

                    vertex->hasDiffuse = true;
                    vertex->diffuse.x = (float)color.mRed;
                    vertex->diffuse.y = (float)color.mGreen;
                    vertex->diffuse.z = (float)color.mBlue;
                    vertex->diffuse.w = (float)color.mAlpha;
                }
                break;
            default:
                break;
            }
        }
    }
}

void loadBlendData(const vector<Vector2>& vertexWeights, Vertex* vertex)
{
    size_t size = vertexWeights.size();

    if (size >= 1)
    {
        vertex->hasWeights= true;
        vertex->blendIndices.x = vertexWeights[0].x;
        vertex->blendWeights.x = vertexWeights[0].y;
    }
    if (size >= 2)
    {
        vertex->blendIndices.y = vertexWeights[1].x;
        vertex->blendWeights.y = vertexWeights[1].y;
    }
    if (size >= 3)
    {
        vertex->blendIndices.z = vertexWeights[2].x;
        vertex->blendWeights.z = vertexWeights[2].y;
    }
    if (size >= 4)
    {
        vertex->blendIndices.w = vertexWeights[3].x;
        vertex->blendWeights.w = vertexWeights[3].y;
    }
    //vertex->normalizeBlendWeight();
}

bool loadBlendWeights(FbxMesh* fbxMesh, vector<vector<Vector2> >& weights)
{
    assert(fbxMesh);
    const int vertexCount = fbxMesh->GetControlPointsCount();

    FbxSkin* fbxSkin = NULL;
    const int deformerCount = fbxMesh->GetDeformerCount();
    for (int i = 0; i < deformerCount; ++i)
    {
        FbxDeformer* deformer = fbxMesh->GetDeformer(i);
        if (deformer->GetDeformerType() == FbxDeformer::eSkin)
        {
            fbxSkin = FbxCast<FbxSkin>(deformer);
            weights.resize(vertexCount);

            const int clusterCount = fbxSkin->GetClusterCount();
            for (int j = 0; j < clusterCount; ++j)
            {
                FbxCluster* cluster = fbxSkin->GetCluster(j);
                assert(cluster);
                const int vertexIndexCount = cluster->GetControlPointIndicesCount();
                for (int k = 0; k < vertexIndexCount; ++k)
                {
                    int index = cluster->GetControlPointIndices()[k];
                    if (index >= vertexCount)
                    {
                        continue;
                    }

                    double weight = cluster->GetControlPointWeights()[k];
                    if (weight == 0.0)
                    {
                        continue;
                    }
                    weights[index].push_back(Vector2((float)j, (float)weight));
                }
            }
            // Only the first skin deformer will be loaded.
            // There probably won't be more than one.
            break;
        }
    }
    return fbxSkin != NULL;
}

void findMinMaxTime(FbxAnimCurve* animCurve, float* startTime, float* stopTime, float* frameRate)
{
    FbxTime start, stop;
    FbxTimeSpan timeSpan;
    animCurve->GetTimeInterval(timeSpan);
    start = timeSpan.GetStart();
    stop = timeSpan.GetStop();
    *startTime = std::min(*startTime, (float)start.GetMilliSeconds());
    *stopTime = std::max(*stopTime, (float)stop.GetMilliSeconds());
    *frameRate = std::max(*frameRate, (float)stop.GetFrameRate(FbxTime::eDefaultMode));
}

void appendKeyFrame(FbxNode* fbxNode, AnimationChannel* channel, float time, const Vector3& scale, const Quaternion& rotation, const Vector3& translation)
{
    // Write key time
    channel->getKeyTimes().push_back(time);

    // Write key values
    vector<float>& keyValues = channel->getKeyValues();
    switch (channel->getTargetAttribute())
    {
        case Transform::ANIMATE_SCALE:
        {
            keyValues.push_back(scale.x);
            keyValues.push_back(scale.y);
            keyValues.push_back(scale.z);
        }
        break;

        case Transform::ANIMATE_SCALE_X:
        {
            keyValues.push_back(scale.x);
        }
        break;

        case Transform::ANIMATE_SCALE_Y:
        {
            keyValues.push_back(scale.y);
        }
        break;

        case Transform::ANIMATE_SCALE_Z:
        {
            keyValues.push_back(scale.z);
        }
        break;

        case Transform::ANIMATE_ROTATE:
        {
            keyValues.push_back(rotation.x);
            keyValues.push_back(rotation.y);
            keyValues.push_back(rotation.z);
            keyValues.push_back(rotation.w);
        }
        break;

        case Transform::ANIMATE_TRANSLATE:
        {
            keyValues.push_back(translation.x);
            keyValues.push_back(translation.y);
            keyValues.push_back(translation.z);
        }
        break;

        case Transform::ANIMATE_TRANSLATE_X:
        {
            keyValues.push_back(translation.x);
        }
        break;

        case Transform::ANIMATE_TRANSLATE_Y:
        {
            keyValues.push_back(translation.y);
        }
        break;

        case Transform::ANIMATE_TRANSLATE_Z:
        {
            keyValues.push_back(translation.z);
        }
        break;

        case Transform::ANIMATE_ROTATE_TRANSLATE:
        {
            keyValues.push_back(rotation.x);
            keyValues.push_back(rotation.y);
            keyValues.push_back(rotation.z);
            keyValues.push_back(rotation.w);
            keyValues.push_back(translation.x);
            keyValues.push_back(translation.y);
            keyValues.push_back(translation.z);
        }
        break;

        case Transform::ANIMATE_SCALE_ROTATE_TRANSLATE:
        {
            keyValues.push_back(scale.x);
            keyValues.push_back(scale.y);
            keyValues.push_back(scale.z);
            keyValues.push_back(rotation.x);
            keyValues.push_back(rotation.y);
            keyValues.push_back(rotation.z);
            keyValues.push_back(rotation.w);
            keyValues.push_back(translation.x);
            keyValues.push_back(translation.y);
            keyValues.push_back(translation.z);
        }
        break;

        case Transform::ANIMATE_SCALE_TRANSLATE:
        {
            keyValues.push_back(scale.x);
            keyValues.push_back(scale.y);
            keyValues.push_back(scale.z);
            keyValues.push_back(translation.x);
            keyValues.push_back(translation.y);
            keyValues.push_back(translation.z);
        }
        break;

        case Transform::ANIMATE_SCALE_ROTATE:
        {
            keyValues.push_back(scale.x);
            keyValues.push_back(scale.y);
            keyValues.push_back(scale.z);
            keyValues.push_back(rotation.x);
            keyValues.push_back(rotation.y);
            keyValues.push_back(rotation.z);
            keyValues.push_back(rotation.w);
        }
        break;

        default:
        {
            LOG(1, "Warning: Invalid animatoin target (%d) attribute for node: %s.\n", channel->getTargetAttribute(), fbxNode->GetName());
        }
        return;
    }
}

void decompose(FbxNode* fbxNode, float time, Vector3* scale, Quaternion* rotation, Vector3* translation)
{
    FbxAMatrix fbxMatrix;
    Matrix matrix;
    FbxTime kTime;
    kTime.SetMilliSeconds((FbxLongLong)time);
    fbxMatrix = fbxNode->EvaluateLocalTransform(kTime);
    copyMatrix(fbxMatrix, matrix);
    matrix.decompose(scale, rotation, translation);
}

AnimationChannel* createAnimationChannel(FbxNode* fbxNode, unsigned int targetAttrib, const vector<float>& keyTimes, const vector<float>& keyValues)
{
    AnimationChannel* channel = new AnimationChannel();
    channel->setTargetId(fbxNode->GetName());
    channel->setKeyTimes(keyTimes);
    channel->setKeyValues(keyValues);
    channel->setInterpolation(AnimationChannel::LINEAR);
    channel->setTargetAttribute(targetAttrib);
    return channel;
}

void addScaleChannel(Animation* animation, FbxNode* fbxNode, float startTime, float stopTime)
{
    vector<float> keyTimes;
    vector<float> keyValues;
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;

    decompose(fbxNode, startTime, &scale, &rotation, &translation);
    keyTimes.push_back(startTime);
    keyValues.push_back(scale.x);
    keyValues.push_back(scale.y);
    keyValues.push_back(scale.z);

    decompose(fbxNode, stopTime, &scale, &rotation, &translation);
    keyTimes.push_back(stopTime);
    keyValues.push_back(scale.x);
    keyValues.push_back(scale.y);
    keyValues.push_back(scale.z);

    AnimationChannel* channel = createAnimationChannel(fbxNode, Transform::ANIMATE_SCALE, keyTimes, keyValues);
    animation->add(channel);
}

void addTranslateChannel(Animation* animation, FbxNode* fbxNode, float startTime, float stopTime)
{
    vector<float> keyTimes;
    vector<float> keyValues;
    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;

    decompose(fbxNode, startTime, &scale, &rotation, &translation);
    keyTimes.push_back(startTime);
    keyValues.push_back(translation.x);
    keyValues.push_back(translation.y);
    keyValues.push_back(translation.z);

    decompose(fbxNode, stopTime, &scale, &rotation, &translation);
    keyTimes.push_back(stopTime);
    keyValues.push_back(translation.x);
    keyValues.push_back(translation.y);
    keyValues.push_back(translation.z);

    AnimationChannel* channel = createAnimationChannel(fbxNode, Transform::ANIMATE_TRANSLATE, keyTimes, keyValues);
    animation->add(channel);
}

void copyMatrix(const FbxMatrix& fbxMatrix, float* matrix)
{
    int i = 0;
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            matrix[i++] = (float)fbxMatrix.Get(row, col);
        }
    }
}

void copyMatrix(const FbxMatrix& fbxMatrix, Matrix& matrix)
{
    int i = 0;
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            matrix.m[i++] = (float)fbxMatrix.Get(row, col);
        }
    }
}

bool isGroupAnimationPossible(FbxScene* fbxScene)
{
    FbxNode* rootNode = fbxScene->GetRootNode();
    if (rootNode)
    {
        if (isGroupAnimationPossible(rootNode))
            return true;
    }
    return false;
}

bool isGroupAnimationPossible(FbxNode* fbxNode)
{
    if (fbxNode)
    {
        FbxMesh* fbxMesh = fbxNode->GetMesh();
        if (isGroupAnimationPossible(fbxMesh))
            return true;
        const int childCount = fbxNode->GetChildCount();
        for (int i = 0; i < childCount; ++i)
        {
            if (isGroupAnimationPossible(fbxNode->GetChild(i)))
                return true;
        }
    }
    return false;
}

bool isGroupAnimationPossible(FbxMesh* fbxMesh)
{
    if (fbxMesh)
    {
        const int deformerCount = fbxMesh->GetDeformerCount();
        for (int i = 0; i < deformerCount; ++i)
        {
            FbxDeformer* deformer = fbxMesh->GetDeformer(i);
            if (deformer->GetDeformerType() == FbxDeformer::eSkin)
            {
                FbxSkin* fbxSkin = FbxCast<FbxSkin>(deformer);
                if (fbxSkin)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool isBlack(FbxDouble3& fbxDouble)
{
    return fbxDouble[0] == 0.0 && fbxDouble[1] == 0.0 && fbxDouble[2] == 0.0;
}

void generateTangentsAndBinormals(FbxNode* fbxNode, const EncoderArguments& arguments)
{
    if (!fbxNode)
        return;
    const char* name = fbxNode->GetName();
    if (name && strlen(name) > 0)
    {
        FbxMesh* fbxMesh = fbxNode->GetMesh();
        if (fbxMesh && arguments.isGenerateTangentBinormalId(string(name)))
        {
            fbxMesh->GenerateTangentsDataForAllUVSets();
        }
    }
    // visit child nodes
    const int childCount = fbxNode->GetChildCount();
    for (int i = 0; i < childCount; ++i)
    {
        generateTangentsAndBinormals(fbxNode->GetChild(i), arguments);
    }
}

FbxAnimCurve* getCurve(FbxPropertyT<FbxDouble3>& prop, FbxAnimLayer* animLayer, const char* pChannel)
{
#if FBXSDK_VERSION_MAJOR == 2013 && FBXSDK_VERSION_MINOR == 1
    return prop.GetCurve<FbxAnimCurve>(animLayer, pChannel);
#else
    return prop.GetCurve(animLayer, pChannel);
#endif
}

std::string toString(const FbxDouble3& fbxDouble)
{
    ostringstream stream;
    stream << fbxDouble[0] << ", " << fbxDouble[1] << ", " << fbxDouble[2];
    return stream.str();
}

std::string toString(const FbxDouble3& fbxDouble, double d)
{
    ostringstream stream;
    stream << fbxDouble[0] << ", " << fbxDouble[1] << ", " << fbxDouble[2] << ", " << d;
    return stream.str();
}

std::string toString(double value)
{
    ostringstream stream;
    stream << value;
    return stream.str();
}
