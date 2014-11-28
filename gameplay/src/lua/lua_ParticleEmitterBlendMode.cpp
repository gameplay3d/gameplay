#include "Base.h"
#include "lua_ParticleEmitterBlendMode.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ParticleEmitterBlendMode_BLEND_NONE = "BLEND_NONE";
static const char* luaEnumString_ParticleEmitterBlendMode_BLEND_ALPHA = "BLEND_ALPHA";
static const char* luaEnumString_ParticleEmitterBlendMode_BLEND_ADDITIVE = "BLEND_ADDITIVE";
static const char* luaEnumString_ParticleEmitterBlendMode_BLEND_MULTIPLIED = "BLEND_MULTIPLIED";

ParticleEmitter::BlendMode lua_enumFromString_ParticleEmitterBlendMode(const char* s)
{
    if (strcmp(s, luaEnumString_ParticleEmitterBlendMode_BLEND_NONE) == 0)
        return ParticleEmitter::BLEND_NONE;
    if (strcmp(s, luaEnumString_ParticleEmitterBlendMode_BLEND_ALPHA) == 0)
        return ParticleEmitter::BLEND_ALPHA;
    if (strcmp(s, luaEnumString_ParticleEmitterBlendMode_BLEND_ADDITIVE) == 0)
        return ParticleEmitter::BLEND_ADDITIVE;
    if (strcmp(s, luaEnumString_ParticleEmitterBlendMode_BLEND_MULTIPLIED) == 0)
        return ParticleEmitter::BLEND_MULTIPLIED;
    return ParticleEmitter::BLEND_NONE;
}

const char* lua_stringFromEnum_ParticleEmitterBlendMode(ParticleEmitter::BlendMode e)
{
    if (e == ParticleEmitter::BLEND_NONE)
        return luaEnumString_ParticleEmitterBlendMode_BLEND_NONE;
    if (e == ParticleEmitter::BLEND_ALPHA)
        return luaEnumString_ParticleEmitterBlendMode_BLEND_ALPHA;
    if (e == ParticleEmitter::BLEND_ADDITIVE)
        return luaEnumString_ParticleEmitterBlendMode_BLEND_ADDITIVE;
    if (e == ParticleEmitter::BLEND_MULTIPLIED)
        return luaEnumString_ParticleEmitterBlendMode_BLEND_MULTIPLIED;
    return enumStringEmpty;
}

}

