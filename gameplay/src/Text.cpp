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

Component::TypeId Text::getTypeId()
{
	return Component::TYPEID_TEXT;
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

std::shared_ptr<Serializable> Text::createObject()
{
	return std::shared_ptr<Text>();
}

}