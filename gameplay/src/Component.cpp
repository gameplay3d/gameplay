#include "Base.h"
#include "Component.h"

namespace gameplay
{

Component::Component() :
    _object(nullptr)
{
}

Component::~Component()
{
}

std::shared_ptr<SceneObject> Component::getObject() const
{
    return _object;
}

void Component::setObject(std::shared_ptr<SceneObject> object)
{
    _object = object;
}

}
