#include "Base.h"
#include "AudioListener.h"
#include "Scene.h"
#include "SceneLoader.h"
#include "MeshSkin.h"
#include "Joint.h"
#include "Terrain.h"
#include "Bundle.h"

namespace gameplay
{

// Global list of active scenes
static std::vector<Scene*> __sceneList;

static inline char lowercase(char c)
{
    if (c >= 'A' && c <='Z')
    {
        c |= 0x20;
    }
    return c;
}

// Returns true if 'str' ends with 'suffix'; false otherwise.
static bool endsWith(const char* str, const char* suffix, bool ignoreCase)
{
    if (str == NULL || suffix == NULL)
        return false;
    size_t length = strlen(str);
    size_t suffixLength = strlen(suffix);

    if (suffixLength > length)
    {
        return false;
    }

    size_t offset = length - suffixLength;

    const char* p = str + offset;
    while (*p != '\0' && *suffix != '\0')
    {
        if (ignoreCase)
        {
            if (lowercase(*p) != lowercase(*suffix))
            {
                return false;
            }
        }
        else if (*p != *suffix)
        {
            return false;
        }
        
        ++p;
        ++suffix;
    }
    return true;
}


Scene::Scene()
    : _id(""), _activeCamera(NULL), _firstNode(NULL), _lastNode(NULL), _nodeCount(0), _bindAudioListenerToCamera(true), 
      _nextItr(NULL), _nextReset(true)
{
    __sceneList.push_back(this);
}

Scene::~Scene()
{
    // Unbind our active camera from the audio listener
    if (_activeCamera)
    {
        AudioListener* audioListener = AudioListener::getInstance();
        if (audioListener && (audioListener->getCamera() == _activeCamera))
        {
            audioListener->setCamera(NULL);
        }

        SAFE_RELEASE(_activeCamera);
    }

    // Remove all nodes from the scene
    removeAllNodes();

    // Remove the scene from global list
    std::vector<Scene*>::iterator itr = std::find(__sceneList.begin(), __sceneList.end(), this);
    if (itr != __sceneList.end())
        __sceneList.erase(itr);
}

Scene* Scene::create(const char* id)
{
    Scene* scene = new Scene();
    scene->setId(id);
    return scene;
}

Scene* Scene::load(const char* filePath)
{
    if (endsWith(filePath, ".gpb", true))
    {
        Scene* scene = NULL;
        Bundle* bundle = Bundle::create(filePath);
        if (bundle)
        {
            scene = bundle->loadScene();
            SAFE_RELEASE(bundle);
        }
        return scene;
    }
    return SceneLoader::load(filePath);
}

Scene* Scene::getScene(const char* id)
{
    if (id == NULL)
        return __sceneList.size() ? __sceneList[0] : NULL;

    for (size_t i = 0, count = __sceneList.size(); i < count; ++i)
    {
        if (__sceneList[i]->_id == id)
            return __sceneList[i];
    }

    return NULL;
}


const char* Scene::getId() const
{
    return _id.c_str();
}

void Scene::setId(const char* id)
{
    _id = id ? id : "";
}

Node* Scene::findNode(const char* id, bool recursive, bool exactMatch) const
{
    GP_ASSERT(id);

    // Search immediate children first.
    for (Node* child = getFirstNode(); child != NULL; child = child->getNextSibling())
    {
        // Does this child's ID match?
        if ((exactMatch && child->_id == id) || (!exactMatch && child->_id.find(id) == 0))
        {
            return child;
        }
    }

    // Recurse.
    if (recursive)
    {
        for (Node* child = getFirstNode(); child != NULL; child = child->getNextSibling())
        {
            Node* match = child->findNode(id, true, exactMatch);
            if (match)
            {
                return match;
            }
        }
    }
    return NULL;
}

unsigned int Scene::findNodes(const char* id, std::vector<Node*>& nodes, bool recursive, bool exactMatch) const
{
    GP_ASSERT(id);

    unsigned int count = 0;

    // Search immediate children first.
    for (Node* child = getFirstNode(); child != NULL; child = child->getNextSibling())
    {
        // Does this child's ID match?
        if ((exactMatch && child->_id == id) || (!exactMatch && child->_id.find(id) == 0))
        {
            nodes.push_back(child);
            ++count;
        }
    }

    // Recurse.
    if (recursive)
    {
        for (Node* child = getFirstNode(); child != NULL; child = child->getNextSibling())
        {
            count += child->findNodes(id, nodes, true, exactMatch);
        }
    }

    return count;
}

void Scene::visitNode(Node* node, const char* visitMethod)
{
    ScriptController* sc = Game::getInstance()->getScriptController();

    // Invoke the visit method for this node.
    bool result;
    if (!sc->executeFunction<bool>(visitMethod, "<Node>", &result, (void*)node) || !result)
        return;

    // If this node has a model with a mesh skin, visit the joint hierarchy within it
    // since we don't add joint hierarcies directly to the scene. If joints are never
    // visited, it's possible that nodes embedded within the joint hierarchy that contain
    // models will never get visited (and therefore never get drawn).
    Model* model = dynamic_cast<Model*>(node->getDrawable());
    if (model && model->_skin && model->_skin->_rootNode)
    {
        visitNode(model->_skin->_rootNode, visitMethod);
    }

    // Recurse for all children.
    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        visitNode(child, visitMethod);
    }
}

Node* Scene::addNode(const char* id)
{
    Node* node = Node::create(id);
    GP_ASSERT(node);
    addNode(node);

    // Call release to decrement the ref count to 1 before returning.
    node->release();

    return node;
}

void Scene::addNode(Node* node)
{
    GP_ASSERT(node);

    if (node->_scene == this)
    {
        // The node is already a member of this scene.
        return;
    }

    node->addRef();

    // If the node is part of another scene, remove it.
    if (node->_scene && node->_scene != this)
    {
        node->_scene->removeNode(node);
    }

    // If the node is part of another node hierarchy, remove it.
    if (node->getParent())
    {
        node->getParent()->removeChild(node);
    }

    // Link the new node into the end of our list.
    if (_lastNode)
    {
        _lastNode->_nextSibling = node;
        node->_prevSibling = _lastNode;
        _lastNode = node;
    }
    else
    {
        _firstNode = _lastNode = node;
    }

    node->_scene = this;

    ++_nodeCount;

    // If we don't have an active camera set, then check for one and set it.
    if (_activeCamera == NULL)
    {
        Camera* camera = node->getCamera();
        if (camera)
        {
            setActiveCamera(camera);
        }
    }
}

void Scene::removeNode(Node* node)
{
    GP_ASSERT(node);

    if (node->_scene != this)
        return;

    if (node == _firstNode)
    {
        _firstNode = node->_nextSibling;
    }
    if (node == _lastNode)
    {
        _lastNode = node->_prevSibling;
    }

    node->remove();
    node->_scene = NULL;

    SAFE_RELEASE(node);

    --_nodeCount;
}

void Scene::removeAllNodes()
{
    while (_lastNode)
    {
        removeNode(_lastNode);
    }
}

unsigned int Scene::getNodeCount() const
{
    return _nodeCount;
}

Node* Scene::getFirstNode() const
{
    return _firstNode;
}

Camera* Scene::getActiveCamera()
{
    return _activeCamera;
}

void Scene::setActiveCamera(Camera* camera)
{
    // Make sure we don't release the camera if the same camera is set twice.
    if (_activeCamera != camera)
    {
        AudioListener* audioListener = AudioListener::getInstance();

        if (_activeCamera)
        {
            // Unbind the active camera from the audio listener
            if (audioListener && (audioListener->getCamera() == _activeCamera))
            {
                audioListener->setCamera(NULL);
            }

            SAFE_RELEASE(_activeCamera);
        }

        _activeCamera = camera;

        if (_activeCamera)
        {
            _activeCamera->addRef();

            if (audioListener && _bindAudioListenerToCamera)
            {
                audioListener->setCamera(_activeCamera);
            }
        }
    }
}

void Scene::bindAudioListenerToCamera(bool bind)
{
    if (_bindAudioListenerToCamera != bind)
    {
        _bindAudioListenerToCamera = bind;

        if (AudioListener::getInstance())
        {
            AudioListener::getInstance()->setCamera(bind ? _activeCamera : NULL);
        }
    }
}

const Vector3& Scene::getAmbientColor() const
{
    return _ambientColor;
}

void Scene::setAmbientColor(float red, float green, float blue)
{
    _ambientColor.set(red, green, blue);
}

void Scene::update(float elapsedTime)
{
    for (Node* node = _firstNode; node != NULL; node = node->_nextSibling)
    {
        if (node->isEnabled())
            node->update(elapsedTime);
    }
}

void Scene::reset()
{
    _nextItr = NULL;
    _nextReset = true;
}

Node* Scene::getNext()
{
    if (_nextReset)
    {
        _nextItr = findNextVisibleSibling(getFirstNode());
        _nextReset = false;
    }
    else if (_nextItr)
    {
        Node* node = findNextVisibleSibling(_nextItr->getFirstChild());
        if (node == NULL)
        {
            node = findNextVisibleSibling(_nextItr->getNextSibling());
            if (node == NULL)
            {
                // Find first parent with a sibling
                node = _nextItr->getParent();
                while (node && (!findNextVisibleSibling(node->getNextSibling())))
                {
                    node = node->getParent();
                }
                if (node)
                {
                    node = findNextVisibleSibling(node->getNextSibling());
                }
            }
        }
        _nextItr = node;
    }
    return _nextItr;
}


Node* Scene::findNextVisibleSibling(Node* node)
{
    while (node != NULL && !isNodeVisible(node))
    {
        node = node->getNextSibling();
    }
    return node;
}

bool Scene::isNodeVisible(Node* node)
{
    if (!node->isEnabled())
        return false;

    if (node->getDrawable() || node->getLight() || node->getCamera())
    {
        return true;
    }
    else
    {
        return node->getBoundingSphere().intersects(_activeCamera->getFrustum());
    }
}

}
