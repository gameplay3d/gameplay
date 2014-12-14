#ifndef LUA_PARTICLEEMITTERBLENDMODE_H_
#define LUA_PARTICLEEMITTERBLENDMODE_H_

#include "ParticleEmitter.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for ParticleEmitter::BlendMode.
ParticleEmitter::BlendMode lua_enumFromString_ParticleEmitterBlendMode(const char* s);
const char* lua_stringFromEnum_ParticleEmitterBlendMode(ParticleEmitter::BlendMode e);

}

#endif
