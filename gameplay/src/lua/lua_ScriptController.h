#ifndef LUA_SCRIPTCONTROLLER_H_
#define LUA_SCRIPTCONTROLLER_H_

namespace gameplay
{

// Lua bindings for ScriptController.
int lua_ScriptController_loadScript(lua_State* state);

void luaRegister_ScriptController();

}

#endif
