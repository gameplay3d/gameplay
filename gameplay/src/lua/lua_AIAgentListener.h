#ifndef LUA_AIAGENTLISTENER_H_
#define LUA_AIAGENTLISTENER_H_

namespace gameplay
{

// Lua bindings for AIAgent::Listener.
int lua_AIAgentListener__gc(lua_State* state);
int lua_AIAgentListener_messageReceived(lua_State* state);

void luaRegister_AIAgentListener();

}

#endif
