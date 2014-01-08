#ifndef LUA_ANIMATIONCLIPLISTENER_H_
#define LUA_ANIMATIONCLIPLISTENER_H_

namespace gameplay
{

// Lua bindings for AnimationClip::Listener.
int lua_AnimationClipListener__gc(lua_State* state);
int lua_AnimationClipListener_animationEvent(lua_State* state);

void luaRegister_AnimationClipListener();

}

#endif
