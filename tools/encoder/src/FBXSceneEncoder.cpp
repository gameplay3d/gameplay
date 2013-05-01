#include <algorithm>
#include <string>
#include <sstream>

#include "FBXSceneEncoder.h"
#include "FBXUtil.h"
#include "Sampler.h"

using namespace gameplay;
using std::string;
using std::vector;
using std::map;
using std::ostringstream;

// Fix bad material names
static void fixMaterialName(string& name);

FBXSceneEncoder::FBXSceneEncoder()
    : _groupAnimation(NULL), _autoGroupAnimations(false)
{
}

FBXSceneEncoder::~FBXSceneEncoder()
{
}

void FBXSceneEncoder::write(const string& filepath, const EncoderArguments& arguments)
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
    {
        generateTangentsAndBinormals(fbxScene->GetRootNode(), arguments);
    }

    print("Loading Scene.");
    loadScene(fbxScene);
    print("Load materials");
    loadMaterials(fbxScene);
    print("Loading animations.");
    loadAnimations(fbxScene, arguments);
    sdkManager->Destroy();

    print("Optimizing GamePlay Binary.");
    _gamePlayFile.adjust();
    if (_autoGroupAnimations)
    {
        _gamePlayFile.groupMeshSkinAnimations();
    }
    
    string outputFilePath = arguments.getOutputFilePath();

    if (arguments.textOutputEnabled())
    {
        int pos = outputFilePath.find_last_of('.');
        if (pos > 2)
        {
            string path = outputFilePath.substr(0, pos);
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

    // Write the material file
    if (arguments.outputMaterialEnabled())
    {
        int pos = outputFilePath.find_last_of('.');
        if (pos > 2)
        {
            string path = outputFilePath.substr(0, pos);
            path.append(".material");
            writeMaterial(path);
        }
    }
}

bool FBXSceneEncoder::writeMaterial(const string& filepath)
{
    FILE* file = fopen(filepath.c_str(), "w");
    if (!file)
    {
        return false;
    }
    // Finds the base materials that are used.
    std::set<Material*> baseMaterialsToWrite;
    for (map<string, Material*>::iterator it = _materials.begin(); it != _materials.end(); ++it)
    {
        baseMaterialsToWrite.insert(it->second->getParent());
    }

    // Write the base materials that are used.
    for (std::set<Material*>::iterator it = baseMaterialsToWrite.begin(); it != baseMaterialsToWrite.end(); ++it)
    {
        Material* material = *it;
        material->writeMaterial(file);
        fprintf(file, "\n");
    }

    // Write all of the non-base materials.
    for (map<string, Material*>::iterator it = _materials.begin(); it != _materials.end(); ++it)
    {
        (*it).second->writeMaterial(file);
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
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
    vector<unsigned int> channelAttribs;
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
    const int channelStart = animation->getAnimationChannelCount();
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

    // If the animation for this node only has only 1 key frame and it is equal to the node's transform then ignore it.
    // This is a work around for a bug in the Blender FBX exporter.
    if (frameCount == 1 && fbxMatrix == fbxNode->EvaluateLocalTransform(0))
    {
        return;
    }

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
            appendKeyFrame(fbxNode, animation->getAnimationChannel(i), (float)time, scale, rotation, translation);
        }
    }

    if (_groupAnimation != animation)
    {
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

    for (int i = 0; i < fbxScene->GetSrcObjectCount<FbxAnimStack>(); ++i)
    {
        FbxAnimStack* animStack = FbxCast<FbxAnimStack>(fbxScene->GetSrcObject<FbxAnimStack>(i));
        int nbAnimLayers = animStack->GetMemberCount<FbxAnimLayer>();
        for (int l = 0; l < nbAnimLayers; ++l)
        {
            FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(l);
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
    _nodeMap[fbxNode] = node;
    return node;
}

Mesh* FBXSceneEncoder::getMesh(FbxUInt64 meshId)
{
    // Check if this mesh was already loaded.
    map<FbxUInt64, Mesh*>::iterator it = _meshes.find(meshId);
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
    float m[16];
    
    if (fbxNode->GetCamera() || fbxNode->GetLight())
    {
        FbxAMatrix rotateAdjust;
        
        if(fbxNode->GetLight())
        {
            /*
             * according to the fbx-documentation the light's forward vector
             * points along a node's negative Y axis.
             * so we have to rotate it by 90° around the X-axis to correct it.
             */
            if(fbxNode->RotationActive.Get())
            {
                const FbxVector4& postRotation = fbxNode->PostRotation.Get();
                fbxNode->SetPostRotation(FbxNode::eSourcePivot, FbxVector4(postRotation.mData[0] + 90.0,
                                                                           postRotation.mData[1],
                                                                           postRotation.mData[2])
                                         );
            }
            else
            {
                // if the rotation is deactivated we have to rotate it anyway to get the correct transformation in the end
                rotateAdjust.SetR(FbxVector4(-90.0, 0.0, 0.0));
            }
            
            matrix = fbxNode->EvaluateLocalTransform() * rotateAdjust;
        }
        else if(fbxNode->GetCamera())
        {
            // TODO: use the EvaluateLocalTransform() function for the transformations for the camera
            /*
             * the current implementation ignores pre- and postrotation among others (usually happens with fbx-export from blender)
             *
             * Some info for a future implementation:
             * according to the fbx-documentation the camera's forward vector
             * points along a node's positive X axis.
             * so we have to correct it if we use the EvaluateLocalTransform-function
             * just rotating it by 90° around the Y axis (similar to above) doesn't work
             */
            matrix.SetTRS(fbxNode->LclTranslation.Get(), fbxNode->LclRotation.Get(), fbxNode->LclScaling.Get());
        }
        
        copyMatrix(matrix, m);
        node->setTransformMatrix(m);
    }
    else
    {
        matrix = fbxNode->EvaluateLocalTransform();
        copyMatrix(matrix, m);
        node->setTransformMatrix(m);
    }
}

Material* FBXSceneEncoder::getBaseMaterial(const char* id)
{
    map<string, Material*>::iterator it = _baseMaterials.find(string(id));
    if (it != _baseMaterials.end())
    {
        return it->second;
    }
    return NULL;
}

static string getBaseMaterialName(Material* material)
{
    ostringstream baseName;
    if (material->isTextured())
    {
        baseName << "Textured";
    }
    else
    {
        baseName << "Colored";
    }

    if (material->isLit())
    {
        if (material->isSpecular())
        {
            baseName << "Specular";
        }
    }
    else
    {
        baseName << "Unlit";
    }
    return baseName.str();
}

Material* FBXSceneEncoder::findBaseMaterial(Material* material)
{
    string baseMaterialName = getBaseMaterialName(material);
    Material* baseMaterial = getBaseMaterial(baseMaterialName.c_str());
    if (baseMaterial)
    {
        return baseMaterial;
    }
    baseMaterial = createBaseMaterial(baseMaterialName, material);
    _baseMaterials[baseMaterial->getId()] = baseMaterial;
    return baseMaterial;
}

Node* FBXSceneEncoder::findNode(FbxNode* fbxNode)
{
    if (fbxNode)
    {
        map<FbxNode*, Node*>::const_iterator it = _nodeMap.find(fbxNode);
        if (it != _nodeMap.end())
        {
            return it->second;
        }
    }
    return NULL;
}

Material* FBXSceneEncoder::createBaseMaterial(const string& baseMaterialName, Material* childMaterial)
{
    Material* baseMaterial = new Material(baseMaterialName);
    baseMaterial->setUniform("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
    baseMaterial->setRenderState("cullFace", "true");
    baseMaterial->setRenderState("depthTest", "true");
    if (childMaterial->isLit())
    {
        baseMaterial->setLit(true);
        baseMaterial->setUniform("u_inverseTransposeWorldViewMatrix", "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX");
        // Always use directional light
        baseMaterial->setUniform("u_lightDirection", "SCENE_LIGHT_DIRECTION");
        baseMaterial->setUniform("u_lightColor", "SCENE_LIGHT_COLOR");

        if (childMaterial->isSpecular())
        {
            baseMaterial->addDefine(SPECULAR);
            baseMaterial->setUniform("u_cameraPosition", "CAMERA_WORLD_POSITION");
        }
    }
    if (childMaterial->isTextured())
    {
        if (childMaterial->isLit())
        {
            if (childMaterial->isBumped())
            {
                baseMaterial->setVertexShader("res/shaders/textured-bumped.vert");
                baseMaterial->setFragmentShader("res/shaders/textured-bumped.frag");
            }
            else
            {
                baseMaterial->setVertexShader("res/shaders/textured.vert");
                baseMaterial->setFragmentShader("res/shaders/textured.frag");
            }
        }
        else
        {
            baseMaterial->setVertexShader("res/shaders/textured-unlit.vert");
            baseMaterial->setFragmentShader("res/shaders/textured-unlit.frag");
        }
        Sampler* sampler = baseMaterial->createSampler(u_diffuseTexture);
        sampler->set("mipmap", "true");
        sampler->set("wrapS", CLAMP);
        sampler->set("wrapT", CLAMP);
        sampler->set(MIN_FILTER, LINEAR_MIPMAP_LINEAR);
        sampler->set(MAG_FILTER, LINEAR);
    }
    else
    {
        if (childMaterial->isLit())
        {
            baseMaterial->setVertexShader("res/shaders/colored.vert");
            baseMaterial->setFragmentShader("res/shaders/colored.frag");
        }
        else
        {
            baseMaterial->setVertexShader("res/shaders/colored-unlit.vert");
            baseMaterial->setFragmentShader("res/shaders/colored-unlit.frag");
        }
    }
    assert(baseMaterial);
    return baseMaterial;
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
        string id(name);
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
        string id(name);
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
            node->resetTransformMatrix();
        }
    }
}

void FBXSceneEncoder::loadMaterials(FbxScene* fbxScene)
{
    FbxNode* rootNode = fbxScene->GetRootNode();
    if (rootNode)
    {
        // Don't include the FBX root node
        const int childCount = rootNode->GetChildCount();
        for (int i = 0; i < childCount; ++i)
        {
            FbxNode* fbxNode = rootNode->GetChild(i);
            if (fbxNode)
            {
                loadMaterial(fbxNode);
            }
        }
    }
}

void FBXSceneEncoder::loadMaterial(FbxNode* fbxNode)
{
    Node* node = findNode(fbxNode);
    Model* model = (node) ? node->getModel() : NULL;

    const int materialCount = fbxNode->GetMaterialCount();
    for (int index = 0; index < materialCount; ++index)
    {
        FbxSurfaceMaterial* fbxMaterial = fbxNode->GetMaterial(index);
        string materialName(fbxMaterial->GetName());
        fixMaterialName(materialName);
        Material* material = NULL;
        map<string, Material*>::iterator it = _materials.find(materialName);
        if (it != _materials.end())
        {
            // This material was already loaded so don't load it again
            material = it->second;
        }
        else
        {
            if (EncoderArguments::getInstance()->outputMaterialEnabled())
            {
                material = createMaterial(materialName, fbxMaterial, node);
            }
            else
            {
                // If outputMaterialEnabled() is not enabled then only create the materials for the purpose of writing 
                // the material name in the GPB file. There is no need to load uniforms and samplers for the material.
                material = new Material(materialName);
            }
            _materials[materialName] = material;
        }

        if (materialCount == 1 && material && model)
        {
            model->setMaterial(material); // TODO: add support for materials per mesh part
        }
        else if (materialCount > 1 && material && model)
        {
            model->setMaterial(material, index);
        }
    }

    const int childCount = fbxNode->GetChildCount();
    for (int i = 0; i < childCount; ++i)
    {
        FbxNode* childNode = fbxNode->GetChild(i);
        if (childNode)
        {
            loadMaterial(childNode);
        }
    }
}

void FBXSceneEncoder::loadMaterialTextures(FbxSurfaceMaterial* fbxMaterial, Material* material)
{
    FbxProperty fbxProperty;
    int textureIndex;
    FBXSDK_FOR_EACH_TEXTURE(textureIndex)
    {
        fbxProperty = fbxMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[textureIndex]);
        //FindAndDisplayTextureInfoByProperty(fbxProperty, lDisplayHeader, lMaterialIndex);
        if ( fbxProperty.IsValid() )
        {
            int textureCount = fbxProperty.GetSrcObjectCount<FbxTexture>();
            for (int j = 0; j < textureCount; ++j)
            {
                FbxLayeredTexture *layeredTexture = fbxProperty.GetSrcObject<FbxLayeredTexture>(j);
                if (layeredTexture)
                {
                    //DisplayInt("    Layered Texture: ", j);
                    FbxLayeredTexture *layeredTexture = fbxProperty.GetSrcObject<FbxLayeredTexture>(j);
                    int lNbTextures = layeredTexture->GetSrcObjectCount<FbxTexture>();
                    for (int k = 0; k<lNbTextures; ++k)
                    {
                        FbxTexture* fbxTexture = layeredTexture->GetSrcObject<FbxTexture>(k);
                        if (fbxTexture)
                        {
                            /*
                            if (pDisplayHeader){
                                DisplayInt("    Textures connected to Material ", pMaterialIndex);
                                pDisplayHeader = false;
                            }
                            */

                            //NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
                            //Why is that?  because one texture can be shared on different layered textures and might
                            //have different blend modes.

                            FbxLayeredTexture::EBlendMode lBlendMode;
                            layeredTexture->GetTextureBlendMode(k, lBlendMode);
                            //DisplayString("    Textures for ", pProperty.GetName());
                            //DisplayInt("        Texture ", k);
                            //DisplayTextureInfo(fbxTexture, (int) lBlendMode);
                        }

                    }
                }
                else if (FbxTexture* fbxTexture = fbxProperty.GetSrcObject<FbxTexture>(j))
                {
                    //no layered texture simply get on the property
                    if (FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(fbxTexture))
                    {
                        loadMaterialFileTexture(fileTexture, material);
                    }
                }
            }
        }
    }
}

void FBXSceneEncoder::loadMaterialFileTexture(FbxFileTexture* fileTexture, Material* material)
{
    FbxTexture::ETextureUse textureUse = fileTexture->GetTextureUse();
    Sampler* sampler = NULL;
    if (textureUse == FbxTexture::eStandard)
    {
        if (!material->getSampler("u_diffuseTexture"))
            sampler = material->createSampler("u_diffuseTexture");
    }
    else if (textureUse == FbxTexture::eBumpNormalMap)
    {
        if (!material->getSampler("u_normalmapTexture"))
            sampler = material->createSampler("u_normalmapTexture");
    }
    if (sampler)
    {
        sampler->set("absolutePath", fileTexture->GetFileName());
        sampler->set("relativePath", fileTexture->GetRelativeFileName());
        sampler->set("wrapS", fileTexture->GetWrapModeU() == FbxTexture::eClamp ? CLAMP : REPEAT);
        sampler->set("wrapT", fileTexture->GetWrapModeV() == FbxTexture::eClamp ? CLAMP : REPEAT);
        //sampler->set(MIN_FILTER, LINEAR_MIPMAP_LINEAR);
        //sampler->set(MAG_FILTER, LINEAR);

        if (textureUse == FbxTexture::eStandard)
        {
            double scaleU = fileTexture->GetScaleU();
            double scaleV = fileTexture->GetScaleV();
            if (scaleU != 1 || scaleV != 1)
            {
                ostringstream stream;
                stream << scaleU << ", " << scaleV;
                material->setUniform("u_textureRepeat", stream.str());
                material->addDefine(TEXTURE_REPEAT);
            }

            double translationU = fileTexture->GetTranslationU();
            double translationV = fileTexture->GetTranslationV();
            if (translationU != 0 || translationV != 0)
            {
                ostringstream stream;
                stream << translationU << ", " << translationV;
                material->setUniform("u_textureOffset", stream.str());
                material->addDefine(TEXTURE_OFFSET);
            }
        }
    }
}

void FBXSceneEncoder::loadMaterialUniforms(FbxSurfaceMaterial* fbxMaterial, Material* material)
{
    
    if ( fbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) )
    {
        FbxSurfaceLambert* lambert = FbxCast<FbxSurfaceLambert>(fbxMaterial);

        if (material->isLit())
        {
            FbxDouble3 ambient = lambert->Ambient.Get();
            if (!isBlack(ambient))
            {
                material->setUniform("u_ambientColor", toString(ambient));
            }
        }
        if (!material->isTextured())
        {
            if (!material->isDefined(VERTEX_COLOR))
            {
                FbxDouble3 diffuse = lambert->Diffuse.Get();
                if (!isBlack(diffuse))
                {
                    material->setUniform("u_diffuseColor", toString(diffuse, 1.0));
                }
            }
        }
    }
    if (fbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
    {
        FbxSurfacePhong* phong = FbxCast<FbxSurfacePhong>(fbxMaterial);
        //FbxDouble specularFactor = phong->SpecularFactor.Get();
        if (material->isLit())
        {
            FbxDouble shininess = phong->Shininess.Get();
            if (shininess > 0)
            {
                ostringstream stream;
                stream << shininess;
                material->setUniform("u_specularExponent", stream.str());
                material->addDefine(SPECULAR);
            }
        }
        //
        //((FbxSurfacePhong *) fbxMaterial)->GetAmbientColor();
        //((FbxSurfacePhong *) fbxMaterial)->GetDiffuseColor();
    }
}

Material* FBXSceneEncoder::createMaterial(const string& name, FbxSurfaceMaterial* fbxMaterial, Node* node)
{
    assert(fbxMaterial);
    Material* material = new Material(name);
    Model* model = (node) ? node->getModel() : NULL;
    Mesh* mesh = (model) ? model->getMesh() : NULL;
    if (mesh)
    {
        // The material should be lit if the model has normals or there are lights in the scene.
        material->setLit(mesh->hasNormals() || _gamePlayFile.getLightCount() > 0);
        if (mesh->hasVertexColors())
        {
            material->addDefine(VERTEX_COLOR);
        }
    }
    MeshSkin* skin = (model) ? model->getSkin() : NULL;
    if (skin && skin->getJointCount() > 0)
    {
        material->setUniform("u_matrixPalette", "MATRIX_PALETTE");
        material->addDefine("SKINNING");
        ostringstream stream;
        stream << "SKINNING_JOINT_COUNT " << skin->getJointCount();
        material->addDefine(stream.str());
    }
    loadMaterialTextures(fbxMaterial, material);
    loadMaterialUniforms(fbxMaterial, material);
    material->setParent(findBaseMaterial(material));
    assert(material);
    return material;
}

void FBXSceneEncoder::loadSkin(FbxMesh* fbxMesh, Model* model)
{
    const int deformerCount = fbxMesh->GetDeformerCount();
    for (int i = 0; i < deformerCount; ++i)
    {
        FbxDeformer* deformer = fbxMesh->GetDeformer(i);
        if (deformer->GetDeformerType() == FbxDeformer::eSkin)
        {
            FbxSkin* fbxSkin = FbxCast<FbxSkin>(deformer);

            MeshSkin* skin = new MeshSkin();

            vector<string> jointNames;
            vector<Node*> joints;
            vector<Matrix> bindPoses;

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
        string id(name);
        id.append("_Mesh");
        mesh->setId(id);
    }

    // The number of mesh parts is equal to the number of materials that affect this mesh.
    // There is always at least one mesh part.
    vector<MeshPart*> meshParts;
    const int materialCount = fbxMesh->GetNode()->GetMaterialCount();
    int meshPartSize = (materialCount > 0) ? materialCount : 1;
    for (int i = 0; i < meshPartSize; ++i)
    {
        meshParts.push_back(new MeshPart());
    }

    // Find the blend weights and blend indices if this mesh is skinned.
    vector<vector<Vector2> > weights;
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
        FbxNodeAttribute::EType type = nodeAttribute->GetAttributeType();
        if (type == FbxNodeAttribute::eMesh ||
            type == FbxNodeAttribute::eNurbs ||
            type == FbxNodeAttribute::eNurbsSurface ||
            type == FbxNodeAttribute::ePatch)
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

// Functions

void fixMaterialName(string& name)
{
    static int unnamedCount = 0;

    for (string::size_type i = 0, len = name.length(); i < len; ++i)
    {
        if (!isalnum(name[i]))
            name[i] = '_';
    }

    if (name.length() == 0)
    {
        ostringstream stream;
        stream << "unnamed_" << (++unnamedCount);
        name = stream.str();
    }
}
