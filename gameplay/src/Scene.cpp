/*
 * Scene.cpp
 */

#include <algorithm>

#include "Base.h"
#include "AudioListener.h"
#include "Scene.h"

namespace gameplay
{

Scene::Scene() : _activeCamera(NULL), _firstNode(NULL), _lastNode(NULL), _nodeCount(0), _bindAudioListenerToCamera(true)
{
}

Scene::Scene(const Scene& copy)
{
}

Scene::~Scene()
{
    removeAllNodes();
}

Scene* Scene::createScene()
{
    return new Scene();
}

const char* Scene::getId() const
{
    return _id.c_str();
}

void Scene::setId(const char* id)
{
    if (id)
    {
        _id = id;
    }
}

Node* Scene::findNode(const char* id, bool recursive, bool exactMatch) const
{
    assert(id);

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
    assert(id);

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

Node* Scene::addNode(const char* id)
{
    Node* node = Node::create(id);
    addNode(node);

    // Call release to decrement the ref count to 1 before returning.
    node->release();

    return node;
}

void Scene::addNode(Node* node)
{
    assert(node);

    // If the node is part of another scene, remove it.
    if (node->_scene && node->_scene != this)
    {
        node->_scene->removeNode(node);
    }

    // If the node is part of another node hierarchy, remove it.
    if (node->getParent())
    {
        node->remove();
    }

    // Link the new node into our list.
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

    node->addRef();

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
    assert(node);

    if (node->_scene != this)
        return;

    node->remove();
    node->_scene = NULL;

    if (node == _firstNode)
    {
        _firstNode = node->_nextSibling;
    }
    if (node == _lastNode)
    {
        _lastNode = node->_prevSibling;
    }

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

Camera* Scene::getActiveCamera() const
{
    return _activeCamera;
}

void Scene::setActiveCamera(Camera* camera)
{
    // Make sure we don't release the camera if the same camera is set twice.
    if (_activeCamera != camera)
    {
        if (_activeCamera)
        {
            SAFE_RELEASE(_activeCamera);
        }

        _activeCamera = camera;
        if (_activeCamera)
        {
            _activeCamera->addRef();
            if (_bindAudioListenerToCamera && AudioListener::getInstance())
                AudioListener::getInstance()->setCamera(_activeCamera);
        }
    }
}

void Scene::bindAudioListenerToCamera(bool bind)
{
    if (_bindAudioListenerToCamera != bind)
    {
        _bindAudioListenerToCamera = bind;
        if (AudioListener::getInstance())
            AudioListener::getInstance()->setCamera(bind ? _activeCamera : NULL);
    }
}

const Viewport& Scene::getViewport() const
{
    return _viewport;
}

void Scene::setViewport(const Viewport& viewport)
{
    _viewport = viewport;
}

const Vector3& Scene::getAmbientColor()
{
    return _ambientColor;
}

void Scene::setAmbientColor(float red, float green, float blue)
{
    _ambientColor.set(red, green, blue);
}

}
