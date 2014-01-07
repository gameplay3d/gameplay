#ifndef LUA_ANIMATION_H_
#define LUA_ANIMATION_H_

namespace gameplay
{

// Lua bindings for Animation.
int lua_Animation__gc(lua_State* state);
int lua_Animation_addRef(lua_State* state);
int lua_Animation_createClip(lua_State* state);
int lua_Animation_createClips(lua_State* state);
int lua_Animation_getClip(lua_State* state);
int lua_Animation_getClipCount(lua_State* state);
int lua_Animation_getDuration(lua_State* state);
int lua_Animation_getId(lua_State* state);
int lua_Animation_getRefCount(lua_State* state);
int lua_Animation_pause(lua_State* state);
int lua_Animation_play(lua_State* state);
int lua_Animation_release(lua_State* state);
int lua_Animation_stop(lua_State* state);
int lua_Animation_targets(lua_State* state);

void luaRegister_Animation();

}

#endif
