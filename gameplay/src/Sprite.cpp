#include "Base.h"
#include "Sprite.h"

namespace gameplay
{

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

std::shared_ptr<Serializable> Sprite::createObject()
{
    return std::shared_ptr<Sprite>();
}

Component::ClassType Sprite::getClassType()
{
    return ClassType::eSprite;
}

std::string Sprite::getClassName()
{
    return "gameplay::Sprite";
}

void Sprite::onSerialize(Serializer* serializer)
{
    Renderer::onSerialize(serializer);
}

void Sprite::onDeserialize(Serializer* serializer)
{
    Renderer::onDeserialize(serializer);
}

}
