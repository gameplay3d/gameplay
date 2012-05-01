#include "Base.h"
#include "Bundle.h"
#include "FileSystem.h"
#include "MeshPart.h"
#include "Scene.h"
#include "Joint.h"

#define BUNDLE_VERSION_MAJOR            1
#define BUNDLE_VERSION_MINOR            2

#define BUNDLE_TYPE_SCENE               1
#define BUNDLE_TYPE_NODE                2
#define BUNDLE_TYPE_ANIMATIONS          3
#define BUNDLE_TYPE_ANIMATION           4
#define BUNDLE_TYPE_ANIMATION_CHANNEL   5
#define BUNDLE_TYPE_MODEL               10
#define BUNDLE_TYPE_MATERIAL            16
#define BUNDLE_TYPE_EFFECT              18
#define BUNDLE_TYPE_CAMERA              32
#define BUNDLE_TYPE_LIGHT               33
#define BUNDLE_TYPE_MESH                34
#define BUNDLE_TYPE_MESHPART            35
#define BUNDLE_TYPE_MESHSKIN            36
#define BUNDLE_TYPE_FONT                128

// For sanity checking string reads
#define BUNDLE_MAX_STRING_LENGTH        5000

namespace gameplay
{

static std::vector<Bundle*> __bundleCache;

Bundle::Bundle(const char* path) :
    _path(path), _referenceCount(0), _references(NULL), _file(NULL), _trackedNodes(NULL)
{
}

Bundle::~Bundle()
{
    clearLoadSession();

    // Remove this Bundle from the cache
    std::vector<Bundle*>::iterator itr = std::find(__bundleCache.begin(), __bundleCache.end(), this);
    if (itr != __bundleCache.end())
    {
        __bundleCache.erase(itr);
    }

    SAFE_DELETE_ARRAY(_references);

    if (_file)
    {
        fclose(_file);
        _file = NULL;
    }
}

template <class T>
bool Bundle::readArray(unsigned int* length, T** ptr)
{
    if (!read(length))
    {
        return false;
    }
    if (*length > 0)
    {
        *ptr = new T[*length];
        if (fread(*ptr, sizeof(T), *length, _file) != *length)
        {
            SAFE_DELETE_ARRAY(*ptr);
            return false;
        }
    }
    return true;
}

template <class T>
bool Bundle::readArray(unsigned int* length, std::vector<T>* values)
{
    if (!read(length))
    {
        return false;
    }
    if (*length > 0 && values)
    {
        values->resize(*length);
        if (fread(&(*values)[0], sizeof(T), *length, _file) != *length)
        {
            return false;
        }
    }
    return true;
}

template <class T>
bool Bundle::readArray(unsigned int* length, std::vector<T>* values, unsigned int readSize)
{
    assert(sizeof(T) >= readSize);

    if (!read(length))
    {
        return false;
    }
    if (*length > 0 && values)
    {
        values->resize(*length);
        if (fread(&(*values)[0], readSize, *length, _file) != *length)
        {
            return false;
        }
    }
    return true;
}

std::string readString(FILE* fp)
{
    unsigned int length;
    if (fread(&length, 4, 1, fp) != 1)
    {
        return std::string();
    }

    // Sanity check to detect if string length is far too big
    assert(length < BUNDLE_MAX_STRING_LENGTH);

    std::string str;
    if (length > 0)
    {
        str.resize(length);
        if (fread(&str[0], 1, length, fp) != length)
        {
            return std::string();
        }
    }
    return str;
}

Bundle* Bundle::create(const char* path)
{
    // Search the cache for this bundle
    for (unsigned int i = 0, count = __bundleCache.size(); i < count; ++i)
    {
        Bundle* p = __bundleCache[i];
        if (p->_path == path)
        {
            // Found a match
            p->addRef();
            return p;
        }
    }

    // Open the bundle
    FILE* fp = FileSystem::openFile(path, "rb");
    if (!fp)
    {
        WARN_VARG("Failed to open file: '%s'.", path);
        return NULL;
    }

    // Read the GPG header info
    char sig[9];
    if (fread(sig, 1, 9, fp) != 9 || memcmp(sig, "«GPB»\r\n\x1A\n", 9) != 0)
    {
        LOG_ERROR_VARG("Invalid bundle header: %s", path);
        fclose(fp);
        return NULL;
    }

    // Read version
    unsigned char ver[2];
    if (fread(ver, 1, 2, fp) != 2 || ver[0] != BUNDLE_VERSION_MAJOR || ver[1] != BUNDLE_VERSION_MINOR)
    {
        LOG_ERROR_VARG("Unsupported version (%d.%d) for bundle: %s (expected %d.%d)", (int)ver[0], (int)ver[1], path, BUNDLE_VERSION_MAJOR, BUNDLE_VERSION_MINOR);
        fclose(fp);
        return NULL;
    }

    // Read ref table
    unsigned int refCount;
    if (fread(&refCount, 4, 1, fp) != 1)
    {
        fclose(fp);
        return NULL;
    }

    // Read all refs
    Reference* refs = new Reference[refCount];
    for (unsigned int i = 0; i < refCount; ++i)
    {
        if ((refs[i].id = readString(fp)).empty() ||
            fread(&refs[i].type, 4, 1, fp) != 1 ||
            fread(&refs[i].offset, 4, 1, fp) != 1)
        {
            fclose(fp);
            SAFE_DELETE_ARRAY(refs);
            return NULL;
        }
    }

    // Keep file open for faster reading later
    Bundle* bundle = new Bundle(path);
    bundle->_referenceCount = refCount;
    bundle->_references = refs;
    bundle->_file = fp;

    return bundle;
}

Bundle::Reference* Bundle::find(const char* id) const
{
    // Search the ref table for the given id (case-sensitive)
    for (unsigned int i = 0; i < _referenceCount; ++i)
    {
        if (_references[i].id == id)
        {
            // Found a match
            return &_references[i];
        }
    }

    return NULL;
}

void Bundle::clearLoadSession()
{
    for (unsigned int i = 0, count = _meshSkins.size(); i < count; ++i)
    {
        SAFE_DELETE(_meshSkins[i]);
    }
    _meshSkins.clear();
}

const char* Bundle::getIdFromOffset() const
{
    return getIdFromOffset((unsigned int) ftell(_file));
}

const char* Bundle::getIdFromOffset(unsigned int offset) const
{
    // Search the ref table for the given offset
    if (offset > 0)
    {
        for (unsigned int i = 0; i < _referenceCount; ++i)
        {
            if (_references[i].offset == offset && _references[i].id.length() > 0)
            {
                return _references[i].id.c_str();
            }
        }
    }
    return NULL;
}

Bundle::Reference* Bundle::seekTo(const char* id, unsigned int type)
{
    Reference* ref = find(id);
    if (ref == NULL)
    {
        LOG_ERROR_VARG("No object with name '%s' in bundle '%s'.", id, _path.c_str());
        return NULL;
    }

    if (ref->type != type)
    {
        LOG_ERROR_VARG("Object '%s' in bundle '%s' has type %d (expected type %d).", id, _path.c_str(), (int)ref->type, (int)type);
        return NULL;
    }

    // Seek to the offset of this object
    if (fseek(_file, ref->offset, SEEK_SET) != 0)
    {
        LOG_ERROR_VARG("Failed to seek to object '%s' in bundle '%s'.", id, _path.c_str());
        return NULL;
    }

    return ref;
}

Bundle::Reference* Bundle::seekToFirstType(unsigned int type)
{
    // for each Reference
    for (unsigned int i = 0; i < _referenceCount; ++i)
    {
        Reference* ref = &_references[i];
        if (ref->type == type)
        {
            // Found a match
            if (fseek(_file, ref->offset, SEEK_SET) != 0)
            {
                LOG_ERROR_VARG("Failed to seek to object '%s' in bundle '%s'.", ref->id.c_str(), _path.c_str());
                return NULL;
            }
            return ref;
        }
    }
    return NULL;
}

bool Bundle::read(unsigned int* ptr)
{
    return fread(ptr, sizeof(unsigned int), 1, _file) == 1;
}

bool Bundle::read(unsigned char* ptr)
{
    return fread(ptr, sizeof(unsigned char), 1, _file) == 1;
}

bool Bundle::read(float* ptr)
{
    return fread(ptr, sizeof(float), 1, _file) == 1;
}

bool Bundle::readMatrix(float* m)
{
    return (fread(m, sizeof(float), 16, _file) == 16);
}

Scene* Bundle::loadScene(const char* id)
{
    clearLoadSession();

    Reference* ref = NULL;
    if (id)
    {
        ref = seekTo(id, BUNDLE_TYPE_SCENE);
    }
    else
    {
        ref = seekToFirstType(BUNDLE_TYPE_SCENE);
    }
    if (!ref)
    {
        return NULL;
    }
    Scene* scene = Scene::createScene();
    scene->setId(getIdFromOffset());

    // Read the number of children
    unsigned int childrenCount;
    if (!read(&childrenCount))
    {
        SAFE_RELEASE(scene);
        return NULL;
    }
    if (childrenCount > 0)
    {
        // Read each child directly into the scene
        for (unsigned int i = 0; i < childrenCount; i++)
        {
            Node* node = readNode(scene, NULL);
            if (node)
            {
                scene->addNode(node);
                node->release(); // scene now owns node
            }
        }
    }
    // Read active camera
    std::string xref = readString(_file);
    if (xref.length() > 1 && xref[0] == '#') // TODO: Handle full xrefs
    {
        Node* node = scene->findNode(xref.c_str() + 1, true);
        Camera* camera = node->getCamera();
        assert(camera);
        scene->setActiveCamera(camera);
    }

    // Read ambient color
    float red, blue, green;
    if (!read(&red))
    {
        SAFE_RELEASE(scene);
        LOG_ERROR_VARG("Failed to read scene ambient %s color in pakcage %s", "red", _path.c_str());
        return NULL;
    }
    if (!read(&green))
    {
        SAFE_RELEASE(scene);
        LOG_ERROR_VARG("Failed to read scene ambient %s color in pakcage %s", "green", _path.c_str());
        return NULL;
    }
    if (!read(&blue))
    {
        SAFE_RELEASE(scene);
        LOG_ERROR_VARG("Failed to read scene ambient %s color in pakcage %s", "blue", _path.c_str());
        return NULL;
    }
    scene->setAmbientColor(red, green, blue);

    // parse animations
    for (unsigned int i = 0; i < _referenceCount; ++i)
    {
        Reference* ref = &_references[i];
        if (ref->type == BUNDLE_TYPE_ANIMATIONS)
        {
            // Found a match
            if (fseek(_file, ref->offset, SEEK_SET) != 0)
            {
                LOG_ERROR_VARG("Failed to seek to object '%s' in bundle '%s'.", ref->id.c_str(), _path.c_str());
                return NULL;
            }
            readAnimations(scene);
        }
    }

    resolveJointReferences(scene, NULL);

    return scene;
}

Node* Bundle::loadNode(const char* id)
{
    return loadNode(id, NULL);
}

Node* Bundle::loadNode(const char* id, Scene* sceneContext)
{
    assert(id);

    clearLoadSession();

    // Load the node and any referenced joints with node tracking enabled.
    _trackedNodes = new std::map<std::string, Node*>();
    Node* node = loadNode(id, sceneContext, NULL);
    if (node)
        resolveJointReferences(sceneContext, node);

    // Load all animations targeting any nodes or mesh skins under this node's hierarchy.
    for (unsigned int i = 0; i < _referenceCount; i++)
    {
        Reference* ref = &_references[i];
        if (ref->type == BUNDLE_TYPE_ANIMATIONS)
        {
            if (fseek(_file, ref->offset, SEEK_SET) != 0)
            {
                LOG_ERROR_VARG("Failed to seek to object '%s' in bundle '%s'.", ref->id.c_str(), _path.c_str());
                SAFE_DELETE(_trackedNodes);
                return NULL;
            }
            
            // Read the number of animations in this object.
            unsigned int animationCount;
            if (!read(&animationCount))
            {
                LOG_ERROR_VARG("Failed to read %s for %s: %s", "animationCount", "Animations");
                SAFE_DELETE(_trackedNodes);
                return NULL;
            }

            for (unsigned int j = 0; j < animationCount; j++)
            {
                const std::string id = readString(_file);

                // Read the number of animation channels in this animation.
                unsigned int animationChannelCount;
                if (!read(&animationChannelCount))
                {
                    LOG_ERROR_VARG("Failed to read %s for %s: %s", "animationChannelCount", "animation", id.c_str());
                    SAFE_DELETE(_trackedNodes);
                    return NULL;
                }

                Animation* animation = NULL;
                for (unsigned int k = 0; k < animationChannelCount; k++)
                {
                    // read targetId
                    std::string targetId = readString(_file);
                    if (targetId.empty())
                    {
                        LOG_ERROR_VARG("Failed to read %s for %s: %s", "targetId", "animation", id.c_str());
                        SAFE_DELETE(_trackedNodes);
                        return NULL;
                    }

                    // If the target is one of the loaded nodes/joints, then load the animation.
                    std::map<std::string, Node*>::iterator iter = _trackedNodes->find(targetId);
                    if (iter != _trackedNodes->end())
                    {
                        // Read target attribute
                        unsigned int targetAttribute;
                        if (!read(&targetAttribute))
                        {
                            LOG_ERROR_VARG("Failed to read %s for %s: %s", "targetAttribute", "animation", id.c_str());
                            SAFE_DELETE(_trackedNodes);
                            return NULL;
                        }

                        AnimationTarget* target = iter->second;
                        if (!target)
                        {
                            LOG_ERROR_VARG("Failed to read %s for %s: %s", "animation target", targetId.c_str(), id.c_str());
                            SAFE_DELETE(_trackedNodes);
                            return NULL;
                        }

                        animation = readAnimationChannelData(animation, id.c_str(), target, targetAttribute);
                    }
                    else
                    {
                        // Skip the animation channel (passing a target attribute of 
                        // 0 causes the animation to not be created).
                        unsigned int data;
                        read(&data);
                        readAnimationChannelData(NULL, id.c_str(), NULL, 0);
                    }
                }
            }
        }
    }

    SAFE_DELETE(_trackedNodes);
    return node;
}

Node* Bundle::loadNode(const char* id, Scene* sceneContext, Node* nodeContext)
{
    assert(id);

    Node* node = NULL;

    // Search the passed in loading contexts (scene/node) first to see
    // if we've already loaded this node during this load session
    if (sceneContext)
    {
        node = sceneContext->findNode(id, true);
    }
    else if (nodeContext)
    {
        node = nodeContext->findNode(id, true);
    }

    if (node == NULL)
    {
        // If not yet found, search the ref table and read
        Reference* ref = seekTo(id, BUNDLE_TYPE_NODE);
        if (ref == NULL)
        {
            return NULL;
        }

        node = readNode(sceneContext, nodeContext);
    }

    return node;
}

bool Bundle::skipNode()
{
    const char* id = getIdFromOffset();

    // Skip the node's type.
    unsigned int nodeType;
    if (!read(&nodeType))
    {
        return false;
    }
    
    // Skip over the node's transform and parent ID.
    fseek(_file, sizeof(float) * 16, SEEK_CUR);
    readString(_file);

    // Skip over the node's children.
    unsigned int childrenCount;
    if (!read(&childrenCount))
    {
        return false;
    }
    else if (childrenCount > 0)
    {
        for (unsigned int i = 0; i < childrenCount; i++)
        {
            if (!skipNode())
                return false;
        }
    }
    
    // Skip over the node's camera, light, and model attachments.
    Camera* camera = readCamera(); SAFE_RELEASE(camera);
    Light* light = readLight(); SAFE_RELEASE(light);
    Model* model = readModel(id); SAFE_RELEASE(model);

    return true;
}

Node* Bundle::readNode(Scene* sceneContext, Node* nodeContext)
{
    const char* id = getIdFromOffset();

    // Read node type
    unsigned int nodeType;
    if (!read(&nodeType))
    {
        return NULL;
    }

    Node* node = NULL;
    switch (nodeType)
    {
    case Node::NODE:
        node = Node::create(id);
        break;
    case Node::JOINT:
        node = Joint::create(id);
        break;
    default:
        return NULL;
    }

    // If we are tracking nodes and it's not in the set yet, add it.
    if (_trackedNodes)
    {
        std::map<std::string, Node*>::iterator iter = _trackedNodes->find(id);
        if (iter != _trackedNodes->end())
        {
            SAFE_RELEASE(node);

            // Skip over the node's transform and parent ID.
            fseek(_file, sizeof(float) * 16, SEEK_CUR);
            readString(_file);

            // Skip over the node's children.
            unsigned int childrenCount;
            if (!read(&childrenCount))
            {
                return NULL;
            }
            else if (childrenCount > 0)
            {
                for (unsigned int i = 0; i < childrenCount; i++)
                {
                    if (!skipNode())
                        return NULL;
                }
            }

            return iter->second;
        }
        else
            _trackedNodes->insert(std::make_pair(id, node));
    }

    // If no loading context is set, set this node to the loading context
    if (sceneContext == NULL && nodeContext == NULL)
    {
        nodeContext = node;
    }

    // Read transform
    float transform[16];
    if (fread(transform, sizeof(float), 16, _file) != 16)
    {
        SAFE_RELEASE(node);
        return NULL;
    }
    setTransform(transform, node);

    // Skip over the parent ID.
    readString(_file);

    // Read children
    unsigned int childrenCount;
    if (!read(&childrenCount))
    {
        SAFE_RELEASE(node);
        return NULL;
    }
    if (childrenCount > 0)
    {
        // Read each child
        for (unsigned int i = 0; i < childrenCount; i++)
        {
            // Search the passed in loading contexts (scene/node) first to see
            // if we've already loaded this child node during this load session.
            Node* child = NULL;
            id = getIdFromOffset();

            if (sceneContext)
            {
                child = sceneContext->findNode(id, true);
            }
            else if (nodeContext)
            {
                child = nodeContext->findNode(id, true);
            }
            
            // If the child node wasn't already loaded, load it.
            if (!child)
                child = readNode(sceneContext, nodeContext);
            else
            {
                // Otherwise, skip over its data in the file.
                readNode(NULL, NULL);
            }

            if (child)
            {
                node->addChild(child);
                child->release(); // 'node' now owns this child
            }
        }
    }

    // Read camera
    Camera* camera = readCamera();
    if (camera)
    {
        node->setCamera(camera);
        SAFE_RELEASE(camera);
    }

    // Read light
    Light* light = readLight();
    if (light)
    {
        node->setLight(light);
        SAFE_RELEASE(light);
    }

    // Read model
    Model* model = readModel(node->getId());
    if (model)
    {
        node->setModel(model);
        SAFE_RELEASE(model);
    }

    return node;
}

Camera* Bundle::readCamera()
{
    unsigned char cameraType;
    if (!read(&cameraType))
    {
        LOG_ERROR_VARG("Failed to load camera type in bundle '%s'.", _path.c_str());
    }

    if (cameraType == 0)
    {
        return NULL;
    }

    // aspect ratio
    float aspectRatio;
    if (!read(&aspectRatio))
    {
        LOG_ERROR_VARG("Failed to load camera aspectRatio in bundle '%s'.", _path.c_str());
    }

    // near plane
    float nearPlane;
    if (!read(&nearPlane))
    {
        LOG_ERROR_VARG("Failed to load camera near plane in bundle '%s'.", _path.c_str());
    }

    // far plane
    float farPlane;
    if (!read(&farPlane))
    {
        LOG_ERROR_VARG("Failed to load camera far plane in bundle '%s'.", _path.c_str());
    }

    Camera* camera = NULL;
    if (cameraType == Camera::PERSPECTIVE)
    {
        // field of view
        float fieldOfView;
        if (!read(&fieldOfView))
        {
            LOG_ERROR_VARG("Failed to load camera field of view in bundle '%s'.", _path.c_str());
        }

        camera = Camera::createPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);
    }
    else if (cameraType == Camera::ORTHOGRAPHIC)
    {
        // magnification
        float zoomX;
        if (!read(&zoomX))
        {
            LOG_ERROR_VARG("Failed to load camera zoomX in bundle '%s'.", _path.c_str());
        }

        float zoomY;
        if (!read(&zoomY))
        {
            LOG_ERROR_VARG("Failed to load camera zoomY in bundle '%s'.", _path.c_str());
        }

        camera = Camera::createOrthographic(zoomX, zoomY, aspectRatio, nearPlane, farPlane);
    }
    else
    {
        LOG_ERROR_VARG("Failed to load camera type in bundle '%s'. Invalid camera type.", _path.c_str());
    }
    return camera;
}

Light* Bundle::readLight()
{
    unsigned char type;
    if (!read(&type))
    {
        LOG_ERROR_VARG("Failed to load light %s in bundle '%s'.", "type", _path.c_str());
    }

    if (type == 0)
    {
        return NULL;
    }

    // read color
    float red, blue, green;
    if (!read(&red) || !read(&blue) || !read(&green))
    {
        LOG_ERROR_VARG("Failed to load light %s in bundle '%s'.", "color", _path.c_str());
    }
    Vector3 color(red, blue, green);

    Light* light = NULL;
    if (type == Light::DIRECTIONAL)
    {
        light = Light::createDirectional(color);
    }
    else if (type == Light::POINT)
    {
        float range;
        if (!read(&range))
        {
            LOG_ERROR_VARG("Failed to load point light %s in bundle '%s'.", "point", _path.c_str());
        }
        light = Light::createPoint(color, range);
    }
    else if (type == Light::SPOT)
    {
        float range, innerAngle, outerAngle;
        if (!read(&range) || !read(&innerAngle) || !read(&outerAngle))
        {
            LOG_ERROR_VARG("Failed to load spot light %s in bundle '%s'.", "spot", _path.c_str());
        }
        light = Light::createSpot(color, range, innerAngle, outerAngle);
    }
    else
    {
        LOG_ERROR_VARG("Failed to load light %s in bundle '%s'.", "type", _path.c_str());
    }
    return light;
}

Model* Bundle::readModel(const char* nodeId)
{
    // Read mesh
    Mesh* mesh = NULL;
    std::string xref = readString(_file);
    if (xref.length() > 1 && xref[0] == '#') // TODO: Handle full xrefs
    {
        mesh = loadMesh(xref.c_str() + 1, nodeId);
        if (mesh)
        {
            Model* model = Model::create(mesh);
            SAFE_RELEASE(mesh);

            // Read skin
            unsigned char hasSkin;
            if (!read(&hasSkin))
            {
                LOG_ERROR_VARG("Failed to load hasSkin in bundle '%s'.", _path.c_str());
                return NULL;
            }
            if (hasSkin)
            {
                MeshSkin* skin = readMeshSkin();
                if (skin)
                {
                    model->setSkin(skin);
                }
            }
            // Read material
            unsigned int materialCount;
            if (!read(&materialCount))
            {
                LOG_ERROR_VARG("Failed to load materialCount in bundle '%s'.", _path.c_str());
                return NULL;
            }
            if (materialCount > 0)
            {
                // TODO: Material loading not supported yet
            }
            return model;
        }
    }

    return NULL;
}

MeshSkin* Bundle::readMeshSkin()
{
    MeshSkin* meshSkin = new MeshSkin();

    // Read bindShape
    float bindShape[16];
    if (!readMatrix(bindShape))
    {
        LOG_ERROR_VARG("Failed to load MeshSkin in bundle '%s'.", _path.c_str());
        SAFE_DELETE(meshSkin);
        return NULL;
    }
    meshSkin->setBindShape(bindShape);

    MeshSkinData* skinData = new MeshSkinData();
    skinData->skin = meshSkin;

    // Read joint count
    unsigned int jointCount;
    if (!read(&jointCount))
    {
        LOG_ERROR_VARG("Failed to load MeshSkin in bundle '%s'.", _path.c_str());
        SAFE_DELETE(meshSkin);
        SAFE_DELETE(skinData);
        return NULL;
    }
    if (jointCount == 0)
    {
        SAFE_DELETE(meshSkin);
        SAFE_DELETE(skinData);
        return NULL;
    }
    meshSkin->setJointCount(jointCount);

    // Read joint xref strings for all joints in the list
    for (unsigned int i = 0; i < jointCount; i++)
    {
        skinData->joints.push_back(readString(_file));
    }

    // read bindposes
    unsigned int jointsBindPosesCount;
    if (!read(&jointsBindPosesCount))
    {
        LOG_ERROR_VARG("Failed to load MeshSkin in bundle '%s'.", _path.c_str());
        SAFE_DELETE(meshSkin);
        SAFE_DELETE(skinData);
        return NULL;
    }
    if (jointsBindPosesCount > 0)
    {
        assert(jointCount * 16 == jointsBindPosesCount);
        float m[16];
        for (unsigned int i = 0; i < jointCount; i++)
        {
            if (!readMatrix(m))
            {
                LOG_ERROR_VARG("Failed to load MeshSkin in bundle '%s'.", _path.c_str());
                SAFE_DELETE(meshSkin);
                SAFE_DELETE(skinData);
                return NULL;
            }
            skinData->inverseBindPoseMatrices.push_back(m);
        }
    }

    // Store the MeshSkinData so we can go back and resolve all joint references later
    _meshSkins.push_back(skinData);

    return meshSkin;
}

void Bundle::resolveJointReferences(Scene* sceneContext, Node* nodeContext)
{
    const unsigned int skinCount = _meshSkins.size();
    for (unsigned int i = 0; i < skinCount; ++i)
    {
        MeshSkinData* skinData = _meshSkins[i];

        // Resolve all joints in skin joint list
        const unsigned int jointCount = skinData->joints.size();
        for (unsigned int j = 0; j < jointCount; ++j)
        {
            // TODO: Handle full xrefs (not just local # xrefs)
            std::string jointId = skinData->joints[j];
            if (jointId.length() > 1 && jointId[0] == '#')
            {
                jointId = jointId.substr(1, jointId.length() - 1);

                Node* n = loadNode(jointId.c_str(), sceneContext, nodeContext);
                if (n && n->getType() == Node::JOINT)
                {
                    Joint* joint = static_cast<Joint*>(n);
                    joint->setInverseBindPose(skinData->inverseBindPoseMatrices[j]);
                    skinData->skin->setJoint(joint, j);
                }
            }
        }

        // Set the root joint
        if (jointCount > 0)
        {
            Joint* rootJoint = skinData->skin->getJoint((unsigned int)0);
            Node* node = rootJoint;
            Node* parent = node->getParent();
            
            while (true)
            {
                if (parent)
                {
                    if (skinData->skin->getJointIndex(static_cast<Joint*>(parent)) != -1)
                    {
                        // Parent is a joint in the MeshSkin, so treat it as the new root
                        rootJoint = static_cast<Joint*>(parent);
                    }

                    node = parent;
                    parent = node->getParent();
                }
                else
                {
                    std::string nodeID = node->getId();

                    while (true)
                    {
                        // Get the node's type.
                        Reference* ref = find(nodeID.c_str());
                        if (ref == NULL)
                        {
                            LOG_ERROR_VARG("No object with name '%s' in bundle '%s'.", nodeID.c_str(), _path.c_str());
                            break;
                        }

                        // Seek to the current node in the file so we can get it's parent ID.
                        seekTo(nodeID.c_str(), ref->type);

                        // Skip over the node type (1 unsigned int) and transform (16 floats) and read the parent id.
                        fseek(_file, sizeof(unsigned int) + sizeof(float)*16, SEEK_CUR);
                        std::string parentID = readString(_file);
                        
                        if (parentID.size() > 0)
                            nodeID = parentID;
                        else
                            break;
                    }

                    if (nodeID != rootJoint->getId())
                        loadNode(nodeID.c_str(), sceneContext, nodeContext);

                    break;
                }
            }

            skinData->skin->setRootJoint(rootJoint);
        }

        // Remove the joint hierarchy from the scene since it is owned by the mesh skin.
        if (sceneContext)
            sceneContext->removeNode(skinData->skin->_rootNode);

        // Done with this MeshSkinData entry
        SAFE_DELETE(_meshSkins[i]);
    }
    _meshSkins.clear();
}

void Bundle::readAnimation(Scene* scene)
{
    const std::string animationId = readString(_file);

    // read the number of animation channels in this animation
    unsigned int animationChannelCount;
    if (!read(&animationChannelCount))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "animationChannelCount", "animation", animationId.c_str());
        return;
    }

    Animation* animation = NULL;

    for (unsigned int i = 0; i < animationChannelCount; i++)
    {
        animation = readAnimationChannel(scene, animation, animationId.c_str());
    }
}

void Bundle::readAnimations(Scene* scene)
{
    // read the number of animations in this object
    unsigned int animationCount;
    if (!read(&animationCount))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "animationCount", "Animations");
        return;
    }

    for (unsigned int i = 0; i < animationCount; i++)
    {
        readAnimation(scene);
    }
}

Animation* Bundle::readAnimationChannel(Scene* scene, Animation* animation, const char* animationId)
{
    const char* id = animationId;

    // read targetId
    std::string targetId = readString(_file);
    if (targetId.empty())
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "targetId", "animation", id);
        return NULL;
    }

    // read target attribute
    unsigned int targetAttribute;
    if (!read(&targetAttribute))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "targetAttribute", "animation", id);
        return NULL;
    }

    //long position = ftell(_file);
    //fseek(_file, position, SEEK_SET);

    AnimationTarget* target = NULL;

    // Search for a node that matches target
    if (!target)
    {
        target = scene->findNode(targetId.c_str());
        if (!target)
        {
            LOG_ERROR_VARG("Failed to read %s for %s: %s", "animation target", targetId.c_str(), id);
            return NULL;
        }
    }

    return readAnimationChannelData(animation, animationId, target, targetAttribute);
}

Animation* Bundle::readAnimationChannelData(Animation* animation, const char* id, AnimationTarget* target, unsigned int targetAttribute)
{
    std::vector<unsigned long> keyTimes;
    std::vector<float> values;
    std::vector<float> tangentsIn;
    std::vector<float> tangentsOut;
    std::vector<unsigned long> interpolation;

    // length of the arrays
    unsigned int keyTimesCount;
    unsigned int valuesCount;
    unsigned int tangentsInCount;
    unsigned int tangentsOutCount;
    unsigned int interpolationCount;

    // read key times
    if (!readArray(&keyTimesCount, &keyTimes, sizeof(unsigned int)))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "keyTimes", "animation", id);
        return NULL;
    }
    
    // read key values
    if (!readArray(&valuesCount, &values))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "values", "animation", id);
        return NULL;
    }
    
    // read tangentsIn
    if (!readArray(&tangentsInCount, &tangentsIn))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "tangentsIn", "animation", id);
        return NULL;
    }
    
    // read tangent_out
    if (!readArray(&tangentsOutCount, &tangentsOut))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "tangentsOut", "animation", id);
        return NULL;
    }
    
    // read interpolations
    if (!readArray(&interpolationCount, &interpolation, sizeof(unsigned int)))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "interpolation", "animation", id);
        return NULL;
    }

    Game* game = Game::getInstance();
    AnimationController* controller = game->getAnimationController();

    // TODO: Handle other target attributes later.
    if (targetAttribute > 0)
    {
        assert(keyTimes.size() > 0 && values.size() > 0);
        if (animation == NULL)
        {
            // TODO: This code currently assumes LINEAR only
            animation = target->createAnimation(id, targetAttribute, keyTimesCount, &keyTimes[0], &values[0], Curve::LINEAR);
        }
        else
        {
            animation->createChannel(target, targetAttribute, keyTimesCount, &keyTimes[0], &values[0], Curve::LINEAR);
        }
    }

    return animation;
}

Mesh* Bundle::loadMesh(const char* id)
{
    return loadMesh(id, false);
}

Mesh* Bundle::loadMesh(const char* id, const char* nodeId)
{
    // Save the file position
    long position = ftell(_file);

    // Seek to the specified Mesh
    Reference* ref = seekTo(id, BUNDLE_TYPE_MESH);
    if (ref == NULL)
    {
        return NULL;
    }

    // Read mesh data
    MeshData* meshData = readMeshData();
    if (meshData == NULL)
    {
        return NULL;
    }

    // Create Mesh
    Mesh* mesh = Mesh::createMesh(meshData->vertexFormat, meshData->vertexCount, false);
    if (mesh == NULL)
    {
        LOG_ERROR_VARG("Failed to create mesh: %s", id);
        SAFE_DELETE_ARRAY(meshData);
        return NULL;
    }

    mesh->_url = _path;
    mesh->_url += "#";
    mesh->_url += id;

    mesh->setVertexData(meshData->vertexData, 0, meshData->vertexCount);

    mesh->_boundingBox.set(meshData->boundingBox);
    mesh->_boundingSphere.set(meshData->boundingSphere);

    // Create mesh parts
    for (unsigned int i = 0; i < meshData->parts.size(); ++i)
    {
        MeshPartData* partData = meshData->parts[i];

        MeshPart* part = mesh->addPart(partData->primitiveType, partData->indexFormat, partData->indexCount, false);
        if (part == NULL)
        {
            LOG_ERROR_VARG("Failed to create mesh part (i=%d): %s", i, id);
            SAFE_DELETE(meshData);
            return NULL;
        }
        part->setIndexData(partData->indexData, 0, partData->indexCount);
    }

    SAFE_DELETE(meshData);

    // Restore file pointer
    fseek(_file, position, SEEK_SET);

    return mesh;
}

Bundle::MeshData* Bundle::readMeshData()
{
    // Read vertex format/elements
    unsigned int vertexElementCount;
    if (fread(&vertexElementCount, 4, 1, _file) != 1 || vertexElementCount < 1)
    {
        return NULL;
    }
    VertexFormat::Element* vertexElements = new VertexFormat::Element[vertexElementCount];
    for (unsigned int i = 0; i < vertexElementCount; ++i)
    {
        unsigned int vUsage, vSize;
        if (fread(&vUsage, 4, 1, _file) != 1 || fread(&vSize, 4, 1, _file) != 1)
        {
            SAFE_DELETE_ARRAY(vertexElements);
            return NULL;
        }

        vertexElements[i].usage = (VertexFormat::Usage)vUsage;
        vertexElements[i].size = vSize;
    }

    MeshData* meshData = new MeshData(VertexFormat(vertexElements, vertexElementCount));

    SAFE_DELETE_ARRAY(vertexElements);

    // Read vertex data
    unsigned int vertexByteCount;
    if (fread(&vertexByteCount, 4, 1, _file) != 1 || vertexByteCount == 0)
    {
        SAFE_DELETE(meshData);
        return NULL;
    }
    meshData->vertexCount = vertexByteCount / meshData->vertexFormat.getVertexSize();
    meshData->vertexData = new unsigned char[vertexByteCount];
    if (fread(meshData->vertexData, 1, vertexByteCount, _file) != vertexByteCount)
    {
        SAFE_DELETE(meshData);
        return NULL;
    }

    // Read mesh bounds (bounding box and bounding sphere)
    if (fread(&meshData->boundingBox.min.x, 4, 3, _file) != 3 || fread(&meshData->boundingBox.max.x, 4, 3, _file) != 3)
    {
        SAFE_DELETE(meshData);
        return NULL;
    }
    if (fread(&meshData->boundingSphere.center.x, 4, 3, _file) != 3 || fread(&meshData->boundingSphere.radius, 4, 1, _file) != 1)
    {
        SAFE_DELETE(meshData);
        return NULL;
    }

    // Read mesh parts
    unsigned int meshPartCount;
    if (fread(&meshPartCount, 4, 1, _file) != 1)
    {
        SAFE_DELETE(meshData);
        return NULL;
    }
    for (unsigned int i = 0; i < meshPartCount; ++i)
    {
        // Read primitive type, index format and index count
        unsigned int pType, iFormat, iByteCount;
        if (fread(&pType, 4, 1, _file) != 1 ||
            fread(&iFormat, 4, 1, _file) != 1 ||
            fread(&iByteCount, 4, 1, _file) != 1)
        {
            SAFE_DELETE(meshData);
            return NULL;
        }

        MeshPartData* partData = new MeshPartData();
        meshData->parts.push_back(partData);

        partData->primitiveType = (Mesh::PrimitiveType)pType;
        partData->indexFormat = (Mesh::IndexFormat)iFormat;
        
        unsigned int indexSize = 0;
        switch (partData->indexFormat)
        {
        case Mesh::INDEX8:
            indexSize = 1;
            break;
        case Mesh::INDEX16:
            indexSize = 2;
            break;
        case Mesh::INDEX32:
            indexSize = 4;
            break;
        }

        partData->indexCount = iByteCount / indexSize;

        partData->indexData = new unsigned char[iByteCount];
        if (fread(partData->indexData, 1, iByteCount, _file) != iByteCount)
        {
            SAFE_DELETE(meshData);
            return NULL;
        }
    }

    return meshData;
}

Bundle::MeshData* Bundle::readMeshData(const char* url)
{
    assert(url);

    unsigned int len = strlen(url);
    if (len == 0)
        return NULL;

    // Parse URL (formatted as 'bundle#id')
    std::string urlstring(url);
    unsigned int pos = urlstring.find('#');
    if (pos == std::string::npos)
        return NULL;

    std::string file = urlstring.substr(0, pos);
    std::string id = urlstring.substr(pos + 1);

    // Load bundle
    Bundle* bundle = Bundle::create(file.c_str());
    if (bundle == NULL)
        return NULL;

    // Seek to mesh with specified ID in bundle
    Reference* ref = bundle->seekTo(id.c_str(), BUNDLE_TYPE_MESH);
    if (ref == NULL)
        return NULL;

    // Read mesh data from current file position
    MeshData* meshData = bundle->readMeshData();

    SAFE_RELEASE(bundle);

    return meshData;
}

Font* Bundle::loadFont(const char* id)
{
    // Seek to the specified Font
    Reference* ref = seekTo(id, BUNDLE_TYPE_FONT);
    if (ref == NULL)
    {
        return NULL;
    }

    // Read font family
    std::string family = readString(_file);
    if (family.empty())
    {
        LOG_ERROR_VARG("Failed to read font family for font: %s", id);
        return NULL;
    }

    // Read font style and size
    unsigned int style, size;
    if (fread(&style, 4, 1, _file) != 1 ||
        fread(&size, 4, 1, _file) != 1)
    {
        LOG_ERROR_VARG("Failed to read style and/or size for font: %s", id);
        return NULL;
    }

    // Read character set
    std::string charset = readString(_file);

    // Read font glyphs
    unsigned int glyphCount;
    if (fread(&glyphCount, 4, 1, _file) != 1 || glyphCount == 0)
    {
        LOG_ERROR_VARG("Failed to read glyph count for font: %s", id);
        return NULL;
    }
    Font::Glyph* glyphs = new Font::Glyph[glyphCount];
    if (fread(glyphs, sizeof(Font::Glyph), glyphCount, _file) != glyphCount)
    {
        LOG_ERROR_VARG("Failed to read %d glyphs for font: %s", glyphCount, id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }

    // Read texture
    unsigned int width, height, textureByteCount;
    if (fread(&width, 4, 1, _file) != 1 ||
        fread(&height, 4, 1, _file) != 1 ||
        fread(&textureByteCount, 4, 1, _file) != 1)
    {
        LOG_ERROR_VARG("Failed to read texture attributes for font: %s", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }
    if (textureByteCount != (width * height))
    {
        LOG_ERROR_VARG("Invalid texture byte for font: %s", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }
    unsigned char* textureData = new unsigned char[textureByteCount];
    if (fread(textureData, 1, textureByteCount, _file) != textureByteCount)
    {
        LOG_ERROR_VARG("Failed to read %d texture bytes for font: %s", textureByteCount, id);
        SAFE_DELETE_ARRAY(glyphs);
        SAFE_DELETE_ARRAY(textureData);
        return NULL;
    }

    // Load the texture for the font
    Texture* texture = Texture::create(Texture::ALPHA, width, height, textureData, true);

    // Free the texture data (no longer needed)
    SAFE_DELETE_ARRAY(textureData);

    if (texture == NULL)
    {
        LOG_ERROR_VARG("Failed to create texture for font: %s", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }

    // Create the font
    Font* font = Font::create(family.c_str(), Font::PLAIN, size, glyphs, glyphCount, texture);

    // Free the glyph array
    SAFE_DELETE_ARRAY(glyphs);

    // Release the texture since the Font now owns it
    SAFE_RELEASE(texture);

    if (font)
    {
        font->_path = _path;
        font->_id = id;
    }

    return font;
}

void Bundle::setTransform(const float* values, Transform* transform)
{
    // Load array into transform
    Matrix matrix(values);
    Vector3 scale, translation;
    Quaternion rotation;
    matrix.decompose(&scale, &rotation, &translation);
    transform->setScale(scale);
    transform->setTranslation(translation);
    transform->setRotation(rotation);
}

bool Bundle::contains(const char* id) const
{
    return (find(id) != NULL);
}

unsigned int Bundle::getObjectCount() const
{
    return _referenceCount;
}

const char* Bundle::getObjectID(unsigned int index) const
{
    return (index >= _referenceCount ? NULL : _references[index].id.c_str());
}

Bundle::Reference::Reference()
    : type(0), offset(0)
{
}

Bundle::Reference::~Reference()
{
}

Bundle::MeshPartData::MeshPartData() :
    indexCount(0), indexData(NULL)
{
}

Bundle::MeshPartData::~MeshPartData()
{
    SAFE_DELETE_ARRAY(indexData);
}

Bundle::MeshData::MeshData(const VertexFormat& vertexFormat)
    : vertexFormat(vertexFormat), vertexCount(0), vertexData(NULL)
{
}

Bundle::MeshData::~MeshData()
{
    SAFE_DELETE_ARRAY(vertexData);

    for (unsigned int i = 0; i < parts.size(); ++i)
    {
        SAFE_DELETE(parts[i]);
    }
}

}
