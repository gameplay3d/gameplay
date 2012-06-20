#ifndef LUA_SCRIPTCONTROLLER_H_
#define LUA_SCRIPTCONTROLLER_H_

namespace gameplay
{

// Lua bindings for ScriptController.
int lua_ScriptController_getBoolPointer(lua_State* state);
int lua_ScriptController_getDoublePointer(lua_State* state);
int lua_ScriptController_getFloatPointer(lua_State* state);
int lua_ScriptController_getIntPointer(lua_State* state);
int lua_ScriptController_getLongPointer(lua_State* state);
int lua_ScriptController_getShortPointer(lua_State* state);
int lua_ScriptController_getUnsignedCharPointer(lua_State* state);
int lua_ScriptController_getUnsignedIntPointer(lua_State* state);
int lua_ScriptController_getUnsignedLongPointer(lua_State* state);
int lua_ScriptController_getUnsignedShortPointer(lua_State* state);
int lua_ScriptController_loadScript(lua_State* state);
int lua_ScriptController_static_getInstance(lua_State* state);

void luaRegister_ScriptController();

}

#endif
