#include "Base.h"
#include "lua_ParticleEmitterTextureBlending.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ParticleEmitterTextureBlending_BLEND_OPAQUE = "BLEND_OPAQUE";
static const char* luaEnumString_ParticleEmitterTextureBlending_BLEND_TRANSPARENT = "BLEND_TRANSPARENT";
static const char* luaEnumString_ParticleEmitterTextureBlending_BLEND_ADDITIVE = "BLEND_ADDITIVE";
static const char* luaEnumString_ParticleEmitterTextureBlending_BLEND_MULTIPLIED = "BLEND_MULTIPLIED";

ParticleEmitter::TextureBlending lua_enumFromString_ParticleEmitterTextureBlending(const char* s)
{
    if (strcmp(s, luaEnumString_ParticleEmitterTextureBlending_BLEND_OPAQUE) == 0)
        return ParticleEmitter::BLEND_OPAQUE;
    if (strcmp(s, luaEnumString_ParticleEmitterTextureBlending_BLEND_TRANSPARENT) == 0)
        return ParticleEmitter::BLEND_TRANSPARENT;
    if (strcmp(s, luaEnumString_ParticleEmitterTextureBlending_BLEND_ADDITIVE) == 0)
        return ParticleEmitter::BLEND_ADDITIVE;
    if (strcmp(s, luaEnumString_ParticleEmitterTextureBlending_BLEND_MULTIPLIED) == 0)
        return ParticleEmitter::BLEND_MULTIPLIED;
    return ParticleEmitter::BLEND_OPAQUE;
}

const char* lua_stringFromEnum_ParticleEmitterTextureBlending(ParticleEmitter::TextureBlending e)
{
    if (e == ParticleEmitter::BLEND_OPAQUE)
        return luaEnumString_ParticleEmitterTextureBlending_BLEND_OPAQUE;
    if (e == ParticleEmitter::BLEND_TRANSPARENT)
        return luaEnumString_ParticleEmitterTextureBlending_BLEND_TRANSPARENT;
    if (e == ParticleEmitter::BLEND_ADDITIVE)
        return luaEnumString_ParticleEmitterTextureBlending_BLEND_ADDITIVE;
    if (e == ParticleEmitter::BLEND_MULTIPLIED)
        return luaEnumString_ParticleEmitterTextureBlending_BLEND_MULTIPLIED;
    return enumStringEmpty;
}

}

