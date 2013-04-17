#ifndef LUA_PLATFORM_H_
#define LUA_PLATFORM_H_

namespace gameplay
{

// Lua bindings for Platform.
int lua_Platform__gc(lua_State* state);
int lua_Platform_enterMessagePump(lua_State* state);
int lua_Platform_static_swapBuffers(lua_State* state);

void luaRegister_Platform();

}

#endif
