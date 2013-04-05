#include "Base.h"
#include "GPBFile.h"
#include "Transform.h"
#include "StringUtil.h"
#include "EncoderArguments.h"
#include "Heightmap.h"

#define EPSILON 1.2e-7f;

namespace gameplay
{

static GPBFile* __instance = NULL;

/**
 * Returns true if the given value is close to one.
 */
static bool isAlmostOne(float value);

/**
 * Returns true if the given value is close to zero.
 */
static bool isAlmostZero(float value);

/**
 * Gets the common node ancestor for the given list of nodes.
 * This function assumes that the nodes share a common ancestor.
 * 
 * @param nodes The list of nodes.
 * 
 * @return The common node ancestor or NULL if the list of was empty.
 */
static Node* getCommonNodeAncestor(const std::vector<Node*>& nodes);

/**
 * Gets the list of node ancestors for the given node.
 * 
 * @param node The node to get the ancestors for.
 * @param ancestors The output list of ancestors. 
 *                  The first element is the root node and the last element is the direct parent of the node.
 */
static void getNodeAncestors(Node* node, std::list<Node*>& ancestors);


GPBFile::GPBFile(void)
    : _file(NULL), _animationsAdded(false)
{
    __instance = this;
}

GPBFile::~GPBFile(void)
{
}

GPBFile* GPBFile::getInstance()
{
    return __instance;
}

bool GPBFile::saveBinary(const std::string& filepath)
{
    _file = fopen(filepath.c_str(), "w+b");
    if (!_file)
    {
        return false;
    }
    size_t n = 0;

    // identifier
    char identifier[] = { '\xAB', 'G', 'P', 'B', '\xBB', '\r', '\n', '\x1A', '\n' };
    n = fwrite(identifier, 1, sizeof(identifier), _file);
    if (n != sizeof(identifier))
    {
        fclose(_file);
        return false;
    }

    // version
    n = fwrite(GPB_VERSION, 1, sizeof(GPB_VERSION), _file);
    if (n != sizeof(GPB_VERSION))
    {
        fclose(_file);
        return false;
    }

    // TODO: Check for errors on all file writing.

    // write refs
    _refTable.writeBinary(_file);

    // meshes
    write((unsigned int)_geometry.size(), _file);
    for (std::list<Mesh*>::const_iterator i = _geometry.begin(); i != _geometry.end(); ++i)
    {
        (*i)->writeBinary(_file);
    }

    // Objects
    write((unsigned int)_objects.size(), _file);
    for (std::list<Object*>::const_iterator i = _objects.begin(); i != _objects.end(); ++i)
    {
        (*i)->writeBinary(_file);
    }

    _refTable.updateOffsets(_file);
    
    fclose(_file);
    return true;
}

bool GPBFile::saveText(const std::string& filepath)
{
    _file = fopen(filepath.c_str(), "w");
    if (!_file)
    {
        return false;
    }

    if (fprintf(_file, "<root>\n") <= 0)
    {
        fclose(_file);
        return false;
    }

    // TODO: Check for errors on all file writing.

    // write refs
    _refTable.writeText(_file);

    // meshes
    for (std::list<Mesh*>::const_iterator i = _geometry.begin(); i != _geometry.end(); ++i)
    {
        (*i)->writeText(_file);
    }

    // Objects
    for (std::list<Object*>::const_iterator i = _objects.begin(); i != _objects.end(); ++i)
    {
        (*i)->writeText(_file);
    }

    fprintf(_file, "</root>");

    fclose(_file);
    return true;
}

void GPBFile::add(Object* obj)
{
    _objects.push_back(obj);
}

void GPBFile::addScene(Scene* scene)
{
    addToRefTable(scene);
    _objects.push_back(scene);
}

void GPBFile::addCamera(Camera* camera)
{
    addToRefTable(camera);
    _cameras.push_back(camera);
}

void GPBFile::addLight(Light* light)
{
    addToRefTable(light);
    _lights.push_back(light);
}

void GPBFile::addMesh(Mesh* mesh)
{
    addToRefTable(mesh);
    _geometry.push_back(mesh);
}

void GPBFile::addNode(Node* node)
{
    addToRefTable(node);
    _nodes.push_back(node);
}

void GPBFile::addScenelessNode(Node* node)
{
    addToRefTable(node);
    _nodes.push_back(node);
    // Nodes are normally written to file as part of a scene. 
    // Nodes that don't belong to a scene need to be written on their own (outside a scene).
    // That is why node is added to the list of objects here.
    _objects.push_back(node);
}

void GPBFile::addAnimation(Animation* animation)
{
    _animations.add(animation);

    if (!_animationsAdded)
    {
        // The animations container should only be added once and only if the file has at least one animation.
        _animationsAdded = true;
        addToRefTable(&_animations);
        add(&_animations);
    }
}

void GPBFile::addToRefTable(Object* obj)
{
    if (obj)
    {
        const std::string& id = obj->getId();
        if (id.length() > 0)
        {
            if (_refTable.get(id) == NULL)
            {
                _refTable.add(id, obj);
            }
        }
    }
}

Object* GPBFile::getFromRefTable(const std::string& id)
{
    return _refTable.get(id);
}

bool GPBFile::idExists(const std::string& id)
{
    return _refTable.get(id) != NULL;
}

Camera* GPBFile::getCamera(const char* id)
{
    if (!id)
        return NULL;
    // TODO: O(n) search is not ideal
    for (std::list<Camera*>::const_iterator i = _cameras.begin(); i != _cameras.end(); ++i)
    {
        const std::string& _id = (*i)->getId();
        if (_id.length() > 0 && strncmp(id, _id.c_str(), 255) == 0)
        {
            return *i;
        }
    }
    return NULL;
}

Light* GPBFile::getLight(const char* id)
{
    if (!id)
        return NULL;
    // TODO: O(n) search is not ideal
    for (std::list<Light*>::const_iterator i = _lights.begin(); i != _lights.end(); ++i)
    {
        const std::string& _id = (*i)->getId();
        if (_id.length() > 0 && strncmp(id, _id.c_str(), 255) == 0)
        {
            return *i;
        }
    }
    return NULL;
}

Mesh* GPBFile::getMesh(const char* id)
{
    if (!id)
        return NULL;
    // TODO: O(n) search is not ideal
    for (std::list<Mesh*>::const_iterator i = _geometry.begin(); i != _geometry.end(); ++i)
    {
        const std::string& _id = (*i)->getId();
        if (_id.length() > 0 && strncmp(id, _id.c_str(), 255) == 0)
        {
            return *i;
        }
    }
    return NULL;
}

Node* GPBFile::getNode(const char* id)
{
    if (!id)
        return NULL;
    // TODO: O(n) search is not ideal
    for (std::list<Node*>::const_iterator i = _nodes.begin(); i != _nodes.end(); ++i)
    {
        const std::string& _id = (*i)->getId();
        if (_id.length() > 0 && strncmp(id, _id.c_str(), 255) == 0)
        {
            return *i;
        }
    }
    return NULL;
}

Animations* GPBFile::getAnimations()
{
    return &_animations;
}

unsigned int GPBFile::getLightCount() const
{
    return (unsigned int)_lights.size();
}

void GPBFile::adjust()
{
    // calculate the ambient color for each scene
    for (std::list<Object*>::iterator i = _objects.begin(); i != _objects.end(); ++i)
    {
        Object* obj = *i;
        if (obj->getTypeId() == Object::SCENE_ID)
        {
            Scene* scene = dynamic_cast<Scene*>(obj);
            scene->calcAmbientColor();
        }
    }

    for (std::list<Node*>::const_iterator i = _nodes.begin(); i != _nodes.end(); ++i)
    {
        computeBounds(*i);
    }

    if (EncoderArguments::getInstance()->optimizeAnimationsEnabled())
    {
        LOG(1, "Optimizing animations.\n");
        optimizeAnimations();
    }

    // TODO:
    // remove ambient _lights
    // for each node
    //   if node has ambient light
    //     if node has no camera, mesh or children but 1 ambient light
    //       delete node and remove from ref table
    //     delete light and remove from ref table
    //
    // merge animations if possible
    //   Search for animations that have the same target and key times and see if they can be merged.
    //   Blender will output a simple translation animation to 3 separate animations with the same key times but targeting X, Y and Z.
    //   This can be merged into one animation. Same for scale animations.

    // Generate heightmaps
    const std::vector<EncoderArguments::HeightmapOption>& heightmaps = EncoderArguments::getInstance()->getHeightmapOptions();
    for (unsigned int i = 0, count = heightmaps.size(); i < count; ++i)
    {
        Heightmap::generate(heightmaps[i].nodeIds, heightmaps[i].width, heightmaps[i].height, heightmaps[i].filename.c_str(), heightmaps[i].isHighPrecision);
    }
}

void GPBFile::groupMeshSkinAnimations()
{
    for (std::list<Node*>::iterator it = _nodes.begin(); it != _nodes.end(); ++it)
    {
        if (Model* model = (*it)->getModel())
        {
            if (MeshSkin* skin = model->getSkin())
            {
                const std::vector<Node*>& joints = skin->getJoints();
                Node* commonAncestor = getCommonNodeAncestor(joints);
                if (commonAncestor)
                {
                    // group the animation channels that target this common ancestor and its child nodes
                    Animation* animation = new Animation();
                    animation->setId("animations");

                    moveAnimationChannels(commonAncestor, animation);
                    _animations.add(animation);
                }
            }
        }
    }
}

void GPBFile::renameAnimations(std::vector<std::string>& animationIds, const char* newId)
{
    const unsigned int animationCount = _animations.getAnimationCount();
    for (unsigned int animationIndex = 0; animationIndex < animationCount; ++animationIndex)
    {
        Animation* animation = _animations.getAnimation(animationIndex);
        assert(animation);
        std::vector<std::string>::const_iterator it = find(animationIds.begin(), animationIds.end(), animation->getId());
        if (it != animationIds.end())
        {
            animation->setId(newId);
        }
    }
}

void GPBFile::computeBounds(Node* node)
{
    assert(node);
    if (Model* model = node->getModel())
    {
        if (Mesh* mesh = model->getMesh())
        {
            mesh->computeBounds();
        }
    }
}

void GPBFile::optimizeAnimations()
{
    const unsigned int animationCount = _animations.getAnimationCount();
    for (unsigned int animationIndex = 0; animationIndex < animationCount; ++animationIndex)
    {
        Animation* animation = _animations.getAnimation(animationIndex);
        assert(animation);

        const int channelCount = animation->getAnimationChannelCount();

        LOG(2, "Optimizing %d channel(s) in animation '%s'.\n", channelCount, animation->getId().c_str());

        // loop backwards because we will be adding and removing channels
        for (int channelIndex = channelCount -1; channelIndex >= 0 ; --channelIndex)
        {
            AnimationChannel* channel = animation->getAnimationChannel(channelIndex);
            assert(channel);

            // Optimize node animation channels
            const Object* obj = _refTable.get(channel->getTargetId());
            if (obj && obj->getTypeId() == Object::NODE_ID)
            {
                if (channel->getTargetAttribute() == Transform::ANIMATE_SCALE_ROTATE_TRANSLATE)
                {
                    decomposeTransformAnimationChannel(animation, channel, channelIndex);

                    animation->remove(channel);
                    SAFE_DELETE(channel);
                }
            }
        }
    }
}

void GPBFile::decomposeTransformAnimationChannel(Animation* animation, AnimationChannel* channel, int channelIndex)
{
    LOG(2, "  Optimizing animaton channel %s:%d.\n", animation->getId().c_str(), channelIndex+1);

    const std::vector<float>& keyTimes = channel->getKeyTimes();
    const std::vector<float>& keyValues = channel->getKeyValues();
    const size_t keyTimesSize = keyTimes.size();
    const size_t keyValuesSize = keyValues.size();

    std::vector<float> scaleKeyValues;
    std::vector<float> rotateKeyValues;
    std::vector<float> translateKeyValues;

    scaleKeyValues.reserve(keyTimesSize * 3);
    rotateKeyValues.reserve(keyTimesSize * 4);
    translateKeyValues.reserve(keyTimesSize * 3);

    for (size_t kv = 0; kv < keyValuesSize; kv += 10)
    {
        scaleKeyValues.push_back(keyValues[kv]);
        scaleKeyValues.push_back(keyValues[kv+1]);
        scaleKeyValues.push_back(keyValues[kv+2]);

        rotateKeyValues.push_back(keyValues[kv+3]);
        rotateKeyValues.push_back(keyValues[kv+4]);
        rotateKeyValues.push_back(keyValues[kv+5]);
        rotateKeyValues.push_back(keyValues[kv+6]);

        translateKeyValues.push_back(keyValues[kv+7]);
        translateKeyValues.push_back(keyValues[kv+8]);
        translateKeyValues.push_back(keyValues[kv+9]);
    }

    // replace transform animation channel with translate, rotate and scale animation channels

    // Don't add the scale channel if all the key values are close to 1.0
    size_t oneCount = (size_t)std::count_if(scaleKeyValues.begin(), scaleKeyValues.end(), isAlmostOne);
    if (scaleKeyValues.size() == oneCount)
    {
        LOG(2, "    Discarding scale channel.\n");
    }
    else
    {
        LOG(3, "    Keeping scale channel.\n");
        AnimationChannel* scaleChannel = new AnimationChannel();
        scaleChannel->setTargetId(channel->getTargetId());
        scaleChannel->setKeyTimes(channel->getKeyTimes());
        scaleChannel->setTangentsIn(channel->getTangentsIn());
        scaleChannel->setTangentsOut(channel->getTangentsOut());
        scaleChannel->setInterpolations(channel->getInterpolationTypes());
        scaleChannel->setTargetAttribute(Transform::ANIMATE_SCALE);
        scaleChannel->setKeyValues(scaleKeyValues);
        scaleChannel->removeDuplicates();
        animation->add(scaleChannel);
    }

    // Don't add the rotation channel if all quaternions are close to identity
    oneCount = 0;
    for (unsigned int i = 0, count = rotateKeyValues.size(); i < count; i += 4)
    {
        float x = rotateKeyValues[i];
        float y = rotateKeyValues[i+1];
        float z = rotateKeyValues[i+2];
        float w = rotateKeyValues[i+3];
        if (ISZERO(x) && ISZERO(y) && ISZERO(z) && ISONE(w))
            ++oneCount;
        else
        {
            LOG(4, "Rotation not identity: %u\n", i);
            Quaternion q(x, y, z, w);
            Vector3 axis;
            float angle = q.toAxisAngle(&axis);
            angle = 0;
        }
    }
    if ((rotateKeyValues.size()>>2) == oneCount)
    {
        LOG(2, "    Discarding rotation channel.\n");
    }
    else
    {
        LOG(3, "    Keeping rotation channel.\n");
        AnimationChannel* rotateChannel = new AnimationChannel();
        rotateChannel->setTargetId(channel->getTargetId());
        rotateChannel->setKeyTimes(channel->getKeyTimes());
        rotateChannel->setTangentsIn(channel->getTangentsIn());
        rotateChannel->setTangentsOut(channel->getTangentsOut());
        rotateChannel->setInterpolations(channel->getInterpolationTypes());
        rotateChannel->setTargetAttribute(Transform::ANIMATE_ROTATE);
        rotateChannel->setKeyValues(rotateKeyValues);
        rotateChannel->removeDuplicates();
        animation->add(rotateChannel);
    }

    // Don't add the translation channel if all values are close to zero
    oneCount = (size_t)std::count_if(translateKeyValues.begin(), translateKeyValues.end(), isAlmostZero);
    if (translateKeyValues.size() == oneCount)
    {
        LOG(2, "    Discarding translation channel.\n");
    }
    else
    {
        LOG(3, "    Keeping translation channel.\n");
        AnimationChannel* translateChannel = new AnimationChannel();
        translateChannel->setTargetId(channel->getTargetId());
        translateChannel->setKeyTimes(channel->getKeyTimes());
        translateChannel->setTangentsIn(channel->getTangentsIn());
        translateChannel->setTangentsOut(channel->getTangentsOut());
        translateChannel->setInterpolations(channel->getInterpolationTypes());
        translateChannel->setTargetAttribute(Transform::ANIMATE_TRANSLATE);
        translateChannel->setKeyValues(translateKeyValues);
        translateChannel->removeDuplicates();
        animation->add(translateChannel);
    }
}

void GPBFile::moveAnimationChannels(Node* node, Animation* dstAnimation)
{
    // Loop through the animations and channels backwards because they will be removed when found.
    int animationCount = _animations.getAnimationCount();
    for (int i = animationCount - 1; i >= 0; --i)
    {
        Animation* animation = _animations.getAnimation(i);
        int channelCount = animation->getAnimationChannelCount();
        for (int j = channelCount - 1; j >= 0; --j)
        {
            AnimationChannel* channel = animation->getAnimationChannel(j);
            if (equals(channel->getTargetId(), node->getId()))
            {
                animation->remove(channel);
                dstAnimation->add(channel);
            }
        }
        if (animation->getAnimationChannelCount() == 0)
        {
            _animations.removeAnimation(i);
        }
    }
    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        moveAnimationChannels(child, dstAnimation);
    }
}

bool isAlmostOne(float value)
{
    return (value - 1.0f) < EPSILON;
}

bool isAlmostZero(float value)
{
    return std::fabs(value) < EPSILON;
}

Node* getCommonNodeAncestor(const std::vector<Node*>& nodes)
{
    if (nodes.empty())
        return NULL;
    if (nodes.size() == 1)
        return nodes.front();

    std::list<Node*> ancestors;
    size_t minAncestorCount = INT_MAX;
    for (std::vector<Node*>::const_iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        Node* node = *it;
        getNodeAncestors(node, ancestors);
        ancestors.push_back(node);
        minAncestorCount = std::min(minAncestorCount, ancestors.size());
    }
    ancestors.resize(minAncestorCount);

    return ancestors.back();
}

void getNodeAncestors(Node* node, std::list<Node*>& ancestors)
{
    ancestors.clear();
    Node* parent = node->getParent();
    while (parent != NULL)
    {
        ancestors.push_front(parent);
        parent = parent->getParent();
    }
}

}
