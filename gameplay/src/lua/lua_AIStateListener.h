#ifndef LUA_AISTATELISTENER_H_
#define LUA_AISTATELISTENER_H_

namespace gameplay
{

// Lua bindings for AIState::Listener.
int lua_AIStateListener__gc(lua_State* state);
int lua_AIStateListener__init(lua_State* state);
int lua_AIStateListener_stateEnter(lua_State* state);
int lua_AIStateListener_stateExit(lua_State* state);
int lua_AIStateListener_stateUpdate(lua_State* state);

void luaRegister_AIStateListener();

}

#endif
