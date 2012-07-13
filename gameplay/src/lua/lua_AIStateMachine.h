#ifndef LUA_AISTATEMACHINE_H_
#define LUA_AISTATEMACHINE_H_

namespace gameplay
{

// Lua bindings for AIStateMachine.
int lua_AIStateMachine_addState(lua_State* state);
int lua_AIStateMachine_getActiveState(lua_State* state);
int lua_AIStateMachine_getAgent(lua_State* state);
int lua_AIStateMachine_getState(lua_State* state);
int lua_AIStateMachine_removeState(lua_State* state);
int lua_AIStateMachine_setState(lua_State* state);

void luaRegister_AIStateMachine();

}

#endif
