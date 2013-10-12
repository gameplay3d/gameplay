#include "Base.h"
#include "VisibleSetDefault.h"

namespace gameplay
{

VisibleSetDefault* VisibleSetDefault::create(Scene* scene)
{
    return new VisibleSetDefault(scene);
}

VisibleSetDefault::VisibleSetDefault(Scene* scene) 
    : _scene(NULL), _next(NULL)
{
    setScene(scene);
}

VisibleSetDefault::VisibleSetDefault(const VisibleSetDefault& copy)
{
}

VisibleSetDefault::~VisibleSetDefault()
{
}

void VisibleSetDefault::setScene(Scene* scene)
{
    _scene = scene;
    reset();
}

Scene* VisibleSetDefault::getScene()
{
    return _scene;
}

void VisibleSetDefault::reset()
{
    _next = _scene->getFirstNode();
    while (_next && !_next->isVisible())
    {
        _next = _next->getNextSibling();
    }
}

Node* VisibleSetDefault::getNext()
{
    Node* node = _next;
    if (_next)
    {
        Node* next = _next->getFirstChild();
        if (!next || !isNodeVisible(next))
        {
            next = _next->getNextSibling();
            if (!next || !isNodeVisible(next))
            {
                next = _next->getParent();
                while (next && (!next->getNextSibling() || !isNodeVisible(next)))
                {
                    next = next->getParent();
                }
                if (next)
                {
                    next = next->getNextSibling();
                }
            }
        }
       _next = next;
    }
    return node;
}

bool VisibleSetDefault::isNodeVisible(Node* node)
{
    if (!node->isVisible())
        return false;

    if (node->getForm() || node->getParticleEmitter() || node->getTerrain() || node->getLight() || node->getCamera())
    {
        return true;
    }
    else
    {
       return node->getBoundingSphere().intersects(_scene->getActiveCamera()->getFrustum());
    }
    return false;
}

}