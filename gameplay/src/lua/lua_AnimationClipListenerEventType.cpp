#include "Base.h"
#include "lua_AnimationClipListenerEventType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_AnimationClipListenerEventType_DEFAULT = "DEFAULT";
static const char* luaEnumString_AnimationClipListenerEventType_BEGIN = "BEGIN";
static const char* luaEnumString_AnimationClipListenerEventType_END = "END";

AnimationClip::Listener::EventType lua_enumFromString_AnimationClipListenerEventType(const char* s)
{
    if (strcmp(s, luaEnumString_AnimationClipListenerEventType_DEFAULT) == 0)
        return AnimationClip::Listener::DEFAULT;
    if (strcmp(s, luaEnumString_AnimationClipListenerEventType_BEGIN) == 0)
        return AnimationClip::Listener::BEGIN;
    if (strcmp(s, luaEnumString_AnimationClipListenerEventType_END) == 0)
        return AnimationClip::Listener::END;
    GP_ERROR("Invalid enumeration value '%s' for enumeration AnimationClip::Listener::EventType.", s);
    return AnimationClip::Listener::DEFAULT;
}

const char* lua_stringFromEnum_AnimationClipListenerEventType(AnimationClip::Listener::EventType e)
{
    if (e == AnimationClip::Listener::DEFAULT)
        return luaEnumString_AnimationClipListenerEventType_DEFAULT;
    if (e == AnimationClip::Listener::BEGIN)
        return luaEnumString_AnimationClipListenerEventType_BEGIN;
    if (e == AnimationClip::Listener::END)
        return luaEnumString_AnimationClipListenerEventType_END;
    GP_ERROR("Invalid enumeration value '%d' for enumeration AnimationClip::Listener::EventType.", e);
    return enumStringEmpty;
}

}

