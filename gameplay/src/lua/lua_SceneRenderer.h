#ifndef LUA_SCENERENDERER_H_
#define LUA_SCENERENDERER_H_

namespace gameplay
{

// Lua bindings for SceneRenderer.
int lua_SceneRenderer__gc(lua_State* state);
int lua_SceneRenderer_isWireframe(lua_State* state);
int lua_SceneRenderer_render(lua_State* state);
int lua_SceneRenderer_setWireframe(lua_State* state);

void luaRegister_SceneRenderer();

}

#endif
