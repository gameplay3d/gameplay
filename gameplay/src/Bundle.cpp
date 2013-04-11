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
    _path(path), _referenceCount(0), _references(NULL), _stream(NULL), _trackedNodes(NULL)
{
}

Bundle::~Bundle()
{
    clearLoadSession();

    // Remove this Bundle from the cache.
    std::vector<Bundle*>::iterator itr = std::find(__bundleCache.begin(), __bundleCache.end(), this);
    if (itr != __bundleCache.end())
    {
        __bundleCache.erase(itr);
    }

    SAFE_DELETE_ARRAY(_references);

    if (_stream)
    {
        SAFE_DELETE(_stream);
    }
}

template <class T>
bool Bundle::readArray(unsigned int* length, T** ptr)
{
    GP_ASSERT(length);
    GP_ASSERT(ptr);
    GP_ASSERT(_stream);

    if (!read(length))
    {
        GP_ERROR("Failed to read the length of an array of data (to be read into an array).");
        return false;
    }
    if (*length > 0)
    {
        *ptr = new T[*length];
        if (_stream->read(*ptr, sizeof(T), *length) != *length)
        {
            GP_ERROR("Failed to read an array of data from bundle (into an array).");
            SAFE_DELETE_ARRAY(*ptr);
            return false;
        }
    }
    return true;
}

template <class T>
bool Bundle::readArray(unsigned int* length, std::vector<T>* values)
{
    GP_ASSERT(length);
    GP_ASSERT(_stream);

    if (!read(length))
    {
        GP_ERROR("Failed to read the length of an array of data (to be read into a std::vector).");
        return false;
    }
    if (*length > 0 && values)
    {
        values->resize(*length);
        if (_stream->read(&(*values)[0], sizeof(T), *length) != *length)
        {
            GP_ERROR("Failed to read an array of data from bundle (into a std::vector).");
            return false;
        }
    }
    return true;
}

template <class T>
bool Bundle::readArray(unsigned int* length, std::vector<T>* values, unsigned int readSize)
{
    GP_ASSERT(length);
    GP_ASSERT(_stream);
    GP_ASSERT(sizeof(T) >= readSize);

    if (!read(length))
    {
        GP_ERROR("Failed to read the length of an array of data (to be read into a std::vector with a specified single element read size).");
        return false;
    }
    if (*length > 0 && values)
    {
        values->resize(*length);
        if (_stream->read(&(*values)[0], readSize, *length) != *length)
        {
            GP_ERROR("Failed to read an array of data from bundle (into a std::vector with a specified single element read size).");
            return false;
        }
    }
    return true;
}

static std::string readString(Stream* stream)
{
    GP_ASSERT(stream);

    unsigned int length;
    if (stream->read(&length, 4, 1) != 1)
    {
        GP_ERROR("Failed to read the length of a string from a bundle.");
        return std::string();
    }

    // Sanity check to detect if string length is far too big.
    GP_ASSERT(length < BUNDLE_MAX_STRING_LENGTH);

    std::string str;
    if (length > 0)
    {
        str.resize(length);
        if (stream->read(&str[0], 1, length) != length)
        {
            GP_ERROR("Failed to read string from bundle.");
            return std::string();
        }
    }
    return str;
}

Bundle* Bundle::create(const char* path)
{
    GP_ASSERT(path);

    // Search the cache for this bundle.
    for (size_t i = 0, count = __bundleCache.size(); i < count; ++i)
    {
        Bundle* p = __bundleCache[i];
        GP_ASSERT(p);
        if (p->_path == path)
        {
            // Found a match
            p->addRef();
            return p;
        }
    }

    // Open the bundle.
    Stream* stream = FileSystem::open(path);
    if (!stream)
    {
        GP_ERROR("Failed to open file '%s'.", path);
        return NULL;
    }

    // Read the GPB header info.
    char sig[9];
    if (stream->read(sig, 1, 9) != 9 || memcmp(sig, "\xABGPB\xBB\r\n\x1A\n", 9) != 0)
    {
        SAFE_DELETE(stream);
        GP_ERROR("Invalid GPB header for bundle '%s'.", path);
        return NULL;
    }

    // Read version.
    unsigned char ver[2];
    if (stream->read(ver, 1, 2) != 2)
    {
        SAFE_DELETE(stream);
        GP_ERROR("Failed to read GPB version for bundle '%s'.", path);
        return NULL;
    }
    if (ver[0] != BUNDLE_VERSION_MAJOR || ver[1] != BUNDLE_VERSION_MINOR)
    {
        SAFE_DELETE(stream);
        GP_ERROR("Unsupported version (%d.%d) for bundle '%s' (expected %d.%d).", (int)ver[0], (int)ver[1], path, BUNDLE_VERSION_MAJOR, BUNDLE_VERSION_MINOR);
        return NULL;
    }

    // Read ref table.
    unsigned int refCount;
    if (stream->read(&refCount, 4, 1) != 1)
    {
        SAFE_DELETE(stream);
        GP_ERROR("Failed to read ref table for bundle '%s'.", path);
        return NULL;
    }

    // Read all refs.
    Reference* refs = new Reference[refCount];
    for (unsigned int i = 0; i < refCount; ++i)
    {
        if ((refs[i].id = readString(stream)).empty() ||
            stream->read(&refs[i].type, 4, 1) != 1 ||
            stream->read(&refs[i].offset, 4, 1) != 1)
        {
            SAFE_DELETE(stream);
            GP_ERROR("Failed to read ref number %d for bundle '%s'.", i, path);
            SAFE_DELETE_ARRAY(refs);
            return NULL;
        }
    }

    // Keep file open for faster reading later.
    Bundle* bundle = new Bundle(path);
    bundle->_referenceCount = refCount;
    bundle->_references = refs;
    bundle->_stream = stream;

    return bundle;
}

Bundle::Reference* Bundle::find(const char* id) const
{
    GP_ASSERT(id);
    GP_ASSERT(_references);

    // Search the ref table for the given id (case-sensitive).
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
    for (size_t i = 0, count = _meshSkins.size(); i < count; ++i)
    {
        SAFE_DELETE(_meshSkins[i]);
    }
    _meshSkins.clear();
}

const char* Bundle::getIdFromOffset() const
{
    GP_ASSERT(_stream);
    return getIdFromOffset((unsigned int) _stream->position());
}

const char* Bundle::getIdFromOffset(unsigned int offset) const
{
    // Search the ref table for the given offset.
    if (offset > 0)
    {
        GP_ASSERT(_references);
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

const std::string& Bundle::getMaterialPath()
{
    if (_materialPath.empty())
    {
        int pos = _path.find_last_of('.');
        if (pos > 2)
        {
            _materialPath = _path.substr(0, pos);
            _materialPath.append(".material");
            if (!FileSystem::fileExists(_materialPath.c_str()))
            {
                _materialPath.clear();
            }
        }
    }
    return _materialPath;
}

Bundle::Reference* Bundle::seekTo(const char* id, unsigned int type)
{
    Reference* ref = find(id);
    if (ref == NULL)
    {
        GP_ERROR("No object with name '%s' in bundle '%s'.", id, _path.c_str());
        return NULL;
    }

    if (ref->type != type)
    {
        GP_ERROR("Object '%s' in bundle '%s' has type %d (expected type %d).", id, _path.c_str(), (int)ref->type, (int)type);
        return NULL;
    }

    // Seek to the offset of this object.
    GP_ASSERT(_stream);
    if (_stream->seek(ref->offset, SEEK_SET) == false)
    {
        GP_ERROR("Failed to seek to object '%s' in bundle '%s'.", id, _path.c_str());
        return NULL;
    }

    return ref;
}

Bundle::Reference* Bundle::seekToFirstType(unsigned int type)
{
    GP_ASSERT(_references);
    GP_ASSERT(_stream);

    for (unsigned int i = 0; i < _referenceCount; ++i)
    {
        Reference* ref = &_references[i];
        if (ref->type == type)
        {
            // Found a match.
            if (_stream->seek(ref->offset, SEEK_SET) == false)
            {
                GP_ERROR("Failed to seek to object '%s' in bundle '%s'.", ref->id.c_str(), _path.c_str());
                return NULL;
            }
            return ref;
        }
    }
    return NULL;
}

bool Bundle::read(unsigned int* ptr)
{
    return _stream->read(ptr, sizeof(unsigned int), 1) == 1;
}

bool Bundle::read(unsigned char* ptr)
{
    return _stream->read(ptr, sizeof(unsigned char), 1) == 1;
}

bool Bundle::read(float* ptr)
{
    return _stream->read(ptr, sizeof(float), 1) == 1;
}

bool Bundle::readMatrix(float* m)
{
    return _stream->read(m, sizeof(float), 16) == 16;
}

Scene* Bundle::loadScene(const char* id)
{
    clearLoadSession();

    Reference* ref = NULL;
    if (id)
    {
        ref = seekTo(id, BUNDLE_TYPE_SCENE);
        if (!ref)
        {
            GP_ERROR("Failed to load scene with id '%s' from bundle.", id);
            return NULL;
        }
    }
    else
    {
        ref = seekToFirstType(BUNDLE_TYPE_SCENE);
        if (!ref)
        {
            GP_ERROR("Failed to load scene from bundle; bundle contains no scene objects.");
            return NULL;
        }
    }

    Scene* scene = Scene::create(getIdFromOffset());

    // Read the number of children.
    unsigned int childrenCount;
    if (!read(&childrenCount))
    {
        GP_ERROR("Failed to read the scene's number of children.");
        SAFE_RELEASE(scene);
        return NULL;
    }
    if (childrenCount > 0)
    {
        // Read each child directly into the scene.
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
    // Read active camera.
    std::string xref = readString(_stream);
    if (xref.length() > 1 && xref[0] == '#') // TODO: Handle full xrefs
    {
        Node* node = scene->findNode(xref.c_str() + 1, true);
        GP_ASSERT(node);
        Camera* camera = node->getCamera();
        GP_ASSERT(camera);
        scene->setActiveCamera(camera);
    }

    // Read ambient color.
    float red, blue, green;
    if (!read(&red))
    {
        GP_ERROR("Failed to read red component of the scene's ambient color in bundle '%s'.", _path.c_str());
        SAFE_RELEASE(scene);
        return NULL;
    }
    if (!read(&green))
    {
        GP_ERROR("Failed to read green component of the scene's ambient color in bundle '%s'.", _path.c_str());
        SAFE_RELEASE(scene);
        return NULL;
    }
    if (!read(&blue))
    {
        GP_ERROR("Failed to read blue component of the scene's ambient color in bundle '%s'.", _path.c_str());
        SAFE_RELEASE(scene);
        return NULL;
    }
    scene->setAmbientColor(red, green, blue);

    // Parse animations.
    GP_ASSERT(_references);
    GP_ASSERT(_stream);
    for (unsigned int i = 0; i < _referenceCount; ++i)
    {
        Reference* ref = &_references[i];
        if (ref->type == BUNDLE_TYPE_ANIMATIONS)
        {
            // Found a match.
            if (_stream->seek(ref->offset, SEEK_SET) == false)
            {
                GP_ERROR("Failed to seek to object '%s' in bundle '%s'.", ref->id.c_str(), _path.c_str());
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
    GP_ASSERT(id);
    GP_ASSERT(_references);
    GP_ASSERT(_stream);

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
            if (_stream->seek(ref->offset, SEEK_SET) == false)
            {
                GP_ERROR("Failed to seek to object '%s' in bundle '%s'.", ref->id.c_str(), _path.c_str());
                SAFE_DELETE(_trackedNodes);
                return NULL;
            }

            // Read the number of animations in this object.
            unsigned int animationCount;
            if (!read(&animationCount))
            {
                GP_ERROR("Failed to read the number of animations for object '%s'.", ref->id.c_str());
                SAFE_DELETE(_trackedNodes);
                return NULL;
            }

            for (unsigned int j = 0; j < animationCount; j++)
            {
                const std::string id = readString(_stream);

                // Read the number of animation channels in this animation.
                unsigned int animationChannelCount;
                if (!read(&animationChannelCount))
                {
                    GP_ERROR("Failed to read the number of animation channels for animation '%s'.", "animationChannelCount", id.c_str());
                    SAFE_DELETE(_trackedNodes);
                    return NULL;
                }

                Animation* animation = NULL;
                for (unsigned int k = 0; k < animationChannelCount; k++)
                {
                    // Read target id.
                    std::string targetId = readString(_stream);
                    if (targetId.empty())
                    {
                        GP_ERROR("Failed to read target id for animation '%s'.", id.c_str());
                        SAFE_DELETE(_trackedNodes);
                        return NULL;
                    }

                    // If the target is one of the loaded nodes/joints, then load the animation.
                    std::map<std::string, Node*>::iterator iter = _trackedNodes->find(targetId);
                    if (iter != _trackedNodes->end())
                    {
                        // Read target attribute.
                        unsigned int targetAttribute;
                        if (!read(&targetAttribute))
                        {
                            GP_ERROR("Failed to read target attribute for animation '%s'.", id.c_str());
                            SAFE_DELETE(_trackedNodes);
                            return NULL;
                        }

                        AnimationTarget* target = iter->second;
                        if (!target)
                        {
                            GP_ERROR("Failed to read %s for %s: %s", "animation target", targetId.c_str(), id.c_str());
                            SAFE_DELETE(_trackedNodes);
                            return NULL;
                        }

                        animation = readAnimationChannelData(animation, id.c_str(), target, targetAttribute);
                    }
                    else
                    {
                        // Skip over the target attribute.
                        unsigned int data;
                        if (!read(&data))
                        {
                            GP_ERROR("Failed to skip over target attribute for animation '%s'.", id.c_str());
                            SAFE_DELETE(_trackedNodes);
                            return NULL;
                        }

                        // Skip the animation channel (passing a target attribute of
                        // 0 causes the animation to not be created).
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
    GP_ASSERT(id);

    Node* node = NULL;

    // Search the passed in loading contexts (scene/node) first to see
    // if we've already loaded this node during this load session.
    if (sceneContext)
    {
        node = sceneContext->findNode(id, true);
        if (node)
            node->addRef();
    }
    if (node == NULL && nodeContext)
    {
        node = nodeContext->findNode(id, true);
        if (node)
            node->addRef();
    }

    if (node == NULL)
    {
        // If not yet found, search the ref table and read.
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
    GP_ASSERT(id);
    GP_ASSERT(_stream);

    // Skip the node's type.
    unsigned int nodeType;
    if (!read(&nodeType))
    {
        GP_ERROR("Failed to skip node type for node '%s'.", id);
        return false;
    }

    // Skip over the node's transform and parent ID.
    if (_stream->seek(sizeof(float) * 16, SEEK_CUR) == false)
    {
        GP_ERROR("Failed to skip over node transform for node '%s'.", id);
        return false;
    }
    readString(_stream);

    // Skip over the node's children.
    unsigned int childrenCount;
    if (!read(&childrenCount))
    {
        GP_ERROR("Failed to skip over node's children count for node '%s'.", id);
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
    GP_ASSERT(id);
    GP_ASSERT(_stream);

    // If we are tracking nodes and it's not in the set yet, add it.
    if (_trackedNodes)
    {
        std::map<std::string, Node*>::iterator iter = _trackedNodes->find(id);
        if (iter != _trackedNodes->end())
        {
            // Skip over this node since we previously read it
            if (!skipNode())
                return NULL;

            iter->second->addRef();
            return iter->second;
        }
    }

    // Read node type.
    unsigned int nodeType;
    if (!read(&nodeType))
    {
        GP_ERROR("Failed to read node type for node '%s'.", id);
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

    if (_trackedNodes)
    {
        // Add the new node to the list of tracked nodes
        _trackedNodes->insert(std::make_pair(id, node));
    }

    // If no loading context is set, set this node as the loading context.
    if (sceneContext == NULL && nodeContext == NULL)
    {
        nodeContext = node;
    }

    // Read transform.
    float transform[16];
    if (_stream->read(transform, sizeof(float), 16) != 16)
    {
        GP_ERROR("Failed to read transform for node '%s'.", id);
        SAFE_RELEASE(node);
        return NULL;
    }
    setTransform(transform, node);

    // Skip the parent ID.
    readString(_stream);

    // Read children.
    unsigned int childrenCount;
    if (!read(&childrenCount))
    {
        GP_ERROR("Failed to read children count for node '%s'.", id);
        SAFE_RELEASE(node);
        return NULL;
    }
    if (childrenCount > 0)
    {
        // Read each child.
        for (unsigned int i = 0; i < childrenCount; i++)
        {
            // Search the passed in loading contexts (scene/node) first to see
            // if we've already loaded this child node during this load session.
            Node* child = NULL;
            id = getIdFromOffset();
            GP_ASSERT(id);

            if (sceneContext)
            {
                child = sceneContext->findNode(id, true);
            }
            if (child == NULL && nodeContext)
            {
                child = nodeContext->findNode(id, true);
            }

            // If the child was already loaded, skip it, otherwise read it
            if (child)
            {
                skipNode();
            }
            else
            {
                child = readNode(sceneContext, nodeContext);
            }

            if (child)
            {
                node->addChild(child);
                child->release(); // 'node' now owns this child
            }
        }
    }

    // Read camera.
    Camera* camera = readCamera();
    if (camera)
    {
        node->setCamera(camera);
        SAFE_RELEASE(camera);
    }

    // Read light.
    Light* light = readLight();
    if (light)
    {
        node->setLight(light);
        SAFE_RELEASE(light);
    }

    // Read model.
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
        GP_ERROR("Failed to load camera type in bundle '%s'.", _path.c_str());
        return NULL;
    }

    // Check if there isn't a camera to load.
    if (cameraType == 0)
    {
        return NULL;
    }

    float aspectRatio;
    if (!read(&aspectRatio))
    {
        GP_ERROR("Failed to load camera aspect ratio in bundle '%s'.", _path.c_str());
        return NULL;
    }

    float nearPlane;
    if (!read(&nearPlane))
    {
        GP_ERROR("Failed to load camera near plane in bundle '%s'.", _path.c_str());
        return NULL;
    }

    float farPlane;
    if (!read(&farPlane))
    {
        GP_ERROR("Failed to load camera far plane in bundle '%s'.", _path.c_str());
        return NULL;
    }

    Camera* camera = NULL;
    if (cameraType == Camera::PERSPECTIVE)
    {
        float fieldOfView;
        if (!read(&fieldOfView))
        {
            GP_ERROR("Failed to load camera field of view in bundle '%s'.", _path.c_str());
            return NULL;
        }

        camera = Camera::createPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);
    }
    else if (cameraType == Camera::ORTHOGRAPHIC)
    {
        float zoomX;
        if (!read(&zoomX))
        {
            GP_ERROR("Failed to load camera zoomX in bundle '%s'.", _path.c_str());
            return NULL;
        }

        float zoomY;
        if (!read(&zoomY))
        {
            GP_ERROR("Failed to load camera zoomY in bundle '%s'.", _path.c_str());
            return NULL;
        }

        camera = Camera::createOrthographic(zoomX, zoomY, aspectRatio, nearPlane, farPlane);
    }
    else
    {
        GP_ERROR("Unsupported camera type (%d) in bundle '%s'.", cameraType, _path.c_str());
        return NULL;
    }
    return camera;
}

Light* Bundle::readLight()
{
    unsigned char type;
    if (!read(&type))
    {
        GP_ERROR("Failed to load light type in bundle '%s'.", _path.c_str());
        return NULL;
    }

    // Check if there isn't a light to load.
    if (type == 0)
    {
        return NULL;
    }

    // Read color.
    float red, blue, green;
    if (!read(&red) || !read(&blue) || !read(&green))
    {
        GP_ERROR("Failed to load light color in bundle '%s'.", _path.c_str());
        return NULL;
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
            GP_ERROR("Failed to load point light range in bundle '%s'.", _path.c_str());
            return NULL;
        }
        light = Light::createPoint(color, range);
    }
    else if (type == Light::SPOT)
    {
        float range, innerAngle, outerAngle;
        if (!read(&range))
        {
            GP_ERROR("Failed to load spot light range in bundle '%s'.", _path.c_str());
            return NULL;
        }
        if (!read(&innerAngle))
        {
            GP_ERROR("Failed to load spot light inner angle in bundle '%s'.", _path.c_str());
            return NULL;
        }
        if (!read(&outerAngle))
        {
            GP_ERROR("Failed to load spot light outer angle in bundle '%s'.", _path.c_str());
            return NULL;
        }
        light = Light::createSpot(color, range, innerAngle, outerAngle);
    }
    else
    {
        GP_ERROR("Unsupported light type (%d) in bundle '%s'.", type, _path.c_str());
        return NULL;
    }
    return light;
}

Model* Bundle::readModel(const char* nodeId)
{
    std::string xref = readString(_stream);
    if (xref.length() > 1 && xref[0] == '#') // TODO: Handle full xrefs
    {
        Mesh* mesh = loadMesh(xref.c_str() + 1, nodeId);
        if (mesh)
        {
            Model* model = Model::create(mesh);
            SAFE_RELEASE(mesh);

            // Read skin.
            unsigned char hasSkin;
            if (!read(&hasSkin))
            {
                GP_ERROR("Failed to load whether model with mesh '%s' has a mesh skin in bundle '%s'.", xref.c_str() + 1, _path.c_str());
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
            // Read material.
            unsigned int materialCount;
            if (!read(&materialCount))
            {
                GP_ERROR("Failed to load material count for model with mesh '%s' in bundle '%s'.", xref.c_str() + 1, _path.c_str());
                return NULL;
            }
            if (materialCount > 0)
            {
                for (unsigned int i = 0; i < materialCount; ++i)
                {
                    std::string materialName = readString(_stream);
                    std::string materialPath = getMaterialPath();
                    materialPath.append("#");
                    materialPath.append(materialName);
                    Material* material = Material::create(materialPath.c_str());
                    if (material)
                    {
                        int partIndex = model->getMesh()->getPartCount() > 0 ? i : -1;
                        model->setMaterial(material, partIndex);
                        SAFE_RELEASE(material);
                    }
                }
            }
            return model;
        }
    }

    return NULL;
}

MeshSkin* Bundle::readMeshSkin()
{
    MeshSkin* meshSkin = new MeshSkin();

    // Read bindShape.
    float bindShape[16];
    if (!readMatrix(bindShape))
    {
        GP_ERROR("Failed to load bind shape for mesh skin in bundle '%s'.", _path.c_str());
        SAFE_DELETE(meshSkin);
        return NULL;
    }
    meshSkin->setBindShape(bindShape);

    MeshSkinData* skinData = new MeshSkinData();
    skinData->skin = meshSkin;

    // Read joint count.
    unsigned int jointCount;
    if (!read(&jointCount))
    {
        GP_ERROR("Failed to load joint count for mesh skin in bundle '%s'.", _path.c_str());
        SAFE_DELETE(meshSkin);
        SAFE_DELETE(skinData);
        return NULL;
    }
    if (jointCount == 0)
    {
        GP_ERROR("Invalid joint count (must be greater than 0) for mesh skin in bundle '%s'.", _path.c_str());
        SAFE_DELETE(meshSkin);
        SAFE_DELETE(skinData);
        return NULL;
    }
    meshSkin->setJointCount(jointCount);

    // Read joint xref strings for all joints in the list.
    for (unsigned int i = 0; i < jointCount; i++)
    {
        skinData->joints.push_back(readString(_stream));
    }

    // Read bind poses.
    unsigned int jointsBindPosesCount;
    if (!read(&jointsBindPosesCount))
    {
        GP_ERROR("Failed to load number of joint bind poses in bundle '%s'.", _path.c_str());
        SAFE_DELETE(meshSkin);
        SAFE_DELETE(skinData);
        return NULL;
    }
    if (jointsBindPosesCount > 0)
    {
        GP_ASSERT(jointCount * 16 == jointsBindPosesCount);
        float m[16];
        for (unsigned int i = 0; i < jointCount; i++)
        {
            if (!readMatrix(m))
            {
                GP_ERROR("Failed to load joint bind pose matrix (for joint with index %d) in bundle '%s'.", i, _path.c_str());
                SAFE_DELETE(meshSkin);
                SAFE_DELETE(skinData);
                return NULL;
            }
            skinData->inverseBindPoseMatrices.push_back(m);
        }
    }

    // Store the MeshSkinData so we can go back and resolve all joint references later.
    _meshSkins.push_back(skinData);

    return meshSkin;
}

void Bundle::resolveJointReferences(Scene* sceneContext, Node* nodeContext)
{
    GP_ASSERT(_stream);

    for (size_t i = 0, skinCount = _meshSkins.size(); i < skinCount; ++i)
    {
        MeshSkinData* skinData = _meshSkins[i];
        GP_ASSERT(skinData);
        GP_ASSERT(skinData->skin);

        // Resolve all joints in skin joint list.
        size_t jointCount = skinData->joints.size();
        for (size_t j = 0; j < jointCount; ++j)
        {
            // TODO: Handle full xrefs (not just local # xrefs).
            std::string jointId = skinData->joints[j];
            if (jointId.length() > 1 && jointId[0] == '#')
            {
                jointId = jointId.substr(1, jointId.length() - 1);

                Node* n = loadNode(jointId.c_str(), sceneContext, nodeContext);
                if (n && n->getType() == Node::JOINT)
                {
                    Joint* joint = static_cast<Joint*>(n);
                    joint->setInverseBindPose(skinData->inverseBindPoseMatrices[j]);
                    skinData->skin->setJoint(joint, (unsigned int)j);
                    SAFE_RELEASE(joint);
                }
            }
        }

        // Set the root joint.
        if (jointCount > 0)
        {
            Joint* rootJoint = skinData->skin->getJoint((unsigned int)0);
            Node* node = rootJoint;
            GP_ASSERT(node);
            Node* parent = node->getParent();

            std::vector<Node*> loadedNodes;
            while (true)
            {
                if (parent)
                {
                    if (skinData->skin->getJointIndex(static_cast<Joint*>(parent)) != -1)
                    {
                        // Parent is a joint in the MeshSkin, so treat it as the new root.
                        rootJoint = static_cast<Joint*>(parent);
                    }

                    node = parent;
                    parent = node->getParent();
                }
                else
                {
                    // No parent currently set for this joint.
                    // Lookup its parentID in case it references a node that was not yet loaded as part
                    // of the mesh skin's joint list.
                    std::string nodeId = node->getId();

                    while (true)
                    {
                        // Get the node's type.
                        Reference* ref = find(nodeId.c_str());
                        if (ref == NULL)
                        {
                            GP_ERROR("No object with name '%s' in bundle '%s'.", nodeId.c_str(), _path.c_str());
                            return;
                        }

                        // Seek to the current node in the file so we can get it's parent ID.
                        seekTo(nodeId.c_str(), ref->type);

                        // Skip over the node type (1 unsigned int) and transform (16 floats) and read the parent id.
                        if (_stream->seek(sizeof(unsigned int) + sizeof(float)*16, SEEK_CUR) == false)
                        {
                            GP_ERROR("Failed to skip over node type and transform for node '%s' in bundle '%s'.", nodeId.c_str(), _path.c_str());
                            return;
                        }
                        std::string parentID = readString(_stream);

                        if (!parentID.empty())
                            nodeId = parentID;
                        else
                            break;
                    }

                    if (nodeId != rootJoint->getId())
                        loadedNodes.push_back(loadNode(nodeId.c_str(), sceneContext, nodeContext));

                    break;
                }
            }

            skinData->skin->setRootJoint(rootJoint);

            // Release all the nodes that we loaded since the nodes are now owned by the mesh skin/joints.
            for (unsigned int i = 0; i < loadedNodes.size(); i++)
            {
                SAFE_RELEASE(loadedNodes[i]);
            }
        }

        // Remove the joint hierarchy from the scene since it is owned by the mesh skin.
        if (sceneContext)
            sceneContext->removeNode(skinData->skin->_rootNode);

        // Done with this MeshSkinData entry.
        SAFE_DELETE(_meshSkins[i]);
    }
    _meshSkins.clear();
}

void Bundle::readAnimation(Scene* scene)
{
    const std::string animationId = readString(_stream);

    // Read the number of animation channels in this animation.
    unsigned int animationChannelCount;
    if (!read(&animationChannelCount))
    {
        GP_ERROR("Failed to read animation channel count for animation '%s'.", animationId.c_str());
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
    // Read the number of animations in this object.
    unsigned int animationCount;
    if (!read(&animationCount))
    {
        GP_ERROR("Failed to read the number of animations in the scene.");
        return;
    }

    for (unsigned int i = 0; i < animationCount; i++)
    {
        readAnimation(scene);
    }
}

Animation* Bundle::readAnimationChannel(Scene* scene, Animation* animation, const char* animationId)
{
    GP_ASSERT(animationId);

    // Read target id.
    std::string targetId = readString(_stream);
    if (targetId.empty())
    {
        GP_ERROR("Failed to read target id for animation '%s'.", animationId);
        return NULL;
    }

    // Read target attribute.
    unsigned int targetAttribute;
    if (!read(&targetAttribute))
    {
        GP_ERROR("Failed to read target attribute for animation '%s'.", animationId);
        return NULL;
    }

    AnimationTarget* target = NULL;

    // Search for a node that matches the target.
    if (!target)
    {
        target = scene->findNode(targetId.c_str());
        if (!target)
        {
            GP_ERROR("Failed to find the animation target (with id '%s') for animation '%s'.", targetId.c_str(), animationId);
            return NULL;
        }
    }

    return readAnimationChannelData(animation, animationId, target, targetAttribute);
}

Animation* Bundle::readAnimationChannelData(Animation* animation, const char* id, AnimationTarget* target, unsigned int targetAttribute)
{
    GP_ASSERT(id);

    std::vector<unsigned int> keyTimes;
    std::vector<float> values;
    std::vector<float> tangentsIn;
    std::vector<float> tangentsOut;
    std::vector<unsigned int> interpolation;

    // Length of the arrays.
    unsigned int keyTimesCount;
    unsigned int valuesCount;
    unsigned int tangentsInCount;
    unsigned int tangentsOutCount;
    unsigned int interpolationCount;

    // Read key times.
    if (!readArray(&keyTimesCount, &keyTimes, sizeof(unsigned int)))
    {
        GP_ERROR("Failed to read key times for animation '%s'.", id);
        return NULL;
    }

    // Read key values.
    if (!readArray(&valuesCount, &values))
    {
        GP_ERROR("Failed to read key values for animation '%s'.", id);
        return NULL;
    }

    // Read in-tangents.
    if (!readArray(&tangentsInCount, &tangentsIn))
    {
        GP_ERROR("Failed to read in tangents for animation '%s'.", id);
        return NULL;
    }

    // Read out-tangents.
    if (!readArray(&tangentsOutCount, &tangentsOut))
    {
        GP_ERROR("Failed to read out tangents for animation '%s'.", id);
        return NULL;
    }

    // Read interpolations.
    if (!readArray(&interpolationCount, &interpolation, sizeof(unsigned int)))
    {
        GP_ERROR("Failed to read the interpolation values for animation '%s'.", id);
        return NULL;
    }

    if (targetAttribute > 0)
    {
        GP_ASSERT(target);
        GP_ASSERT(keyTimes.size() > 0 && values.size() > 0);
        if (animation == NULL)
        {
            // TODO: This code currently assumes LINEAR only.
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
    return loadMesh(id, NULL);
}

Mesh* Bundle::loadMesh(const char* id, const char* nodeId)
{
    GP_ASSERT(_stream);
    GP_ASSERT(id);

    // Save the file position.
    long position = _stream->position();
    if (position == -1L)
    {
        GP_ERROR("Failed to save the current file position before loading mesh '%s'.", id);
        return NULL;
    }

    // Seek to the specified mesh.
    Reference* ref = seekTo(id, BUNDLE_TYPE_MESH);
    if (ref == NULL)
    {
        GP_ERROR("Failed to locate ref for mesh '%s'.", id);
        return NULL;
    }

    // Read mesh data.
    MeshData* meshData = readMeshData();
    if (meshData == NULL)
    {
        GP_ERROR("Failed to load mesh data for mesh '%s'.", id);
        return NULL;
    }

    // Create mesh.
    Mesh* mesh = Mesh::createMesh(meshData->vertexFormat, meshData->vertexCount, false);
    if (mesh == NULL)
    {
        GP_ERROR("Failed to create mesh '%s'.", id);
        SAFE_DELETE_ARRAY(meshData);
        return NULL;
    }

    mesh->_url = _path;
    mesh->_url += "#";
    mesh->_url += id;

    mesh->setVertexData((float*)meshData->vertexData, 0, meshData->vertexCount);

    mesh->_boundingBox.set(meshData->boundingBox);
    mesh->_boundingSphere.set(meshData->boundingSphere);

    // Create mesh parts.
    for (unsigned int i = 0; i < meshData->parts.size(); ++i)
    {
        MeshPartData* partData = meshData->parts[i];
        GP_ASSERT(partData);

        MeshPart* part = mesh->addPart(partData->primitiveType, partData->indexFormat, partData->indexCount, false);
        if (part == NULL)
        {
            GP_ERROR("Failed to create mesh part (with index %d) for mesh '%s'.", i, id);
            SAFE_DELETE(meshData);
            return NULL;
        }
        part->setIndexData(partData->indexData, 0, partData->indexCount);
    }

    SAFE_DELETE(meshData);

    // Restore file pointer.
    if (_stream->seek(position, SEEK_SET) == false)
    {
        GP_ERROR("Failed to restore file pointer after loading mesh '%s'.", id);
        return NULL;
    }

    return mesh;
}

Bundle::MeshData* Bundle::readMeshData()
{
    // Read vertex format/elements.
    unsigned int vertexElementCount;
    if (_stream->read(&vertexElementCount, 4, 1) != 1)
    {
        GP_ERROR("Failed to load vertex element count.");
        return NULL;
    }
    if (vertexElementCount < 1)
    {
        GP_ERROR("Failed to load mesh data; invalid vertex element count (must be greater than 0).");
        return NULL;
    }

    VertexFormat::Element* vertexElements = new VertexFormat::Element[vertexElementCount];
    for (unsigned int i = 0; i < vertexElementCount; ++i)
    {
        unsigned int vUsage, vSize;
        if (_stream->read(&vUsage, 4, 1) != 1)
        {
            GP_ERROR("Failed to load vertex usage.");
            SAFE_DELETE_ARRAY(vertexElements);
            return NULL;
        }
        if (_stream->read(&vSize, 4, 1) != 1)
        {
            GP_ERROR("Failed to load vertex size.");
            SAFE_DELETE_ARRAY(vertexElements);
            return NULL;
        }

        vertexElements[i].usage = (VertexFormat::Usage)vUsage;
        vertexElements[i].size = vSize;
    }

    MeshData* meshData = new MeshData(VertexFormat(vertexElements, vertexElementCount));
    SAFE_DELETE_ARRAY(vertexElements);

    // Read vertex data.
    unsigned int vertexByteCount;
    if (_stream->read(&vertexByteCount, 4, 1) != 1)
    {
        GP_ERROR("Failed to load vertex byte count.");
        SAFE_DELETE(meshData);
        return NULL;
    }
    if (vertexByteCount == 0)
    {
        GP_ERROR("Failed to load mesh data; invalid vertex byte count of 0.");
        SAFE_DELETE(meshData);
        return NULL;
    }

    GP_ASSERT(meshData->vertexFormat.getVertexSize());
    meshData->vertexCount = vertexByteCount / meshData->vertexFormat.getVertexSize();
    meshData->vertexData = new unsigned char[vertexByteCount];
    if (_stream->read(meshData->vertexData, 1, vertexByteCount) != vertexByteCount)
    {
        GP_ERROR("Failed to load vertex data.");
        SAFE_DELETE(meshData);
        return NULL;
    }

    // Read mesh bounds (bounding box and bounding sphere).
    if (_stream->read(&meshData->boundingBox.min.x, 4, 3) != 3 || _stream->read(&meshData->boundingBox.max.x, 4, 3) != 3)
    {
        GP_ERROR("Failed to load mesh bounding box.");
        SAFE_DELETE(meshData);
        return NULL;
    }
    if (_stream->read(&meshData->boundingSphere.center.x, 4, 3) != 3 || _stream->read(&meshData->boundingSphere.radius, 4, 1) != 1)
    {
        GP_ERROR("Failed to load mesh bounding sphere.");
        SAFE_DELETE(meshData);
        return NULL;
    }

    // Read mesh parts.
    unsigned int meshPartCount;
    if (_stream->read(&meshPartCount, 4, 1) != 1)
    {
        GP_ERROR("Failed to load mesh part count.");
        SAFE_DELETE(meshData);
        return NULL;
    }
    for (unsigned int i = 0; i < meshPartCount; ++i)
    {
        // Read primitive type, index format and index count.
        unsigned int pType, iFormat, iByteCount;
        if (_stream->read(&pType, 4, 1) != 1)
        {
            GP_ERROR("Failed to load primitive type for mesh part with index %d.", i);
            SAFE_DELETE(meshData);
            return NULL;
        }
        if (_stream->read(&iFormat, 4, 1) != 1)
        {
            GP_ERROR("Failed to load index format for mesh part with index %d.", i);
            SAFE_DELETE(meshData);
            return NULL;
        }
        if (_stream->read(&iByteCount, 4, 1) != 1)
        {
            GP_ERROR("Failed to load index byte count for mesh part with index %d.", i);
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
        default:
            GP_ERROR("Unsupported index format for mesh part with index %d.", i);
            return NULL;
        }

        GP_ASSERT(indexSize);
        partData->indexCount = iByteCount / indexSize;

        partData->indexData = new unsigned char[iByteCount];
        if (_stream->read(partData->indexData, 1, iByteCount) != iByteCount)
        {
            GP_ERROR("Failed to read index data for mesh part with index %d.", i);
            SAFE_DELETE(meshData);
            return NULL;
        }
    }

    return meshData;
}

Bundle::MeshData* Bundle::readMeshData(const char* url)
{
    GP_ASSERT(url);

    size_t len = strlen(url);
    if (len == 0)
    {
        GP_ERROR("Mesh data URL must be non-empty.");
        return NULL;
    }

    // Parse URL (formatted as 'bundle#id').
    std::string urlstring(url);
    size_t pos = urlstring.find('#');
    if (pos == std::string::npos)
    {
        GP_ERROR("Invalid mesh data URL '%s' (must be of the form 'bundle#id').", url);
        return NULL;
    }

    std::string file = urlstring.substr(0, pos);
    std::string id = urlstring.substr(pos + 1);

    // Load bundle.
    Bundle* bundle = Bundle::create(file.c_str());
    if (bundle == NULL)
    {
        GP_ERROR("Failed to load bundle '%s'.", file.c_str());
        return NULL;
    }

    // Seek to mesh with specified ID in bundle.
    Reference* ref = bundle->seekTo(id.c_str(), BUNDLE_TYPE_MESH);
    if (ref == NULL)
    {
        GP_ERROR("Failed to load ref from bundle '%s' for mesh with id '%s'.", file.c_str(), id.c_str());
        return NULL;
    }

    // Read mesh data from current file position.
    MeshData* meshData = bundle->readMeshData();

    SAFE_RELEASE(bundle);

    return meshData;
}

Font* Bundle::loadFont(const char* id)
{
    GP_ASSERT(id);
    GP_ASSERT(_stream);

    // Seek to the specified font.
    Reference* ref = seekTo(id, BUNDLE_TYPE_FONT);
    if (ref == NULL)
    {
        GP_ERROR("Failed to load ref for font '%s'.", id);
        return NULL;
    }

    // Read font family.
    std::string family = readString(_stream);
    if (family.empty())
    {
        GP_ERROR("Failed to read font family for font '%s'.", id);
        return NULL;
    }

    // Read font style and size.
    unsigned int style, size;
    if (_stream->read(&style, 4, 1) != 1)
    {
        GP_ERROR("Failed to read style for font '%s'.", id);
        return NULL;
    }
    if (_stream->read(&size, 4, 1) != 1)
    {
        GP_ERROR("Failed to read size for font '%s'.", id);
        return NULL;
    }

    // Read character set.
    std::string charset = readString(_stream);

    // Read font glyphs.
    unsigned int glyphCount;
    if (_stream->read(&glyphCount, 4, 1) != 1)
    {
        GP_ERROR("Failed to read glyph count for font '%s'.", id);
        return NULL;
    }
    if (glyphCount == 0)
    {
        GP_ERROR("Invalid glyph count (must be greater than 0) for font '%s'.", id);
        return NULL;
    }

    Font::Glyph* glyphs = new Font::Glyph[glyphCount];
    if (_stream->read(glyphs, sizeof(Font::Glyph), glyphCount) != glyphCount)
    {
        GP_ERROR("Failed to read glyphs for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }

    // Read texture attributes.
    unsigned int width, height, textureByteCount;
    if (_stream->read(&width, 4, 1) != 1)
    {
        GP_ERROR("Failed to read texture width for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }
    if (_stream->read(&height, 4, 1) != 1)
    {
        GP_ERROR("Failed to read texture height for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }
    if (_stream->read(&textureByteCount, 4, 1) != 1)
    {
        GP_ERROR("Failed to read texture byte count for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }
    if (textureByteCount != (width * height))
    {
        GP_ERROR("Invalid texture byte count for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }

    // Read texture data.
    unsigned char* textureData = new unsigned char[textureByteCount];
    if (_stream->read(textureData, 1, textureByteCount) != textureByteCount)
    {
        GP_ERROR("Failed to read texture data for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        SAFE_DELETE_ARRAY(textureData);
        return NULL;
    }

    // Create the texture for the font.
    Texture* texture = Texture::create(Texture::ALPHA, width, height, textureData, true);

    // Free the texture data (no longer needed).
    SAFE_DELETE_ARRAY(textureData);

    if (texture == NULL)
    {
        GP_ERROR("Failed to create texture for font '%s'.", id);
        SAFE_DELETE_ARRAY(glyphs);
        return NULL;
    }

    // Create the font.
    Font* font = Font::create(family.c_str(), Font::PLAIN, size, glyphs, glyphCount, texture);

    // Free the glyph array.
    SAFE_DELETE_ARRAY(glyphs);

    // Release the texture since the Font now owns it.
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
    GP_ASSERT(transform);

    // Load array into transform.
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

const char* Bundle::getObjectId(unsigned int index) const
{
    GP_ASSERT(_references);
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
