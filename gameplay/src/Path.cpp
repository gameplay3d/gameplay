#include "Base.h"
#include "Path.h"

namespace gameplay
{

Path::Path()
{
}

Path::~Path()
{
}

Component::TypeId Path::getTypeId()
{
    return Component::TYPEID_TEXT;
}

std::string Path::getClassName()
{
    return "gameplay::Path";
}

void Path::onSerialize(Serializer* serializer)
{
    Renderer::onSerialize(serializer);
}

void Path::onDeserialize(Serializer* serializer)
{
    Renderer::onDeserialize(serializer);
}

std::shared_ptr<Serializable> Path::createObject()
{
    return std::shared_ptr<Path>();
}


}