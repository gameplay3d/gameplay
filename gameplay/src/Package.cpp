/*
 * Package.cpp
 */

#include "Base.h"
#include "Package.h"
#include "FileSystem.h"
#include "MeshPart.h"
#include "Scene.h"
#include "Joint.h"

#define GPB_PACKAGE_VERSION_MAJOR 1
#define GPB_PACKAGE_VERSION_MINOR 0

#define PACKAGE_TYPE_SCENE 1
#define PACKAGE_TYPE_NODE 2
#define PACKAGE_TYPE_ANIMATIONS 3
#define PACKAGE_TYPE_ANIMATION 4
#define PACKAGE_TYPE_ANIMATION_CHANNEL 5
#define PACKAGE_TYPE_MMODEL 10
#define PACKAGE_TYPE_MATERIAL 16
#define PACKAGE_TYPE_EFFECT 18
#define PACKAGE_TYPE_CAMERA 32
#define PACKAGE_TYPE_LIGHT 33
#define PACKAGE_TYPE_MESH 34
#define PACKAGE_TYPE_MESHPART 35
#define PACKAGE_TYPE_MESHSKIN 36
#define PACKAGE_TYPE_FONT 128

// For sanity checking string reads
#define PACKAGE_MAX_STRING_LENGTH 5000

namespace gameplay
{

static std::vector<Package*> __packageCache;

Package::Package(const char* path) :
    _path(path), _referenceCount(0), _references(NULL), _file(NULL)
{
}

Package::~Package()
{
    clearLoadSession();

    // Remove this Package from the cache
    std::vector<Package*>::iterator itr = std::find(__packageCache.begin(), __packageCache.end(), this);
    if (itr != __packageCache.end())
    {
        __packageCache.erase(itr);
    }

    SAFE_DELETE_ARRAY(_references);

    if (_file)
    {
        fclose(_file);
        _file = NULL;
    }
}

template <class T>
bool Package::readArray(unsigned int* length, T** ptr)
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

std::string readString(FILE* fp)
{
    unsigned int length;
    if (fread(&length, 4, 1, fp) != 1)
    {
        return std::string();
    }

    // Sanity check to detect if string length is far too big
    assert(length < PACKAGE_MAX_STRING_LENGTH);

    char* str = new char[length + 1];
    if (length > 0)
    {
        if (fread(str, 1, length, fp) != length)
        {
            SAFE_DELETE_ARRAY(str);
            return std::string();
        }
    }

    str[length] = '\0';
    std::string result(str);
    SAFE_DELETE_ARRAY(str);
    return result;
}

Package* Package::create(const char* path)
{
    // Search the cache for this package
    for (unsigned int i = 0, count = __packageCache.size(); i < count; ++i)
    {
        Package* p = __packageCache[i];
        if (p->_path == path)
        {
            // Found a match
            p->addRef();
            return p;
        }
    }

    // Open the package
    FILE* fp = FileSystem::openFile(path, "rb");

    // Read the GPG header info
    char sig[9];
    if (fread(sig, 1, 9, fp) != 9 || memcmp(sig, "«GPB»\r\n\x1A\n", 9) != 0)
    {
        LOG_ERROR_VARG("Invalid package header: %s", path);
        fclose(fp);
        return NULL;
    }

    // Read version
    unsigned char ver[2];
    if (fread(ver, 1, 2, fp) != 2 || ver[0] != GPB_PACKAGE_VERSION_MAJOR || ver[1] != GPB_PACKAGE_VERSION_MINOR)
    {
        LOG_ERROR_VARG("Unsupported version (%d.%d) for package: %s (expected %d.%d)", (int)ver[0], (int)ver[1], path, GPB_PACKAGE_VERSION_MAJOR, GPB_PACKAGE_VERSION_MINOR);
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
    Package* pkg = new Package(path);
    pkg->_referenceCount = refCount;
    pkg->_references = refs;
    pkg->_file = fp;

    return pkg;
}

Package::Reference* Package::find(const char* id) const
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

void Package::clearLoadSession()
{
    for (unsigned int i = 0, count = _meshSkins.size(); i < count; ++i)
    {
        SAFE_DELETE(_meshSkins[i]);
    }
    _meshSkins.clear();
}

const char* Package::getIdFromOffset() const
{
    return getIdFromOffset((unsigned int) ftell(_file));
}

const char* Package::getIdFromOffset(unsigned int offset) const
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

Package::Reference* Package::seekTo(const char* id, unsigned int type)
{
    Reference* ref = find(id);
    if (ref == NULL)
    {
        LOG_ERROR_VARG("No object with name '%s' in package '%s'.", id, _path.c_str());
        return NULL;
    }

    if (ref->type != type)
    {
        LOG_ERROR_VARG("Object '%s' in package '%s' has type %d (expected type %d).", id, _path.c_str(), (int)ref->type, (int)type);
        return NULL;
    }

    // Seek to the offset of this object
    if (fseek(_file, ref->offset, SEEK_SET) != 0)
    {
        LOG_ERROR_VARG("Failed to seek to object '%s' in package '%s'.", id, _path.c_str());
        return NULL;
    }

    return ref;
}

Package::Reference* Package::seekToFirstType(unsigned int type)
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
                LOG_ERROR_VARG("Failed to seek to object '%s' in package '%s'.", ref->id.c_str(), _path.c_str());
                return NULL;
            }
            return ref;
        }
    }
    return NULL;
}

bool Package::read(unsigned int* ptr)
{
    return fread(ptr, sizeof(unsigned int), 1, _file) == 1;
}

bool Package::read(unsigned char* ptr)
{
    return fread(ptr, sizeof(unsigned char), 1, _file) == 1;
}

bool Package::read(float* ptr)
{
    return fread(ptr, sizeof(float), 1, _file) == 1;
}

bool Package::readMatrix(float* m)
{
    return (fread(m, sizeof(float), 16, _file) == 16);
}

Scene* Package::loadScene(const char* id)
{
    clearLoadSession();

    Reference* ref = NULL;
    if (id)
    {
        ref = seekTo(id, PACKAGE_TYPE_SCENE);
    }
    else
    {
        ref = seekToFirstType(PACKAGE_TYPE_SCENE);
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
        if (ref->type == PACKAGE_TYPE_ANIMATIONS)
        {
            // Found a match
            if (fseek(_file, ref->offset, SEEK_SET) != 0)
            {
                LOG_ERROR_VARG("Failed to seek to object '%s' in package '%s'.", ref->id.c_str(), _path.c_str());
                return NULL;
            }
            readAnimations(scene);
        }
    }

    resolveJointReferences(scene, NULL);

    return scene;
}

Node* Package::loadNode(const char* id)
{
    assert(id);

    clearLoadSession();

    Node* node = loadNode(id, NULL, NULL);
   
    if (node)
    {
        resolveJointReferences(NULL, node);
    }

    return node;
}

Node* Package::loadNode(const char* id, Scene* sceneContext, Node* nodeContext)
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
        Reference* ref = seekTo(id, PACKAGE_TYPE_NODE);
        if (ref == NULL)
        {
            return NULL;
        }

        node = readNode(sceneContext, nodeContext);
    }

    return node;
}

Node* Package::readNode(Scene* sceneContext, Node* nodeContext)
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
            Node* child = readNode(sceneContext, nodeContext);
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
    Model* model = readModel(sceneContext, nodeContext);
    if (model)
    {
        node->setModel(model);
        SAFE_RELEASE(model);
    }

    return node;
}

Camera* Package::readCamera()
{
    unsigned char cameraType;
    if (!read(&cameraType))
    {
        LOG_ERROR_VARG("Failed to load camera type in package '%s'.", _path.c_str());
    }

    if (cameraType == 0)
    {
        return NULL;
    }

    // aspect ratio
    float aspectRatio;
    if (!read(&aspectRatio))
    {
        LOG_ERROR_VARG("Failed to load camera aspectRatio in package '%s'.", _path.c_str());
    }

    // near plane
    float nearPlane;
    if (!read(&nearPlane))
    {
        LOG_ERROR_VARG("Failed to load camera near plane in package '%s'.", _path.c_str());
    }

    // far plane
    float farPlane;
    if (!read(&farPlane))
    {
        LOG_ERROR_VARG("Failed to load camera far plane in package '%s'.", _path.c_str());
    }

    Camera* camera = NULL;
    if (cameraType == Camera::PERSPECTIVE)
    {
        // field of view
        float fieldOfView;
        if (!read(&fieldOfView))
        {
            LOG_ERROR_VARG("Failed to load camera field of view in package '%s'.", _path.c_str());
        }

        camera = Camera::createPerspective(fieldOfView, aspectRatio, nearPlane, farPlane);
    }
    else if (cameraType == Camera::ORTHOGRAPHIC)
    {
        // magnification
        float zoomX;
        if (!read(&zoomX))
        {
            LOG_ERROR_VARG("Failed to load camera zoomX in package '%s'.", _path.c_str());
        }

        float zoomY;
        if (!read(&zoomY))
        {
            LOG_ERROR_VARG("Failed to load camera zoomY in package '%s'.", _path.c_str());
        }

        camera = Camera::createOrthographic(zoomX, zoomY, aspectRatio, nearPlane, farPlane);
    }
    else
    {
        LOG_ERROR_VARG("Failed to load camera type in package '%s'. Invalid camera type.", _path.c_str());
    }
    return camera;
}

Light* Package::readLight()
{
    unsigned char type;
    if (!read(&type))
    {
        LOG_ERROR_VARG("Failed to load light %s in package '%s'.", "type", _path.c_str());
    }

    if (type == 0)
    {
        return NULL;
    }

    // read color
    float red, blue, green;
    if (!read(&red) || !read(&blue) || !read(&green))
    {
        LOG_ERROR_VARG("Failed to load light %s in package '%s'.", "color", _path.c_str());
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
            LOG_ERROR_VARG("Failed to load point light %s in package '%s'.", "point", _path.c_str());
        }
        light = Light::createPoint(color, range);
    }
    else if (type == Light::SPOT)
    {
        float range, innerAngle, outerAngle;
        if (!read(&range) || !read(&innerAngle) || !read(&outerAngle))
        {
            LOG_ERROR_VARG("Failed to load spot light %s in package '%s'.", "spot", _path.c_str());
        }
        light = Light::createSpot(color, range, innerAngle, outerAngle);
    }
    else
    {
        LOG_ERROR_VARG("Failed to load light %s in package '%s'.", "type", _path.c_str());
    }
    return light;
}

Model* Package::readModel(Scene* sceneContext, Node* nodeContext)
{
    // Read mesh
    Mesh* mesh = NULL;
    std::string xref = readString(_file);
    if (xref.length() > 1 && xref[0] == '#') // TODO: Handle full xrefs
    {
        mesh = loadMesh(xref.c_str() + 1);
        if (mesh)
        {
            Model* model = Model::create(mesh);
            SAFE_RELEASE(mesh);

            // Read skin
            unsigned char hasSkin;
            if (!read(&hasSkin))
            {
                LOG_ERROR_VARG("Failed to load hasSkin in package '%s'.", _path.c_str());
                return NULL;
            }
            if (hasSkin)
            {
                MeshSkin* skin = readMeshSkin(sceneContext, nodeContext);
                if (skin)
                {
                    model->setSkin(skin);
                }
            }
            // Read material
            unsigned int materialCount;
            if (!read(&materialCount))
            {
                LOG_ERROR_VARG("Failed to load materialCount in package '%s'.", _path.c_str());
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

MeshSkin* Package::readMeshSkin(Scene* sceneContext, Node* nodeContext)
{
    MeshSkin* meshSkin = new MeshSkin();

    // Read bindShape
    float bindShape[16];
    if (!readMatrix(bindShape))
    {
        LOG_ERROR_VARG("Failed to load MeshSkin in package '%s'.", _path.c_str());
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
        LOG_ERROR_VARG("Failed to load MeshSkin in package '%s'.", _path.c_str());
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
        LOG_ERROR_VARG("Failed to load MeshSkin in package '%s'.", _path.c_str());
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
                LOG_ERROR_VARG("Failed to load MeshSkin in package '%s'.", _path.c_str());
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

void Package::resolveJointReferences(Scene* sceneContext, Node* nodeContext)
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

        // Done with this MeshSkinData entry
        SAFE_DELETE(_meshSkins[i]);
    }
    _meshSkins.clear();
}

void Package::readAnimation(Scene* scene)
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

void Package::readAnimations(Scene* scene)
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

Animation* Package::readAnimationChannel(Scene* scene, Animation* animation, const char* animationId)
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

    unsigned long* keyTimes = NULL;
    float* values = NULL;
    float* tangentsIn = NULL;
    float* tangentsOut = NULL;
    float* interpolation = NULL;

    // length of the arrays
    unsigned int keyTimesCount;
    unsigned int valuesCount;
    unsigned int tangentsInCount;
    unsigned int tangentsOutCount;
    unsigned int interpolationCount;

    // read key times
    if (!readArray(&keyTimesCount, &keyTimes))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "keyTimes", "animation", id);
        SAFE_DELETE_ARRAY(keyTimes);
        return NULL;
    }
    
    // read key values
    if (!readArray(&valuesCount, &values))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "values", "animation", id);
        SAFE_DELETE_ARRAY(keyTimes);
        SAFE_DELETE_ARRAY(values);
        return NULL;
    }
    
    // read tangentsIn
    if (!readArray(&tangentsInCount, &tangentsIn))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "tangentsIn", "animation", id);
        SAFE_DELETE_ARRAY(keyTimes);
        SAFE_DELETE_ARRAY(values);
        SAFE_DELETE_ARRAY(tangentsIn);
        return NULL;
    }
    
    // read tangent_out
    if (!readArray(&tangentsOutCount, &tangentsOut))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "tangentsOut", "animation", id);
        SAFE_DELETE_ARRAY(keyTimes);
        SAFE_DELETE_ARRAY(values);
        SAFE_DELETE_ARRAY(tangentsIn);
        SAFE_DELETE_ARRAY(tangentsOut);
        return NULL;
    }
    
    // read interpolations
    if (!readArray(&interpolationCount, &interpolation))
    {
        LOG_ERROR_VARG("Failed to read %s for %s: %s", "interpolation", "animation", id);
        SAFE_DELETE_ARRAY(keyTimes);
        SAFE_DELETE_ARRAY(values);
        SAFE_DELETE_ARRAY(tangentsIn);
        SAFE_DELETE_ARRAY(tangentsOut);
        SAFE_DELETE_ARRAY(interpolation);
        return NULL;
    }
    
    Game* game = Game::getInstance();
    AnimationController* controller = game->getAnimationController();

    // TODO: Handle other target attributes later.
    if (targetAttribute > 0)
    {
        if (animation == NULL)
        {
            // TODO: This code currently assumes LINEAR only
            animation = controller->createAnimation(animationId, target, targetAttribute, keyTimesCount, keyTimes, values, Curve::LINEAR);
        }
        else
        {
            animation->createChannel(target, targetAttribute, keyTimesCount, keyTimes, values, Curve::LINEAR);
        }
    }

    SAFE_DELETE_ARRAY(keyTimes);
    SAFE_DELETE_ARRAY(values);
    SAFE_DELETE_ARRAY(tangentsIn);
    SAFE_DELETE_ARRAY(tangentsOut);
    SAFE_DELETE_ARRAY(interpolation);

    return animation;
}

Mesh* Package::loadMesh(const char* id)
{
    // save the file position
    long position = ftell(_file);

    // Seek to the specified Mesh
    Reference* ref = seekTo(id, PACKAGE_TYPE_MESH);
    if (ref == NULL)
    {
        return NULL;
    }

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

    // Create VertexFormat
    VertexFormat* vertexFormat = VertexFormat::create(vertexElements, vertexElementCount);
    SAFE_DELETE_ARRAY(vertexElements);
    if (vertexFormat == NULL)
    {
        return NULL;
    }

    // Read vertex data
    unsigned int vertexByteCount;
    if (fread(&vertexByteCount, 4, 1, _file) != 1 || vertexByteCount == 0)
    {
        SAFE_RELEASE(vertexFormat);
        return NULL;
    }
    unsigned char* vertexData = new unsigned char[vertexByteCount];
    if (fread(vertexData, 1, vertexByteCount, _file) != vertexByteCount)
    {
        LOG_ERROR_VARG("Failed to read %d vertex data bytes for mesh: %s", vertexByteCount, id);
        SAFE_RELEASE(vertexFormat);
        return NULL;
    }

    // Read mesh bounds (bounding box and bounding sphere)
    Vector3 boundsMin, boundsMax, boundsCenter;
    float boundsRadius;
    if (fread(&boundsMin.x, 4, 3, _file) != 3 || fread(&boundsMax.x, 4, 3, _file) != 3)
    {
        LOG_ERROR_VARG("Failed to read bounding box for mesh: %s", id);
        SAFE_RELEASE(vertexFormat);
        return NULL;
    }
    if (fread(&boundsCenter.x, 4, 3, _file) != 3 || fread(&boundsRadius, 4, 1, _file) != 1)
    {
        LOG_ERROR_VARG("Failed to read bounding sphere for mesh: %s", id);
        SAFE_RELEASE(vertexFormat);
        return NULL;
    }

    // Create Mesh
    int vertexCount = vertexByteCount / vertexFormat->getVertexSize();
    Mesh* mesh = Mesh::createMesh(vertexFormat, vertexCount, false);
    SAFE_RELEASE(vertexFormat);
    if (mesh == NULL)
    {
        LOG_ERROR_VARG("Failed to create mesh: %s", id);
        SAFE_DELETE_ARRAY(vertexData);
        return NULL;
    }
    mesh->setVertexData(vertexData, 0, vertexCount);
    SAFE_DELETE_ARRAY(vertexData);

    // Set mesh bounding volumes
    mesh->_boundingBox.set(boundsMin, boundsMax);
    mesh->_boundingSphere.set(boundsCenter, boundsRadius);

    // Read mesh parts
    unsigned int meshPartCount;
    if (fread(&meshPartCount, 4, 1, _file) != 1)
    {
        SAFE_RELEASE(mesh);
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
            LOG_ERROR_VARG("Failed to read mesh part (i=%d): %s", i, id);
            SAFE_RELEASE(mesh);
            return NULL;
        }

        unsigned char* indexData = new unsigned char[iByteCount];
        if (fread(indexData, 1, iByteCount, _file) != iByteCount)
        {
            LOG_ERROR_VARG("Failed to read %d index data bytes for mesh part (i=%d): %s", iByteCount, i, id);
            SAFE_DELETE_ARRAY(indexData);
            SAFE_RELEASE(mesh);
            return NULL;
        }

        Mesh::IndexFormat indexFormat = (Mesh::IndexFormat)iFormat;
        unsigned int indexSize;
        switch (indexFormat)
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
        unsigned int indexCount = iByteCount / indexSize;
        MeshPart* part = mesh->addPart((Mesh::PrimitiveType)pType, indexFormat, indexCount, false);
        if (part == NULL)
        {
            LOG_ERROR_VARG("Failed to create mesh part (i=%d): %s", i, id);
            SAFE_DELETE_ARRAY(indexData);
            SAFE_RELEASE(mesh);
            return NULL;
        }
        part->setIndexData(indexData, 0, indexCount);
        SAFE_DELETE_ARRAY(indexData);
    }

    fseek(_file, position, SEEK_SET);
    return mesh;
}

Font* Package::loadFont(const char* id)
{
    // Seek to the specified Font
    Reference* ref = seekTo(id, PACKAGE_TYPE_FONT);
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

void Package::setTransform(const float* values, Transform* transform)
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

bool Package::contains(const char* id) const
{
    return (find(id) != NULL);
}

unsigned int Package::getObjectCount() const
{
    return _referenceCount;
}

const char* Package::getObjectID(unsigned int index) const
{
    return (index >= _referenceCount ? NULL : _references[index].id.c_str());
}

Package::Reference::Reference() :
    type(0), offset(0)
{
}

Package::Reference::~Reference()
{
}

}
