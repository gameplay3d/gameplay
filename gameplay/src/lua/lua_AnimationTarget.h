#ifndef LUA_ANIMATIONTARGET_H_
#define LUA_ANIMATIONTARGET_H_

namespace gameplay
{

// Lua bindings for AnimationTarget.
int lua_AnimationTarget_createAnimation(lua_State* state);
int lua_AnimationTarget_createAnimationFromBy(lua_State* state);
int lua_AnimationTarget_createAnimationFromTo(lua_State* state);
int lua_AnimationTarget_destroyAnimation(lua_State* state);
int lua_AnimationTarget_getAnimation(lua_State* state);
int lua_AnimationTarget_getAnimationPropertyComponentCount(lua_State* state);
int lua_AnimationTarget_getAnimationPropertyValue(lua_State* state);
int lua_AnimationTarget_setAnimationPropertyValue(lua_State* state);

void luaRegister_AnimationTarget();

}

#endif
