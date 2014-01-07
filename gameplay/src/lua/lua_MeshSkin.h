#ifndef LUA_MESHSKIN_H_
#define LUA_MESHSKIN_H_

namespace gameplay
{

// Lua bindings for MeshSkin.
int lua_MeshSkin_getBindShape(lua_State* state);
int lua_MeshSkin_getJoint(lua_State* state);
int lua_MeshSkin_getJointCount(lua_State* state);
int lua_MeshSkin_getJointIndex(lua_State* state);
int lua_MeshSkin_getMatrixPalette(lua_State* state);
int lua_MeshSkin_getMatrixPaletteSize(lua_State* state);
int lua_MeshSkin_getModel(lua_State* state);
int lua_MeshSkin_getRootJoint(lua_State* state);
int lua_MeshSkin_setBindShape(lua_State* state);
int lua_MeshSkin_setRootJoint(lua_State* state);
int lua_MeshSkin_transformChanged(lua_State* state);

void luaRegister_MeshSkin();

}

#endif
