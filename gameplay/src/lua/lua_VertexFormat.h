#ifndef LUA_VERTEXFORMAT_H_
#define LUA_VERTEXFORMAT_H_

namespace gameplay
{

// Lua bindings for VertexFormat.
int lua_VertexFormat__gc(lua_State* state);
int lua_VertexFormat__init(lua_State* state);
int lua_VertexFormat_getElement(lua_State* state);
int lua_VertexFormat_getElementCount(lua_State* state);
int lua_VertexFormat_getVertexSize(lua_State* state);
int lua_VertexFormat_static_toString(lua_State* state);

void luaRegister_VertexFormat();

}

#endif
