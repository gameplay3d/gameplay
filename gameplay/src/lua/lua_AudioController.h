#ifndef LUA_AUDIOCONTROLLER_H_
#define LUA_AUDIOCONTROLLER_H_

namespace gameplay
{

// Lua bindings for AudioController.
int lua_AudioController__gc(lua_State* state);

void luaRegister_AudioController();

}

#endif
