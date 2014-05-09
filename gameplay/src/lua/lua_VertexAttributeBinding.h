#ifndef LUA_VERTEXATTRIBUTEBINDING_H_
#define LUA_VERTEXATTRIBUTEBINDING_H_

namespace gameplay
{

// Lua bindings for VertexAttributeBinding.
int lua_VertexAttributeBinding__gc(lua_State* state);
int lua_VertexAttributeBinding_addRef(lua_State* state);
int lua_VertexAttributeBinding_bind(lua_State* state);
int lua_VertexAttributeBinding_getRefCount(lua_State* state);
int lua_VertexAttributeBinding_release(lua_State* state);
int lua_VertexAttributeBinding_static_create(lua_State* state);
int lua_VertexAttributeBinding_unbind(lua_State* state);

void luaRegister_VertexAttributeBinding();

}

#endif
