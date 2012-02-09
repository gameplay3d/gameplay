#include "Base.h"
#include "Game.h"
#include "Package.h"
#include "SceneLoader.h"

namespace gameplay
{

// Static member variables.
std::map<std::string, Properties*> SceneLoader::_propertiesFromFile;
std::vector<SceneLoader::SceneAnimation> SceneLoader::_animations;
std::vector<SceneLoader::SceneNodeProperty> SceneLoader::_nodeProperties;
std::vector<std::string> SceneLoader::_nodesWithMeshRB;
std::map<std::string, SceneLoader::MeshRigidBodyData>* SceneLoader::_meshRigidBodyData = NULL;
std::string SceneLoader::_path;

Scene* SceneLoader::load(const char* filePath)
{
    assert(filePath);

    // Load the scene properties from file.
    Properties* properties = Properties::create(filePath);
    assert(properties);
    if (properties == NULL)
    {
        WARN_VARG("Failed to load scene file: %s", filePath);
        return NULL;
    }

    // Check if the properties object is valid and has a valid namespace.
    Properties* sceneProperties = properties->getNextNamespace();
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

    // Calculate the node IDs that need to be loaded with mesh rigid body support.
    calculateNodesWithMeshRigidBodies(sceneProperties);

    // Set up for storing the mesh rigid body data.
    if (_nodesWithMeshRB.size() > 0)
    {
        // We do not currently support loading more than one scene simultaneously.
        if (_meshRigidBodyData)
        {
            WARN("Attempting to load multiple scenes simultaneously; mesh rigid bodies will not load properly.");
        }

        _meshRigidBodyData = new std::map<std::string, MeshRigidBodyData>();
    }

    // Load the main scene data from GPB and apply the global scene properties.
    Scene* scene = loadMainSceneData(sceneProperties);
    if (!scene)
    {
        SAFE_DELETE(properties);
        return NULL;
    }

    // First apply the node url properties. Following that,
    // apply the normal node properties and create the animations.
    applyNodeUrls(scene);
    applyNodeProperties(scene, sceneProperties);
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

    // Clean up mesh rigid body data.
    if (_meshRigidBodyData)
    {
        std::map<std::string, MeshRigidBodyData>::iterator iter = _meshRigidBodyData->begin();
        for (; iter != _meshRigidBodyData->end(); iter++)
        {
            for (unsigned int i = 0; i < iter->second.indexData.size(); i++)
            {
                SAFE_DELETE_ARRAY(iter->second.indexData[i]);
            }

            SAFE_DELETE_ARRAY(iter->second.vertexData);
        }

        SAFE_DELETE(_meshRigidBodyData);
    }

    // Clear all temporary data stores.
    _propertiesFromFile.clear();
    _animations.clear();
    _nodeProperties.clear();
    _nodesWithMeshRB.clear();

    return scene;
}

void SceneLoader::addMeshRigidBodyData(std::string package, std::string id, Mesh* mesh, unsigned char* vertexData, unsigned int vertexByteCount)
{
    if (!_meshRigidBodyData)
    {
        WARN("Attempting to add mesh rigid body data outside of scene loading; ignoring request.");
        return;
    }

    // If the node with the mesh rigid body is renamed, we need to find the new id.
    for (unsigned int i = 0; i < _nodeProperties.size(); i++)
    {
        if (_nodeProperties[i]._type == SceneNodeProperty::URL &&
            _nodeProperties[i]._id == id)
        {
            if ((package == _path && _nodeProperties[i]._file.size() == 0) ||
                (package == _nodeProperties[i]._file))
            {
                id = _nodeProperties[i]._nodeID;
                break;
            }
        }
    }

    (*_meshRigidBodyData)[id].mesh = mesh;
    (*_meshRigidBodyData)[id].vertexData = new unsigned char[vertexByteCount];
    memcpy((*_meshRigidBodyData)[id].vertexData, vertexData, vertexByteCount);
}

void SceneLoader::addMeshRigidBodyData(std::string package, std::string id, unsigned char* indexData, unsigned int indexByteCount)
{
    if (!_meshRigidBodyData)
    {
        WARN("Attempting to add mesh rigid body data outside of scene loading; ignoring request.");
        return;
    }

    // If the node with the mesh rigid body is renamed, we need to find the new id.
    for (unsigned int i = 0; i < _nodeProperties.size(); i++)
    {
        if (_nodeProperties[i]._type == SceneNodeProperty::URL &&
            _nodeProperties[i]._id == id)
        {
            if ((package == _path && _nodeProperties[i]._file.size() == 0) ||
                (package == _nodeProperties[i]._file))
            {
                id = _nodeProperties[i]._nodeID;
                break;
            }
        }
    }

    unsigned char* indexDataCopy = new unsigned char[indexByteCount];
    memcpy(indexDataCopy, indexData, indexByteCount);
    (*_meshRigidBodyData)[id].indexData.push_back(indexDataCopy);
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

void SceneLoader::addSceneNodeProperty(SceneNodeProperty::Type type, const char* nodeID, const char* url)
{
    // Calculate the file and id from the given url.
    std::string file;
    std::string id;
    splitURL(url, &file, &id);
    
    // If there is a non-GPB file that needs to be loaded later, add an 
    // empty entry to the properties table to signify it.
    if (file.length() > 0 && file.find(".gpb") == file.npos && _propertiesFromFile.count(file) == 0)
        _propertiesFromFile[file] = NULL;

    // Add the node property to the list of node properties to be resolved later.
    _nodeProperties.push_back(SceneNodeProperty(type, nodeID, file, id));
}

void SceneLoader::applyNodeProperties(const Scene* scene, const Properties* sceneProperties)
{
    // Apply all of the remaining scene node properties except rigid body (we apply that last).
    for (unsigned int i = 0; i < _nodeProperties.size(); i++)
    {
        // If the referenced node doesn't exist in the scene, then we
        // can't do anything so we skip to the next scene node property.
        Node* node = scene->findNode(_nodeProperties[i]._nodeID);
        if (!node)
        {
            WARN_VARG("Attempting to set a property for node '%s', which does not exist in the scene.", _nodeProperties[i]._nodeID);
            continue;
        }

        if (_nodeProperties[i]._type == SceneNodeProperty::AUDIO ||
            _nodeProperties[i]._type == SceneNodeProperty::MATERIAL ||
            _nodeProperties[i]._type == SceneNodeProperty::PARTICLE ||
            _nodeProperties[i]._type == SceneNodeProperty::RIGIDBODY)
        {
            // Check to make sure the referenced properties object was loaded properly.
            Properties* p = _propertiesFromFile[_nodeProperties[i]._file];
            if (!p)
            {
                WARN_VARG("The referenced node data in file '%s' failed to load.", _nodeProperties[i]._file.c_str());
                continue;
            }

            // If a specific namespace within the file was specified, load that namespace.
            if (_nodeProperties[i]._id.size() > 0)
            {
                p = p->getNamespace(_nodeProperties[i]._id.c_str());
                if (!p)
                {
                    WARN_VARG("The referenced node data at '%s#%s' failed to load.", _nodeProperties[i]._file.c_str(), _nodeProperties[i]._id.c_str());
                    continue;
                }
            }
            else
            {
                // Otherwise, use the first namespace.
                p->rewind();
                p = p->getNextNamespace();
            }

            switch (_nodeProperties[i]._type)
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
                    WARN_VARG("Attempting to set a material on node '%s', which has no model.", _nodeProperties[i]._nodeID);
                else
                {
                    Material* material = Material::create(p);
                    node->getModel()->setMaterial(material);
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
            case SceneNodeProperty::RIGIDBODY:
                // Process this last in a separate loop to allow scale, translate, rotate to be applied first.
                break;
            default:
                // This cannot happen.
                break;
            }
        }
        else
        {
            Properties* np = sceneProperties->getNamespace(_nodeProperties[i]._nodeID);
            const char* name = NULL;

            switch (_nodeProperties[i]._type)
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
            default:
                WARN_VARG("Unsupported node property type: %d.", _nodeProperties[i]._type);
                break;
            }
        }
    }

    // Process rigid body properties.
    for (unsigned int i = 0; i < _nodeProperties.size(); i++)
    {
        if (_nodeProperties[i]._type == SceneNodeProperty::RIGIDBODY)
        {
            // If the referenced node doesn't exist in the scene, then we
            // can't do anything so we skip to the next scene node property.
            Node* node = scene->findNode(_nodeProperties[i]._nodeID);
            if (!node)
            {
                WARN_VARG("Attempting to set a property for node '%s', which does not exist in the scene.", _nodeProperties[i]._nodeID);
                continue;
            }

            // Check to make sure the referenced properties object was loaded properly.
            Properties* p = _propertiesFromFile[_nodeProperties[i]._file];
            if (!p)
            {
                WARN_VARG("The referenced node data in file '%s' failed to load.", _nodeProperties[i]._file.c_str());
                continue;
            }

            // If a specific namespace within the file was specified, load that namespace.
            if (_nodeProperties[i]._id.size() > 0)
            {
                p = p->getNamespace(_nodeProperties[i]._id.c_str());
                if (!p)
                {
                    WARN_VARG("The referenced node data at '%s#%s' failed to load.", _nodeProperties[i]._file.c_str(), _nodeProperties[i]._id.c_str());
                    continue;
                }
            }
            else
            {
                // Otherwise, use the first namespace.
                p->rewind();
                p = p->getNextNamespace();
            }

            // If the scene file specifies a rigid body model, use it for creating the rigid body.
            Properties* np = sceneProperties->getNamespace(_nodeProperties[i]._nodeID);
            const char* name = NULL;
            if (np && (name = np->getString("rigidbodymodel")))
            {
                Node* modelNode = scene->findNode(name);
                if (!modelNode)
                    WARN_VARG("Node '%s' does not exist; attempting to use its model for rigid body creation.", name);
                else
                {
                    if (!modelNode->getModel())
                        WARN_VARG("Node '%s' does not have a model; attempting to use its model for rigid body creation.", name);
                    else
                    {
                        // Set the specified model during physics rigid body creation.
                        Model* model = node->getModel();
                        node->setModel(modelNode->getModel());
                        node->setPhysicsRigidBody(p);
                        node->setModel(model);
                    }
                }
            }
            else if (!node->getModel())
                WARN_VARG("Attempting to set a rigid body on node '%s', which has no model.", _nodeProperties[i]._nodeID);
            else
                node->setPhysicsRigidBody(p);
        }
    }
}

void SceneLoader::applyNodeUrls(Scene* scene)
{
    // Apply all URL node properties so that when we go to apply
    // the other node properties, the node is in the scene.
    for (unsigned int i = 0; i < _nodeProperties.size(); )
    {
        if (_nodeProperties[i]._type == SceneNodeProperty::URL)
        {
            // Make sure that the ID we are using to insert the node into the scene with is unique.
            if (scene->findNode(_nodeProperties[i]._nodeID) != NULL)
                WARN_VARG("Attempting to insert or rename a node to an ID that already exists: ID='%s'", _nodeProperties[i]._nodeID);
            else
            {
                // If a file was specified, load the node from file and then insert it into the scene with the new ID.
                if (_nodeProperties[i]._file.size() > 0)
                {
                    Package* tmpPackage = Package::create(_nodeProperties[i]._file.c_str());
                    if (!tmpPackage)
                        WARN_VARG("Failed to load GPB file '%s' for node stitching.", _nodeProperties[i]._file.c_str());
                    else
                    {
                        bool loadWithMeshRBSupport = false;
                        for (unsigned int j = 0; j < _nodesWithMeshRB.size(); j++)
                        {
                            if (_nodeProperties[i]._id == _nodesWithMeshRB[j])
                            {
                                loadWithMeshRBSupport = true;
                                break;
                            }
                        }

                        Node* node = tmpPackage->loadNode(_nodeProperties[i]._id.c_str(), loadWithMeshRBSupport);
                        if (!node)
                            WARN_VARG("Could not load node '%s' in GPB file '%s'.", _nodeProperties[i]._id.c_str(), _nodeProperties[i]._file.c_str());
                        else
                        {
                            node->setId(_nodeProperties[i]._nodeID);
                            scene->addNode(node);
                            SAFE_RELEASE(node);
                        }
                        
                        SAFE_RELEASE(tmpPackage);
                    }
                }
                else
                {
                    // TODO: Should we do all nodes with this case first to allow users to stitch in nodes with
                    // IDs equal to IDs that were in the original GPB file but were changed in the scene file?

                    // Otherwise, the node is from the main GPB and should just be renamed.
                    Node* node = scene->findNode(_nodeProperties[i]._id.c_str());
                    if (!node)
                        WARN_VARG("Could not find node '%s' in main scene GPB file.", _nodeProperties[i]._id.c_str());
                    else
                        node->setId(_nodeProperties[i]._nodeID);
                }
            }

            // Remove the node property since we are done applying it.
            _nodeProperties.erase(_nodeProperties.begin() + i);
        }
        else
            i++;
    }
}

void SceneLoader::buildReferenceTables(Properties* sceneProperties)
{
    // Go through the child namespaces of the scene.
    Properties* ns;
    const char* name = NULL;
    while (ns = sceneProperties->getNextNamespace())
    {
        if (strcmp(ns->getNamespace(), "node") == 0)
        {
            if (strlen(ns->getId()) == 0)
            {
                WARN("Nodes must have an ID; skipping the current node.");
                continue;
            }

            while (name = ns->getNextProperty())
            {
                if (strcmp(name, "url") == 0)
                {
                    addSceneNodeProperty(SceneNodeProperty::URL, ns->getId(), ns->getString());
                }
                else if (strcmp(name, "audio") == 0)
                {
                    addSceneNodeProperty(SceneNodeProperty::AUDIO, ns->getId(), ns->getString());
                }
                else if (strcmp(name, "material") == 0)
                {
                    addSceneNodeProperty(SceneNodeProperty::MATERIAL, ns->getId(), ns->getString());
                }
                else if (strcmp(name, "particle") == 0)
                {
                    addSceneNodeProperty(SceneNodeProperty::PARTICLE, ns->getId(), ns->getString());
                }
                else if (strcmp(name, "rigidbody") == 0)
                {
                    addSceneNodeProperty(SceneNodeProperty::RIGIDBODY, ns->getId(), ns->getString());
                }
                else if (strcmp(name, "rigidbodymodel") == 0)
                {
                    // Ignore this for now. We process this when we do rigid body creation.
                }
                else if (strcmp(name, "translate") == 0)
                {
                    addSceneNodeProperty(SceneNodeProperty::TRANSLATE, ns->getId());
                }
                else if (strcmp(name, "rotate") == 0)
                {
                    addSceneNodeProperty(SceneNodeProperty::ROTATE, ns->getId());
                }
                else if (strcmp(name, "scale") == 0)
                {
                    addSceneNodeProperty(SceneNodeProperty::SCALE, ns->getId());
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
            while (animation = ns->getNextNamespace())
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
            WARN_VARG("Unsupported child namespace (of 'scene'): %s", ns->getNamespace());
        }
    }
}

void SceneLoader::calculateNodesWithMeshRigidBodies(const Properties* sceneProperties)
{
    const char* name = NULL;

    // Make a list of all nodes with triangle mesh rigid bodies.
    for (unsigned int i = 0; i < _nodeProperties.size(); i++)
    {
        if (_nodeProperties[i]._type == SceneNodeProperty::RIGIDBODY)
        {
            Properties* p = _propertiesFromFile[_nodeProperties[i]._file];
            if (p)
            {
                if (_nodeProperties[i]._id.size() > 0)
                {
                    p = p->getNamespace(_nodeProperties[i]._id.c_str());
                }
                else
                {
                    p = p->getNextNamespace();
                }

                if (p && strcmp(p->getNamespace(), "rigidbody") == 0 &&
                    strcmp(p->getString("type"), "MESH") == 0)
                {
                    // If the node specifies a rigidbodymodel, then use
                    // that node's ID; otherwise, use its ID.
                    Properties* p = sceneProperties->getNamespace(_nodeProperties[i]._nodeID);
                    if (p && (name = p->getString("rigidbodymodel")))
                        _nodesWithMeshRB.push_back(name);
                    else
                    {
                        const char* id = _nodeProperties[i]._nodeID;
                        for (unsigned int j = 0; j < _nodeProperties.size(); j++)
                        {
                            if (_nodeProperties[j]._type == SceneNodeProperty::URL &&
                                _nodeProperties[j]._nodeID == _nodeProperties[i]._nodeID)
                            {
                                id = _nodeProperties[j]._id.c_str();
                                break;
                            }
                        }
                        _nodesWithMeshRB.push_back(id);
                    }
                }
            }
        }
    }
}

void SceneLoader::createAnimations(const Scene* scene)
{
    // Create the scene animations.
    for (unsigned int i = 0; i < _animations.size(); i++)
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

        Game::getInstance()->getAnimationController()->createAnimation(_animations[i]._animationID, node, p);
    }
}

const SceneLoader::MeshRigidBodyData* SceneLoader::getMeshRigidBodyData(std::string id)
{
    if (!_meshRigidBodyData)
    {
        WARN("Attempting to get mesh rigid body data, but none has been loaded; ignoring request.");
        return NULL;
    }

    return (_meshRigidBodyData->count(id) > 0) ? &(*_meshRigidBodyData)[id] : NULL;
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
    Package* package = Package::create(_path.c_str());
    if (!package)
    {
        WARN_VARG("Failed to load scene GPB file '%s'.", _path.c_str());
        return NULL;
    }
    
    Scene* scene = package->loadScene(NULL, &_nodesWithMeshRB);
    if (!scene)
    {
        WARN_VARG("Failed to load scene from '%s'.", _path.c_str());
        SAFE_RELEASE(package);
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

    SAFE_RELEASE(package);
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
    while (constraint = physics->getNextNamespace())
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
            PhysicsRigidBody* rbA = rbANode->getPhysicsRigidBody();
            if (!rbA)
            {
                WARN_VARG("Node '%s' to be used as 'rigidBodyA' does not have a rigid body.", name);
                continue;
            }

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
                rbB = rbBNode->getPhysicsRigidBody();
                if (!rbB)
                {
                    WARN_VARG("Node '%s' to be used as 'rigidBodyB' does not have a rigid body.", name);
                    continue;
                }
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
