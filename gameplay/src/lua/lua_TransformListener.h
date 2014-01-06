#ifndef LUA_TRANSFORMLISTENER_H_
#define LUA_TRANSFORMLISTENER_H_

namespace gameplay
{

// Lua bindings for Transform::Listener.
int lua_TransformListener__gc(lua_State* state);
int lua_TransformListener_transformChanged(lua_State* state);

void luaRegister_TransformListener();

}

#endif
