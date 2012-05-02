#include "Base.h"
#include "Game.h"
#include "Bundle.h"
#include "SceneLoader.h"

namespace gameplay
{

std::map<std::string, Properties*> SceneLoader::_propertiesFromFile;
std::vector<SceneLoader::SceneAnimation> SceneLoader::_animations;
std::vector<SceneLoader::SceneNode> SceneLoader::_sceneNodes;
std::string SceneLoader::_path;

Scene* SceneLoader::load(const char* url)
{
    assert(url);

    // Load the scene properties from file.
    Properties* properties = Properties::create(url);
    assert(properties);
    if (properties == NULL)
    {
        WARN_VARG("Failed to load scene file: %s", url);
        return NULL;
    }

    // Check if the properties object is valid and has a valid namespace.
    Properties* sceneProperties = (strlen(properties->getNamespace()) > 0) ? properties : properties->getNextNamespace();
    assert(sceneProperties);
    if (!sceneProperties || !(strcmp(sceneProperties->getNamespace(), "scene") == 0))
    {
        WARN("Failed to load scene from properties object: must be non-null object and have namespace equal to 'scene'.");
        SAFE_DELETE(properties);
        return NULL;
    }

    // Get the path to the main GPB.
    _path = sceneProperties->getString("path");

    // Build the node URL/property and animation reference tables and load the referenced files.
    buildReferenceTables(sceneProperties);
    loadReferencedFiles();

    // Load the main scene data from GPB and apply the global scene properties.
    Scene* scene = loadMainSceneData(sceneProperties);
    if (!scene)
    {
        SAFE_DELETE(properties);
        return NULL;
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
        SceneNodeProperty::ROTATE |
        SceneNodeProperty::SCALE |
        SceneNodeProperty::TRANSLATE | 
        SceneNodeProperty::TRANSPARENT |
        SceneNodeProperty::DYNAMIC);
    applyNodeProperties(scene, sceneProperties, SceneNodeProperty::CHARACTER | SceneNodeProperty::GHOSTOBJECT | SceneNodeProperty::RIGIDBODY);
    createAnimations(scene);

    // Find the physics properties object.
    Properties* physics = NULL;
    Properties* ns = NULL;
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
    for (; iter != _propertiesFromFile.end(); iter++)
    {
        SAFE_DELETE(iter->second);
    }

    // Clean up the .scene file's properties object.
    SAFE_DELETE(properties);

    // Clear all temporary data stores.
    _propertiesFromFile.clear();
    _animations.clear();
    _sceneNodes.clear();

    return scene;
}

void SceneLoader::addSceneAnimation(const char* animationID, const char* targetID, const char* url)
{
    // Calculate the file and id from the given url.
    std::string file;
    std::string id;
    splitURL(url, &file, &id);

    // If there is a file that needs to be loaded later, add an 
    // empty entry to the properties table to signify it.
    if (file.length() > 0 && _propertiesFromFile.count(file) == 0)
        _propertiesFromFile[file] = NULL;

    // Add the animation to the list of animations to be resolved later.
    _animations.push_back(SceneAnimation(animationID, targetID, file, id));
}

void SceneLoader::addSceneNodeProperty(SceneNode& sceneNode, SceneNodeProperty::Type type, const char* url, int index)
{
    // Calculate the file and id from the given url.
    std::string file;
    std::string id;
    splitURL(url, &file, &id);

    // If there is a non-GPB file that needs to be loaded later, add an 
    // empty entry to the properties table to signify it.
    if (file.length() > 0 && file.find(".gpb") == file.npos && _propertiesFromFile.count(file) == 0)
        _propertiesFromFile[file] = NULL;

    SceneNodeProperty prop(type, file, id, index);

    // Parse for wildcharacter character (only supported on the URL attribute)
    if (type == SceneNodeProperty::URL)
    {
        if (id.length() > 1 && id.at(id.length()-1) == '*')
        {
            prop._id = id.substr(0, id.length()-1);
            sceneNode._exactMatch = false;
        }
    }

    // Add the node property to the list of node properties to be resolved later.
    sceneNode._properties.push_back(prop);
}

void SceneLoader::applyNodeProperties(const Scene* scene, const Properties* sceneProperties, unsigned int typeFlags)
{
    for (unsigned int i = 0, ncount = _sceneNodes.size(); i < ncount; ++i)
    {
        SceneNode& sceneNode = _sceneNodes[i];

        if (sceneNode._exactMatch)
        {
            // Find the node matching the specified ID exactly
            Node* node = scene->findNode(sceneNode._nodeID);
            if (!node)
            {
                WARN_VARG("Attempting to set a property for node '%s', which does not exist in the scene.", sceneNode._nodeID);
                continue;
            }

            for (unsigned int j = 0, pcount = sceneNode._properties.size(); j < pcount; ++j)
            {
                SceneNodeProperty& snp = sceneNode._properties[j];
                if (typeFlags & snp._type)
                    applyNodeProperty(sceneNode, node, sceneProperties, snp);
            }
        }
        else
        {
            // Find all nodes matching the specified ID
            std::vector<Node*> nodes;
            unsigned int nodeCount = scene->findNodes(sceneNode._nodeID, nodes, true, false);
            if (nodeCount == 0)
                continue;
            
            for (unsigned int j = 0, pcount = sceneNode._properties.size(); j < pcount; ++j)
            {
                SceneNodeProperty& snp = sceneNode._properties[j];
                if ((typeFlags & snp._type) == 0)
                    continue;

                for (unsigned int k = 0; k < nodeCount; ++k)
                    applyNodeProperty(sceneNode, nodes[k], sceneProperties, snp);
            }
        }
    }
}

void SceneLoader::applyNodeProperty(SceneNode& sceneNode, Node* node, const Properties* sceneProperties, const SceneNodeProperty& snp)
{
    if (snp._type == SceneNodeProperty::AUDIO ||
        snp._type == SceneNodeProperty::MATERIAL ||
        snp._type == SceneNodeProperty::PARTICLE ||
        snp._type == SceneNodeProperty::CHARACTER ||
        snp._type == SceneNodeProperty::GHOSTOBJECT ||
        snp._type == SceneNodeProperty::RIGIDBODY)
    {
        // Check to make sure the referenced properties object was loaded properly.
        Properties* p = _propertiesFromFile[snp._file];
        if (!p)
        {
            WARN_VARG("The referenced node data in file '%s' failed to load.", snp._file.c_str());
            return;
        }

        // If a specific namespace within the file was specified, load that namespace.
        if (snp._id.size() > 0)
        {
            p = p->getNamespace(snp._id.c_str());
            if (!p)
            {
                WARN_VARG("The referenced node data at '%s#%s' failed to load.", snp._file.c_str(), snp._id.c_str());
                return;
            }
        }
        else
        {
            // Otherwise, use the first namespace.
            p->rewind();
            p = p->getNextNamespace();
        }

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
                WARN_VARG("Attempting to set a material on node '%s', which has no model.", sceneNode._nodeID);
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
        case SceneNodeProperty::CHARACTER:
        case SceneNodeProperty::GHOSTOBJECT:
        case SceneNodeProperty::RIGIDBODY:
        {
            // Check to make sure the referenced properties object was loaded properly.
            Properties* p = _propertiesFromFile[snp._file];
            if (!p)
            {
                WARN_VARG("The referenced node data in file '%s' failed to load.", snp._file.c_str());
                return;
            }

            // If a specific namespace within the file was specified, load that namespace.
            if (snp._id.size() > 0)
            {
                p = p->getNamespace(snp._id.c_str());
                if (!p)
                {
                    WARN_VARG("The referenced node data at '%s#%s' failed to load.", snp._file.c_str(), snp._id.c_str());
                    return;
                }
            }
            else
            {
                // Otherwise, use the first namespace.
                p->rewind();
                p = p->getNextNamespace();
            }

            // Check to make sure the type of the namespace used to load the physics collision object is correct.
            if (snp._type == SceneNodeProperty::CHARACTER && strcmp(p->getNamespace(), "character") != 0)
            {
                WARN_VARG("Attempting to set a 'character' (physics collision object attribute) on a node using a '%s' definition.", p->getNamespace());
            }
            else if (snp._type == SceneNodeProperty::GHOSTOBJECT && strcmp(p->getNamespace(), "ghostObject") != 0)
            {
                WARN_VARG("Attempting to set a 'ghostObject' (physics collision object attribute) on a node using a '%s' definition.", p->getNamespace());
            }
            else if (snp._type == SceneNodeProperty::RIGIDBODY && strcmp(p->getNamespace(), "rigidBody") != 0)
            {
                WARN_VARG("Attempting to set a 'rigidBody' (physics collision object attribute) on a node using a '%s' definition.", p->getNamespace());
            }
            else
            {
                // If the scene file specifies a rigid body model, use it for creating the collision object.
                Properties* np = sceneProperties->getNamespace(sceneNode._nodeID);
                const char* name = NULL;
                if (np && (name = np->getString("rigidBodyModel")))
                {
                    Node* modelNode = node->getScene()->findNode(name);
                    if (!modelNode)
                        WARN_VARG("Node '%s' does not exist; attempting to use its model for collision object creation.", name);
                    else
                    {
                        if (!modelNode->getModel())
                            WARN_VARG("Node '%s' does not have a model; attempting to use its model for collision object creation.", name);
                        else
                        {
                            // Temporarily set rigidBody model on model so it's used during collision object creation.
                            Model* model = node->getModel();
                            assert(model);
                        
                            // Up ref count to prevent node from releasing the model when we swap it.
                            model->addRef(); 
                        
                            // Create collision object with new rigidBodyModel set.
                            node->setModel(modelNode->getModel());
                            node->setCollisionObject(p);

                            // Restore original model.
                            node->setModel(model);
                        
                            // Decrement temporarily added reference.
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
            // This cannot happen.
            break;
        }
    }
    else
    {
        // Handle Scale, Rotate and Translate
        Properties* np = sceneProperties->getNamespace(sceneNode._nodeID);
        const char* name = NULL;

        switch (snp._type)
        {
        case SceneNodeProperty::TRANSLATE:
        {
            Vector3 t;
            if (np && np->getVector3("translate", &t))
                node->setTranslation(t);
            break;
        }
        case SceneNodeProperty::ROTATE:
        {
            Quaternion r;
            if (np && np->getQuaternionFromAxisAngle("rotate", &r))
                node->setRotation(r);
            break;
        }
        case SceneNodeProperty::SCALE:
        {
            Vector3 s;
            if (np && np->getVector3("scale", &s))
                node->setScale(s);
            break;
        }
        case SceneNodeProperty::TRANSPARENT:
        {
            node->setTransparent(true);
            break;
        }
        case SceneNodeProperty::DYNAMIC:
        {
            node->setDynamic(true);
            break;
        }
        default:
            WARN_VARG("Unsupported node property type: %d.", snp._type);
            break;
        }
    }
}

void SceneLoader::applyNodeUrls(Scene* scene)
{
    // Apply all URL node properties so that when we go to apply
    // the other node properties, the node is in the scene.
    for (unsigned int i = 0, ncount = _sceneNodes.size(); i < ncount; ++i)
    {
        SceneNode& sceneNode = _sceneNodes[i];

        // Iterate backwards over the properties list so we can remove properties as we go
        // without danger of indexing out of bounds.
        for (int j = sceneNode._properties.size() - 1; j >= 0; --j)
        {
            SceneNodeProperty& snp = sceneNode._properties[j];
            if (snp._type != SceneNodeProperty::URL)
                continue;

            if (snp._file.empty())
            {
                // The node is from the main GPB and should just be renamed.

                // TODO: Should we do all nodes with this case first to allow users to stitch in nodes with
                // IDs equal to IDs that were in the original GPB file but were changed in the scene file?
                if (sceneNode._exactMatch)
                {
                    Node* node = scene->findNode(snp._id.c_str());
                    if (node)
                    {
                        node->setId(sceneNode._nodeID);
                    }
                    else
                    {
                        WARN_VARG("Could not find node '%s' in main scene GPB file.", snp._id.c_str());
                    }
                }
                else
                {
                    // Search for nodes using a partial match
                    std::string partialMatch = snp._id;
                    std::vector<Node*> nodes;
                    unsigned int nodeCount = scene->findNodes(snp._id.c_str(), nodes, true, false);
                    if (nodeCount > 0)
                    {
                        for (unsigned int k = 0; k < nodeCount; ++k)
                        {
                            // Construct a new node ID using _nodeID plus the remainder of the partial match.
                            Node* node = nodes[k];
                            std::string newID(sceneNode._nodeID);
                            newID += (node->getId() + snp._id.length());
                            node->setId(newID.c_str());
                        }
                    }
                    else
                    {
                        WARN_VARG("Could not find any nodes matching '%s' in main scene GPB file.", snp._id.c_str());
                    }
                }
            }
            else
            {
                // An external file was referenced, so load the node from file and then insert it into the scene with the new ID.

                // TODO: Revisit this to determine if we should cache Bundle objects for the duration of the scene
                // load to prevent constantly creating/destroying the same externally referenced bundles each time
                // a url with a file is encountered.
                Bundle* tmpBundle = Bundle::create(snp._file.c_str());
                if (tmpBundle)
                {
                    if (sceneNode._exactMatch)
                    {
                        Node* node = tmpBundle->loadNode(snp._id.c_str(), scene);
                        if (node)
                        {
                            node->setId(sceneNode._nodeID);
                            scene->addNode(node);
                            SAFE_RELEASE(node);
                        }
                        else
                        {
                            WARN_VARG("Could not load node '%s' in GPB file '%s'.", snp._id.c_str(), snp._file.c_str());
                        }
                    }
                    else
                    {
                        // Search for nodes in the bundle using a partial match
                        std::string partialMatch = snp._id;
                        unsigned int objectCount = tmpBundle->getObjectCount();
                        unsigned int matchCount = 0;
                        for (unsigned int k = 0; k < objectCount; ++k)
                        {
                            const char* objid = tmpBundle->getObjectID(k);
                            if (strstr(objid, snp._id.c_str()) == objid)
                            {
                                // This object ID matches (starts with).
                                // Try to load this object as a Node.
                                Node* node = tmpBundle->loadNode(objid);
                                if (node)
                                {
                                    // Construct a new node ID using _nodeID plus the remainder of the partial match.
                                    std::string newID(sceneNode._nodeID);
                                    newID += (node->getId() + snp._id.length());
                                    node->setId(newID.c_str());
                                    scene->addNode(node);
                                    SAFE_RELEASE(node);
                                    matchCount++;
                                }
                            }
                        }
                        if (matchCount == 0)
                        {
                            WARN_VARG("Could not find any nodes matching '%s' in GPB file '%s'.", snp._id.c_str(), snp._file.c_str());
                        }
                    }

                    SAFE_RELEASE(tmpBundle);
                }
                else
                {
                    WARN_VARG("Failed to load GPB file '%s' for node stitching.", snp._file.c_str());
                }
            }

            // Remove the node property since we are done applying it.
            sceneNode._properties.erase(sceneNode._properties.begin() + j);
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
                WARN("Nodes must have an ID; skipping the current node.");
                continue;
            }

            // Add a SceneNode to the end of the list
            _sceneNodes.resize(_sceneNodes.size() + 1);
            SceneNode& sceneNode = _sceneNodes[_sceneNodes.size()-1];
            sceneNode._nodeID = ns->getId();

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
                else if (strcmp(name, "character") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::CHARACTER, ns->getString());
                }
                else if (strcmp(name, "ghostObject") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::GHOSTOBJECT, ns->getString());
                }
                else if (strcmp(name, "rigidBody") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::RIGIDBODY, ns->getString());
                }
                else if (strcmp(name, "rigidBodyModel") == 0)
                {
                    // Ignore this for now. We process this when we do rigid body creation.
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
                else if (strcmp(name, "transparent") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::TRANSPARENT);
                }
                else if (strcmp(name, "dynamic") == 0)
                {
                    addSceneNodeProperty(sceneNode, SceneNodeProperty::DYNAMIC);
                }
                else
                {
                    WARN_VARG("Unsupported node property: %s = %s", name, ns->getString());
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
                        WARN("Animations must have an ID; skipping the current animation.");
                        continue;
                    }

                    const char* url = animation->getString("url");
                    if (!url)
                    {
                        WARN_VARG("Animations must have a URL; skipping animation '%s'.", animationID);
                        continue;
                    }
                    const char* targetID = animation->getString("target");
                    if (!targetID)
                    {
                        WARN_VARG("Animations must have a target; skipping animation '%s'.", animationID);
                        continue;
                    }

                    addSceneAnimation(animationID, targetID, url);
                }
                else
                {
                    WARN_VARG("Unsupported child namespace (of 'animations'): %s", ns->getNamespace());
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
            WARN_VARG("Unsupported child namespace (of 'scene'): %s", ns->getNamespace());
        }
    }
}

void SceneLoader::createAnimations(const Scene* scene)
{
    // Create the scene animations.
    for (unsigned int i = 0, count = _animations.size(); i < count; i++)
    {
        // If the target node doesn't exist in the scene, then we
        // can't do anything so we skip to the next animation.
        Node* node = scene->findNode(_animations[i]._targetID);
        if (!node)
        {
            WARN_VARG("Attempting to create an animation targeting node '%s', which does not exist in the scene.", _animations[i]._targetID);
            continue;
        }

        // Check to make sure the referenced properties object was loaded properly.
        Properties* p = _propertiesFromFile[_animations[i]._file];
        if (!p)
        {
            WARN_VARG("The referenced animation data in file '%s' failed to load.", _animations[i]._file.c_str());
            continue;
        }
        if (_animations[i]._id.size() > 0)
        {
            p = p->getNamespace(_animations[i]._id.c_str());
            if (!p)
            {
                WARN_VARG("The referenced animation data at '%s#%s' failed to load.", _animations[i]._file.c_str(), _animations[i]._id.c_str());
                continue;
            }
        }

        node->createAnimation(_animations[i]._animationID, p);
    }
}

PhysicsConstraint* SceneLoader::loadGenericConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB)
{
    PhysicsGenericConstraint* physicsConstraint;

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

    // Attempt to load the hinge limits first as a Vector3 and if that doesn't work, try loading as a Vector2.
    // We do this because the user can specify just the min and max angle, or both angle along with bounciness.
    Vector3 fullLimits;
    Vector2 angleLimits;
    if (constraint->getVector3("limits", &fullLimits))
        physicsConstraint->setLimits(MATH_DEG_TO_RAD(fullLimits.x), MATH_DEG_TO_RAD(fullLimits.y), fullLimits.z);
    else if (constraint->getVector2("limits", &angleLimits))
        physicsConstraint->setLimits(angleLimits.x, angleLimits.y);

    return physicsConstraint;
}

Scene* SceneLoader::loadMainSceneData(const Properties* sceneProperties)
{
    // Load the main scene from the specified path.
    Bundle* bundle = Bundle::create(_path.c_str());
    if (!bundle)
    {
        WARN_VARG("Failed to load scene GPB file '%s'.", _path.c_str());
        return NULL;
    }

    // TODO: Support loading a specific scene from a GPB file using the URL syntax (i.e. "res/scene.gpb#myscene").
    Scene* scene = bundle->loadScene(NULL);
    if (!scene)
    {
        WARN_VARG("Failed to load scene from '%s'.", _path.c_str());
        SAFE_RELEASE(bundle);
        return NULL;
    }

    // Go through the supported scene properties and apply them to the scene.
    const char* name = sceneProperties->getString("activeCamera");
    if (name)
    {
        Node* camera = scene->findNode(name);
        if (camera && camera->getCamera())
            scene->setActiveCamera(camera->getCamera());
    }

    SAFE_RELEASE(bundle);
    return scene;
}

void SceneLoader::loadPhysics(Properties* physics, Scene* scene)
{
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
                WARN_VARG("Missing property 'rigidBodyA' for constraint %s", constraint->getId());
                continue;
            }
            Node* rbANode = scene->findNode(name);
            if (!rbANode)
            {
                WARN_VARG("Node '%s' to be used as 'rigidBodyA' for constraint %s cannot be found.", name, constraint->getId());
                continue;
            }
            if (!rbANode->getCollisionObject() || rbANode->getCollisionObject()->getType() != PhysicsCollisionObject::RIGID_BODY)
            {
                WARN_VARG("Node '%s' to be used as 'rigidBodyA' does not have a rigid body.", name);
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
                    WARN_VARG("Node '%s' to be used as 'rigidBodyB' for constraint %s cannot be found.", name, constraint->getId());
                    continue;
                }
                if (!rbBNode->getCollisionObject() || rbBNode->getCollisionObject()->getType() != PhysicsCollisionObject::RIGID_BODY)
                {
                    WARN_VARG("Node '%s' to be used as 'rigidBodyB' does not have a rigid body.", name);
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
            
            // If the constraint failed to load, continue on to the next one.
            if (!physicsConstraint)
                continue;

            // If the breaking impulse was specified, apply it to the constraint.
            if (constraint->getString("breakingImpulse"))
                physicsConstraint->setBreakingImpulse(constraint->getFloat("breakingImpulse"));
        }
        else
        {
            WARN_VARG("Unsupported child namespace (of 'physics'): %s", physics->getNamespace());
        }
    }
}

void SceneLoader::loadReferencedFiles()
{
    // Load all referenced properties files.
    std::map<std::string, Properties*>::iterator iter = _propertiesFromFile.begin();
    for (; iter != _propertiesFromFile.end(); iter++)
    {
        Properties* p = Properties::create(iter->first.c_str());
        assert(p);
        if (p == NULL)
            WARN_VARG("Failed to load referenced file: %s", iter->first.c_str());

        iter->second = p;
    }
}

PhysicsConstraint* SceneLoader::loadSocketConstraint(const Properties* constraint, PhysicsRigidBody* rbA, PhysicsRigidBody* rbB)
{
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
    if (!rbB)
    {
        WARN("Spring constraints require two rigid bodies.");
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

void SceneLoader::splitURL(const char* url, std::string* file, std::string* id)
{
    if (!url)
        return;

    std::string urlString = url;

    // Check if the url references a file (otherwise, it only references a node within the main GPB).
    unsigned int loc = urlString.rfind(".");
    if (loc != urlString.npos)
    {
        // If the url references a specific namespace within the file,
        // set the id out parameter appropriately. Otherwise, set the id out
        // parameter to the empty string so we know to load the first namespace.
        loc = urlString.rfind("#");
        if (loc != urlString.npos)
        {
            *file = urlString.substr(0, loc);
            *id = urlString.substr(loc + 1);
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

}
