#include "Base.h"
#include "Text.h"

namespace gameplay
{

Text::Text()
{
}

Text::~Text()
{
}

std::shared_ptr<Serializable> Text::createObject()
{
    return std::shared_ptr<Text>();
}

Component::ClassType Text::getClassType()
{
    return ClassType::eText;
}

std::string Text::getClassName()
{
    return "gameplay::Text";
}

void Text::onSerialize(Serializer* serializer)
{
    Renderer::onSerialize(serializer);
}

void Text::onDeserialize(Serializer* serializer)
{
    Renderer::onDeserialize(serializer);
}

}
