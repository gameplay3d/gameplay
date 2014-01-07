#ifndef LUA_PHYSICSCONTROLLER_H_
#define LUA_PHYSICSCONTROLLER_H_

namespace gameplay
{

// Lua bindings for PhysicsController.
int lua_PhysicsController_addScriptCallback(lua_State* state);
int lua_PhysicsController_addStatusListener(lua_State* state);
int lua_PhysicsController_createFixedConstraint(lua_State* state);
int lua_PhysicsController_createGenericConstraint(lua_State* state);
int lua_PhysicsController_createHingeConstraint(lua_State* state);
int lua_PhysicsController_createSocketConstraint(lua_State* state);
int lua_PhysicsController_createSpringConstraint(lua_State* state);
int lua_PhysicsController_drawDebug(lua_State* state);
int lua_PhysicsController_getGravity(lua_State* state);
int lua_PhysicsController_rayTest(lua_State* state);
int lua_PhysicsController_removeScriptCallback(lua_State* state);
int lua_PhysicsController_removeStatusListener(lua_State* state);
int lua_PhysicsController_setGravity(lua_State* state);
int lua_PhysicsController_sweepTest(lua_State* state);

void luaRegister_PhysicsController();

}

#endif
