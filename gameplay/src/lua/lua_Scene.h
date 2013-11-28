#ifndef LUA_SCENE_H_
#define LUA_SCENE_H_

namespace gameplay
{

// Lua bindings for Scene.
int lua_Scene__gc(lua_State* state);
int lua_Scene_addNode(lua_State* state);
int lua_Scene_addRef(lua_State* state);
int lua_Scene_bindAudioListenerToCamera(lua_State* state);
int lua_Scene_findNode(lua_State* state);
int lua_Scene_getActiveCamera(lua_State* state);
int lua_Scene_getAmbientColor(lua_State* state);
int lua_Scene_getFirstNode(lua_State* state);
int lua_Scene_getId(lua_State* state);
int lua_Scene_getNext(lua_State* state);
int lua_Scene_getNodeCount(lua_State* state);
int lua_Scene_getRefCount(lua_State* state);
int lua_Scene_release(lua_State* state);
int lua_Scene_removeAllNodes(lua_State* state);
int lua_Scene_removeNode(lua_State* state);
int lua_Scene_reset(lua_State* state);
int lua_Scene_setActiveCamera(lua_State* state);
int lua_Scene_setAmbientColor(lua_State* state);
int lua_Scene_setId(lua_State* state);
int lua_Scene_static_create(lua_State* state);
int lua_Scene_static_getScene(lua_State* state);
int lua_Scene_static_load(lua_State* state);
int lua_Scene_update(lua_State* state);
int lua_Scene_visit(lua_State* state);

void luaRegister_Scene();

}

#endif
