#ifndef LUA_PARTICLEEMITTER_H_
#define LUA_PARTICLEEMITTER_H_

namespace gameplay
{

// Lua bindings for ParticleEmitter.
int lua_ParticleEmitter__gc(lua_State* state);
int lua_ParticleEmitter_addRef(lua_State* state);
int lua_ParticleEmitter_clone(lua_State* state);
int lua_ParticleEmitter_draw(lua_State* state);
int lua_ParticleEmitter_emitOnce(lua_State* state);
int lua_ParticleEmitter_getAcceleration(lua_State* state);
int lua_ParticleEmitter_getAccelerationVariance(lua_State* state);
int lua_ParticleEmitter_getColorEnd(lua_State* state);
int lua_ParticleEmitter_getColorEndVariance(lua_State* state);
int lua_ParticleEmitter_getColorStart(lua_State* state);
int lua_ParticleEmitter_getColorStartVariance(lua_State* state);
int lua_ParticleEmitter_getEmissionRate(lua_State* state);
int lua_ParticleEmitter_getEnergyMax(lua_State* state);
int lua_ParticleEmitter_getEnergyMin(lua_State* state);
int lua_ParticleEmitter_getNode(lua_State* state);
int lua_ParticleEmitter_getOrbitAcceleration(lua_State* state);
int lua_ParticleEmitter_getOrbitPosition(lua_State* state);
int lua_ParticleEmitter_getOrbitVelocity(lua_State* state);
int lua_ParticleEmitter_getParticleCountMax(lua_State* state);
int lua_ParticleEmitter_getParticlesCount(lua_State* state);
int lua_ParticleEmitter_getPosition(lua_State* state);
int lua_ParticleEmitter_getPositionVariance(lua_State* state);
int lua_ParticleEmitter_getRefCount(lua_State* state);
int lua_ParticleEmitter_getRotationAxis(lua_State* state);
int lua_ParticleEmitter_getRotationAxisVariance(lua_State* state);
int lua_ParticleEmitter_getRotationPerParticleSpeedMax(lua_State* state);
int lua_ParticleEmitter_getRotationPerParticleSpeedMin(lua_State* state);
int lua_ParticleEmitter_getRotationSpeedMax(lua_State* state);
int lua_ParticleEmitter_getRotationSpeedMin(lua_State* state);
int lua_ParticleEmitter_getSizeEndMax(lua_State* state);
int lua_ParticleEmitter_getSizeEndMin(lua_State* state);
int lua_ParticleEmitter_getSizeStartMax(lua_State* state);
int lua_ParticleEmitter_getSizeStartMin(lua_State* state);
int lua_ParticleEmitter_getSpriteFrameCount(lua_State* state);
int lua_ParticleEmitter_getSpriteFrameDuration(lua_State* state);
int lua_ParticleEmitter_getSpriteFrameRandomOffset(lua_State* state);
int lua_ParticleEmitter_getSpriteHeight(lua_State* state);
int lua_ParticleEmitter_getSpriteWidth(lua_State* state);
int lua_ParticleEmitter_getTexture(lua_State* state);
int lua_ParticleEmitter_getTextureBlending(lua_State* state);
int lua_ParticleEmitter_getVelocity(lua_State* state);
int lua_ParticleEmitter_getVelocityVariance(lua_State* state);
int lua_ParticleEmitter_isActive(lua_State* state);
int lua_ParticleEmitter_isEllipsoid(lua_State* state);
int lua_ParticleEmitter_isSpriteAnimated(lua_State* state);
int lua_ParticleEmitter_isSpriteLooped(lua_State* state);
int lua_ParticleEmitter_isStarted(lua_State* state);
int lua_ParticleEmitter_release(lua_State* state);
int lua_ParticleEmitter_setAcceleration(lua_State* state);
int lua_ParticleEmitter_setColor(lua_State* state);
int lua_ParticleEmitter_setEllipsoid(lua_State* state);
int lua_ParticleEmitter_setEmissionRate(lua_State* state);
int lua_ParticleEmitter_setEnergy(lua_State* state);
int lua_ParticleEmitter_setOrbit(lua_State* state);
int lua_ParticleEmitter_setParticleCountMax(lua_State* state);
int lua_ParticleEmitter_setPosition(lua_State* state);
int lua_ParticleEmitter_setRotation(lua_State* state);
int lua_ParticleEmitter_setRotationPerParticle(lua_State* state);
int lua_ParticleEmitter_setSize(lua_State* state);
int lua_ParticleEmitter_setSpriteAnimated(lua_State* state);
int lua_ParticleEmitter_setSpriteFrameCoords(lua_State* state);
int lua_ParticleEmitter_setSpriteFrameDuration(lua_State* state);
int lua_ParticleEmitter_setSpriteFrameRandomOffset(lua_State* state);
int lua_ParticleEmitter_setSpriteLooped(lua_State* state);
int lua_ParticleEmitter_setSpriteTexCoords(lua_State* state);
int lua_ParticleEmitter_setTexture(lua_State* state);
int lua_ParticleEmitter_setTextureBlending(lua_State* state);
int lua_ParticleEmitter_setVelocity(lua_State* state);
int lua_ParticleEmitter_start(lua_State* state);
int lua_ParticleEmitter_static_create(lua_State* state);
int lua_ParticleEmitter_static_getTextureBlendingFromString(lua_State* state);
int lua_ParticleEmitter_stop(lua_State* state);
int lua_ParticleEmitter_update(lua_State* state);

void luaRegister_ParticleEmitter();

}

#endif
