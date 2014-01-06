#ifndef LUA_BUNDLE_H_
#define LUA_BUNDLE_H_

namespace gameplay
{

// Lua bindings for Bundle.
int lua_Bundle__gc(lua_State* state);
int lua_Bundle_addRef(lua_State* state);
int lua_Bundle_contains(lua_State* state);
int lua_Bundle_getObjectCount(lua_State* state);
int lua_Bundle_getObjectId(lua_State* state);
int lua_Bundle_getRefCount(lua_State* state);
int lua_Bundle_getVersionMajor(lua_State* state);
int lua_Bundle_getVersionMinor(lua_State* state);
int lua_Bundle_loadFont(lua_State* state);
int lua_Bundle_loadMesh(lua_State* state);
int lua_Bundle_loadNode(lua_State* state);
int lua_Bundle_loadScene(lua_State* state);
int lua_Bundle_release(lua_State* state);
int lua_Bundle_static_create(lua_State* state);

void luaRegister_Bundle();

}

#endif
