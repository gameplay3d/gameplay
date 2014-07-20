#ifndef LUA_AUDIOSOURCE_H_
#define LUA_AUDIOSOURCE_H_

namespace gameplay
{

// Lua bindings for AudioSource.
int lua_AudioSource__gc(lua_State* state);
int lua_AudioSource_addRef(lua_State* state);
int lua_AudioSource_getGain(lua_State* state);
int lua_AudioSource_getNode(lua_State* state);
int lua_AudioSource_getPitch(lua_State* state);
int lua_AudioSource_getRefCount(lua_State* state);
int lua_AudioSource_getState(lua_State* state);
int lua_AudioSource_getVelocity(lua_State* state);
int lua_AudioSource_isLooped(lua_State* state);
int lua_AudioSource_isStreamed(lua_State* state);
int lua_AudioSource_pause(lua_State* state);
int lua_AudioSource_play(lua_State* state);
int lua_AudioSource_release(lua_State* state);
int lua_AudioSource_resume(lua_State* state);
int lua_AudioSource_rewind(lua_State* state);
int lua_AudioSource_setGain(lua_State* state);
int lua_AudioSource_setLooped(lua_State* state);
int lua_AudioSource_setPitch(lua_State* state);
int lua_AudioSource_setVelocity(lua_State* state);
int lua_AudioSource_static_create(lua_State* state);
int lua_AudioSource_stop(lua_State* state);

void luaRegister_AudioSource();

}

#endif
