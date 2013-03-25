#ifdef USE_FBX

#include <algorithm>
#include <string>

#include "FBXSceneEncoder.h"
#include "EncoderArguments.h"

using namespace gameplay;

/**
 * Returns the aspect ratio from the given camera.
 * 
 * @param fbxCamera The FBX camera to get the aspect ratio from.
 * 
 * @return The aspect ratio from the camera.
 */
static float getAspectRatio(FbxCamera* fbxCamera);

/**
 * Returns the field of view Y from the given camera.
 * 
 * @param fbxCamera The camera to get the fiew of view from.
 * 
 * @return The field of view Y.
 */
static float getFieldOfView(FbxCamera* fbxCamera);

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
static void loadTextureCoords(FbxMesh* fbxMesh, const FbxGeometryElementUV* uvs, int uvSetIndex, int polyIndex, int posInPoly, int meshVertexIndex, Vertex* vertex);

/**
 * Loads the normal from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to get the polygon from.
 * @param vertexIndex The vertex index in the mesh.
 * @param controlPointIndex The control point index.
 * @param vertex The vertex to copy to.
 */
static void loadNormal(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex);

/**
 * Loads the tangent from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to load from.
 * @param vertexIndex The index of the vertex within fbxMesh.
 * @param controlPointIndex The control point index.
 * @param vertex The vertex to copy to.
 */
static void loadTangent(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex);

/**
 * Loads the binormal from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to load from.
 * @param vertexIndex The index of the vertex within fbxMesh.
 * @param controlPointIndex The control point index.
 * @param vertex The vertex to copy to.
 */
static void loadBinormal(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex);

/**
 * Loads the vertex diffuse color from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to load from.
 * @param vertexIndex The index of the vertex within fbxMesh.
 * @param controlPointIndex The control point index.
 * @param vertex The vertex to copy to.
 */
static void loadVertexColor(FbxMesh* fbxMesh, int vertexIndex, int controlPointIndex, Vertex* vertex);

/**
 * Loads the blend weight and blend indices data into the vertex.
 * 
 * @param vertexWeights List of vertex weights. The x member contains the blendIndices. The y member contains the blendWeights.
 * @param vertex The vertex to copy the blend data to.
 */
static void loadBlendData(const std::vector<Vector2>& vertexWeights, Vertex* vertex);

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
static bool loadBlendWeights(FbxMesh* fbxMesh, std::vector<std::vector<Vector2> >& weights);

/**
 * Copies from an FBX matrix to a float[16] array.
 */
static void copyMatrix(const FbxMatrix& fbxMatrix, float* matrix);

/**
 * Copies from an FBX matrix to a gameplay matrix.
 */
static void copyMatrix(const FbxMatrix& fbxMatrix, Matrix& matrix);

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
static void findMinMaxTime(FbxAnimCurve* animCurve, float* startTime, float* stopTime, float* frameRate);

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
static void appendKeyFrame(FbxNode* fbxNode, AnimationChannel* channel, float time, const Vector3& scale, const Quaternion& rotation, const Vector3& translation);

/**
 * Decomposes the given node's matrix transform at the given time and copies to scale, rotation and translation.
 * 
 * @param fbxNode The node to get the matrix transform from.
 * @param time The time to get the matrix transform from.
 * @param scale The scale to copy to.
 * @param rotation The rotation to copy to.
 * @param translation The translation to copy to.
 */
static void decompose(FbxNode* fbxNode, float time, Vector3* scale, Quaternion* rotation, Vector3* translation);

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
static AnimationChannel* createAnimationChannel(FbxNode* fbxNode, unsigned int targetAttrib, const std::vector<float>& keyTimes, const std::vector<float>& keyValues);

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

/**
 * Recursively generates the tangents and binormals for all nodes that were specified in the command line arguments.
 */
void generateTangentsAndBinormals(FbxNode* fbxNode, const EncoderArguments& arguments);

FbxAnimCurve* getCurve(FbxPropertyT<FbxDouble3>& prop, FbxAnimLayer* animLayer, const char* pChannel)
{
#if FBXSDK_VERSION_MAJOR == 2013 && FBXSDK_VERSION_MINOR == 1
    return prop.GetCurve<FbxAnimCurve>(animLayer, pChannel);
#else
    return prop.GetCurve(animLayer, pChannel);
#endif
}

////////////////////////////////////
// Member Functions
////////////////////////////////////

FBXSceneEncoder::FBXSceneEncoder()
    : _groupAnimation(NULL), _autoGroupAnimations(false)
{
}

FBXSceneEncoder::~FBXSceneEncoder()
{
}

void FBXSceneEncoder::write(const std::string& filepath, const EncoderArguments& arguments)
{
    FbxManager* sdkManager = FbxManager::Create();
    FbxIOSettings *ios = FbxIOSettings::Create(sdkManager, IOSROOT);
    sdkManager->SetIOSettings(ios);
    FbxImporter* importer = FbxImporter::Create(sdkManager,"");
    
    if (!importer->Initialize(filepath.c_str(), -1, sdkManager->GetIOSettings()))
    {
        LOG(1, "Call to FbxImporter::Initialize() failed.\n");
        LOG(1, "Error returned: %s\n\n", importer->GetLastErrorString());
        exit(-1);
    }
    
    FbxScene* fbxScene = FbxScene::Create(sdkManager,"__FBX_SCENE__");

    print("Loading FBX file.");
    importer->Import(fbxScene);
    importer->Destroy();

    // Determine if animations should be grouped.
    if (arguments.getGroupAnimationAnimationId().empty() && isGroupAnimationPossible(fbxScene))
    {
        if ( arguments.getAnimationGrouping()==EncoderArguments::ANIMATIONGROUP_AUTO || 
            (arguments.getAnimationGrouping()==EncoderArguments::ANIMATIONGROUP_PROMPT && promptUserGroupAnimations()))
        {
            _autoGroupAnimations = true;
        }
    }

    if (arguments.tangentBinormalIdCount() > 0)
        generateTangentsAndBinormals(fbxScene->GetRootNode(), arguments);

    print("Loading Scene.");
    loadScene(fbxScene);
    print("Loading animations.");
    loadAnimations(fbxScene, arguments);
    sdkManager->Destroy();

    print("Optimizing GamePlay Binary.");
    _gamePlayFile.adjust();
    if (_autoGroupAnimations)
    {
        _gamePlayFile.groupMeshSkinAnimations();
    }
    
    std::string outputFilePath = arguments.getOutputFilePath();

    if (arguments.textOutputEnabled())
    {
        int pos = outputFilePath.find_last_of('.');
        if (pos > 2)
        {
            std::string path = outputFilePath.substr(0, pos);
            path.append(".xml");
            LOG(1, "Saving debug file: %s\n", path.c_str());
            if (!_gamePlayFile.saveText(path))
            {
                LOG(1, "Error writing text file: %s\n", path.c_str());
            }
        }
    }
    else
    {
        LOG(1, "Saving binary file: %s\n", outputFilePath.c_str());
        if (!_gamePlayFile.saveBinary(outputFilePath))
        {
            LOG(1, "Error writing binary file: %s\n", outputFilePath.c_str());
        }
    }
}

void FBXSceneEncoder::loadScene(FbxScene* fbxScene)
{
    Scene* scene = new Scene();
    scene->setId(fbxScene->GetName());
    if (scene->getId().length() == 0)
    {
        scene->setId("__SCENE__");
    }

    // Load all of the nodes and their contents.
    FbxNode* rootNode = fbxScene->GetRootNode();
    if (rootNode)
    {
        print("Triangulate.");
        triangulateRecursive(rootNode);

        print("Load nodes.");
        // Don't include the FBX root node in the GPB.
        const int childCount = rootNode->GetChildCount();
        for (int i = 0; i < childCount; ++i)
        {
            Node* node = loadNode(rootNode->GetChild(i));
            if (node)
            {
                scene->add(node);
            }
        }
    }

    // Load the MeshSkin information from the scene's poses.
    loadBindShapes(fbxScene);

    // Find the ambient light of the scene
    FbxColor ambientColor = fbxScene->GetGlobalSettings().GetAmbientColor();
    scene->setAmbientColor((float)ambientColor.mRed, (float)ambientColor.mGreen, (float)ambientColor.mBlue);
    
    // Assign the first camera node (if there is one) in the scene as the active camera
    // This ensures that if there's a camera in the scene that it is assigned as the 
    // active camera.
    // TODO: add logic to find the "active" camera node in the fbxScene
    scene->setActiveCameraNode(scene->getFirstCameraNode());
    
    _gamePlayFile.addScene(scene);
}

void FBXSceneEncoder::loadAnimationChannels(FbxAnimLayer* animLayer, FbxNode* fbxNode, Animation* animation)
{
    const char* name = fbxNode->GetName();
    //Node* node = _gamePlayFile.getNode(name);

    // Determine which properties are animated on this node
    // Find the transform at each key frame
    // TODO: Ignore properties that are not animated (scale, rotation, translation)
    // This should result in only one animation channel per animated node.

    float startTime = FLT_MAX, stopTime = -1.0f, frameRate = -FLT_MAX;
    bool tx = false, ty = false, tz = false, rx = false, ry = false, rz = false, sx = false, sy = false, sz = false;
    FbxAnimCurve* animCurve = NULL;
    animCurve = getCurve(fbxNode->LclTranslation, animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (animCurve)
    {
        tx = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = getCurve(fbxNode->LclTranslation, animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (animCurve)
    {
        ty = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = getCurve(fbxNode->LclTranslation, animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (animCurve)
    {
        tz = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = getCurve(fbxNode->LclRotation, animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (animCurve)
    {
        rx = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = getCurve(fbxNode->LclRotation, animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (animCurve)
    {
        ry = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = getCurve(fbxNode->LclRotation, animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (animCurve)
    {
        rz = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = getCurve(fbxNode->LclScaling, animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (animCurve)
    {
        sx = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = getCurve(fbxNode->LclScaling, animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (animCurve)
    {
        sy = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = getCurve(fbxNode->LclScaling, animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (animCurve)
    {
        sz = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }

    if (!(sx || sy || sz || rx || ry || rz || tx || ty || tz))
        return; // no animation channels

    assert(startTime != FLT_MAX);
    assert(stopTime >= 0.0f);

    // Determine which animation channels to create
    std::vector<unsigned int> channelAttribs;
    if (sx && sy && sz)
    {
        if (rx || ry || rz)
        {
            if (tx && ty && tz)
            {
                channelAttribs.push_back(Transform::ANIMATE_SCALE_ROTATE_TRANSLATE);
            }
            else
            {
                channelAttribs.push_back(Transform::ANIMATE_SCALE_ROTATE);
                if (tx)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_X);
                if (ty)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_Y);
                if (tz)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_Z);
            }
        }
        else
        {
            if (tx && ty && tz)
            {
                channelAttribs.push_back(Transform::ANIMATE_SCALE_TRANSLATE);
            }
            else
            {
                channelAttribs.push_back(Transform::ANIMATE_SCALE);
                if (tx)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_X);
                if (ty)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_Y);
                if (tz)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_Z);
            }
        }
    }
    else
    {
        if (rx || ry || rz)
        {
            if (tx && ty && tz)
            {
                channelAttribs.push_back(Transform::ANIMATE_ROTATE_TRANSLATE);
            }
            else
            {
                channelAttribs.push_back(Transform::ANIMATE_ROTATE);
                if (tx)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_X);
                if (ty)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_Y);
                if (tz)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_Z);
            }
        }
        else
        {
            if (tx && ty && tz)
            {
                channelAttribs.push_back(Transform::ANIMATE_TRANSLATE);
            }
            else
            {
                if (tx)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_X);
                if (ty)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_Y);
                if (tz)
                    channelAttribs.push_back(Transform::ANIMATE_TRANSLATE_Z);
            }
        }

        if (sx)
            channelAttribs.push_back(Transform::ANIMATE_SCALE_X);
        if (sy)
            channelAttribs.push_back(Transform::ANIMATE_SCALE_Y);
        if (sz)
            channelAttribs.push_back(Transform::ANIMATE_SCALE_Z);
    }
    unsigned int channelCount = channelAttribs.size();
    assert(channelCount > 0);

    // Allocate channel list
    int channelStart = animation->getAnimationChannelCount();
    for (unsigned int i = 0; i < channelCount; ++i)
    {
        AnimationChannel* channel = new AnimationChannel();
        channel->setTargetId(name);
        channel->setInterpolation(AnimationChannel::LINEAR);
        channel->setTargetAttribute(channelAttribs[i]);
        animation->add(channel);
    }

    // Evaulate animation curve in increments of frameRate and populate channel data.
    FbxAMatrix fbxMatrix;
    Matrix matrix;
    double increment = 1000.0 / (double)frameRate;
    int frameCount = (int)ceil((double)(stopTime - startTime) / increment) + 1; // +1 because stop time is inclusive
    for (int frame = 0; frame < frameCount; ++frame)
    {
        double time = startTime + (frame * (double)increment);

        // Note: We used to clamp time to stop time, but FBX sdk does not always produce
        // and accurate stopTime (sometimes it is rounded down for some reason), so I'm
        // disabling this clamping for now as it seems more accurate under normal circumstances.
        //time = std::min(time, (double)stopTime);

        // Evalulate the animation at this time
        FbxTime kTime;
        kTime.SetMilliSeconds((FbxLongLong)time);
        fbxMatrix = fbxNode->EvaluateLocalTransform(kTime);
        copyMatrix(fbxMatrix, matrix);

        // Decompose the evalulated transformation matrix into separate
        // scale, rotation and translation.
        Vector3 scale;
        Quaternion rotation;
        Vector3 translation;
        matrix.decompose(&scale, &rotation, &translation);
        rotation.normalize();

        // Append keyframe data to all channels
        for (unsigned int i = channelStart, channelEnd = channelStart + channelCount; i < channelEnd; ++i)
        {
            appendKeyFrame(fbxNode, animation->getAnimationChannel(i), time, scale, rotation, translation);
        }
    }

    if (_groupAnimation != animation)
    {
        // TODO explain
        _gamePlayFile.addAnimation(animation);
    }
}

void FBXSceneEncoder::loadAnimationLayer(FbxAnimLayer* fbxAnimLayer, FbxNode* fbxNode, const EncoderArguments& arguments)
{
    bool animationGroupId = false;
    const char* name = fbxNode->GetName();
    // Check if this node's animations are supposed to be grouped
    if (name && arguments.containsGroupNodeId(name))
    {
        animationGroupId = true;
        _groupAnimation = new Animation();
        _groupAnimation->setId(arguments.getAnimationId(name));
    }
    Animation* animation = _groupAnimation;
    if (!animation)
    {
        animation = new Animation();
        animation->setId(name);
    }
    loadAnimationChannels(fbxAnimLayer, fbxNode, animation);

    const int childCount = fbxNode->GetChildCount();
    for (int modelCount = 0; modelCount < childCount; ++modelCount)
    {
        loadAnimationLayer(fbxAnimLayer, fbxNode->GetChild(modelCount), arguments);
    }
    if (animationGroupId)
    {
        _gamePlayFile.addAnimation(_groupAnimation);
        _groupAnimation = NULL;
    }
}

void FBXSceneEncoder::loadAnimations(FbxScene* fbxScene, const EncoderArguments& arguments)
{
    FbxAnimEvaluator* evaluator = fbxScene->GetEvaluator();
    if (!evaluator)
        return;
    FbxAnimStack* animStack = evaluator->GetContext();
    if (!animStack)
        return;

    for (int i = 0; i < fbxScene->GetSrcObjectCount(FBX_TYPE(FbxAnimStack)); ++i)
    {
        FbxAnimStack* animStack = FbxCast<FbxAnimStack>(fbxScene->GetSrcObject(FBX_TYPE(FbxAnimStack), i));
        int nbAnimLayers = animStack->GetMemberCount(FBX_TYPE(FbxAnimLayer));
        for (int l = 0; l < nbAnimLayers; ++l)
        {
            FbxAnimLayer* animLayer = animStack->GetMember(FBX_TYPE(FbxAnimLayer), l);
            loadAnimationLayer(animLayer, fbxScene->GetRootNode(), arguments);
        }
    }
}

Node* FBXSceneEncoder::loadNode(FbxNode* fbxNode)
{
    Node* node = NULL;

    // Check if this node has already been loaded
    const char* id = fbxNode->GetName();
    if (id && strlen(id) > 0)
    {
        node = _gamePlayFile.getNode(fbxNode->GetName());
        if (node)
        {
            return node;
        }
    }
    node = new Node();
    if (id)
    {
        node->setId(id);
    }
    _gamePlayFile.addNode(node);

    transformNode(fbxNode, node);
    
    loadCamera(fbxNode, node);
    loadLight(fbxNode, node);
    loadModel(fbxNode, node);

    if (fbxNode->GetSkeleton())
    {
        // Indicate that this is a joint node for the purpose of debugging.
        // The XML debug output will print that this node is a joint.
        node->setIsJoint(true);
    }

    // Load child nodes
    const int childCount = fbxNode->GetChildCount();
    for (int i = 0; i < childCount; ++i)
    {
        Node* child = loadNode(fbxNode->GetChild(i));
        if (child)
        {
            node->addChild(child);
        }
    }
    return node;
}

Mesh* FBXSceneEncoder::getMesh(FbxUInt64 meshId)
{
    // Check if this mesh was already loaded.
    std::map<FbxUInt64, Mesh*>::iterator it = _meshes.find(meshId);
    if (it != _meshes.end())
    {
        return it->second;
    }
    return NULL;
}

void FBXSceneEncoder::saveMesh(FbxUInt64 meshId, Mesh* mesh)
{
    assert(mesh);
    if (!getMesh(meshId))
    {
        _meshes[meshId] = mesh;
    }
}

void FBXSceneEncoder::print(const char* str)
{
    LOG(1, "%s\n", str);
}

void FBXSceneEncoder::transformNode(FbxNode* fbxNode, Node* node)
{
    FbxAMatrix matrix;
    if (fbxNode->GetCamera() || fbxNode->GetLight())
    {
        // TODO: Why is this necessary for Camera and Light?
        matrix.SetTRS(fbxNode->LclTranslation.Get(), fbxNode->LclRotation.Get(), fbxNode->LclScaling.Get());
    }
    else
    {
        matrix = fbxNode->EvaluateLocalTransform();
    }

    float m[16];
    copyMatrix(matrix, m);
    node->setTransformMatrix(m);
}

void FBXSceneEncoder::loadBindShapes(FbxScene* fbxScene)
{
    float m[16];
    const int poseCount = fbxScene->GetPoseCount();
    for (int i = 0; i < poseCount; ++i)
    {
        FbxPose* pose = fbxScene->GetPose(i);
        assert(pose);
        if (pose->IsBindPose() && pose->GetCount() > 0)
        {
            FbxNode* fbxNode = pose->GetNode(0);
            if (fbxNode->GetMesh() != NULL)
            {
                Node* node = _gamePlayFile.getNode(fbxNode->GetName());
                assert(node && node->getModel());

                Model* model = node->getModel();
                if (model && model->getSkin())
                {
                    MeshSkin* skin = model->getSkin();
                    copyMatrix(pose->GetMatrix(0), m);
                    skin->setBindShape(m);
                }
            }
        }
    }
}

void FBXSceneEncoder::loadCamera(FbxNode* fbxNode, Node* node)
{
    FbxCamera* fbxCamera = fbxNode->GetCamera();
    if (!fbxCamera)
    {
        return;
    }
    Camera* camera = new Camera();
    const char* name = fbxNode->GetName();
    if (name)
    {
        std::string id(name);
        id.append("_Camera");
        camera->setId(id);
    }
    camera->setAspectRatio(getAspectRatio(fbxCamera));
    camera->setNearPlane((float)fbxCamera->NearPlane.Get());
    camera->setFarPlane((float)fbxCamera->FarPlane.Get());

    if (fbxCamera->ProjectionType.Get() == FbxCamera::eOrthogonal)
    {
        camera->setOrthographic();
        camera->setViewportWidth((float)fbxCamera->GetApertureWidth());
        camera->setViewportWidth((float)fbxCamera->GetApertureHeight());
        // xmag in FBX can be calculated from: OrthoZoom * 30.0 / 2.0
        camera->setViewportWidth((float)fbxCamera->OrthoZoom.Get() * 15.0f);
    }
    else if (fbxCamera->ProjectionType.Get() == FbxCamera::ePerspective)
    {
        camera->setPerspective();
        camera->setFieldOfView(getFieldOfView(fbxCamera));
    }
    else
    {
        LOG(2, "Warning: Unknown camera type in node.\n");
        return;
    }
    _gamePlayFile.addCamera(camera);
    node->setCamera(camera);
}

void FBXSceneEncoder::loadLight(FbxNode* fbxNode, Node* node)
{
    FbxLight* fbxLight = fbxNode->GetLight();
    if (!fbxLight)
    {
        return;
    }
    Light* light = new Light();
    const char* name = fbxNode->GetName();
    if (name)
    {
        std::string id(name);
        id.append("_Light");
        light->setId(id);
    }

    FbxDouble3 color = fbxLight->Color.Get();
    light->setColor((float)color[0], (float)color[1], (float)color[2]);
    
    switch (fbxLight->LightType.Get())
    {
    case FbxLight::ePoint:
    {
        FbxLight::EDecayType decayType = fbxLight->DecayType.Get();
        switch (decayType)
        {
        case FbxLight::eNone:
            // FBX does not support ambients lights so ambient lights are converted 
            // to point lights with no decay and visibility set to false.
            if (fbxNode->GetVisibility())
            {
                light->setPointLight();
            }
            else
            {
                light->setAmbientLight();
            }
            break;
        case FbxLight::eLinear:
            light->setPointLight();
            light->setLinearAttenuation((float)fbxLight->DecayStart.Get());
            break;
        case FbxLight::eQuadratic:
            light->setPointLight();
            light->setQuadraticAttenuation((float)fbxLight->DecayStart.Get());
            break;
        case FbxLight::eCubic:
        default:
            // Not supported..
            break;
        }
        break;
    }
    case FbxLight::eDirectional:
    {
        light->setDirectionalLight();
        break;
    }
    case FbxLight::eSpot:
    {
        light->setSpotLight();

        FbxLight::EDecayType decayType = fbxLight->DecayType.Get();
        switch (decayType)
        {
        case FbxLight::eNone:
            // No decay.
            break;
        case FbxLight::eLinear:
            light->setLinearAttenuation((float)fbxLight->DecayStart.Get());
            break;  
        case FbxLight::eQuadratic:
            light->setQuadraticAttenuation((float)fbxLight->DecayStart.Get());
            break;
        case FbxLight::eCubic:
            // Not supported..
            break;
        }

        light->setFalloffAngle(MATH_DEG_TO_RAD((float)fbxLight->OuterAngle.Get())); // fall off angle
        break;
    }
    default:
    {
        LOG(2, "Warning: Unknown light type in node.\n");
        return;
    }
    }

    _gamePlayFile.addLight(light);
    node->setLight(light);
}

void FBXSceneEncoder::loadModel(FbxNode* fbxNode, Node* node)
{
    FbxMesh* fbxMesh = fbxNode->GetMesh();
    if (!fbxMesh)
    {
        return;
    }
    if (fbxMesh->IsTriangleMesh())
    {
        Mesh* mesh = loadMesh(fbxMesh);
        Model* model = new Model();
        model->setMesh(mesh);
        node->setModel(model);
        loadSkin(fbxMesh, model);
        if (model->getSkin())
        {
            // TODO: explain
            node->resetTransformMatrix();
        }
    }
}

void FBXSceneEncoder::loadSkin(FbxMesh* fbxMesh, Model* model)
{
    const int deformerCount = fbxMesh->GetDeformerCount();
    for (int i = 0; i < deformerCount; ++i)
    {
        FbxDeformer* deformer = fbxMesh->GetDeformer(i);
        if (deformer->GetDeformerType() == FbxDeformer::eSkin)
        {
            FbxSkin* fbxSkin = static_cast<FbxSkin*>(deformer);

            MeshSkin* skin = new MeshSkin();

            std::vector<std::string> jointNames;
            std::vector<Node*> joints;
            std::vector<Matrix> bindPoses;

            const int clusterCount = fbxSkin->GetClusterCount();
            for (int j = 0; j < clusterCount; ++j)
            {
                FbxCluster* cluster = fbxSkin->GetCluster(j);
                assert(cluster);
                FbxNode* linkedNode = cluster->GetLink();
                if (linkedNode && linkedNode->GetSkeleton())
                {
                    const char* jointName = linkedNode->GetName();
                    assert(jointName);
                    jointNames.push_back(jointName);
                    Node* joint = loadNode(linkedNode);
                    assert(joint);
                    joints.push_back(joint);

                    FbxAMatrix matrix;
                    cluster->GetTransformLinkMatrix(matrix);
                    Matrix m;
                    copyMatrix(matrix.Inverse(), m);
                    bindPoses.push_back(m);
                }
            }
            skin->setJointNames(jointNames);
            skin->setJoints(joints);
            skin->setBindPoses(bindPoses);
            model->setSkin(skin);
            break;
        }
    }
}

Mesh* FBXSceneEncoder::loadMesh(FbxMesh* fbxMesh)
{
    // Check if this mesh has already been loaded.
    Mesh* mesh = getMesh(fbxMesh->GetUniqueID());
    if (mesh)
    {
        return mesh;
    }
    mesh = new Mesh();
    // GamePlay requires that a mesh have a unique ID but FbxMesh doesn't have a string ID.
    const char* name = fbxMesh->GetNode()->GetName();
    if (name)
    {
        std::string id(name);
        id.append("_Mesh");
        mesh->setId(id);
    }

    // The number of mesh parts is equal to the number of materials that affect this mesh.
    // There is always at least one mesh part.
    std::vector<MeshPart*> meshParts;
    const int materialCount = fbxMesh->GetNode()->GetMaterialCount();
    int meshPartSize = (materialCount > 0) ? materialCount : 1;
    for (int i = 0; i < meshPartSize; ++i)
    {
        meshParts.push_back(new MeshPart());
    }

    // Find the blend weights and blend indices if this mesh is skinned.
    std::vector<std::vector<Vector2> > weights;
    bool hasSkin = loadBlendWeights(fbxMesh, weights);
    
    // Get list of uv sets for mesh
    FbxStringList uvSetNameList;
    fbxMesh->GetUVSetNames(uvSetNameList);
    const int uvSetCount = uvSetNameList.GetCount();

    int vertexIndex = 0;
    FbxVector4* controlPoints = fbxMesh->GetControlPoints();
    const int polygonCount = fbxMesh->GetPolygonCount();
    for (int polyIndex = 0; polyIndex < polygonCount; ++polyIndex)
    {
        const int polygonSize = fbxMesh->GetPolygonSize(polyIndex);
        for (int posInPoly = 0; posInPoly < polygonSize; ++posInPoly)
        {
            int controlPointIndex = fbxMesh->GetPolygonVertex(polyIndex, posInPoly);
            Vertex vertex;

            FbxVector4& position = controlPoints[controlPointIndex];
            vertex.position.x = (float)position[0];
            vertex.position.y = (float)position[1];
            vertex.position.z = (float)position[2];

            // Load tex coords for all uv sets
            for (int uvSetIndex = 0; uvSetIndex < uvSetCount; ++uvSetIndex)
            {
                const FbxGeometryElementUV* uvElement = fbxMesh->GetElementUV(uvSetNameList.GetStringAt(uvSetIndex));
                if (uvElement)
                    loadTextureCoords(fbxMesh, uvElement, uvSetIndex, polyIndex, posInPoly, vertexIndex, &vertex);
            }

            // Load other data
            loadNormal(fbxMesh, vertexIndex, controlPointIndex, &vertex);
            loadTangent(fbxMesh, vertexIndex, controlPointIndex, &vertex);
            loadBinormal(fbxMesh, vertexIndex, controlPointIndex, &vertex);
            loadVertexColor(fbxMesh, vertexIndex, controlPointIndex, &vertex);

            if (hasSkin)
            {
                loadBlendData(weights[controlPointIndex], &vertex);
            }

            // Determine which mesh part this vertex index should be added to based on the material that affects it.
            int meshPartIndex = 0;
            const int elementMatrialCount = fbxMesh->GetElementMaterialCount();
            for (int k = 0; k < elementMatrialCount; ++k)
            {
                FbxGeometryElementMaterial* elementMaterial = fbxMesh->GetElementMaterial(k);
                meshPartIndex = elementMaterial->GetIndexArray().GetAt(polyIndex);
            }

            // Add the vertex to the mesh if it hasn't already been added and find the vertex index.
            unsigned int index;
            if (mesh->contains(vertex))
            {
                index = mesh->getVertexIndex(vertex);
            }
            else
            {
                index = mesh->addVertex(vertex);
            }
            meshParts[meshPartIndex]->addIndex(index);
            vertexIndex++;
        }
    }

    const size_t meshpartsSize = meshParts.size();
    for (size_t i = 0; i < meshpartsSize; ++i)
    {
        mesh->addMeshPart(meshParts[i]);
    }

    // The order that the vertex elements are add to the list matters.
    // It should be the same order as how the Vertex data is written.

    // Position
    mesh->addVetexAttribute(POSITION, Vertex::POSITION_COUNT);

    const Vertex& vertex = mesh->vertices[0];
    // Normals
    if (vertex.hasNormal)
    {
        mesh->addVetexAttribute(NORMAL, Vertex::NORMAL_COUNT);
    }
    // Tangents
    if (vertex.hasTangent)
    {
        mesh->addVetexAttribute(TANGENT, Vertex::TANGENT_COUNT);
    }
    // Binormals
    if (vertex.hasBinormal)
    {
        mesh->addVetexAttribute(BINORMAL, Vertex::BINORMAL_COUNT);
    }
    // Texture Coordinates
    for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
    {
        if (vertex.hasTexCoord[i])
        {
            mesh->addVetexAttribute(TEXCOORD0 + i, Vertex::TEXCOORD_COUNT);
        }
    }
    // Diffuse Color
    if (vertex.hasDiffuse)
    {
        mesh->addVetexAttribute(COLOR, Vertex::DIFFUSE_COUNT);
    }
    // Skinning BlendWeights BlendIndices
    if (vertex.hasWeights)
    {
        mesh->addVetexAttribute(BLENDWEIGHTS, Vertex::BLEND_WEIGHTS_COUNT);
        mesh->addVetexAttribute(BLENDINDICES, Vertex::BLEND_INDICES_COUNT);
    }

    _gamePlayFile.addMesh(mesh);
    saveMesh(fbxMesh->GetUniqueID(), mesh);
    return mesh;
}

void FBXSceneEncoder::triangulateRecursive(FbxNode* fbxNode)
{
    // Triangulate all NURBS, patch and mesh under this node recursively.
    FbxNodeAttribute* nodeAttribute = fbxNode->GetNodeAttribute();

    if (nodeAttribute)
    {
        if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
            nodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
            nodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbsSurface ||
            nodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
        {
            FbxGeometryConverter converter(fbxNode->GetFbxManager());
            converter.TriangulateInPlace(fbxNode);
        }
    }

    const int childCount = fbxNode->GetChildCount();
    for (int childIndex = 0; childIndex < childCount; ++childIndex)
    {
        triangulateRecursive(fbxNode->GetChild(childIndex));
    }
}

////////////////////////////////////
// Functions
////////////////////////////////////

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
    assert(fbxMesh && polyIndex >=0 && posInPoly >= 0);

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

void loadBlendData(const std::vector<Vector2>& vertexWeights, Vertex* vertex)
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

bool loadBlendWeights(FbxMesh* fbxMesh, std::vector<std::vector<Vector2> >& weights)
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
            fbxSkin = static_cast<FbxSkin*>(deformer);
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
    std::vector<float>& keyValues = channel->getKeyValues();
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

AnimationChannel* createAnimationChannel(FbxNode* fbxNode, unsigned int targetAttrib, const std::vector<float>& keyTimes, const std::vector<float>& keyValues)
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
    std::vector<float> keyTimes;
    std::vector<float> keyValues;
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
    std::vector<float> keyTimes;
    std::vector<float> keyValues;
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
                FbxSkin* fbxSkin = static_cast<FbxSkin*>(deformer);
                if (fbxSkin)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void generateTangentsAndBinormals(FbxNode* fbxNode, const EncoderArguments& arguments)
{
    if (!fbxNode)
        return;
    const char* name = fbxNode->GetName();
    if (name && strlen(name) > 0)
    {
        FbxMesh* fbxMesh = fbxNode->GetMesh();
        if (fbxMesh && arguments.isGenerateTangentBinormalId(std::string(name)))
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

#endif
