#ifndef LUA_VERTEXFORMATELEMENT_H_
#define LUA_VERTEXFORMATELEMENT_H_

namespace gameplay
{

// Lua bindings for VertexFormat::Element.
int lua_VertexFormatElement__gc(lua_State* state);
int lua_VertexFormatElement__init(lua_State* state);
int lua_VertexFormatElement_size(lua_State* state);
int lua_VertexFormatElement_usage(lua_State* state);

void luaRegister_VertexFormatElement();

}

#endif
