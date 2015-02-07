#include "Script.h"
#include "ScriptController.h"

namespace gameplay
{

Script::Script() : _scope(GLOBAL), _env(0)
{
}

Script::~Script()
{
    Game::getInstance()->getScriptController()->unloadScript(this);
}

const char* Script::getPath() const
{
    return _path.c_str();
}

Script::Scope Script::getScope() const
{
    return _scope;
}

bool Script::functionExists(const char* name) const
{
    return Game::getInstance()->getScriptController()->functionExists(name, this);
}

bool Script::reload()
{
    ScriptController* sc = Game::getInstance()->getScriptController();

    // First unload our current script
    sc->unloadScript(this);

    // Now attempt to reload the script
    return Game::getInstance()->getScriptController()->loadScript(this);
}

}
