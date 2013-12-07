#ifndef LUA_MODEL_H_
#define LUA_MODEL_H_

namespace gameplay
{

// Lua bindings for Model.
int lua_Model__gc(lua_State* state);
int lua_Model_addRef(lua_State* state);
int lua_Model_draw(lua_State* state);
int lua_Model_getMaterial(lua_State* state);
int lua_Model_getMesh(lua_State* state);
int lua_Model_getMeshPartCount(lua_State* state);
int lua_Model_getNode(lua_State* state);
int lua_Model_getRefCount(lua_State* state);
int lua_Model_getSkin(lua_State* state);
int lua_Model_hasMaterial(lua_State* state);
int lua_Model_release(lua_State* state);
int lua_Model_setMaterial(lua_State* state);
int lua_Model_setNode(lua_State* state);
int lua_Model_static_create(lua_State* state);

void luaRegister_Model();

}

#endif
