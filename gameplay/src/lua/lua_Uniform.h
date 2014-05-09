#ifndef LUA_UNIFORM_H_
#define LUA_UNIFORM_H_

namespace gameplay
{

// Lua bindings for Uniform.
int lua_Uniform_getEffect(lua_State* state);
int lua_Uniform_getName(lua_State* state);
int lua_Uniform_getType(lua_State* state);

void luaRegister_Uniform();

}

#endif
