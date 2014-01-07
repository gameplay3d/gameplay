#ifndef LUA_PHYSICSCONTROLLERLISTENER_H_
#define LUA_PHYSICSCONTROLLERLISTENER_H_

namespace gameplay
{

// Lua bindings for PhysicsController::Listener.
int lua_PhysicsControllerListener_statusEvent(lua_State* state);

void luaRegister_PhysicsControllerListener();

}

#endif
