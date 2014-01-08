#ifndef LUA_PHYSICSCOLLISIONSHAPE_H_
#define LUA_PHYSICSCOLLISIONSHAPE_H_

namespace gameplay
{

// Lua bindings for PhysicsCollisionShape.
int lua_PhysicsCollisionShape__gc(lua_State* state);
int lua_PhysicsCollisionShape_addRef(lua_State* state);
int lua_PhysicsCollisionShape_getRefCount(lua_State* state);
int lua_PhysicsCollisionShape_getType(lua_State* state);
int lua_PhysicsCollisionShape_release(lua_State* state);
int lua_PhysicsCollisionShape_static_box(lua_State* state);
int lua_PhysicsCollisionShape_static_capsule(lua_State* state);
int lua_PhysicsCollisionShape_static_heightfield(lua_State* state);
int lua_PhysicsCollisionShape_static_mesh(lua_State* state);
int lua_PhysicsCollisionShape_static_sphere(lua_State* state);

void luaRegister_PhysicsCollisionShape();

}

#endif
