#include "Base.h"
#include "AudioSource.h"
#include "Game.h"
#include "Bundle.h"
#include "SceneLoader.h"
#include "Terrain.h"
#include "Light.h"

namespace gameplay
{

// Utility functions (shared with Properties).
extern void calculateNamespacePath(const std::string& urlString, std::string& fileString, std::vector<std::string>& namespacePath);
extern Properties* getPropertiesFromNamespacePath(Properties* properties, const std::vector<std::string>& namespacePath);

Scene* SceneLoader::load(const char* url)
{
    SceneLoader loader;
    return loader.loadInternal(url);
}

Scene* SceneLoader::loadInternal(const char* url)
{
    // Get the file part of the url that we are loading the scene from.
    std::string urlStr = url ? url : "";
    std::string id;
    splitURL(urlStr, &_path, &id);

    // Load the scene properties from file.
    Properties* properties = Properties::create(url);
    if (properties == NULL)
    {
        GP_ERROR("Failed to load scene file '%s'.", url);
        return NULL;
    }

    // Check if the properties object is valid and has a valid namespace.
    Properties* sceneProperties = (strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace();
    if (!sceneProperties || !(strcmp(sceneProperties->getNamespace(), "scene") == 0))
    {
        GP_ERROR("Failed to load scene from properties object: must be non-null object and have namespace equal to 'scene'.");
        SAFE_DELETE(properties);
        return NULL;
    }

    // Get the path to the main GPB.
    std::string path;
    if (sceneProperties->getPath("path", &path))
    {
        _gpbPath = path;
    }

    // Build the node URL/property and animation reference tables and load the referenced files/store the inline properties objects.
    buildReferenceTables(sceneProperties);
    loadReferencedFiles();

    // Load the main scene data from GPB and apply the global scene properties.
    Scene* scene = NULL;
    if (!_gpbPath.empty())
    {
        // Load scene from bundle
        scene = loadMainSceneData(sceneProperties);
        if (!scene)
        {
            GP_ERROR("Failed to load main scene from bundle.");
            SAFE_DELETE(properties);
            return NULL;
        }
    }
    else
    {
        // Create a new empty scene
        scene = Scene::create(sceneProperties->getId());
    }

    // First apply the node url properties. Following that,
    // apply the normal node properties and create the animations.
    // We apply physics properties after all other node properties
    // so that the transform (SRT) properties get applied before
    // processing physics collision objects.
    applyNodeUrls(scene);
    applyNodeProperties(scene, sceneProperties, 
        SceneNodeProperty::AUDIO | 
        SceneNodeProperty::MATERIAL | 
        SceneNodeProperty::PARTICLE |
        SceneNodeProperty::TERRAIN |
        SceneNodeProperty::LIGHT |
        SceneNodeProperty::CAMERA |
        SceneNodeProperty::ROTATE |
        SceneNodeProperty::SCALE |
        SceneNodeProperty::TRANSLATE);
    applyNodeProperties(scene, sceneProperties, SceneNodeProperty::COLLISION_OBJECT);

    // Apply node tags
    for (size_t i = 0, sncount = _sceneNodes.size(); i < sncount; ++i)
    {
        SceneNode& sceneNode = _sceneNodes[i];
        for (std::map<std::string, std::string>::const_iterator itr = sceneNode._tags.begin(); itr != sceneNode._tags.end(); ++itr)
        {
            for (size_t n = 0, ncount = sceneNode._nodes.size(); n < ncount; ++n)
                sceneNode._nodes[n]->setTag(itr->first.c_str(), itr->second.c_str());
        }
    }

    // Set active camera
    const char* activeCamera = sceneProperties->getString("activeCamera");
    if (activeCamera)
    {
        Node* camera = scene->findNode(activeCamera);
        if (camera && camera->getCamera())
            scene->setActiveCamera(camera->getCamera());
    }

    // Set ambient and light properties
    Vector3 vec3;
    if (sceneProperties->getVector3("ambientColor", &vec3))
        scene->setAmbientColor(vec3.x, vec3.y, vec3.z);
    if (sceneProperties->getVector3("lightColor", &vec3))
        scene->setLightColor(vec3.x, vec3.y, vec3.z);
    if (sceneProperties->getVector3("lightDirection", &vec3))
        scene->setLightDirection(vec3);

    // Create animations for scene
    createAnimations(scene);

    // Find the physics properties object.
    Properties* physics = NULL;
    sceneProperties->rewind();
    while (true)
    {
        Properties* ns = sceneProperties->getNextNamespace();
        if (ns == NULL || strcmp(ns->getNamespace(), "physics") == 0)
        {
            physics = ns;
            break;
        }
    }

    // Load physics properties and constraints.
    if (physics)
        loadPhysics(physics, scene);

    // Clean up all loaded properties objects.
    std::map<std::string, Properties*>::iterator iter = _propertiesFromFile.begin();
    for (; iter != _propertiesFromFile.end(); ++iter)
    {
        SAFE_DELETE(iter->second);
    }

    // Clean up the .scene file's properties object.
    SAFE_DELETE(properties);

    return scene;
}

void SceneLoader::addSceneAnimation(const char* animationID, const char* targetID, const char* url)
{
    std::string urlStr = url ? url : "";

    // If there is a file that needs to be loaded later, add an 
    // empty entry to the properties table to signify it.
    if (urlStr.length() > 0 && _properties.count(urlStr) == 0)
        _properties[urlStr] = NULL;

    // Add the animation to the list of animations to be resolved later.
    _animations.push_back(SceneAnimation(animationID, targetID, urlStr));
}

void SceneLoader::addSceneNodeProperty(SceneNode& sceneNode, SceneNodeProperty::Type type, const char* url, int index)
{
    std::string urlStr = url ? url : "";

    // If there is a non-GPB file that needs to be loaded later, add an 
    // empty entry to the properties table to signify it.
    if (urlStr.length() > 0 && urlStr.find(".") != std::string::npos && urlStr.find(".gpb") == std::string::npos && _properties.count(urlStr) == 0)
        _properties[urlStr] = NULL;

    SceneNodeProperty prop(type, urlStr, index);

    // Parse for wildcharacter character (only supported on the URL attribute)
    if (type == SceneNodeProperty::URL)
    {
        if (urlStr.length() > 1 && urlStr.at(urlStr.length()-1) == '*')
        {
            prop._url = urlStr.substr(0, urlStr.length()-1);
            sceneNode._exactMatch = false;
        }
    }

    // Add the node property to the list of node properties to be resolved later.
    sceneNode._properties.push_back(prop);
}

void SceneLoader::applyNodeProperties(const Scene* scene, const Properties* sceneProperties, unsigned int typeFlags)
{
    for (size_t i = 0, sncount = _sceneNodes.size(); i < sncount; ++i)
    {
        SceneNode& sceneNode = _sceneNodes[i];

        for (size_t p = 0, pcount = sceneNode._properties.size(); p < pcount; ++p)
        {
            SceneNodeProperty& snp = sceneNode._properties[p];
            if (typeFlags & snp._type)
            {
                for (size_t n = 0, ncount = sceneNode._nodes.size(); n < ncount; ++n)
                    applyNodeProperty(sceneNode, sceneNode._nodes[n], sceneProperties, snp, scene);
            }
        }
    }
}

void SceneLoader::applyNodeProperty(SceneNode& sceneNode, Node* node, const Properties* sceneProperties, const SceneNodeProperty& snp, const Scene* scene)
{
    if (snp._type == SceneNodeProperty::AUDIO ||
        snp._type == SceneNodeProperty::MATERIAL ||
        snp._type == SceneNodeProperty::PARTICLE ||
        snp._type == SceneNodeProperty::TERRAIN ||
        snp._type == SceneNodeProperty::LIGHT ||
        snp._type == SceneNodeProperty::CAMERA ||
        snp._type == SceneNodeProperty::COLLISION_OBJECT)
    {
        // Check to make sure the referenced properties object was loaded properly.
        Properties* p = _properties[snp._url];
        if (!p)
        {
            GP_ERROR("The referenced node data at url '%s' failed to load.", snp._url.c_str());
            return;
        }
        p->rewind();

        // If the URL didn't specify a particular namespace within the file, pick the first one.
        p = (strlen(p->getNamespace()) > 0) ? p : p->getNextNamespace();

        switch (snp._type)
        {
        case SceneNodeProperty::AUDIO:
        {
            AudioSource* audioSource = AudioSource::create(p);
            node->setAudioSource(audioSource);
            SAFE_RELEASE(audioSource);
            break;
        }
        case SceneNodeProperty::MATERIAL:
            if (!node->getModel())
            {
                GP_ERROR("Attempting to set a material on node '%s', which has no model.", sceneNode._nodeID);
                return;
            }
            else
            {
                Material* material = Material::create(p);
                node->getModel()->setMaterial(material, snp._index);
                SAFE_RELEASE(material);
            }
            break;
        case SceneNodeProperty::PARTICLE:
        {
            ParticleEmitter* particleEmitter = ParticleEmitter::create(p);
            node->setParticleEmitter(particleEmitter);
            SAFE_RELEASE(particleEmitter);
            break;
        }
        case SceneNodeProperty::TERRAIN:
        {
            Terrain* terrain = Terrain::create(p);
            node->setTerrain(terrain);
            SAFE_RELEASE(terrain);
            break;
        }
        case SceneNodeProperty::LIGHT:
        {
            Light* light = Light::create(p);
            node->setLight(light);
            SAFE_RELEASE(light);
            break;
        }
        case SceneNodeProperty::CAMERA:
        {
            Camera* camera = Camera::create(p);
            node->setCamera(camera);
            SAFE_RELEASE(camera);
            break;
        }
        case SceneNodeProperty::COLLISION_OBJECT:
        {
            // Check to make sure the type of the namespace used to load the physics collision object is correct.
            if (snp._type == SceneNodeProperty::COLLISION_OBJECT && strcmp(p->getNamespace(), "collisionObject") != 0)
            {
                GP_ERROR("Attempting to set a physics collision object on a node using a '%s' definition.", p->getNamespace());
                return;
            }
            else
            {
                // If the scene file specifies a rigid body model, use it for creating the collision object.
                Properties* np = sceneProperties->getNamespace(sceneNode._nodeID);
                const char* name = NULL;

                // Allow both property names
                if (np && !(name = np->getString("rigidBodyModel")))
                {
                    name = np->getString("collisionMesh");
                }

                if (name)
                {
                    GP_ASSERT(scene);
                    Node* modelNode = scene->findNode(name);
                    if (!modelNode)
                    {
                        GP_ERROR("Node '%s' does not exist; attempting to use its model for collision object creation.", name);
                        return;
                    }
                    else
                    {
                        if (!modelNode->getModel())
                        {
                            GP_ERROR("Node '%s' does not have a model; attempting to use its model for collision object creation.", name);
                        }
                        else
                        {
                            // Temporarily set rigidBody model on model so it's used during collision object creation.
                            Model* model = node->getModel();
                        
                            // Up ref count to prevent node from releasing the model when we swap it.
                            if (model)
                                model->addRef(); 
                        
                            // Create collision object with new rigidBodyModel (aka collisionMesh) set.
                            node->setModel(modelNode->getModel());
                            node->setCollisionObject(p);

                            // Restore original model.
                            node->setModel(model);
                        
                            // Decrement temporarily added reference.
                            if (model)
                                model->release();
                        }
                    }
                }
                else
                    node->setCollisionObject(p);
            }
            break;
        }
        default:
            GP_ERROR("Unsupported node property type (%d).", snp._type);
            break;
        }
    }
    else
    {
        // Handle scale, rotate and translate.
        Properties* np = sceneProperties->getNamespace(sceneNode._nodeID);

        switch (snp._type)
        {
        case SceneNodeProperty::TRANSLATE:
        {
            Vector3 t;
            if (np && np->getVector3("translate", &t))
                node->translate(t);
            break;
        }
        case SceneNodeProperty::ROTATE:
        {
            Quaternion r;
            if (np && np->getQuaternionFromAxisAngle("rotate", &r))
                node->rotate(r);
            break;
        }
        case SceneNodeProperty::SCALE:
        {
            Vector3 s;
            if (np && np->getVector3("scale", &s))
                node->scale(s);
            break;
        }
        default:
            GP_ERROR("Unsupported node property type (%d).", snp._type);
            break;
        }
    }
}

void SceneLoader::applyNodeUrls(Scene* scene)
{
    GP_ASSERT(scene);

    // Apply all URL node properties so that when we go to apply
    // the other node properties, the node is in the scene.
    for (size_t i = 0, ncount = _sceneNodes.size(); i < ncount; ++i)
    {
        SceneNode& sceneNode = _sceneNodes[i];

        // Iterate backwards over the properties list so we can remove properties as we go
        // without danger of indexing out of bounds.
        bool hasURL = false;
        for (int j = (int)sceneNode._properties.size() - 1; j >= 0; --j)
        {
            SceneNodeProperty& snp = sceneNode._properties[j];
            if (snp._type != SceneNodeProperty::URL)
                continue;

            hasURL = true;

            std::string file;
            std::string id;
            splitURL(snp._url, &file, &id);

            if (file.empty())
            {
                // The node is from the main GPB and should just be renamed.

                // TODO: Should we do all nodes with this case first to allow users to stitch in nodes with
                // IDs equal to IDs that were in the original GPB file but were changed in the scene file?
                if (sceneNode._exactMatch)
                {
                    Node* node = scene->findNode(id.c_str());
                    if (node)
                    {
                        node->setId(sceneNode._nodeID);
                    }
                    else
                    {
                        GP_ERROR("Could not find node '%s' in main scene GPB file.", id.c_str());
                    }
                    sceneNode._nodes.push_back(node);
                }
                else
                {
                    // Search for nodes using a partial match
                    std::vector<Node*> nodes;
                    unsigned int nodeCount = scene->findNodes(id.c_str(), nodes, true, false);
                    if (nodeCount > 0)
                    {
                        for (unsigned int k = 0; k < nodeCount; ++k)
                        {
                            // Construct a new node ID using _nodeID plus the remainder of the partial match.
                            Node* node = nodes[k];
                            std::string newID(sceneNode._nodeID);
                            newID += (node->getId() + id.length());
                            node->setId(newID.c_str());
                            sceneNode._nodes.push_back(node);
                        }
                    }
                    else
                    {
                        GP_ERROR("Could not find any nodes matching '%s' in main scene GPB file.", id.c_str());
                    }
                }
            }
            else
            {
                // An external file was referenced, so load the node(s) from file and then insert it into the scene with the new ID.

                // TODO: Revisit this to determine if we should cache Bundle objects for the duration of the scene
                // load to prevent constantly creating/destroying the same externally referenced bundles each time
                // a url with a file is encountered.
                Bundle* tmpBundle = Bundle::create(file.c_str());
                if (tmpBundle)
                {
                    if (sceneNode._exactMatch)
                    {
                        Node* node = tmpBundle->loadNode(id.c_str(), scene);
                        if (node)
                        {
                            node->setId(sceneNode._nodeID);
                            scene->addNode(node);
                            sceneNode._nodes.push_back(node);
                            SAFE_RELEASE(node);
                        }
                        else
                        {
                            GP_ERROR("Could not load node '%s' from GPB file '%s'.", id.c_str(), file.c_str());
                        }
                    }
                    else
                    {
                        // Search for nodes in the package using a partial match
                        unsigned int objectCount = tmpBundle->getObjectCount();
                        unsigned int matchCount = 0;
                        for (unsigned int k = 0; k < objectCount; ++k)
                        {
                            const char* objid = tmpBundle->getObjectId(k);
                            if (strstr(objid, id.c_str()) == objid)
                            {
                                // This object ID matches (starts with).
                                // Try to load this object as a Node.
                                Node* node = tmpBundle->loadNode(objid);
                                if (node)
                                {
                                    // Construct a new node ID using _nodeID plus the remainder of the partial match.
                                    std::string newID(sceneNode._nodeID);
                                    newID += (node->getId() + id.length());
                                    node->setId(newID.c_str());
                                    scene->addNode(node);
                                    sceneNode._nodes.push_back(node);
                                    SAFE_RELEASE(node);
                                    matchCount++;
                                }
                            }
                        }
                        if (matchCount == 0)
                        {
                            GP_ERROR("Could not find any nodes matching '%s' in GPB file '%s'.", id.c_str(), file.c_str());
                        }
                    }

                    SAFE_RELEASE(tmpBundle);
                }
                else
                {
                    GP_ERROR("Failed to load GPB file '%s' for node stitching.", file.c_str());
                }
            }

            // Remove the 'url' node property since we are done applying it.
            sceneNode._properties.erase(sceneNode._properties.begin() + j);

            // Processed URL property, no need to inspect remaining properties
            break;
        }

        if (!hasURL)
        {
            // No explicit URL, find the node in the main scene with the existing ID
            Node* node = scene->findNode(sceneNode._nodeID);
            if (node)
            {
                sceneNode._nodes.push_back(node);
            }
            else
            {
                // There is no node in the scene with this ID, so create a new empty node
                sceneNode._nodes.push_back(scene->addNode(sceneNode._nodeID));
            }
        }
    }
}

void SceneLoader::buildReferenceTables(Properties* sceneProperties)
{
    // Go through the child namespaces of the scene.
    Properties* ns;
    const char* name = NULL;
    while ((ns = sceneProperties->getNextNamespace()) != NULL)
    {
        if (strcmp(ns->getNamespace(), "node") == 0)
        {
            if (strlen(ns->getId()) == 0)
            {
                GP_ERROR("Attempting to load a node without an ID.");
                continue;
            }

            // Add a SceneNode to the end of the list.
            _sceneNodes.resize(_sceneNodes.size() + 1);
            SceneNode& sceneNode = _sceneNodes[_sceneNodes.size()-1];
            sceneNode._nodeID = ns->getId();

            // Parse the node's sub-namespaces.
            Properties* subns;
            std::string propertyUrl = _path + "#" + ns->getId() + "/";
            while ((subns = ns->getNextNamespace()) != NULL)
            {
                if (strcmp(subns->getNamespace(), "audio") == 0)
                {
                    propertyUrl += "audio/" + std::string(subns->getId());
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::AUDIO, propertyUrl.c_str());
                    _properties[propertyUrl] = subns;
                }
                else if (strcmp(subns->getNamespace(), "material") == 0)
                {
                    propertyUrl += "material/" + std::string(subns->getId());
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::MATERIAL, propertyUrl.c_str());
                    _properties[propertyUrl] = subns;
                }
                else if (strcmp(subns->getNamespace(), "particle") == 0)
                {
                    propertyUrl += "particle/" + std::string(subns->getId());
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::PARTICLE, propertyUrl.c_str());
                    _properties[propertyUrl] = subns;
                }
                else if (strcmp(subns->getNamespace(), "terrain") == 0)
                {
                    propertyUrl += "terrain/" + std::string(subns->getId());
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::TERRAIN, propertyUrl.c_str());
                    _properties[propertyUrl] = subns;
                }
                else if (strcmp(subns->getNamespace(), "light") == 0)
                {
                    propertyUrl += "light/" + std::string(subns->getId());
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::LIGHT, propertyUrl.c_str());
                    _properties[propertyUrl] = subns;
                }
                else if (strcmp(subns->getNamespace(), "camera") == 0)
                {
                    propertyUrl += "camera/" + std::string(subns->getId());
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::CAMERA, propertyUrl.c_str());
                    _properties[propertyUrl] = subns;
                }
                else if (strcmp(subns->getNamespace(), "collisionObject") == 0)
                {
                    propertyUrl += "collisionObject/" + std::string(subns->getId());
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::COLLISION_OBJECT, propertyUrl.c_str());
                    _properties[propertyUrl] = subns;
                }
                else if (strcmp(subns->getNamespace(), "tags") == 0)
                {
                    while ((name = subns->getNextProperty()) != NULL)
                    {
                        sceneNode._tags[name] = subns->getString();
                    }
                }
                else
                {
                    GP_ERROR("Unsupported child namespace '%s' of 'node' namespace.", subns->getNamespace());
                }
            }

            // Parse the node's attributes.
            while ((name = ns->getNextProperty()) != NULL)
            {
                if (strcmp(name, "url") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::URL, ns->getString());
                }
                else if (strcmp(name, "audio") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::AUDIO, ns->getString());
                }
                else if (strncmp(name, "material", 8) == 0)
                {
                    int materialIndex = -1;
                    name = strchr(name, '[');
                    if (name && strlen(name) >= 3)
                    {
                        std::string indexString(name);
                        indexString = indexString.substr(1, indexString.size()-2);
                        materialIndex = (unsigned int)atoi(indexString.c_str());
                    }
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::MATERIAL, ns->getString(), materialIndex);
                }
                else if (strcmp(name, "particle") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::PARTICLE, ns->getString());
                }
                else if (strcmp(name, "terrain") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::TERRAIN, ns->getString());
                }
                else if (strcmp(name, "light") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::LIGHT, ns->getString());
                }
                else if (strcmp(name, "camera") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::CAMERA, ns->getString());
                }
                else if (strcmp(name, "collisionObject") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::COLLISION_OBJECT, ns->getString());
                }
                else if (strcmp(name, "rigidBodyModel") == 0)
                {
                    // Ignore this for now. We process this when we do rigid body creation.
                }
                else if (strcmp(name, "collisionMesh") == 0)
                {
                    // Ignore this for now (new alias for rigidBodyModel). We process this when we do rigid body creation.
                }
                else if (strcmp(name, "translate") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::TRANSLATE);
                }
                else if (strcmp(name, "rotate") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::ROTATE);
                }
                else if (strcmp(name, "scale") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::SCALE);
                }
                else
                {
                    GP_ERROR("Unsupported node property: %s = %s", name, ns->getString());
                }
            }
        }
        else if (strcmp(ns->getNamespace(), "animations") == 0)
        {
            // Load all the animations.
            Properties* animation;
            while ((animation = ns->getNextNamespace()) != NULL)
            {
                if (strcmp(animation->getNamespace(), "animation") == 0)
                {
                    const char* animationID = animation->getId();
                    if (strlen(animationID) == 0)
                    {
                        GP_ERROR("Attempting to load an animation without an ID.");
                        continue;
                    }

                    const char* url = animation->getString("url");
                    if (!url)
                    {
                        GP_ERROR("Attempting to load animation '%s' without a URL.", animationID);
                        continue;
                    }
                    const char* targetID = animation->getString("target");
                    if (!targetID)
                    {
                        GP_ERROR("Attempting to load animation '%s' without a target.", animationID);
                        continue;
                    }

                    addSceneAnimation(animationID, targetID, url);
                }
                else
                {
                    GP_ERROR("Unsupported child namespace (of 'animations'): %s", ns->getNamespace());
                }
            }
        }
        else if (strcmp(ns->getNamespace(), "physics") == 0)
        {
            // Note: we don't load physics until the whole scene file has been 
            // loaded so that all node references (i.e. for constraints) can be resolved.
        }
        else
        {
            // TODO: Should we ignore these items? They could be used for generic properties file inheritance.
            GP_ERROR("Unsupported child namespace (of 'scene'): %s", ns->getNamespace());
        }
    }
}

void SceneLoader::createAnimations(const Scene* scene)
{
    // Create the scene animations.
    for (size_t i = 0, count = _animations.size(); i < count; i++)
    {
        // If the target node doesn't exist in the scene, then we
        // can't do anything so we skip to the next animation.
        Node* node = scene->findNode(_animations[i]._targetID);
        if (!node)
        {
            GP_ERROR("Attempting to create an animation targeting node '%s', which does not exist in the scene.", _animations[i]._targetID);
            continue;
        }

        // Check to make sure the referenced properties object was loaded properly.
        Properties* p = _properties[_animations[i]._url];
        if (!p)
        {
            GP_ERROR("The referenced animation data at url '%s' failed to load.", _animations[i]._url.c_str());
            continue;
        }

        node->createAnimation(_animations[i]._animationID, p);
    }
}

PhysicsConstraint* SceneLoader::loadGenericConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB)
{
    GP_ASSERT(rbA);
    GP_ASSERT(constraint);
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    PhysicsGenericConstraint* physicsConstraint = NULL;

    // Create the constraint from the specified properties.
    Quaternion roA;
    Vector3 toA;
    bool offsetSpecified = constraint->getQuaternionFromAxisAngle("rotationOffsetA", &roA);
    offsetSpecified |= constraint->getVector3("translationOffsetA", &toA);

    if (offsetSpecified)
    {
        if (rbB)
        {
            Quaternion roB;
            Vector3 toB;
            constraint->getQuaternionFromAxisAngle("rotationOffsetB", &roB);
            constraint->getVector3("translationOffsetB", &toB);

            physicsConstraint = Game::getInstance()->getPhysicsController()->createGenericConstraint(rbA, roA, toB, rbB, roB, toB);
        }
        else
        {
            physicsConstraint = Game::getInstance()->getPhysicsController()->createGenericConstraint(rbA, roA, toA);
        }
    }
    else
    {
        physicsConstraint = Game::getInstance()->getPhysicsController()->createGenericConstraint(rbA, rbB);
    }
    GP_ASSERT(physicsConstraint);

    // Set the optional parameters that were specified.
    Vector3 v;
    if (constraint->getVector3("angularLowerLimit", &v))
        physicsConstraint->setAngularLowerLimit(v);
    if (constraint->getVector3("angularUpperLimit", &v))
        physicsConstraint->setAngularUpperLimit(v);
    if (constraint->getVector3("linearLowerLimit", &v))
        physicsConstraint->setLinearLowerLimit(v);
    if (constraint->getVector3("linearUpperLimit", &v))
        physicsConstraint->setLinearUpperLimit(v);

    return physicsConstraint;
}

PhysicsConstraint* SceneLoader::loadHingeConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB)
{
    GP_ASSERT(rbA);
    GP_ASSERT(constraint);
    GP_ASSERT(Game::getInstance()->getPhysicsController());
    PhysicsHingeConstraint* physicsConstraint = NULL;

    // Create the constraint from the specified properties.
    Quaternion roA;
    Vector3 toA;
    constraint->getQuaternionFromAxisAngle("rotationOffsetA", &roA);
    constraint->getVector3("translationOffsetA", &toA);
    if (rbB)
    {
        Quaternion roB;
        Vector3 toB;
        constraint->getQuaternionFromAxisAngle("rotationOffsetB", &roB);
        constraint->getVector3("translationOffsetB", &toB);

        physicsConstraint = Game::getInstance()->getPhysicsController()->createHingeConstraint(rbA, roA, toB, rbB, roB, toB);
    }
    else
    {
        physicsConstraint = Game::getInstance()->getPhysicsController()->createHingeConstraint(rbA, roA, toA);
    }

    // Load the hinge angle limits (lower and upper) and the hinge bounciness (if specified).
    const char* limitsString = constraint->getString("limits");
    if (limitsString)
    {
        float lowerLimit, upperLimit;
        int scanned;
        scanned = sscanf(limitsString, "%f,%f", &lowerLimit, &upperLimit);
        if (scanned == 2)
        {
            physicsConstraint->setLimits(MATH_DEG_TO_RAD(lowerLimit), MATH_DEG_TO_RAD(upperLimit));
        }
        else
        {
            float bounciness;
            scanned = sscanf(limitsString, "%f,%f,%f", &lowerLimit, &upperLimit, &bounciness);
            if (scanned == 3)
            {
                physicsConstraint->setLimits(MATH_DEG_TO_RAD(lowerLimit), MATH_DEG_TO_RAD(upperLimit), bounciness);
            }
            else
            {
                GP_ERROR("Failed to parse 'limits' attribute for hinge constraint '%s'.", constraint->getId());
            }
        }
    }

    return physicsConstraint;
}

Scene* SceneLoader::loadMainSceneData(const Properties* sceneProperties)
{
    GP_ASSERT(sceneProperties);

    // Load the main scene from the specified path.
    Bundle* bundle = Bundle::create(_gpbPath.c_str());
    if (!bundle)
    {
        GP_ERROR("Failed to load scene GPB file '%s'.", _gpbPath.c_str());
        return NULL;
    }

    // TODO: Support loading a specific scene from a GPB file using the URL syntax (i.e. "res/scene.gpb#myscene").
    Scene* scene = bundle->loadScene(NULL);
    if (!scene)
    {
        GP_ERROR("Failed to load scene from '%s'.", _gpbPath.c_str());
        SAFE_RELEASE(bundle);
        return NULL;
    }

    SAFE_RELEASE(bundle);
    return scene;
}

void SceneLoader::loadPhysics(Properties* physics, Scene* scene)
{
    GP_ASSERT(physics);
    GP_ASSERT(scene);
    GP_ASSERT(Game::getInstance()->getPhysicsController());

    // Go through the supported global physics properties and apply them.
    Vector3 gravity;
    if (physics->getVector3("gravity", &gravity))
        Game::getInstance()->getPhysicsController()->setGravity(gravity);

    Properties* constraint;
    const char* name;
    while ((constraint = physics->getNextNamespace()) != NULL)
    {
        if (strcmp(constraint->getNamespace(), "constraint") == 0)
        {
            // Get the constraint type.
            std::string type = constraint->getString("type");

            // Attempt to load the first rigid body. If the first rigid body cannot
            // be loaded or found, then continue to the next constraint (error).
            name = constraint->getString("rigidBodyA");
            if (!name)
            {
                GP_ERROR("Missing property 'rigidBodyA' for constraint '%s'.", constraint->getId());
                continue;
            }
            Node* rbANode = scene->findNode(name);
            if (!rbANode)
            {
                GP_ERROR("Node '%s' to be used as 'rigidBodyA' for constraint '%s' cannot be found.", name, constraint->getId());
                continue;
            }
            if (!rbANode->getCollisionObject() || rbANode->getCollisionObject()->getType() != PhysicsCollisionObject::RIGID_BODY)
            {
                GP_ERROR("Node '%s' to be used as 'rigidBodyA' does not have a rigid body.", name);
                continue;
            }
            PhysicsRigidBody* rbA = static_cast<PhysicsRigidBody*>(rbANode->getCollisionObject());

            // Attempt to load the second rigid body. If the second rigid body is not
            // specified, that is usually okay (only spring constraints require both and
            // we check that below), but if the second rigid body is specified and it doesn't
            // load properly, then continue to the next constraint (error).
            name = constraint->getString("rigidBodyB");
            PhysicsRigidBody* rbB = NULL;
            if (name)
            {
                Node* rbBNode = scene->findNode(name);
                if (!rbBNode)
                {
                    GP_ERROR("Node '%s' to be used as 'rigidBodyB' for constraint '%s' cannot be found.", name, constraint->getId());
                    continue;
                }
                if (!rbBNode->getCollisionObject() || rbBNode->getCollisionObject()->getType() != PhysicsCollisionObject::RIGID_BODY)
                {
                    GP_ERROR("Node '%s' to be used as 'rigidBodyB' does not have a rigid body.", name);
                    continue;
                }
                rbB = static_cast<PhysicsRigidBody*>(rbBNode->getCollisionObject());
            }

            PhysicsConstraint* physicsConstraint = NULL;

            // Load the constraint based on its type.
            if (type == "FIXED")
            {
                physicsConstraint = Game::getInstance()->getPhysicsController()->createFixedConstraint(rbA, rbB);
            }
            else if (type == "GENERIC")
            {
                physicsConstraint = loadGenericConstraint(constraint, rbA, rbB);
            }
            else if (type == "HINGE")
            {
                physicsConstraint = loadHingeConstraint(constraint, rbA, rbB);
            }
            else if (type == "SOCKET")
            {
                physicsConstraint = loadSocketConstraint(constraint, rbA, rbB);
            }
            else if (type == "SPRING")
            {
                physicsConstraint = loadSpringConstraint(constraint, rbA, rbB);
            }
            else
            {
                GP_ERROR("Unsupported physics constraint type '%s'.", type.c_str());
            }
            
            // If the constraint failed to load, continue on to the next one.
            if (!physicsConstraint)
            {
                GP_ERROR("Failed to create physics constraint.");
                continue;
            }

            // If the breaking impulse was specified, apply it to the constraint.
            if (constraint->exists("breakingImpulse"))
                physicsConstraint->setBreakingImpulse(constraint->getFloat("breakingImpulse"));
        }
        else
        {
            GP_ERROR("Unsupported 'physics' child namespace '%s'.", physics->getNamespace());
        }
    }
}

void SceneLoader::loadReferencedFiles()
{
    // Load all referenced properties files.
    std::map<std::string, Properties*>::iterator iter = _properties.begin();
    for (; iter != _properties.end(); ++iter)
    {
        if (iter->second == NULL)
        {
            std::string fileString;
            std::vector<std::string> namespacePath;
            calculateNamespacePath(iter->first, fileString, namespacePath);

            // Check if the referenced properties file has already been loaded.
            Properties* properties = NULL;
            std::map<std::string, Properties*>::iterator pffIter = _propertiesFromFile.find(fileString);
            if (pffIter != _propertiesFromFile.end() && pffIter->second)
            {
                properties = pffIter->second;
            }
            else
            {
                properties = Properties::create(fileString.c_str());
                if (properties == NULL)
                {
                    GP_ERROR("Failed to load referenced properties file '%s'.", fileString.c_str());
                    continue;
                }

                // Add the properties object to the cache.
                _propertiesFromFile.insert(std::make_pair(fileString, properties));
            }

            Properties* p = getPropertiesFromNamespacePath(properties, namespacePath);
            if (!p)
            {
                GP_ERROR("Failed to load referenced properties from url '%s'.", iter->first.c_str());
                continue;
            }
            iter->second = p;
        }
    }
}

PhysicsConstraint* SceneLoader::loadSocketConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB)
{
    GP_ASSERT(rbA);
    GP_ASSERT(constraint);
    GP_ASSERT(Game::getInstance()->getPhysicsController());

    PhysicsSocketConstraint* physicsConstraint = NULL;
    Vector3 toA;
    bool offsetSpecified = constraint->getVector3("translationOffsetA", &toA);

    if (offsetSpecified)
    {
        if (rbB)
        {
            Vector3 toB;
            constraint->getVector3("translationOffsetB", &toB);

            physicsConstraint = Game::getInstance()->getPhysicsController()->createSocketConstraint(rbA, toA, rbB, toB);
        }
        else
        {
            physicsConstraint = Game::getInstance()->getPhysicsController()->createSocketConstraint(rbA, toA);
        }
    }
    else
    {
        physicsConstraint = Game::getInstance()->getPhysicsController()->createSocketConstraint(rbA, rbB);
    }

    return physicsConstraint;
}

PhysicsConstraint* SceneLoader::loadSpringConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB)
{
    GP_ASSERT(rbA);
    GP_ASSERT(constraint);
    GP_ASSERT(Game::getInstance()->getPhysicsController());

    if (!rbB)
    {
        GP_ERROR("Spring constraints require two rigid bodies.");
        return NULL;
    }

    PhysicsSpringConstraint* physicsConstraint = NULL;

    // Create the constraint from the specified properties.
    Quaternion roA, roB;
    Vector3 toA, toB;
    bool offsetsSpecified = constraint->getQuaternionFromAxisAngle("rotationOffsetA", &roA);
    offsetsSpecified |= constraint->getVector3("translationOffsetA", &toA);
    offsetsSpecified |= constraint->getQuaternionFromAxisAngle("rotationOffsetB", &roB);
    offsetsSpecified |= constraint->getVector3("translationOffsetB", &toB);

    if (offsetsSpecified)
    {
        physicsConstraint = Game::getInstance()->getPhysicsController()->createSpringConstraint(rbA, roA, toB, rbB, roB, toB);
    }
    else
    {
        physicsConstraint = Game::getInstance()->getPhysicsController()->createSpringConstraint(rbA, rbB);
    }
    GP_ASSERT(physicsConstraint);

    // Set the optional parameters that were specified.
    Vector3 v;
    if (constraint->getVector3("angularLowerLimit", &v))
        physicsConstraint->setAngularLowerLimit(v);
    if (constraint->getVector3("angularUpperLimit", &v))
        physicsConstraint->setAngularUpperLimit(v);
    if (constraint->getVector3("linearLowerLimit", &v))
        physicsConstraint->setLinearLowerLimit(v);
    if (constraint->getVector3("linearUpperLimit", &v))
        physicsConstraint->setLinearUpperLimit(v);
    if (constraint->getString("angularDampingX"))
        physicsConstraint->setAngularDampingX(constraint->getFloat("angularDampingX"));
    if (constraint->getString("angularDampingY"))
        physicsConstraint->setAngularDampingY(constraint->getFloat("angularDampingY"));
    if (constraint->getString("angularDampingZ"))
        physicsConstraint->setAngularDampingZ(constraint->getFloat("angularDampingZ"));
    if (constraint->getString("angularStrengthX"))
        physicsConstraint->setAngularStrengthX(constraint->getFloat("angularStrengthX"));
    if (constraint->getString("angularStrengthY"))
        physicsConstraint->setAngularStrengthY(constraint->getFloat("angularStrengthY"));
    if (constraint->getString("angularStrengthZ"))
        physicsConstraint->setAngularStrengthZ(constraint->getFloat("angularStrengthZ"));
    if (constraint->getString("linearDampingX"))
        physicsConstraint->setLinearDampingX(constraint->getFloat("linearDampingX"));
    if (constraint->getString("linearDampingY"))
        physicsConstraint->setLinearDampingY(constraint->getFloat("linearDampingY"));
    if (constraint->getString("linearDampingZ"))
        physicsConstraint->setLinearDampingZ(constraint->getFloat("linearDampingZ"));
    if (constraint->getString("linearStrengthX"))
        physicsConstraint->setLinearStrengthX(constraint->getFloat("linearStrengthX"));
    if (constraint->getString("linearStrengthY"))
        physicsConstraint->setLinearStrengthY(constraint->getFloat("linearStrengthY"));
    if (constraint->getString("linearStrengthZ"))
        physicsConstraint->setLinearStrengthZ(constraint->getFloat("linearStrengthZ"));

    return physicsConstraint;
}

void splitURL(const std::string& url, std::string* file, std::string* id)
{
    if (url.empty())
    {
        // This is allowed since many scene node properties do not use the URL.
        return;
    }

    // Check if the url references a file (otherwise, it only references a node within the main GPB).
    size_t loc = url.rfind(".");
    if (loc != std::string::npos)
    {
        // If the url references a specific namespace within the file,
        // set the id out parameter appropriately. Otherwise, set the id out
        // parameter to the empty string so we know to load the first namespace.
        loc = url.rfind("#");
        if (loc != std::string::npos)
        {
            *file = url.substr(0, loc);
            *id = url.substr(loc + 1);
        }
        else
        {
            *file = url;
            *id = std::string();
        }
    }
    else
    {
        *file = std::string();
        *id = url;
    }
}

SceneLoader::SceneNode::SceneNode()
    : _nodeID(""), _exactMatch(true)
{
}

SceneLoader::SceneNodeProperty::SceneNodeProperty(Type type, const std::string& url, int index)
    : _type(type), _url(url), _index(index)
{
}

}
