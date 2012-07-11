#ifndef LUA_SCENELOADER_H_
#define LUA_SCENELOADER_H_

namespace gameplay
{

// Lua bindings for SceneLoader.
int lua_SceneLoader__gc(lua_State* state);
int lua_SceneLoader__init(lua_State* state);

void luaRegister_SceneLoader();

}

#endif
