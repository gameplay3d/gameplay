#ifndef LUA_AUDIOBUFFER_H_
#define LUA_AUDIOBUFFER_H_

namespace gameplay
{

// Lua bindings for AudioBuffer.
int lua_AudioBuffer__gc(lua_State* state);
int lua_AudioBuffer_addRef(lua_State* state);
int lua_AudioBuffer_getRefCount(lua_State* state);
int lua_AudioBuffer_release(lua_State* state);

void luaRegister_AudioBuffer();

}

#endif
