#ifndef LUA_MESHPART_H_
#define LUA_MESHPART_H_

namespace gameplay
{

// Lua bindings for MeshPart.
int lua_MeshPart__gc(lua_State* state);
int lua_MeshPart_getIndexBuffer(lua_State* state);
int lua_MeshPart_getIndexCount(lua_State* state);
int lua_MeshPart_getIndexFormat(lua_State* state);
int lua_MeshPart_getMeshIndex(lua_State* state);
int lua_MeshPart_getPrimitiveType(lua_State* state);
int lua_MeshPart_isDynamic(lua_State* state);

void luaRegister_MeshPart();

}

#endif
