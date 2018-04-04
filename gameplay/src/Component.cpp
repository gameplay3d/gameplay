#include "Base.h"
#include "Component.h"

namespace gameplay
{

Component::Component() :
    _enabled(true)
{
}

Component::~Component()
{
}

bool Component::isEnabled() const
{
    return _enabled;
}

void Component::setEnabled(bool enabled)
{
    _enabled = enabled;
}

std::shared_ptr<SceneObject> Component::getObject() const
{
    return _object.lock();
}

void Component::setObject(std::shared_ptr<SceneObject> object)
{
    _object = object;
}

void Component::onAdded()
{
}

void Component::onRemoved()
{
}

}
