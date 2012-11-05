/*
 * DAESceneEncoder.h
 */
#include "Base.h"

#include "DAESceneEncoder.h"
#include "DAEOptimizer.h"
#include "DAEMaterialEncoder.h"
//#define ENCODER_PRINT_TIME 1

namespace gameplay
{

DAESceneEncoder::DAESceneEncoder()
    : _collada(NULL), _dom(NULL), file(NULL), _vertexBlendWeights(NULL), _vertexBlendIndices(NULL)
{
}

DAESceneEncoder::~DAESceneEncoder()
{
}

unsigned int getMaxOffset(domInputLocalOffset_Array& inputArray)
{
    unsigned int maxOffset = 0;
    for (unsigned int i = 0; i < (int)inputArray.getCount(); ++i)
    {
        if ( inputArray[i]->getOffset() > maxOffset )
        {
            maxOffset = (unsigned int)inputArray[i]->getOffset();
        }
    }

    return maxOffset;
}

void DAESceneEncoder::optimizeCOLLADA(const EncoderArguments& arguments, domCOLLADA* dom)
{
    const std::vector<std::string>& groupAnimatioNodeIds = arguments.getGroupAnimationNodeId();
    const std::vector<std::string>& groupAnimatioIds = arguments.getGroupAnimationAnimationId();
    assert(groupAnimatioNodeIds.size() == groupAnimatioIds.size());
    if (!groupAnimatioNodeIds.empty())
    {
        size_t size = groupAnimatioNodeIds.size();
        if (size > 0)
        {
            DAEOptimizer optimizer(dom);
            begin();
            for (size_t i = 0; i < size; ++i)
            {
                optimizer.combineAnimations(groupAnimatioNodeIds[i], groupAnimatioIds[i]);
            }
            end("groupAnimation");
        }
    }
    else
    {
        // Determine if there are any mesh skins that are animated that have more than 1 animation targeting its joints.
        // (candidates for grouping)
        std::vector<std::string> nodeIds;
        if (findGroupAnimationNodes(dom, nodeIds))
        {
            // Ask the user if they want to group animations automatically.
            if (promptUserGroupAnimations())
            {
                LOG(2, "Grouping animations...\n");

                DAEOptimizer optimizer(dom);
                begin();
                char buffer[20];
                size_t size = nodeIds.size();
                for (size_t i = 0; i < size; ++i)
                {
                    // In COLLADA, ids must be unique but they don't have to be unique in GPB.
                    // Save the animation id as "animations___#" and then rename it once the GPB objects are created
                    // but before the GPB is written to file.
                    sprintf(buffer, "animations___%lu", i);
                    std::string animationId(buffer);
                    _tempGroupAnimationIds.push_back(animationId);
                    optimizer.combineAnimations(nodeIds[i], animationId);
                }
                end("groupAnimation");
            }
        }
    }
    if (arguments.DAEOutputEnabled())
    {
        if (!_collada->writeTo(arguments.getFilePath(), arguments.getDAEOutputPath()))
        {
            LOG(1, "Error: COLLADA failed to write the dom for file: %s\n", arguments.getDAEOutputPath().c_str());
        }
    }
}

void DAESceneEncoder::triangulate(DAE* dae)
{
    daeDatabase* dataBase = dae->getDatabase();

    int geometryCount = (int)(dataBase->getElementCount(0, "geometry"));
    for (int i = 0; i < geometryCount; ++i)
    {
        // Find the next geometry element.
        domGeometry* domGeometry;
        dataBase->getElement((daeElement**)&domGeometry, i, 0, "geometry");

        // Get the mesh out of the geometry.
        const domMeshRef domMesh = domGeometry->getMesh();
        if (!domMesh)
        {
            continue;
        }

        // Loop over all the polygons elements.
        int polygonsCount = (int)(domMesh->getPolygons_array().getCount());
        for (int j = 0; j < polygonsCount; ++j)
        {
            // Get the polygons out of the mesh.
            domPolygons* domPolygons = domMesh->getPolygons_array()[j];
            // Create the triangles from the polygons
            createTrianglesFromPolygons(domMesh, domPolygons);
        }
        while (domMesh->getPolygons_array().getCount() > 0)
        {
            domPolygons* domPolygons = domMesh->getPolygons_array().get(0);
            // Remove the polygons from the mesh.
            domMesh->removeChildElement(domPolygons);
        }

        // Loop over all the polylist elements.
        int polylistCount = (int)(domMesh->getPolylist_array().getCount());
        for (int j = 0; j < polylistCount; ++j)
        {
            // Get the polylist out of the mesh.
            domPolylist* domPolylist = domMesh->getPolylist_array()[j];
            // Create the triangles from the polygon list
            createTrianglesFromPolylist(domMesh, domPolylist);
        }
        while (domMesh->getPolylist_array().getCount() > 0)
        {
            domPolylist* domPolylist = domMesh->getPolylist_array().get(0);
            // Remove the polylist from the mesh.
            domMesh->removeChildElement(domPolylist);
        }
    }
}

void DAESceneEncoder::createTrianglesFromPolygons(domMesh* domMesh, domPolygons* domPolygons)
{
    // Create a new <triangles> inside the mesh that has the same material as the <polygons>.
    domTriangles* triangles = (domTriangles*)domMesh->createAndPlace("triangles");
    triangles->setCount(0);
    triangles->setMaterial(domPolygons->getMaterial());
    domP* domTrianglesP = (domP*)triangles->createAndPlace("p");
    
    // Give the new <triangles> the same <_dae> and <parameters> as the old  <polygons>.
    for (unsigned int i = 0; i < domPolygons->getInput_array().getCount(); ++i)
    {
        triangles->placeElement(domPolygons->getInput_array()[i]->clone());
    }
    
    // Get the number of inputs and primitives for the polygons array.
    unsigned int inputCount = getMaxOffset(domPolygons->getInput_array()) + 1;
    unsigned int primitiveCount = domPolygons->getP_array().getCount();
    
    // Triangulate all the primitives, this generates all the triangles in a single <p> element.
    for (unsigned int j = 0; j < primitiveCount; ++j)
    {
        // Check the polygons for consistancy (some exported files have had the wrong number of indices).
        domP* domCurrentP = domPolygons->getP_array()[j];
        int elementCount = (int)(domCurrentP->getValue().getCount());
        if ( (elementCount % inputCount) != 0 )
        {
            // Skip this case.
        }
        else
        {
            unsigned int triangleCount = (elementCount / inputCount) - 2;
            
            // Write out the primitives as triangles, just fan using the first element as the base.
            unsigned int index = inputCount;
            for (unsigned int k = 0; k < triangleCount; ++k)
            {
                // First vertex.
                for (unsigned int l = 0; l < inputCount; ++l)
                {
                    domTrianglesP->getValue().append(domCurrentP->getValue()[l]);
                }
                // Second vertex.
                for (unsigned int l = 0; l < inputCount; ++l)
                {
                    domTrianglesP->getValue().append(domCurrentP->getValue()[index + l]);
                }
                // Third vertex.
                index += inputCount;
                for (unsigned int l = 0; l < inputCount; ++l)
                {
                    domTrianglesP->getValue().append(domCurrentP->getValue()[index + l]);
                }
                triangles->setCount(triangles->getCount() + 1);
            }
        }
    }
}

void DAESceneEncoder::createTrianglesFromPolylist(domMesh* domMesh, domPolylist* domPolylist)
{
    // Create a new <triangles> inside the mesh that has the same material as the <polylist>.
    domTriangles* triangles = (domTriangles*)domMesh->createAndPlace("triangles");
    triangles->setMaterial(domPolylist->getMaterial());
    domP* domTrianglesP = (domP*)triangles->createAndPlace("p");
    
    // Give the new <triangles> the same <_dae> and <parameters> as the old <polylist>.
    for (int i = 0; i < (int)(domPolylist->getInput_array().getCount()); ++i)
    {
        triangles->placeElement(domPolylist->getInput_array()[i]->clone());
    }
    
    // Get the number of inputs and primitives for the polygons array.
    unsigned int inputCount = getMaxOffset(domPolylist->getInput_array()) + 1;
    unsigned int primitiveCount = domPolylist->getVcount()->getValue().getCount();
    
    unsigned int offset = 0;
    unsigned int trianglesProcessed = 0;
    
    // Triangulate all the primitives, this generates all the triangles in a single <p> element.
    for (unsigned int j = 0; j < primitiveCount; ++j)
    {
        unsigned int triangleCount = (unsigned int)domPolylist->getVcount()->getValue()[j] - 2;
        
        // Write out the primitives as triangles, just fan using the first element as the base.
        int index = inputCount;
        for (unsigned int k = 0; k < triangleCount; ++k)
        {
            // First vertex.
            for (unsigned int l = 0; l < inputCount; ++l)
            {
                domTrianglesP->getValue().append(domPolylist->getP()->getValue()[offset + l]);
            }
            // Second vertex.
            for (unsigned int l = 0; l < inputCount; ++l)
            {
                domTrianglesP->getValue().append(domPolylist->getP()->getValue()[offset + index + l]);
            }
            // Third vertex.
            index += inputCount;
            for (unsigned int l = 0; l < inputCount; ++l)
            {
                domTrianglesP->getValue().append(domPolylist->getP()->getValue()[offset + index + l]);
            }
            
            trianglesProcessed++;
        }
        
        offset += (unsigned int)domPolylist->getVcount()->getValue()[j] * inputCount;
    }
    triangles->setCount(trianglesProcessed);
}

void DAESceneEncoder::write(const std::string& filepath, const EncoderArguments& arguments)
{
    _begin = clock();
    const char* nodeId = arguments.getNodeId();
    
    // Load the collada document
    _collada = new DAE();
    begin();
    _dom = (domCOLLADA*)_collada->open(filepath);
    end("Open file");
    if (!_dom)
    {
        LOG(1, "Error: COLLADA failed to open file: %s\n", filepath.c_str());
        if (_collada)
        {
            delete _collada;
            _collada = NULL;
        }
        return;
    }
    
    // Run collada conditioners
    begin();
    triangulate(_collada);
    end("triangulate");

    // Optimize the dom before encoding
    optimizeCOLLADA(arguments, _dom);

    // Find the <visual_scene> element within the <scene>
    const domCOLLADA::domSceneRef& domScene = _dom->getScene();
    if (domScene && domScene->getInstance_visual_scene())
    {
        daeElement* scene = getVisualScene(domScene);
        if (scene)
        {
            if (nodeId == NULL)
            {
                // If the -i <node_id> parameter was not passed then write out the entire scene.
                begin();
                loadScene((domVisual_scene*)scene);
                end("load scene");
            }
            else
            {
                // Resolve/Search for the node the user specified with the -i <node_id> parameter.
                daeSIDResolver resolver(scene, nodeId);
                domNode* nodeElement = daeSafeCast<domNode>(resolver.getElement());
                if (nodeElement)
                {
                    Node* node = loadNode(nodeElement, NULL);
                    if (node)
                    {
                        _gamePlayFile.addScenelessNode(node);
                    }
                    else
                    {
                        LOG(1, "COLLADA File loaded to the dom, but failed to load node %s.\n", nodeId);
                    }
                }
                else
                {
                    LOG(1, "COLLADA File loaded to the dom, but node was not found with node ID %s.\n", nodeId);
                }
            }
        }
        else
        {
             LOG(1, "COLLADA File loaded to the dom, but query for the dom assets failed.\n");
        }
    }
    else
    {
        LOG(1, "COLLADA File loaded to the dom, but missing <visual_scene>.\n");
    }
    
    // The animations should be loaded last
    begin();
    loadAnimations(_dom);
    end("loadAnimations");

    _gamePlayFile.adjust();
    _gamePlayFile.renameAnimations(_tempGroupAnimationIds, "animations");

    // Write the output file
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
        begin();
        if (!_gamePlayFile.saveBinary(outputFilePath))
        {
            LOG(1, "Error writing binary file: %s\n", outputFilePath.c_str());
        }
        end("save binary");
    }
    
    // create material
    if(arguments.materialOutputEnabled())
    {
        begin();
        DAEMaterialEncoder *m = new DAEMaterialEncoder();
        m->processMaterial(arguments, _dom);
        end("create material");
    }

    // Cleanup
    if (file)
    {
        fclose(file);
    }
    if (_collada)
    {
        delete _collada;
        _collada = NULL;
    }
}

void DAESceneEncoder::loadAnimations(const domCOLLADA* dom)
{
    // Call loadAnimation on all <animation> elements in all <library_animations>
    const domLibrary_animations_Array& animationLibrarys = dom->getLibrary_animations_array();
    size_t animationLibrarysCount = animationLibrarys.getCount();
    for (size_t i = 0; i < animationLibrarysCount; ++i)
    {
        const domLibrary_animationsRef& libraryAnimation = animationLibrarys.get(i);
        const domAnimation_Array& animationArray = libraryAnimation->getAnimation_array();
        size_t animationCount = animationArray.getCount();
        for (size_t j = 0; j < animationCount; ++j)
        {
            const domAnimationRef& animationRef = animationArray.get(j);
            loadAnimation(animationRef);
        }
    }
}

void DAESceneEncoder::loadAnimation(const domAnimationRef animationRef, const char* altId)
{
    // Animations can contain other animations.
    const domAnimation_Array& animationArray = animationRef->getAnimation_array();
    unsigned int animationCount = animationArray.getCount();
    
    if (animationCount == 1)
    {
        // DAE_FBX nests 1 animation within another animation for some reason.
        loadAnimation(animationArray.get(0), animationRef->getId());
    }
    else if ( animationCount > 1)
    {
        loadAnimation(animationArray.get(0));
    }

    // <channel> points to one <sampler>
    // <sampler> points to multiple <input> elements

    // <channel>
    const domChannel_Array& channelArray = animationRef->getChannel_array();
    size_t channelArrayCount = channelArray.getCount();
    if (channelArrayCount > 0)
    {
        Animation* animation = new Animation();
        const char* str = animationRef->getId();
        if (str)
        {
            animation->setId(str);
        }
        else if (altId)
        {
            animation->setId(altId);
        }

        for (size_t i = 0; i < channelArrayCount; ++i)
        {
            AnimationChannel* animationChannel = new AnimationChannel();

            const domChannelRef& channelRef = channelArray.get(i);

            // <sampler>
            const domSamplerRef sampler = getSampler(channelRef);
            assert(sampler);

            // <input>
            const domInputLocal_Array& inputArray = sampler->getInput_array();
            size_t inputArrayCount = inputArray.getCount();
            for (size_t j = 0; j < inputArrayCount; ++j)
            {
                const domInputLocalRef& inputLocal = inputArray.get(j);

                // <source>
                const domSourceRef source = getSource(inputLocal, animationRef);

                std::string semantic = inputLocal->getSemantic();
                if (equals(semantic, "INTERPOLATION"))
                {
                    // Interpolation source is a list of strings
                    loadInterpolation(source, animationChannel);
                }
                else
                {
                    // The other sources are lists of floats.
                    std::vector<float> floats;
                    copyFloats(source->getFloat_array(), &floats);
                    if (equals(semantic, "INPUT"))
                    {
                        // TODO: Ensure param name is TIME?
                        for (std::vector<float>::iterator k = floats.begin(); k != floats.end(); ++k)
                        {
                            // Convert seconds to milliseconds
                            *k = *k * 1000.0f;
                        }
                        animationChannel->setKeyTimes(floats);
                    }
                    else if (equals(semantic, "OUTPUT"))
                    {
                        animationChannel->setKeyValues(floats);
                    }
                    else if (equals(semantic, "IN_TANGENT"))
                    {
                        animationChannel->setTangentsIn(floats);
                    }
                    else if (equals(semantic, "OUT_TANGENT"))
                    {
                        animationChannel->setTangentsOut(floats);
                    }
                }
            }
            
            // get target attribute enum value
            if (loadTarget(channelRef, animationChannel))
            {
                if (animationChannel->getKeyTimes().size() > 0)
                {
                    animation->add(animationChannel);
                }
            }
        }
        if (animation->getAnimationChannelCount() > 0)
        {
            _gamePlayFile.addAnimation(animation);
        }
        else
        {
            delete animation;
        }
    }
}

void DAESceneEncoder::loadInterpolation(const domSourceRef source, AnimationChannel* animationChannel)
{
    // COLLADA stores the interpolations as a list of strings while GBP uses unsigned int
    std::vector<unsigned int> values;
    const domName_arrayRef nameArray = getSourceNameArray(source);
    assert(nameArray);
    const domListOfNames& names = nameArray->getValue();
    size_t count = (size_t)names.getCount();
    values.resize(count);
    if (count > 0)
    {
        for (size_t i = 0; i < count; ++i)
        {
            values[i] = AnimationChannel::getInterpolationType(names.get(i));
        }

        // If all of the interpolation types are the same then only store the interpolation once
        // instead of storing the same type for each key frame.
        unsigned int firstType = values[0];
        bool allEqual = true;
        for (size_t i = 1; i < count; ++i)
        {
            if (firstType != values[i])
            {
                allEqual = false;
                break;
            }
        }
        if (allEqual)
        {
            values.resize(1);
        }
    }
    animationChannel->setInterpolations(values);
}

bool DAESceneEncoder::loadTarget(const domChannelRef& channelRef, AnimationChannel* animationChannel)
{
    // GamePlay requires that animations are baked. Use "Bake Transforms" in your 3D modeling tool.
    // If the target of an animation is not a matrix then an error will be printed.

    const static char* TRANSFORM_WARNING_FORMAT = "Warning: Node \"%s\":\n %s %s\n";
    const static char* TRANSFORM_MESSAGE = "transform found but not supported.\n Use \"Bake Transforms\" option when exporting.";

    unsigned int targetProperty = 0;
    DAEChannelTarget channelTarget(channelRef);

    const char* targetId = channelTarget.getTargetId().c_str();

    // TODO: Do we want to support more than one? If yes then this needs to be fixed.
    for (size_t i = 0; i < channelTarget.getTargetAttributeCount(); ++i)
    {
        std::string prop;
        channelTarget.getPropertyName(i, &prop);
        daeElement* attributeElement = channelTarget.getTargetAttribute(i);
        if (attributeElement)
        {
            daeInt type = attributeElement->typeID();
            if (type == domRotate::ID())
            {
                LOG(1, TRANSFORM_WARNING_FORMAT, targetId, "Rotate", TRANSFORM_MESSAGE);
                return false;
                /*
                // <rotate>
                const domRotate* rotate = daeSafeCast<domRotate>(attributeElement);

                if (prop.size() > 0)
                {
                    if (equalsIgnoreCase(prop, "ANGLE"))
                    {
                        targetProperty = Transform::ANIMATE_ROTATE;

                        // get the rotation axis
                        const domFloat4& f = rotate->getValue();
                        float x = (float)f.get(0);
                        float y = (float)f.get(1);
                        float z = (float)f.get(2);
                        
                        // Get the angle values that were already read
                        const std::vector<float>& keyValues = animationChannel->getKeyValues();
                        size_t size = keyValues.size();
                        assert(size > 0);
                        // COLLADA only targeted a single prop but GBP requires all 4 rotate values.
                        // Convert (ANGLE ANGLE ANGLE) to (X Y Z ANGLE X Y Z ANGLE X Y Z ANGLE)
                        std::vector<float> floats(size * 4);
                        // Duplicate rotation axis. We will replace only the angle that COLLADA is targeting.
                        for (size_t j = 0; j < size; ++j)
                        {
                            size_t k = j * 4;
                            floats[k+0] = x;
                            floats[k+1] = y;
                            floats[k+2] = z;
                            floats[k+3] = keyValues[j]; // angle
                        }
                        animationChannel->setKeyValues(floats);
                    }
                }
                */
            }
            else if (type == domScale::ID())
            {
                LOG(1, TRANSFORM_WARNING_FORMAT, targetId, "Scale", TRANSFORM_MESSAGE);
                return false;
                /*
                // <scale>
                //const domScale* scale = daeSafeCast<domScale>(attributeElement);
                if (equalsIgnoreCase(prop, "X"))
                {
                    targetProperty = Transform::ANIMATE_SCALE_X;
                }
                else if (equalsIgnoreCase(prop, "Y"))
                {
                    targetProperty = Transform::ANIMATE_SCALE_Y;
                }
                else if (equalsIgnoreCase(prop, "Z"))
                {
                    targetProperty = Transform::ANIMATE_SCALE_Z;
                }
                else
                {
                    targetProperty = Transform::ANIMATE_SCALE;
                }
                */
            }
            else if (type == domTranslate::ID())
            {
                LOG(1, TRANSFORM_WARNING_FORMAT, targetId, "Translate", TRANSFORM_MESSAGE);
                return false;
                /*
                // <translate>
                //const domTranslate* translate = daeSafeCast<domTranslate>(attributeElement);
                if (equalsIgnoreCase(prop, "X"))
                {
                    targetProperty = Transform::ANIMATE_TRANSLATE_X;
                }
                else if (equalsIgnoreCase(prop, "Y"))
                {
                    targetProperty = Transform::ANIMATE_TRANSLATE_Y;
                }
                else if (equalsIgnoreCase(prop, "Z"))
                {
                    targetProperty = Transform::ANIMATE_TRANSLATE_Z;
                }
                else
                {
                    targetProperty = Transform::ANIMATE_TRANSLATE;
                }
                */
            }
            else if (type == domMatrix::ID())
            {
                // If the animation is targeting a matrix then convert it into
                // a scale, rotate, translate animation by decomposing the matrix.
                targetProperty = Transform::ANIMATE_SCALE_ROTATE_TRANSLATE;

                const std::vector<float>& keyValues = animationChannel->getKeyValues();
                assert(keyValues.size() % 16 == 0);
                // The matrix was 16 floats and the new values will be 10 floats
                size_t newSize = keyValues.size() / 16 * 10;
                std::vector<float> floats(newSize);

                size_t matrixCount = keyValues.size() / 16;
                for (size_t i = 0; i < matrixCount; ++i)
                {
                    size_t j = i * 16;
                    // COLLADA used row-major but the Matrix class uses column-major
                    Matrix matrix(
                        keyValues[j+0], keyValues[j+4], keyValues[j+8], keyValues[j+12],
                        keyValues[j+1], keyValues[j+5], keyValues[j+9], keyValues[j+13],
                        keyValues[j+2], keyValues[j+6], keyValues[j+10], keyValues[j+14],
                        keyValues[j+3], keyValues[j+7], keyValues[j+11], keyValues[j+15]);
                    Vector3 scale;
                    Quaternion rotation;
                    Vector3 translation;
                    matrix.decompose(&scale, &rotation, &translation);
                    rotation.normalize();

                    size_t k = i * 10;
                    floats[k+0] = scale.x;
                    floats[k+1] = scale.y;
                    floats[k+2] = scale.z;
                    floats[k+3] = rotation.x;
                    floats[k+4] = rotation.y;
                    floats[k+5] = rotation.z;
                    floats[k+6] = rotation.w;
                    floats[k+7] = translation.x;
                    floats[k+8] = translation.y;
                    floats[k+9] = translation.z;
                }
                animationChannel->setKeyValues(floats);
            }
        }
    }
    animationChannel->setTargetAttribute(targetProperty);
    animationChannel->setTargetId(channelTarget.getTargetId());
    //animationChannel->removeDuplicates();
    return true;
}

void DAESceneEncoder::begin()
{
    #ifdef ENCODER_PRINT_TIME
    _begin = clock();
    #endif
}

void DAESceneEncoder::end(const char* str)
{
    #ifdef ENCODER_PRINT_TIME
    clock_t time = clock() - _begin;
    LOG(1, "%5d %s\n", time, str);
    #endif
}

void DAESceneEncoder::copyFloats(const domFloat_array* source, std::vector<float>* target)
{
    std::vector<float>& t = *target;

    size_t count = (size_t)source->getCount();
    t.resize(count);
    const domListOfFloats& listOfFloats = source->getValue();
    for (size_t i = 0; i < count; ++i)
    {
        t[i] = (float)listOfFloats.get(i);
    }
}

void DAESceneEncoder::loadScene(const domVisual_scene* visualScene)
{
    Scene* scene = new Scene();

    const domNode_Array& nodes = visualScene->getNode_array();
    scene->setId(visualScene->getId());
    if (scene->getId().length() == 0)
    {
        scene->setId("__SCENE__");
    }

    size_t childCount = nodes.getCount();
    for (size_t i = 0; i < childCount; ++i)
    {
        scene->add(loadNode(nodes[i], NULL));
    }
    
    Node* activeCameraNode = findSceneActiveCameraNode(visualScene, scene);
    if (activeCameraNode)
    {
        scene->setActiveCameraNode(activeCameraNode);
    }

    _gamePlayFile.addScene(scene);
}

Node* DAESceneEncoder::findSceneActiveCameraNode(const domVisual_scene* visualScene, Scene* scene)
{
    // Loops through each evaluate_scene's render until an active camera node is found.
    // Returns the first one found.

    // Find the active camera
    const domVisual_scene::domEvaluate_scene_Array& evaluateScenes = visualScene->getEvaluate_scene_array();
    size_t evaluateSceneCount = evaluateScenes.getCount();
    for (size_t i = 0; i < evaluateSceneCount; ++i)
    {
        const domVisual_scene::domEvaluate_scene::domRender_Array& renders = evaluateScenes[i]->getRender_array();
        size_t renderCount = renders.getCount();
        for (size_t j = 0; j < renderCount; ++j)
        {
            xsAnyURI cameraNodeURI = renders[i]->getCamera_node();
            domNode* nodeRef = daeSafeCast<domNode>(cameraNodeURI.getElement());
            if (nodeRef)
            {
                std::string id = nodeRef->getId();
                Node* node = _gamePlayFile.getNode(id.c_str());
                if (node)
                {
                    return node;
                }
            }
        }
    }
    // Find the first node in the scene that contains a camera.
    return scene->getFirstCameraNode();
}

Node* DAESceneEncoder::loadNode(domNode* n, Node* parent)
{
    Node* node = NULL;

    // Check if this node has already been loaded
    const char* id = n->getID();
    if (id && strlen(id) > 0)
    {
        node = _gamePlayFile.getNode(n->getID());
        if (node)
        {
            return node;
        }
    }
    
    // Load the node
    node = new Node();

    if (parent)
    {
        parent->addChild(node);
    }
    
    if (n->getType() == NODETYPE_JOINT)
    {
        node->setIsJoint(true);
    }

    // Set node id
    node->setId(n->getId());

    // If this node has an id then add it to the ref table
    _gamePlayFile.addNode(node);

    transformNode(n, node);
    loadControllerInstance(n, node);
    loadCameraInstance(n, node);
    loadLightInstance(n, node);
    loadGeometryInstance(n, node);
    
    // Load child nodes
    const domNode_Array& childNodes = n->getNode_array();
    size_t childCount = childNodes.getCount();
    for (size_t i = 0; i < childCount; ++i)
    {
        loadNode(childNodes.get(i), node);
    }
    return node;
}

void DAESceneEncoder::transformNode(domNode* domNode, Node* node)
{
    // Apply the transform.
    // Note that we only honor the first matrix transform specified for the DOM node.
    const domMatrix_Array& matrixArray = domNode->getMatrix_array();
    if (matrixArray.getCount() > 0)
    {
        const domMatrixRef& matrix = matrixArray.get(0);
        if (!matrix)
        {
            return;
        }
        const domFloat4x4& tx = matrix->getValue();
        float transform[] = {(float)tx.get(0), (float)tx.get(4), (float)tx.get(8), (float)tx.get(12),
                              (float)tx.get(1), (float)tx.get(5), (float)tx.get(9), (float)tx.get(13),
                              (float)tx.get(2), (float)tx.get(6), (float)tx.get(10), (float)tx.get(14),
                              (float)tx.get(3), (float)tx.get(7), (float)tx.get(11), (float)tx.get(15)};
        node->setTransformMatrix(transform);
    }
    else
    {
        Matrix transform;
        calcTransform(domNode, transform);
        node->setTransformMatrix(transform.m);
    }

    // TODO: Handle transforming by other types (SRT, etc) (see "Node" child elements spec)

    /*Vector3 scale;
    Quaternion rotation;
    Vector3 translation;

    localTransform.Decompose(&scale, &rotation, &translation);

    node->SetScale(scale);
    node->SetRotation(rotation);
    node->SetTranslation(translation);*/
}

void DAESceneEncoder::calcTransform(domNode* domNode, Matrix& dstTransform)
{
    daeTArray<daeSmartRef<daeElement> > children;
    domNode->getChildren(children);
    size_t childCount = children.getCount();
    for (size_t i = 0; i < childCount; ++i)
    {
        daeElementRef childElement = children[i];
        daeInt typeID = childElement->typeID();
        if (typeID == domTranslate::ID())
        {
            domTranslateRef translateNode = daeSafeCast<domTranslate>(childElement);
            float x = (float)translateNode->getValue().get(0);
            float y = (float)translateNode->getValue().get(1);
            float z = (float)translateNode->getValue().get(2);
            dstTransform.translate(x, y, z);
        }
        if (typeID == domRotate::ID())
        {
            domRotateRef rotateNode = daeSafeCast<domRotate>(childElement);
            float x = (float)rotateNode->getValue().get(0);
            float y = (float)rotateNode->getValue().get(1);
            float z = (float)rotateNode->getValue().get(2);
            float angle = MATH_DEG_TO_RAD((float)rotateNode->getValue().get(3)); // COLLADA uses degrees, gameplay uses radians
            if (x == 1.0f && y == 0.0f && z == 0.0f)
            {
                dstTransform.rotateX(angle);
            }
            else if (x == 0.0f && y == 1.0f && z == 0.0f)
            {
                dstTransform.rotateY(angle);
            }
            else if (x == 0.0f && y == 0.0f && z == 1.0f)
            {
                dstTransform.rotateZ(angle);
            }
            else
            {
                dstTransform.rotate(x, y, z, angle);
            }
        }
        if (typeID == domScale::ID())
        {
            domScaleRef scaleNode = daeSafeCast<domScale>(childElement);
            float x = (float)scaleNode->getValue().get(0);
            float y = (float)scaleNode->getValue().get(1);
            float z = (float)scaleNode->getValue().get(2);
            dstTransform.scale(x, y, z);
        }
        if (typeID == domSkew::ID())
        {
            LOG(1, "Warning: Skew transform found but not supported.\n");
        }
        if (typeID == domLookat::ID())
        {
            LOG(1, "Warning: Lookat transform found but not supported.\n");
        }
    }
}

void DAESceneEncoder::loadCameraInstance(const domNode* n, Node* node)
{
    // Does this node have any camera instances?
    const domInstance_camera_Array& instanceCameras = n->getInstance_camera_array();
    size_t instanceCameraCount = instanceCameras.getCount();
    for (size_t i = 0; i < instanceCameraCount; ++i)
    {
        // Get the camrea object
        const domInstance_camera* cameraInstanceRef = instanceCameras.get(i);
        xsAnyURI cameraURI = cameraInstanceRef->getUrl();
        domCamera* cameraRef = daeSafeCast<domCamera>(cameraURI.getElement());

        if (cameraRef)
        {
            Camera* camera = loadCamera(cameraRef);
            if (camera)
            {
                node->setCamera(camera);
            }
        }
        else
        {
            // warning
        }
    }
}

void DAESceneEncoder::loadLightInstance(const domNode* n, Node* node)
{
    // Does this node have any light instances?
    const domInstance_light_Array& instanceLights = n->getInstance_light_array();
    size_t instanceLightCount = instanceLights.getCount();
    for (size_t i = 0; i < instanceLightCount; ++i)
    {
        // Get the camrea object
        const domInstance_light* lightInstanceRef = instanceLights.get(i);
        xsAnyURI lightURI = lightInstanceRef->getUrl();
        domLight* lightRef = daeSafeCast<domLight>(lightURI.getElement());

        if (lightRef)
        {
            Light* light = loadLight(lightRef);
            if (light)
            {
                node->setLight(light);
            }
        }
        else
        {
            // warning
        }
    }
}

void DAESceneEncoder::loadGeometryInstance(const domNode* n, Node* node)
{
    // Does this node have any geometry instances?
    const domInstance_geometry_Array& instanceGeometries = n->getInstance_geometry_array();
    size_t instanceGeometryCount = instanceGeometries.getCount();
    for (size_t i = 0; i < instanceGeometryCount; ++i)
    {
        // Get the geometry object
        const domInstance_geometryRef geometryInstanceRef = instanceGeometries.get(i);
        xsAnyURI geometryURI = geometryInstanceRef->getUrl();
        domGeometry* geometry = daeSafeCast<domGeometry>(geometryURI.getElement());

        // Load the model from this geometry
        if (geometry)
        {
            Model* model = loadGeometry(geometry, geometryInstanceRef->getBind_material());
            if (model)
            {
                node->setModel(model);
            }
        }
        else
        {
            LOG(1, "Failed to resolve geometry url: %s\n", geometryURI.getURI());
        }
    }
}

void DAESceneEncoder::loadControllerInstance(const domNode* n, Node* node)
{
    // Does this node have any controller instances?
    const domInstance_controller_Array& instanceControllers = n->getInstance_controller_array();
    size_t instanceControllerCount = instanceControllers.getCount();
    for (size_t i = 0; i < instanceControllerCount; ++i)
    {
        const domInstance_controllerRef instanceControllerRef = instanceControllers.get(i);
        xsAnyURI controllerURI = instanceControllerRef->getUrl();
        domController* controllerRef = daeSafeCast<domController>(controllerURI.getElement());

        if (controllerRef)
        {
            const domSkin* skinElement = controllerRef->getSkin();
            if (skinElement)
            {
                Model* model = loadSkin(skinElement);
                if (model)
                {
                    domInstance_controller::domSkeleton_Array& skeletons = instanceControllerRef->getSkeleton_array();
                    if (skeletons.getCount() == 0)
                    {
                        domNode* rootJoint = getRootJointNode(skinElement);
                        if (rootJoint)
                        {
                            loadSkeleton(rootJoint, model->getSkin());
                            node->setModel(model);
                        }
                    }
                    else
                    {
                        // Load the skeleton for this skin
                        domInstance_controller::domSkeletonRef skeleton = getSkeleton(instanceControllerRef);
                        assert(skeleton);
                        loadSkeleton(skeleton, model->getSkin());
                        node->setModel(model);
                    }
                }
            }
        }
        else
        {
            // warning
        }
        _jointLookupTable.clear();
        _jointInverseBindPoseMatrices.clear();
    }
}

Camera* DAESceneEncoder::loadCamera(const domCamera* cameraRef)
{
    Camera* camera = new Camera();
    camera->setId(cameraRef->getId());

    // Optics
    const domCamera::domOpticsRef opticsRef = cameraRef->getOptics();
    if (opticsRef.cast())
    {
        const domCamera::domOptics::domTechnique_commonRef techRef = opticsRef->getTechnique_common();

        // Orthographics
        const domCamera::domOptics::domTechnique_common::domOrthographicRef orthographicRef = techRef->getOrthographic();
        if (orthographicRef.cast())
        {
            camera->setOrthographic();
            camera->setAspectRatio((float)orthographicRef->getAspect_ratio()->getValue());
            camera->setNearPlane((float)orthographicRef->getZnear()->getValue());
            camera->setFarPlane((float)orthographicRef->getZfar()->getValue());

            const domTargetableFloatRef xmag = orthographicRef->getXmag();
            const domTargetableFloatRef ymag = orthographicRef->getYmag();
            // Viewport width
            if (xmag.cast())
            {
                camera->setViewportWidth((float)xmag->getValue());
            }
            // Viewport height
            if (ymag.cast())
            {
                camera->setViewportHeight((float)ymag->getValue());
            }
            // TODO: Viewport x and y?
        }

        // Perspective
        const domCamera::domOptics::domTechnique_common::domPerspectiveRef perspectiveRef = techRef->getPerspective();
        if (perspectiveRef.cast())
        {
            camera->setPerspective();
            camera->setNearPlane((float)perspectiveRef->getZnear()->getValue());
            camera->setFarPlane((float)perspectiveRef->getZfar()->getValue());

            float aspectRatio = -1.0f;
            if (perspectiveRef->getAspect_ratio().cast())
            {
                aspectRatio = (float)perspectiveRef->getAspect_ratio()->getValue();
                camera->setAspectRatio(aspectRatio);
            }
            if (perspectiveRef->getYfov().cast())
            {
                camera->setFieldOfView((float)perspectiveRef->getYfov()->getValue());
            }
            else if (perspectiveRef->getXfov().cast() && aspectRatio > 0.0f)
            {
                // The gameplaybinary stores the yfov but collada might have specified
                // an xfov and an aspect ratio. So use those to calculate the yfov.
                float xfov = (float)perspectiveRef->getXfov()->getValue();
                float yfov = xfov / aspectRatio;
                camera->setFieldOfView(yfov);
            }
        }
    }
    _gamePlayFile.addCamera(camera);
    return camera;
}

Light* DAESceneEncoder::loadLight(const domLight* lightRef)
{
    Light* light = new Light();
    light->setId(lightRef->getId());

    const domLight::domTechnique_commonRef techRef = lightRef->getTechnique_common();

    // Ambient light
    {
        const domLight::domTechnique_common::domAmbientRef ambientRef = techRef->getAmbient();
        if (ambientRef.cast())
        {
            light->setAmbientLight();
            // color
            const domTargetableFloat3Ref float3Ref = ambientRef->getColor();
            const domFloat3& color3 = float3Ref->getValue();
            light->setColor((float)color3.get(0), (float)color3.get(1), (float)color3.get(2));
        }
    }

    // Directional light
    {
        const domLight::domTechnique_common::domDirectionalRef direcitonalRef = techRef->getDirectional();
        if (direcitonalRef.cast())
        {
            light->setDirectionalLight();
            // color
            const domTargetableFloat3Ref float3Ref = direcitonalRef->getColor();
            const domFloat3& color3 = float3Ref->getValue();
            light->setColor((float)color3.get(0), (float)color3.get(1), (float)color3.get(2));
        }
    }

    // Spot light
    {
        const domLight::domTechnique_common::domSpotRef spotRef = techRef->getSpot();
        if (spotRef.cast())
        {
            light->setSpotLight();
            // color
            const domTargetableFloat3Ref float3Ref = spotRef->getColor();
            const domFloat3& color3 = float3Ref->getValue();
            light->setColor((float)color3.get(0), (float)color3.get(1), (float)color3.get(2));
                
            const domTargetableFloatRef& constAtt = spotRef->getConstant_attenuation();
            if (constAtt.cast())
            {
                light->setConstantAttenuation((float)constAtt->getValue());
            }

            const domTargetableFloatRef& linearAtt = spotRef->getLinear_attenuation();
            if (linearAtt.cast())
            {
                light->setLinearAttenuation((float)linearAtt->getValue());
            }

            const domTargetableFloatRef& quadAtt = spotRef->getQuadratic_attenuation();
            if (quadAtt.cast())
            {
                light->setQuadraticAttenuation((float)quadAtt->getValue());
            }

            const domTargetableFloatRef& falloffAngle = spotRef->getFalloff_angle();
            if (falloffAngle.cast())
            {
                light->setFalloffAngle((float)falloffAngle->getValue());
            }

            const domTargetableFloatRef& falloffExp = spotRef->getFalloff_exponent();
            if (falloffExp.cast())
            {
                light->setFalloffExponent((float)falloffExp->getValue());
            }
        }
    }

    // Point light
    {
        const domLight::domTechnique_common::domPointRef pointRef = techRef->getPoint();
        if (pointRef.cast())
        {
            light->setPointLight();
            // color
            const domTargetableFloat3Ref float3Ref = pointRef->getColor();
            const domFloat3& color3 = float3Ref->getValue();
            light->setColor((float)color3.get(0), (float)color3.get(1), (float)color3.get(2));

            const domTargetableFloatRef& constAtt = pointRef->getConstant_attenuation();
            if (constAtt.cast())
            {
                light->setConstantAttenuation((float)constAtt->getValue());
            }

            const domTargetableFloatRef& linearAtt = pointRef->getLinear_attenuation();
            if (linearAtt.cast())
            {
                light->setLinearAttenuation((float)linearAtt->getValue());
            }

            const domTargetableFloatRef& quadAtt = pointRef->getQuadratic_attenuation();
            if (quadAtt.cast())
            {
                light->setQuadraticAttenuation((float)quadAtt->getValue());
            }

            // When Maya exports DAE_FBX, the ambient lights are converted into point lights but with not attenuation elements.
            // If this point light has no attenuation then assume it is ambient.
            if (!(constAtt.cast() && linearAtt.cast() && quadAtt.cast()))
            {
                light->setAmbientLight();
            }
        }
    }
    _gamePlayFile.addLight(light);
    return light;
}


void DAESceneEncoder::loadSkeleton(domInstance_controller::domSkeleton* skeletonElement, MeshSkin* skin)
{
    xsAnyURI skeletonUri = skeletonElement->getValue();
    daeString skeletonId = skeletonUri.getID();
    daeSIDResolver resolver(skeletonUri.getElement(), skeletonId);
    domNode* rootNode = daeSafeCast<domNode>(resolver.getElement());
    loadSkeleton(rootNode, skin);
}

void DAESceneEncoder::loadSkeleton(domNode* rootNode, MeshSkin* skin)
{
    // Get the lookup scene id (sid) and joint index.
    std::string id = std::string(rootNode->getId());

    // Has the skeleton (root joint) been loaded yet?
    Node* skeleton = (Node*)_gamePlayFile.getFromRefTable(id);

    // The skeleton node is not loaded yet, so let's load it now
    if (skeleton == NULL)
    {
        // Find the top most parent of rootNode that has not yet been loaded
        domNode* topLevelParent = rootNode;
        while (
            topLevelParent->getParent() &&
            topLevelParent->getParent()->typeID() == domNode::ID() &&
            _gamePlayFile.getFromRefTable(topLevelParent->getParent()->getID()) == NULL)
        {
            topLevelParent = (domNode*)topLevelParent->getParent();
        }

        // Is the parent of this node loaded yet?
        Node* parentNode = NULL;
        if (topLevelParent->getParent() &&
            topLevelParent->getParent()->typeID() == domNode::ID() &&
            _gamePlayFile.getFromRefTable(topLevelParent->getParent()->getID()) != NULL)
        {
            parentNode = (Node*)_gamePlayFile.getFromRefTable(topLevelParent->getParent()->getID());
        }

        // Finally, load the node hierarchy that includes the skeleton
        skeleton = loadNode(topLevelParent, parentNode);
    }

    if (skeleton == NULL)
    {
        // This shouldn't really happen..
        skeleton = new Node();
        skeleton->setId(id);
        _gamePlayFile.addNode(skeleton);
    }

    // Resolve and set joints array for skin
    std::vector<Node*> _joints;
    const std::vector<std::string>& jointNames = skin->getJointNames();
    for (std::vector<std::string>::const_iterator i = jointNames.begin(); i != jointNames.end(); ++i)
    {
        Object* obj = _gamePlayFile.getFromRefTable(*i);
        if (obj && obj->getTypeId() == Object::NODE_ID)
        {
            Node* node = static_cast<Node*>(obj);
            _joints.push_back(node);
        }
    }
    skin->setJoints(_joints);
}

Model* DAESceneEncoder::loadSkin(const domSkin* skinElement)
{
    ///////////////////////////// SKIN
    Model* model = new Model();
    MeshSkin* skin = new MeshSkin();

    // Bind Shape Matrix
    const domSkin::domBind_shape_matrix* bindShapeMatrix = skinElement->getBind_shape_matrix();
    if (bindShapeMatrix)
    {
        const domFloat4x4& m = bindShapeMatrix->getValue();
        float transform[] = {(float)m.get(0), (float)m.get(4), (float)m.get(8),  (float)m.get(12),
                             (float)m.get(1), (float)m.get(5), (float)m.get(9),  (float)m.get(13),
                             (float)m.get(2), (float)m.get(6), (float)m.get(10), (float)m.get(14),
                             (float)m.get(3), (float)m.get(7), (float)m.get(11), (float)m.get(15)};
        skin->setBindShape(transform);
    }

    // Read and set our joints
    domSkin::domJointsRef _joints = skinElement->getJoints();
    domInputLocal_Array& jointInputs = _joints->getInput_array();

    // Process "JOINT" input semantic first (we need to do this to set the joint count)
    unsigned int jointCount = 0;
    for (unsigned int i = 0; i < jointInputs.getCount(); ++i)
    {
        domInputLocalRef input = jointInputs.get(i);
        std::string inputSemantic = std::string(input->getSemantic());
        domURIFragmentType* sourceURI = &input->getSource();
        sourceURI->resolveElement();
        const domSourceRef source = (domSource*)(daeElement*)sourceURI->getElement();

        if (equals(inputSemantic, "JOINT"))
        {
            // Get the joint Ids's
            std::vector<std::string> list;
            getJointNames(source, list);

            // Go through the joint list and convert them from sid to id because the sid information is
            // lost when converting to the gameplay binary format.
            for (std::vector<std::string>::iterator i = list.begin(); i != list.end(); ++i)
            {
                daeSIDResolver resolver(source->getDocument()->getDomRoot(), i->c_str());
                daeElement* element = resolver.getElement();
                if (element && element->typeID() == domNode::ID())
                {
                    domNodeRef node = daeSafeCast<domNode>(element);
                    const char* nodeId = node->getId();
                    if (nodeId && !equals(*i, nodeId))
                    {
                        *i = nodeId;
                    }
                }
            }

            // Get the joint count and set the capacities for both the
            jointCount = list.size();
            _jointInverseBindPoseMatrices.reserve(jointCount);
            unsigned int j = 0;
            for (std::vector<std::string>::const_iterator i = list.begin(); i != list.end(); ++i)
            {
                _jointLookupTable[*i] = j++;
            }
            skin->setJointNames(list);
        }
    }

    // Make sure we have some joints
    if (jointCount == 0)
    {
        LOG(1, "Warning: No joints found for skin: %s\n", skinElement->getID());
        return NULL;
    }

    // Process "INV_BIND_MATRIX" next
    for (unsigned int i = 0; i < jointInputs.getCount(); ++i)
    {
        domInputLocalRef input = jointInputs.get(i);
        std::string inputSemantic = std::string(input->getSemantic());
        domURIFragmentType* sourceURI = &input->getSource();
        sourceURI->resolveElement();
        domSource* source = (domSource*)(daeElement*)sourceURI->getElement();

        if (equals(inputSemantic, "INV_BIND_MATRIX"))
        {
            domListOfFloats& matrixFloats = source->getFloat_array()->getValue();
            //unsigned int matrixFloatsCount = (unsigned int)source->getFloat_array()->getCount();
            unsigned int jointIndex = 0;

            for (unsigned int j = 0; j < jointCount; ++j)
            {
                Matrix matrix((float)matrixFloats.get(jointIndex + 0), (float)matrixFloats.get(jointIndex + 4), (float)matrixFloats.get(jointIndex + 8), (float)matrixFloats.get(jointIndex + 12),
                              (float)matrixFloats.get(jointIndex + 1), (float)matrixFloats.get(jointIndex + 5), (float)matrixFloats.get(jointIndex + 9), (float)matrixFloats.get(jointIndex + 13),
                              (float)matrixFloats.get(jointIndex + 2), (float)matrixFloats.get(jointIndex + 6), (float)matrixFloats.get(jointIndex + 10), (float)matrixFloats.get(jointIndex + 14),
                              (float)matrixFloats.get(jointIndex + 3), (float)matrixFloats.get(jointIndex + 7), (float)matrixFloats.get(jointIndex + 11), (float)matrixFloats.get(jointIndex + 15));

                _jointInverseBindPoseMatrices.push_back(matrix);
                jointIndex += 16;
            }
        }
    }

    skin->setBindPoses(_jointInverseBindPoseMatrices);

    // Get the vertex weights inputs
    domSkin::domVertex_weights* vertexWeights =  skinElement->getVertex_weights();
    domInputLocalOffset_Array& vertexWeightsInputs = vertexWeights->getInput_array();
    unsigned int vertexWeightsCount = (unsigned int)vertexWeights->getCount();
    domListOfFloats jointWeights;

    for (unsigned int i = 0; i < jointInputs.getCount(); ++i)
    {
        domInputLocalOffsetRef input = vertexWeightsInputs.get(i);
        std::string inputSemantic = std::string(input->getSemantic());
        domURIFragmentType* sourceURI = &input->getSource();
        sourceURI->resolveElement();
        domSource* source = (domSource*)(daeElement*)sourceURI->getElement();

        if (equals(inputSemantic, "WEIGHT"))
        {
            domFloat_array* weights = source->getFloat_array();
            if (weights)
            {
                jointWeights = weights->getValue();
            }
        }
    }
    
    // Get the number of joint influences per vertex
    domSkin::domVertex_weights::domVcount* vCountElement = vertexWeights->getVcount();
    domListOfUInts skinVertexInfluenceCounts = vCountElement->getValue();
    // Get the joint/weight pair data.
    domSkin::domVertex_weights::domV* vElement = vertexWeights->getV();
    domListOfInts skinVertexJointWeightPairIndices = vElement->getValue();
        
    // Get the vertex influence count for any given vertex (up to max of 4)
    unsigned int maxVertexInfluencesCount = SCENE_SKIN_VERTEXINFLUENCES_MAX;
    skin->setVertexInfluenceCount(maxVertexInfluencesCount);

    // Get the vertex blend weights and joint indices and
    // allocate our vertex blend weights and blend indices arrays.
    // These will be used and cleaned up later in LoadMesh
    int skinVertexInfluenceCountTotal = skinVertexInfluenceCounts.getCount();
    int totalVertexInfluencesCount = vertexWeightsCount * maxVertexInfluencesCount;
    _vertexBlendWeights = new float[totalVertexInfluencesCount];
    _vertexBlendIndices = new unsigned int[totalVertexInfluencesCount];

    // Preset the default blend weights to 0.0f (no effect) and blend indices to 0 (uses the first which when multiplied
    // will have no effect anyhow.
    memset(_vertexBlendWeights, 0, totalVertexInfluencesCount * sizeof(float));
    memset(_vertexBlendIndices , 0, totalVertexInfluencesCount * sizeof(unsigned int));
    
    int vOffset = 0;
    int weightOffset = 0;

    // Go through all the skin vertex influence weights from the indexed data.
    for (int i = 0; i < skinVertexInfluenceCountTotal; ++i)
    {
        // Get the influence count and directly get the vertext blend weights and indices.
        unsigned int vertexInfluenceCount = (unsigned int)skinVertexInfluenceCounts.get(i);
        float vertexInfluencesTotalWeights = 0.0f;
        std::vector<SkinnedVertexWeightPair> vertexInfluences;
        //vertexInfluences.SetCapacity(vertexInfluenceCount);

        // Get the index/weight pairs and some the weight totals while at it.
        for (unsigned int j = 0; j < vertexInfluenceCount; ++j)
        {
            float weight = (float)jointWeights.get((unsigned int)skinVertexJointWeightPairIndices[vOffset + 1]);
            int index = (int)skinVertexJointWeightPairIndices[vOffset];
            
            // Set invalid index corresponding weights to zero
            if (index < 0 || index > (int)vertexWeightsCount)
            {
                weight = 0.0f;
                index = 0;
            }

            SkinnedVertexWeightPair pair(weight, index);
            vertexInfluences.push_back(pair);
            vertexInfluencesTotalWeights += weight;

            vOffset+=2;
        }

        // Get up the the maximum vertex weight influence count.
         for (unsigned int j = 0; j < maxVertexInfluencesCount; ++j)
        {
            if (j < vertexInfluenceCount)
            {
                SkinnedVertexWeightPair pair = vertexInfluences[j];
                _vertexBlendIndices[weightOffset] = pair.BlendIndex;
                    
                if (vertexInfluencesTotalWeights > 0.0f)
                {
                    _vertexBlendWeights[weightOffset] = pair.BlendWeight;
                }
                else
                {
                    if (j == 0)
                    {
                        _vertexBlendWeights[weightOffset] = 1.0f;
                    }
                    else
                    {
                        _vertexBlendWeights[weightOffset] = 0.0f;
                    }
                }
            }

            weightOffset++;
        }
    }

    model->setSkin(skin);

    ///////////////////////////////////////////////////////////
    // get geometry
    xsAnyURI geometryURI = skinElement->getSource();
    domGeometry* geometry = daeSafeCast<domGeometry>(geometryURI.getElement());
    if (geometry)
    {
        const domMesh* meshElement = geometry->getMesh();
        if (meshElement)
        {
            Mesh* mesh = loadMesh(meshElement, geometry->getId());
            if (mesh)
            {
                model->setMesh(mesh);
            }
        }
    }
    ///////////////////////////////////////////////////////////

    return model;
}

Model* DAESceneEncoder::loadGeometry(const domGeometry* geometry, const domBind_materialRef bindMaterial)
{
    // Does this geometry have a valid mesh?
    // Get the mesh for the geometry (if it has one)
    const domMesh* meshElement = geometry->getMesh();
    if (meshElement == NULL)
    {
        LOG(1, "Warning: No mesh found for geometry: %s\n", geometry->getId());
        return NULL;
    }

    ///////////////////////////// GEOMETRY

    // Load the mesh for this model
    Mesh* mesh = loadMesh(meshElement, geometry->getId());
    if (mesh == NULL)
    {
        return NULL;
    }

    // Mesh instance
    Model* model = new Model();
    model->setMesh(mesh);
    return model;
}

Mesh* DAESceneEncoder::loadMesh(const domMesh* meshElement, const std::string& geometryId)
{
    const domTriangles_Array& trianglesArray = meshElement->getTriangles_array();
    unsigned int trianglesArrayCount = (unsigned int)trianglesArray.getCount();

    // Ensure the data is exported as triangles.
    if (trianglesArrayCount == 0)
    {
        LOG(1, "Warning: Geometry mesh has no triangles: %s\n", geometryId.c_str());
        return NULL;
    }

    // Check if this mesh already exists
    Mesh* mesh = _gamePlayFile.getMesh(geometryId.c_str());
    if (mesh)
    {
        return mesh;
    }
    mesh = new Mesh();
    mesh->setId(geometryId.c_str());

    std::vector<DAEPolygonInput*> polygonInputs;

    // Quickly just go through each triangles array and make sure they have the same number of inputs
    // with the same layout.
    // const domSource_Array& sourceArray = meshElement->getSource_array();
    const domInputLocal_Array& vertexArray = meshElement->getVertices()->getInput_array();

    unsigned int inputCount = (unsigned int)-1;

    // Loop through our set of triangle lists (each list of triangles corresponds to a single MeshPart)
    for (unsigned int i = 0; i < trianglesArrayCount; ++i)
    {
        const domTrianglesRef& triangles = trianglesArray.get(i);
        const domInputLocalOffset_Array& inputArray = triangles->getInput_array();

        // If not set then determine the number of input for all the triangles.
        if (inputCount == -1)
        {
            inputCount = (unsigned int)inputArray.getCount();

            int texCoordCount = 0;
            for (unsigned int j = 0; j < inputCount; ++j)
            {
                const domInputLocalOffsetRef& input = inputArray.get(j);
                std::string inputSemantic = input->getSemantic();

                // If its a vertex first do an extra lookup for the inclusive inputs
                if (equals(inputSemantic, "VERTEX"))
                {
                    unsigned int vertexArrayCount = (unsigned int)vertexArray.getCount();
                    for (unsigned int k = 0; k < vertexArrayCount; ++k)
                    {
                        const domInputLocalRef& vertexInput = vertexArray.get(k);
                        
                        std::string semantic = std::string(vertexInput->getSemantic());
                        int type = getVertexUsageType(semantic);
                        if (type == -1)
                        {
                            LOG(1, "Warning: Vertex semantic (%s) is invalid/unsupported for geometry mesh: %s\n", semantic.c_str(), geometryId.c_str());
                        }

                        DAEPolygonInput* polygonInput = new DAEPolygonInput();
                        domURIFragmentType& sourceURI = vertexInput->getSource();
                        sourceURI.resolveElement();
                        domSource* source = (domSource*)(daeElement*)sourceURI.getElement();
                        polygonInput->offset = 0;
                        polygonInput->sourceValues = source->getFloat_array()->getValue();
                        polygonInput->type = type;
                        polygonInputs.push_back(polygonInput);
                    }
                }
                else
                {
                    std::string semantic = input->getSemantic();
                    int type = getVertexUsageType(semantic);
                    if (type == -1)
                    {
                        LOG(1, "Warning: Semantic (%s) is invalid/unsupported for geometry mesh: %s\n", semantic.c_str(), geometryId.c_str());
                        break;
                    }
                    if (type == TEXCOORD0)
                    {
                        // Some meshes have multiple texture coordinates
                        assert(texCoordCount <= 7);
                        type += texCoordCount;
                        ++texCoordCount;
                    }

                    DAEPolygonInput* polygonInput = new DAEPolygonInput();
                    domURIFragmentType& sourceURI = input->getSource();
                    sourceURI.resolveElement();
                    domSource* source = (domSource*)(daeElement*)sourceURI.getElement();
                    polygonInput->offset = (unsigned int)input->getOffset();
                    polygonInput->sourceValues = source->getFloat_array()->getValue();
                    polygonInput->type = type;

                    // Get the accessor info
                    const domSource::domTechnique_commonRef& technique = source->getTechnique_common();
                    if (technique.cast())
                    {
                        const domAccessorRef& accessor = technique->getAccessor();
                        polygonInput->accessor = accessor;
                    }

                    polygonInputs.push_back(polygonInput);
                }
            }
        }
        else
        {
            // If there is a triangle array with a different number of inputs, this is not supported.
            if (inputCount != (unsigned int)inputArray.getCount())
            {
                for (size_t j = 0; j < polygonInputs.size(); ++j)
                {
                    delete polygonInputs[j];
                }
                LOG(1, "Warning: Triangles do not all have the same number of input sources for geometry mesh: %s\n", geometryId.c_str());
                return NULL;
            }
            else
            {
                // TODO: Check if they are in the same order...
            }
        }
    }
    
    // Now we have validated that all input in all triangles are the same and in the same input layout.
    // Lets start to read them and build our subsets.
    for (unsigned int i = 0; i < trianglesArrayCount; ++i)
    {
        // Subset to be built.
        MeshPart* subset = new MeshPart();

        // All of the information about the triangles and the sources to access the data from.
        domTriangles* triangles = daeSafeCast<domTriangles>(trianglesArray.get(i));

        // Parse the material for this subset
        //string materialName = triangles->getMaterial() == NULL ? _T("") : triangles->getMaterial();
        //if (materialName.size() > 0)
        ///    subset->material = ParseMaterial(bindMaterial, materialName);

        //const domInputLocalOffset_Array& inputArray = triangles->getInput_array();
        const domListOfUInts& polyInts = triangles->getP()->getValue();
        unsigned int polyIntsCount = (unsigned int)polyInts.getCount();
        unsigned int poly = 0;
        unsigned int inputSourceCount = (unsigned int)polygonInputs.size();
        unsigned int maxOffset = 0;

        // Go through the polygon indices for each input source retrieve the values
        // and iterate by its offset.

        Vertex vertex;
        for (unsigned int k = 0; k < inputSourceCount && poly < polyIntsCount;)
        {
            const domListOfFloats& source = polygonInputs[k]->sourceValues;
            unsigned int offset = polygonInputs[k]->offset;
            if (offset > maxOffset)
            {
                maxOffset = offset;
            }
            int polyIndexInt = (int) polyInts.get(poly + offset);
            unsigned int polyIndex = (unsigned int) polyInts.get(poly + offset);

            switch (polygonInputs[k]->type)
            {
            case POSITION:
                vertex = Vertex(); // TODO
                if (_vertexBlendWeights && _vertexBlendIndices)
                {
                    vertex.hasWeights = true;

                    vertex.blendWeights.x =  _vertexBlendWeights[polyIndex * 4];
                    vertex.blendWeights.y =  _vertexBlendWeights[polyIndex * 4 + 1];
                    vertex.blendWeights.z =  _vertexBlendWeights[polyIndex * 4 + 2];
                    vertex.blendWeights.w =  _vertexBlendWeights[polyIndex * 4 + 3];

                    vertex.blendIndices.x =  (float)_vertexBlendIndices[polyIndex * 4];
                    vertex.blendIndices.y =  (float)_vertexBlendIndices[polyIndex * 4 + 1];
                    vertex.blendIndices.z =  (float)_vertexBlendIndices[polyIndex * 4 + 2];
                    vertex.blendIndices.w =  (float)_vertexBlendIndices[polyIndex * 4 + 3];
                }

                vertex.position.x = (float)source.get(polyIndex * 3);
                vertex.position.y = (float)source.get(polyIndex * 3 + 1);
                vertex.position.z = (float)source.get(polyIndex * 3 + 2);
                break;
        
            case NORMAL:
                vertex.hasNormal = true;
                vertex.normal.x = (float)source.get(polyIndex * 3);
                vertex.normal.y = (float)source.get(polyIndex * 3 + 1);
                vertex.normal.z = (float)source.get(polyIndex * 3 + 2);
                break;

            // TODO: We must examine the Collada input accessor and read the stride/count to verify this - not ONLY for Color, but we should be doing this for ALL components (i.e. Position, Normal, etc).
            case COLOR:
            {
                domAccessor* accessor = polygonInputs[k]->accessor;
                if (accessor)
                {
                    vertex.hasDiffuse = true;
                    vertex.diffuse.w = 1.0f;
                    unsigned int stride = (unsigned int)polygonInputs[k]->accessor->getStride();
                    unsigned int index = polyIndex * stride;

                    const domParam_Array& paramArray = accessor->getParam_array();
                    const size_t paramArrayCount = paramArray.getCount();

                    for (size_t i = 0; i < paramArrayCount; ++i)
                    {
                        const domParamRef& param = paramArray.get(i);
                        const char* name = param->getName();
                        if (name)
                        {
                            switch (name[0])
                            {
                            case 'r':
                            case 'R':
                                vertex.diffuse.x = (float)source.get(index + i); // red
                                break;
                            case 'g':
                            case 'G':
                                vertex.diffuse.y = (float)source.get(index + i); // green
                                break;
                            case 'b':
                            case 'B':
                                vertex.diffuse.z = (float)source.get(index+ i ); // blue
                                break;
                            case 'a':
                            case 'A':
                                vertex.diffuse.w = (float)source.get(index + i); // alpha
                                break;
                            default:
                                break;
                            }
                        }
                    }
                }
                break;
            }

            case TANGENT:
                vertex.hasTangent = true;
                vertex.tangent.x = (float)source.get(polyIndex * 3);
                vertex.tangent.y = (float)source.get(polyIndex * 3 + 1);
                vertex.tangent.z = (float)source.get(polyIndex * 3 + 2);
                break;

            case BINORMAL:
                vertex.hasBinormal = true;
                vertex.binormal.x = (float)source.get(polyIndex * 3);
                vertex.binormal.y = (float)source.get(polyIndex * 3 + 1);
                vertex.binormal.z = (float)source.get(polyIndex * 3 + 2);
                break;

            case TEXCOORD0:
            case TEXCOORD1:
            case TEXCOORD2:
            case TEXCOORD3:
            case TEXCOORD4:
            case TEXCOORD5:
            case TEXCOORD6:
            case TEXCOORD7:
                {
                    unsigned int index = polygonInputs[k]->type - TEXCOORD0;
                    //for (unsigned int i = 0; i < uvSetCount; ++i)
                    //{
                        vertex.hasTexCoord[index] = true;
                        if (polygonInputs[k]->accessor)
                        {
                            // TODO: This assumes (s, t) are first
                            unsigned int stride = (unsigned int)polygonInputs[k]->accessor->getStride();
                            if (polyIndexInt < 0)
                            {
                                unsigned int i = (unsigned int)((int)polygonInputs[k]->accessor->getCount()) + polyIndexInt;
                                vertex.texCoord[index].x = (float)source.get(i * stride);
                                vertex.texCoord[index].y = (float)source.get(i * stride + 1);
                            }
                            else
                            {
                                vertex.texCoord[index].x = (float)source.get(polyIndex * stride);
                                vertex.texCoord[index].y = (float)source.get(polyIndex * stride + 1);
                            }
                        }
                        else
                        {
                            vertex.texCoord[index].x = (float)source.get(polyIndex * 2);
                            vertex.texCoord[index].y = (float)source.get(polyIndex * 2 + 1);
                        }
                    //}
                }
                break;

            default:
                break;
            }

            // On the last input source attempt to add the vertex or index an existing one.
            if (k == (inputSourceCount - 1))
            {
                // Only add unique vertices, use a hashtable and compare the hash functions of the
                // vertices. If they exist simply lookup the index of the existing ones.
                // otherwise add and new one and index it.
                unsigned int index;
                if (mesh->contains(vertex))
                {
                    index = mesh->getVertexIndex(vertex);
                }
                else
                {
                    index = mesh->addVertex(vertex);
                }
                subset->addIndex(index);

                poly += (maxOffset+1);
                k = 0;
            }
            else
            {
                k++;
            }
        }
        // Add our new subset for the mesh.
        mesh->addMeshPart(subset);
    }

    // The order that the vertex elements are add to the list matters.
    // It should be the same order as how the Vertex data is written.

    // Position
    mesh->addVetexAttribute(POSITION, Vertex::POSITION_COUNT);
    
    // Normals
    if (mesh->vertices[0].hasNormal)
    {
        mesh->addVetexAttribute(NORMAL, Vertex::NORMAL_COUNT);
    }
    // Tangents
    if (mesh->vertices[0].hasTangent)
    {
        mesh->addVetexAttribute(TANGENT, Vertex::TANGENT_COUNT);
    }
    // Binormals
    if (mesh->vertices[0].hasBinormal)
    {
        mesh->addVetexAttribute(BINORMAL, Vertex::BINORMAL_COUNT);
    }
    // Texture Coordinates
    for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
    {
        if (mesh->vertices[0].hasTexCoord[i])
        {
            mesh->addVetexAttribute(TEXCOORD0 + i, Vertex::TEXCOORD_COUNT);
        }
    }
    // Diffuse Color
    if (mesh->vertices[0].hasDiffuse)
    {
        mesh->addVetexAttribute(COLOR, Vertex::DIFFUSE_COUNT);
    }
    // Skinning BlendWeights BlendIndices
    if (mesh->vertices[0].hasWeights)
    {
        mesh->addVetexAttribute(BLENDWEIGHTS, Vertex::BLEND_WEIGHTS_COUNT);
        mesh->addVetexAttribute(BLENDINDICES, Vertex::BLEND_INDICES_COUNT);
    }

    _gamePlayFile.addMesh(mesh);
    return mesh;
}

int DAESceneEncoder::getVertexUsageType(const std::string& semantic)
{
    if (semantic.length() > 0)
    {
        switch (semantic[0])
        {
        case 'P':
            if (equals(semantic, "POSITION"))
            {
                return POSITION;
            }
        case 'N':
            if (equals(semantic, "NORMAL"))
            {
                return NORMAL;
            }
        case 'C':
            if (equals(semantic, "COLOR"))
            {
                return COLOR;
            }
        case 'T':
            if (equals(semantic, "TANGENT"))
            {
                return TANGENT;
            }
            else if (equals(semantic, "TEXCOORD"))
            {
                return TEXCOORD0;
            }
            else if (equals(semantic, "TEXTANGENT"))
            {
                // Treat TEXTANGENT as TANGENT
                return TANGENT;
            }
            else if (equals(semantic, "TEXBINORMAL"))
            {
                // Treat TEXBINORMAL as BINORMAL
                return BINORMAL;
            }
        case 'B':
            if (equals(semantic, "BINORMAL"))
            {
                return BINORMAL;
            }
        default:
            return -1;
        }
    }
    return -1;
}

DAESceneEncoder::DAEPolygonInput::DAEPolygonInput(void) :
    offset(0),
    type(0),
    accessor(NULL)
{
}

DAESceneEncoder::DAEPolygonInput::~DAEPolygonInput(void)
{
}

}
