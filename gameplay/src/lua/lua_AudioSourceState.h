#ifndef LUA_AUDIOSOURCESTATE_H_
#define LUA_AUDIOSOURCESTATE_H_

#include "AudioSource.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for AudioSource::State.
AudioSource::State lua_enumFromString_AudioSourceState(const char* s);
const char* lua_stringFromEnum_AudioSourceState(AudioSource::State e);

}

#endif
