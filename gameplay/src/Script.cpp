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

std::shared_ptr<Script> Script::loadCppScript(std::string classUrl)
{
    return std::shared_ptr<Script>();
}

std::shared_ptr<Script> Script::loadLuaScript(std::string scriptUrl)
{
    return std::shared_ptr<Script>();
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
