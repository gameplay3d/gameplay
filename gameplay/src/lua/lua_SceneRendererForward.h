#ifndef LUA_SCENERENDERERFORWARD_H_
#define LUA_SCENERENDERERFORWARD_H_

namespace gameplay
{

// Lua bindings for SceneRendererForward.
int lua_SceneRendererForward_render(lua_State* state);
int lua_SceneRendererForward_static_create(lua_State* state);

void luaRegister_SceneRendererForward();

}

#endif
