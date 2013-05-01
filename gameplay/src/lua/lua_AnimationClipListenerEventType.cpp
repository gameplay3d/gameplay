#include "Base.h"
#include "lua_AnimationClipListenerEventType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_AnimationClipListenerEventType_BEGIN = "BEGIN";
static const char* luaEnumString_AnimationClipListenerEventType_END = "END";
static const char* luaEnumString_AnimationClipListenerEventType_TIME = "TIME";

AnimationClip::Listener::EventType lua_enumFromString_AnimationClipListenerEventType(const char* s)
{
    if (strcmp(s, luaEnumString_AnimationClipListenerEventType_BEGIN) == 0)
        return AnimationClip::Listener::BEGIN;
    if (strcmp(s, luaEnumString_AnimationClipListenerEventType_END) == 0)
        return AnimationClip::Listener::END;
    if (strcmp(s, luaEnumString_AnimationClipListenerEventType_TIME) == 0)
        return AnimationClip::Listener::TIME;
    return AnimationClip::Listener::BEGIN;
}

const char* lua_stringFromEnum_AnimationClipListenerEventType(AnimationClip::Listener::EventType e)
{
    if (e == AnimationClip::Listener::BEGIN)
        return luaEnumString_AnimationClipListenerEventType_BEGIN;
    if (e == AnimationClip::Listener::END)
        return luaEnumString_AnimationClipListenerEventType_END;
    if (e == AnimationClip::Listener::TIME)
        return luaEnumString_AnimationClipListenerEventType_TIME;
    return enumStringEmpty;
}

}

