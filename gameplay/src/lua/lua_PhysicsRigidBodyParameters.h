#ifndef LUA_PHYSICSRIGIDBODYPARAMETERS_H_
#define LUA_PHYSICSRIGIDBODYPARAMETERS_H_

namespace gameplay
{

// Lua bindings for PhysicsRigidBody::Parameters.
int lua_PhysicsRigidBodyParameters__gc(lua_State* state);
int lua_PhysicsRigidBodyParameters__init(lua_State* state);
int lua_PhysicsRigidBodyParameters_angularDamping(lua_State* state);
int lua_PhysicsRigidBodyParameters_angularFactor(lua_State* state);
int lua_PhysicsRigidBodyParameters_anisotropicFriction(lua_State* state);
int lua_PhysicsRigidBodyParameters_friction(lua_State* state);
int lua_PhysicsRigidBodyParameters_kinematic(lua_State* state);
int lua_PhysicsRigidBodyParameters_linearDamping(lua_State* state);
int lua_PhysicsRigidBodyParameters_linearFactor(lua_State* state);
int lua_PhysicsRigidBodyParameters_mass(lua_State* state);
int lua_PhysicsRigidBodyParameters_restitution(lua_State* state);

void luaRegister_PhysicsRigidBodyParameters();

}

#endif
