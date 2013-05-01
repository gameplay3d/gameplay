#ifndef LUA_ANIMATIONCLIP_H_
#define LUA_ANIMATIONCLIP_H_

namespace gameplay
{

// Lua bindings for AnimationClip.
int lua_AnimationClip__gc(lua_State* state);
int lua_AnimationClip_addBeginListener(lua_State* state);
int lua_AnimationClip_addEndListener(lua_State* state);
int lua_AnimationClip_addListener(lua_State* state);
int lua_AnimationClip_addRef(lua_State* state);
int lua_AnimationClip_crossFade(lua_State* state);
int lua_AnimationClip_getActiveDuration(lua_State* state);
int lua_AnimationClip_getAnimation(lua_State* state);
int lua_AnimationClip_getBlendWeight(lua_State* state);
int lua_AnimationClip_getDuration(lua_State* state);
int lua_AnimationClip_getElapsedTime(lua_State* state);
int lua_AnimationClip_getEndTime(lua_State* state);
int lua_AnimationClip_getId(lua_State* state);
int lua_AnimationClip_getLoopBlendTime(lua_State* state);
int lua_AnimationClip_getRefCount(lua_State* state);
int lua_AnimationClip_getRepeatCount(lua_State* state);
int lua_AnimationClip_getSpeed(lua_State* state);
int lua_AnimationClip_getStartTime(lua_State* state);
int lua_AnimationClip_isPlaying(lua_State* state);
int lua_AnimationClip_pause(lua_State* state);
int lua_AnimationClip_play(lua_State* state);
int lua_AnimationClip_release(lua_State* state);
int lua_AnimationClip_setActiveDuration(lua_State* state);
int lua_AnimationClip_setBlendWeight(lua_State* state);
int lua_AnimationClip_setLoopBlendTime(lua_State* state);
int lua_AnimationClip_setRepeatCount(lua_State* state);
int lua_AnimationClip_setSpeed(lua_State* state);
int lua_AnimationClip_static_REPEAT_INDEFINITE(lua_State* state);
int lua_AnimationClip_stop(lua_State* state);

void luaRegister_AnimationClip();

}

#endif
