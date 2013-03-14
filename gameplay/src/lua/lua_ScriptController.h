#ifndef LUA_SCRIPTCONTROLLER_H_
#define LUA_SCRIPTCONTROLLER_H_

namespace gameplay
{

// Lua bindings for ScriptController.
int lua_ScriptController_loadScript(lua_State* state);
int lua_ScriptController_loadUrl(lua_State* state);
int lua_ScriptController_registerCallback(lua_State* state);
int lua_ScriptController_static_print(lua_State* state);
int lua_ScriptController_unregisterCallback(lua_State* state);

void luaRegister_ScriptController();

}

#endif
