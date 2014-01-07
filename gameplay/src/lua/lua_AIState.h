#ifndef LUA_AISTATE_H_
#define LUA_AISTATE_H_

namespace gameplay
{

// Lua bindings for AIState.
int lua_AIState__gc(lua_State* state);
int lua_AIState_addRef(lua_State* state);
int lua_AIState_addScriptCallback(lua_State* state);
int lua_AIState_getId(lua_State* state);
int lua_AIState_getRefCount(lua_State* state);
int lua_AIState_release(lua_State* state);
int lua_AIState_removeScriptCallback(lua_State* state);
int lua_AIState_setListener(lua_State* state);
int lua_AIState_static_create(lua_State* state);

void luaRegister_AIState();

}

#endif
