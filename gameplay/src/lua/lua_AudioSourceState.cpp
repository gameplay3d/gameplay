#include "Base.h"
#include "lua_AudioSourceState.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_AudioSourceState_INITIAL = "INITIAL";
static const char* luaEnumString_AudioSourceState_PLAYING = "PLAYING";
static const char* luaEnumString_AudioSourceState_PAUSED = "PAUSED";
static const char* luaEnumString_AudioSourceState_STOPPED = "STOPPED";

AudioSource::State lua_enumFromString_AudioSourceState(const char* s)
{
    if (strcmp(s, luaEnumString_AudioSourceState_INITIAL) == 0)
        return AudioSource::INITIAL;
    if (strcmp(s, luaEnumString_AudioSourceState_PLAYING) == 0)
        return AudioSource::PLAYING;
    if (strcmp(s, luaEnumString_AudioSourceState_PAUSED) == 0)
        return AudioSource::PAUSED;
    if (strcmp(s, luaEnumString_AudioSourceState_STOPPED) == 0)
        return AudioSource::STOPPED;
    return AudioSource::INITIAL;
}

const char* lua_stringFromEnum_AudioSourceState(AudioSource::State e)
{
    if (e == AudioSource::INITIAL)
        return luaEnumString_AudioSourceState_INITIAL;
    if (e == AudioSource::PLAYING)
        return luaEnumString_AudioSourceState_PLAYING;
    if (e == AudioSource::PAUSED)
        return luaEnumString_AudioSourceState_PAUSED;
    if (e == AudioSource::STOPPED)
        return luaEnumString_AudioSourceState_STOPPED;
    return enumStringEmpty;
}

}

