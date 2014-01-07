#ifndef LUA_AIAGENT_H_
#define LUA_AIAGENT_H_

namespace gameplay
{

// Lua bindings for AIAgent.
int lua_AIAgent__gc(lua_State* state);
int lua_AIAgent_addRef(lua_State* state);
int lua_AIAgent_addScriptCallback(lua_State* state);
int lua_AIAgent_getId(lua_State* state);
int lua_AIAgent_getNode(lua_State* state);
int lua_AIAgent_getRefCount(lua_State* state);
int lua_AIAgent_getStateMachine(lua_State* state);
int lua_AIAgent_isEnabled(lua_State* state);
int lua_AIAgent_release(lua_State* state);
int lua_AIAgent_removeScriptCallback(lua_State* state);
int lua_AIAgent_setEnabled(lua_State* state);
int lua_AIAgent_setListener(lua_State* state);
int lua_AIAgent_static_create(lua_State* state);

void luaRegister_AIAgent();

}

#endif
