#ifndef LUA_SCRIPTTARGETEVENTREGISTRYEVENT_H_
#define LUA_SCRIPTTARGETEVENTREGISTRYEVENT_H_

namespace gameplay
{

// Lua bindings for ScriptTarget::EventRegistry::Event.
int lua_ScriptTargetEventRegistryEvent__gc(lua_State* state);
int lua_ScriptTargetEventRegistryEvent__init(lua_State* state);
int lua_ScriptTargetEventRegistryEvent_args(lua_State* state);
int lua_ScriptTargetEventRegistryEvent_name(lua_State* state);
int lua_ScriptTargetEventRegistryEvent_registry(lua_State* state);

void luaRegister_ScriptTargetEventRegistryEvent();

}

#endif
