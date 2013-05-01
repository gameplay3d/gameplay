#ifndef LUA_PHYSICSGHOSTOBJECT_H_
#define LUA_PHYSICSGHOSTOBJECT_H_

namespace gameplay
{

// Lua bindings for PhysicsGhostObject.
int lua_PhysicsGhostObject_addCollisionListener(lua_State* state);
int lua_PhysicsGhostObject_asCharacter(lua_State* state);
int lua_PhysicsGhostObject_asGhostObject(lua_State* state);
int lua_PhysicsGhostObject_asRigidBody(lua_State* state);
int lua_PhysicsGhostObject_asVehicle(lua_State* state);
int lua_PhysicsGhostObject_asVehicleWheel(lua_State* state);
int lua_PhysicsGhostObject_collidesWith(lua_State* state);
int lua_PhysicsGhostObject_getCollisionShape(lua_State* state);
int lua_PhysicsGhostObject_getNode(lua_State* state);
int lua_PhysicsGhostObject_getShapeType(lua_State* state);
int lua_PhysicsGhostObject_getType(lua_State* state);
int lua_PhysicsGhostObject_isDynamic(lua_State* state);
int lua_PhysicsGhostObject_isEnabled(lua_State* state);
int lua_PhysicsGhostObject_isKinematic(lua_State* state);
int lua_PhysicsGhostObject_isStatic(lua_State* state);
int lua_PhysicsGhostObject_removeCollisionListener(lua_State* state);
int lua_PhysicsGhostObject_setEnabled(lua_State* state);
int lua_PhysicsGhostObject_transformChanged(lua_State* state);

void luaRegister_PhysicsGhostObject();

}

#endif
