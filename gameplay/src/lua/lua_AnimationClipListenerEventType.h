#ifndef LUA_ANIMATIONCLIPLISTENEREVENTTYPE_H_
#define LUA_ANIMATIONCLIPLISTENEREVENTTYPE_H_

#include "AnimationClip.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for AnimationClip::Listener::EventType.
AnimationClip::Listener::EventType lua_enumFromString_AnimationClipListenerEventType(const char* s);
const char* lua_stringFromEnum_AnimationClipListenerEventType(AnimationClip::Listener::EventType e);

}

#endif
