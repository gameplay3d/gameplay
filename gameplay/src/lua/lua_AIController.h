#ifndef LUA_AICONTROLLER_H_
#define LUA_AICONTROLLER_H_

namespace gameplay
{

// Lua bindings for AIController.
int lua_AIController_findAgent(lua_State* state);
int lua_AIController_sendMessage(lua_State* state);

void luaRegister_AIController();

}

#endif
