#ifndef LUA_AUDIOLISTENER_H_
#define LUA_AUDIOLISTENER_H_

namespace gameplay
{

// Lua bindings for AudioListener.
int lua_AudioListener_getCamera(lua_State* state);
int lua_AudioListener_getGain(lua_State* state);
int lua_AudioListener_getOrientationForward(lua_State* state);
int lua_AudioListener_getOrientationUp(lua_State* state);
int lua_AudioListener_getPosition(lua_State* state);
int lua_AudioListener_getVelocity(lua_State* state);
int lua_AudioListener_setCamera(lua_State* state);
int lua_AudioListener_setGain(lua_State* state);
int lua_AudioListener_setOrientation(lua_State* state);
int lua_AudioListener_setPosition(lua_State* state);
int lua_AudioListener_setVelocity(lua_State* state);
int lua_AudioListener_static_getInstance(lua_State* state);

void luaRegister_AudioListener();

}

#endif
