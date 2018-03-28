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

std::shared_ptr<Serializable> Path::createObject()
{
    return std::shared_ptr<Path>();
}

Component::ClassType Path::getClassType()
{
    return ClassType::ePath;
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

}
