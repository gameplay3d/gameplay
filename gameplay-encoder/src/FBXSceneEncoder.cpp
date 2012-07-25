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
 * @param polyIndex The index of the polygon.
 * @param posInPoly The position in the polygon.
 * @param vertex The vertex to copy the texture coordinates to.
 */
static void loadTextureCoords(FbxMesh* fbxMesh, int polyIndex, int posInPoly, Vertex* vertex);

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
 * @param vertex The vertex to copy to.
 */
static void loadTangent(FbxMesh* fbxMesh, int vertexIndex, Vertex* vertex);

/**
 * Loads the binormal from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to load from.
 * @param vertexIndex The index of the vertex within fbxMesh.
 * @param vertex The vertex to copy to.
 */
static void loadBinormal(FbxMesh* fbxMesh, int vertexIndex, Vertex* vertex);

/**
 * Loads the vertex diffuse color from the mesh and adds it to the given vertex.
 * 
 * @param fbxMesh The mesh to load from.
 * @param vertexIndex The index of the vertex within fbxMesh.
 * @param vertex The vertex to copy to.
 */
static void loadVertexColor(FbxMesh* fbxMesh, int vertexIndex, Vertex* vertex);

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
 * Appends a key frame of the given node's transform at the given time.
 * 
 * @param fbxNode The node to get the matrix transform from.
 * @param time The key time to add and the time to get the transform from.
 * @param keyTimes The list of key times to append to.
 * @param keyValues The list of key values to append to.
 */
static void appendKeyFrame(FbxNode* fbxNode, float time, std::vector<float>* keyTimes, std::vector<float>* keyValues);

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
        printf("Call to FbxImporter::Initialize() failed.\n");
        printf("Error returned: %s\n\n", importer->GetLastErrorString());
        exit(-1);
    }
    
    FbxScene* fbxScene = FbxScene::Create(sdkManager,"__FBX_SCENE__");

    print("Loading FBX file.");
    importer->Import(fbxScene);
    importer->Destroy();

    // Determine if animations should be grouped.
    if (arguments.getGroupAnimationAnimationId().empty() && isGroupAnimationPossible(fbxScene))
    {
        if (promptUserGroupAnimations())
        {
            _autoGroupAnimations = true;
        }
    }

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
            fprintf(stderr, "Saving debug file: %s\n", path.c_str());
            if (!_gamePlayFile.saveText(path))
            {
                fprintf(stderr,"Error writing text file: %s\n", path.c_str());
            }
        }
    }
    else
    {
        fprintf(stderr, "Saving binary file: %s\n", outputFilePath.c_str());
        if (!_gamePlayFile.saveBinary(outputFilePath))
        {
            fprintf(stderr,"Error writing binary file: %s\n", outputFilePath.c_str());
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
    const std::string* targetId = NULL;

    const char* name = fbxNode->GetName();
    Node* node = _gamePlayFile.getNode(name);
    if (node)
    {
        targetId = &node->getId();
    }
    
    // Determine which properties are animated on this node
    // Find the transform at each key frame
    // TODO: Ignore properties that are not animated (scale, rotation, translation)
    // This should result in only one animation channel per animated node.

    float startTime = FLT_MAX, stopTime = -1.0f, frameRate = -FLT_MAX;
    bool tx = false, ty = false, tz = false, rx = false, ry = false, rz = false, sx = false, sy = false, sz = false;
    FbxAnimCurve* animCurve = NULL;
    animCurve = fbxNode->LclTranslation.GetCurve<FbxAnimCurve>(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (animCurve)
    {
        tx = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = fbxNode->LclTranslation.GetCurve<FbxAnimCurve>(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (animCurve)
    {
        ty = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = fbxNode->LclTranslation.GetCurve<FbxAnimCurve>(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (animCurve)
    {
        tz = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = fbxNode->LclRotation.GetCurve<FbxAnimCurve>(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (animCurve)
    {
        rx = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = fbxNode->LclRotation.GetCurve<FbxAnimCurve>(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (animCurve)
    {
        ry = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = fbxNode->LclRotation.GetCurve<FbxAnimCurve>(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (animCurve)
    {
        rz = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = fbxNode->LclScaling.GetCurve<FbxAnimCurve>(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
    if (animCurve)
    {
        sx = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = fbxNode->LclScaling.GetCurve<FbxAnimCurve>(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
    if (animCurve)
    {
        sy = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }
    animCurve = fbxNode->LclScaling.GetCurve<FbxAnimCurve>(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
    if (animCurve)
    {
        sz = true;
        findMinMaxTime(animCurve, &startTime, &stopTime, &frameRate);
    }

    bool translate = tx | ty | tz;
    bool scale = sx | sy | sz;
    bool rotate = rx | ry | rz;

    if (translate || rotate || scale)
    {
        assert(startTime != FLT_MAX);
        assert(stopTime >= 0.0f);
        AnimationChannel* channel = new AnimationChannel();
        channel->setTargetId(name);
        channel->setTargetAttribute(Transform::ANIMATE_SCALE_ROTATE_TRANSLATE);
        
        float increment = 1000.0f / frameRate;
        std::vector<float> keyTimes;
        std::vector<float> keyValues;
        for (float time = startTime; time < stopTime; time += increment)
        {
            appendKeyFrame(fbxNode, time, &keyTimes, &keyValues);
        }
        // Add the last key frame at exactly stopTime
        appendKeyFrame(fbxNode, stopTime, &keyTimes, &keyValues);

        channel->setKeyTimes(keyTimes);
        /*
        std::vector<float> newKeyValues;
        for (size_t i = 0, size = keyValues.size(); i < size; i += 10)
        {
            if (translate)
            {
                newKeyValues.push_back(keyValues[i+0]);
                newKeyValues.push_back(keyValues[i+1]);
                newKeyValues.push_back(keyValues[i+2]);
            }
            if (rotate)
            {
                newKeyValues.push_back(keyValues[i+3]);
                newKeyValues.push_back(keyValues[i+4]);
                newKeyValues.push_back(keyValues[i+5]);
                newKeyValues.push_back(keyValues[i+6]);
            }
            if (scale)
            {
                newKeyValues.push_back(keyValues[i+7]);
                newKeyValues.push_back(keyValues[i+8]);
                newKeyValues.push_back(keyValues[i+9]);
            }
        }
        channel->setKeyValues(newKeyValues);
        */
        channel->setKeyValues(keyValues);
        channel->setInterpolation(AnimationChannel::LINEAR);
        animation->add(channel);
        /*
        if (!translate)
        {
            addTranslateChannel(animation, fbxNode, startTime, stopTime);
        }
        if (!rotate)
        {
            printf("rotate?\n"); // TODO
        }
        if (!scale)
        {
            addScaleChannel(animation, fbxNode, startTime, stopTime);
        }
        */
        if (_groupAnimation != animation)
        {
            // TODO explains
            _gamePlayFile.addAnimation(animation);
        }
    }
}

void FBXSceneEncoder::loadAnimationLayer(FbxAnimLayer* fbxAnimLayer, FbxNode* fbxNode, const EncoderArguments& arguments)
{
    bool animationGroupId = false;
    const char* name = fbxNode->GetName();
    // Check if this node's animations are supposed to be grouped
    if (name)
    {
        std::string str = name;
        if (arguments.containsGroupNodeId(str))
        {
            animationGroupId = true;
            _groupAnimation = new Animation();
            _groupAnimation->setId(arguments.getAnimationId(str));
        }
    }
    Animation* animation = _groupAnimation;
    if (!_groupAnimation)
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
    fprintf(stderr,"%s\n", str);
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
    int i = 0;
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            m[i++] = (float)matrix.Get(row, col);
        }
    }
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
        warning("Unknown camera type in node");
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
            // No decay. Can assume we have an ambient light, because ambient lights in the scene are 
            // converted to point lights with no decay when exporting to FBX.
            light->setAmbientLight();
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
        warning("Unknown light type in node.");
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
                assert(linkedNode);
                if (linkedNode->GetSkeleton())
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

            loadTextureCoords(fbxMesh, polyIndex, posInPoly, &vertex);
            loadNormal(fbxMesh, vertexIndex, controlPointIndex, &vertex);
            loadTangent(fbxMesh, vertexIndex, &vertex);
            loadBinormal(fbxMesh, vertexIndex, &vertex);
            loadVertexColor(fbxMesh, vertexIndex, &vertex);

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
    if (vertex.hasTexCoord)
    {
        mesh->addVetexAttribute(TEXCOORD0, Vertex::TEXCOORD_COUNT);
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

void FBXSceneEncoder::warning(const std::string& message)
{
    printf("Warning: %s\n", message.c_str());
}

void FBXSceneEncoder::warning(const char* message)
{
    printf("Warning: %s\n", message);
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

void loadTextureCoords(FbxMesh* fbxMesh, int polyIndex, int posInPoly, Vertex* vertex)
{
    assert(fbxMesh && polyIndex >=0 && posInPoly >= 0);
    if (fbxMesh->GetElementUVCount() > 0)
    {
        // Get only the first UV coordinates.
        FbxGeometryElementUV* uv = fbxMesh->GetElementUV(0);
        switch (uv->GetMappingMode())
        {
        case FbxGeometryElement::eByControlPoint:
            switch (uv->GetReferenceMode())
            {
            case FbxGeometryElement::eDirect:
                vertex->hasTexCoord = true;
                vertex->texCoord.x = (float)uv->GetDirectArray().GetAt(polyIndex)[0];
                vertex->texCoord.y = (float)uv->GetDirectArray().GetAt(polyIndex)[1];
                break;
            case FbxGeometryElement::eIndexToDirect:
                {
                    int id = uv->GetIndexArray().GetAt(polyIndex);
                    vertex->hasTexCoord = true;
                    vertex->texCoord.x = (float)uv->GetDirectArray().GetAt(id)[0];
                    vertex->texCoord.y = (float)uv->GetDirectArray().GetAt(id)[1];
                }
                break;
            default:
                break;
            }
            break;
        case FbxGeometryElement::eByPolygonVertex:
            {
                int lTextureUVIndex = fbxMesh->GetTextureUVIndex(polyIndex, posInPoly);
                switch (uv->GetReferenceMode())
                {
                case FbxGeometryElement::eDirect:
                case FbxGeometryElement::eIndexToDirect:
                    vertex->hasTexCoord = true;
                    vertex->texCoord.x = (float)uv->GetDirectArray().GetAt(lTextureUVIndex)[0];
                    vertex->texCoord.y = (float)uv->GetDirectArray().GetAt(lTextureUVIndex)[1];
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
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

void loadTangent(FbxMesh* fbxMesh, int vertexIndex, Vertex* vertex)
{
    if (fbxMesh->GetElementTangentCount() > 0)
    {
        // Get only the first tangent
        FbxGeometryElementTangent* tangent = fbxMesh->GetElementTangent(0);
        if (tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
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

void loadBinormal(FbxMesh* fbxMesh, int vertexIndex, Vertex* vertex)
{
    if (fbxMesh->GetElementBinormalCount() > 0)
    {
        // Get only the first binormal.
        FbxGeometryElementBinormal* binormal = fbxMesh->GetElementBinormal(0);
        if (binormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
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

void loadVertexColor(FbxMesh* fbxMesh, int vertexIndex, Vertex* vertex)
{
    if (fbxMesh->GetElementVertexColorCount() > 0)
    {
        // Get only the first vertex color.
        FbxGeometryElementVertexColor* vertexColor = fbxMesh->GetElementVertexColor(0);
        if (vertexColor->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
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
                FbxNode* linkedNode = cluster->GetLink();
                assert(linkedNode);

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

void appendKeyFrame(FbxNode* fbxNode, float time, std::vector<float>* keyTimes, std::vector<float>* keyValues)
{
    FbxAMatrix fbxMatrix;
    Matrix matrix;
    FbxTime kTime;
    kTime.SetMilliSeconds((FbxLongLong)time);
    fbxMatrix = fbxNode->EvaluateLocalTransform(kTime);
    copyMatrix(fbxMatrix, matrix);

    Vector3 scale;
    Quaternion rotation;
    Vector3 translation;
    matrix.decompose(&scale, &rotation, &translation);
    rotation.normalize();

    keyTimes->push_back(time);
    keyValues->push_back(scale.x);
    keyValues->push_back(scale.y);
    keyValues->push_back(scale.z);
    keyValues->push_back(rotation.x);
    keyValues->push_back(rotation.y);
    keyValues->push_back(rotation.z);
    keyValues->push_back(rotation.w);
    keyValues->push_back(translation.x);
    keyValues->push_back(translation.y);
    keyValues->push_back(translation.z);
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

#endif