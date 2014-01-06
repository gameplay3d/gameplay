#ifndef LUA_PHYSICSCOLLISIONSHAPEDEFINITION_H_
#define LUA_PHYSICSCOLLISIONSHAPEDEFINITION_H_

namespace gameplay
{

// Lua bindings for PhysicsCollisionShape::Definition.
int lua_PhysicsCollisionShapeDefinition__gc(lua_State* state);
int lua_PhysicsCollisionShapeDefinition__init(lua_State* state);
int lua_PhysicsCollisionShapeDefinition_isEmpty(lua_State* state);

void luaRegister_PhysicsCollisionShapeDefinition();

}

#endif
