#include "Base.h"
#include "Script.h"

namespace gameplay
{

Script::Script() : Component()
{
}

Script::~Script()
{
}

std::shared_ptr<Script> Script::load(const std::string& url)
{
	return nullptr;
}

void Script::onInitialize()
{
}

void Script::onFinalize()
{
}

void Script::onUpdate(float elapsedTime)
{
}

void Script::onMessage(std::shared_ptr<SceneObject> sender, std::shared_ptr<Message> message, void* data)
{
}

}
