#include "Base.h"
#include "Tileset.h"

namespace gameplay
{

Tileset::Tileset()
{
}

Tileset::~Tileset()
{
}

std::shared_ptr<Serializable> Tileset::createObject()
{
    return std::shared_ptr<Tileset>();
}

std::string Tileset::getClassName()
{
    return "gameplay::Tileset";
}

void Tileset::onSerialize(Serializer* serializer)
{
    Renderer::onSerialize(serializer);
}

void Tileset::onDeserialize(Serializer* serializer)
{
    Renderer::onDeserialize(serializer);
}

}
