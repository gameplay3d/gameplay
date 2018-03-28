#include "Base.h"
#include "Scene.h"

#define SCENE_NAME ""
#define SCENE_STREAMING false
#define SCENEOBJECT_DIRTY_MATRIX_LOCAL 1
#define SCENEOBJECT_DIRTY_MATRIX_WORLD 2

namespace gameplay
{

Scene::Scene() :
    _name(SCENE_NAME),
    _streaming(SCENE_STREAMING),
    _camera(nullptr)
{
}

Scene::~Scene()
{
}

std::string Scene::getName() const
{
    return _name;
}

void Scene::setName(const std::string& name)
{
    _name = name;
}

bool Scene::isStreaming() const
{
    return _streaming;
}

void Scene::setStreaming(bool streaming)
{
    _streaming = streaming;
}


void Scene::setCamera(std::shared_ptr<Camera> camera)
{
    _camera = camera;
}

std::shared_ptr<Camera> Scene::getCamera() const
{
    return _camera;
}

void Scene::addChild(std::shared_ptr<SceneObject> object)
{
    if (object->_parent.lock())
        removeChild(object);

    _children.push_back(object);

    //object->_parent = nullptr;
    object->_dirtyBits |= SCENEOBJECT_DIRTY_MATRIX_LOCAL;
}

void Scene::removeChild(std::shared_ptr<SceneObject> object)
{
     auto itr = std::find(_children.begin(), _children.end(), object);
     if (itr != _children.end())
     {
         (*itr)->_parent.reset();
         _children.erase(itr);
     }
}

void Scene::clearChildren()
{
    for (auto itr = _children.begin(); itr != _children.end();)
    {
        (*itr)->_parent.reset();
        _children.erase(itr);
    }
}

size_t Scene::getChildCount() const
{
    return _children.size();
}

std::vector<std::shared_ptr<SceneObject>> Scene::getChildren()
{
    std::vector<std::shared_ptr<SceneObject>> children;
    for (auto child : _children)
    {
        if (child)
        {
            children.push_back(child);
        }
    }
    return children;
}

std::shared_ptr<SceneObject> Scene::findObject(const std::string& name, bool recursive, bool exactMatch) 
{
    for (auto child : _children)
    {
        if ((exactMatch && child->_name == name) || (!exactMatch && child->_name.find(name) == 0))
        {
            return child;
        }
    }
    if (recursive)
    {
        for (auto child : _children)
        {
            std::shared_ptr<SceneObject> match = child->findObject(name, true, exactMatch);
            if (match)
            {
                return match;
            }
        }
    }
    return nullptr;
}

size_t Scene::findObjects(const std::string& name, std::vector<std::shared_ptr<SceneObject>>& objects, bool recursive, bool exactMatch)
{
    size_t count = 0;
    for (auto child : _children)
    {
        if ((exactMatch && child->_name == name) || (!exactMatch && child->_name.find(name) == 0))
        {
            objects.push_back(child);
            ++count;
        }
    }
    if (recursive)
    {
        for (auto child : _children)
        {
            count += child->findObjects(name, objects, recursive, exactMatch);
        }
    }
    return count;
}

std::shared_ptr<Serializable> Scene::createObject()
{
    return std::static_pointer_cast<Serializable>(std::make_shared<Scene>());
}

std::string Scene::getClassName()
{
    return "gameplay::Scene";
}

void Scene::onSerialize(Serializer* serializer)
{
    serializer->writeString("name", _name.c_str(), SCENE_NAME);
    serializer->writeBool("streaming", isStreaming(), SCENE_STREAMING);
    if (_children.size() > 0)
    {
        serializer->writeObjectList("children", _children.size());
        for (auto child : _children)
        {
             serializer->writeObject(nullptr, std::static_pointer_cast<Serializable>(child));
        }
    }
}

void Scene::onDeserialize(Serializer* serializer)
{
    serializer->readString("name", _name, SCENE_NAME);
    _streaming = serializer->readBool("streaming", SCENE_STREAMING);
    size_t childCount = serializer->readObjectList("children");
    if (childCount > 0)
    {
        _children.resize(childCount);
        for (size_t i = 0; i < _children.size(); i++)
        {
            _children[i] = std::static_pointer_cast<SceneObject>(serializer->readObject(nullptr));
        }
    }
}

}
