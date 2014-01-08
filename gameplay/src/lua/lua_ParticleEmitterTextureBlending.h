#ifndef LUA_PARTICLEEMITTERTEXTUREBLENDING_H_
#define LUA_PARTICLEEMITTERTEXTUREBLENDING_H_

#include "ParticleEmitter.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for ParticleEmitter::TextureBlending.
ParticleEmitter::TextureBlending lua_enumFromString_ParticleEmitterTextureBlending(const char* s);
const char* lua_stringFromEnum_ParticleEmitterTextureBlending(ParticleEmitter::TextureBlending e);

}

#endif
