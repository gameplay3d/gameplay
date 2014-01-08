#ifndef LUA_CONTROLLISTENER_H_
#define LUA_CONTROLLISTENER_H_

namespace gameplay
{

// Lua bindings for Control::Listener.
int lua_ControlListener__gc(lua_State* state);
int lua_ControlListener_controlEvent(lua_State* state);

void luaRegister_ControlListener();

}

#endif
