#ifndef LUA_MESH_H_
#define LUA_MESH_H_

namespace gameplay
{

// Lua bindings for Mesh.
int lua_Mesh__gc(lua_State* state);
int lua_Mesh_addPart(lua_State* state);
int lua_Mesh_addRef(lua_State* state);
int lua_Mesh_getBoundingBox(lua_State* state);
int lua_Mesh_getBoundingSphere(lua_State* state);
int lua_Mesh_getPart(lua_State* state);
int lua_Mesh_getPartCount(lua_State* state);
int lua_Mesh_getPrimitiveType(lua_State* state);
int lua_Mesh_getRefCount(lua_State* state);
int lua_Mesh_getUrl(lua_State* state);
int lua_Mesh_getVertexBuffer(lua_State* state);
int lua_Mesh_getVertexCount(lua_State* state);
int lua_Mesh_getVertexFormat(lua_State* state);
int lua_Mesh_getVertexSize(lua_State* state);
int lua_Mesh_isDynamic(lua_State* state);
int lua_Mesh_release(lua_State* state);
int lua_Mesh_setBoundingBox(lua_State* state);
int lua_Mesh_setBoundingSphere(lua_State* state);
int lua_Mesh_setPrimitiveType(lua_State* state);
int lua_Mesh_setVertexData(lua_State* state);
int lua_Mesh_static_createBoundingBox(lua_State* state);
int lua_Mesh_static_createLines(lua_State* state);
int lua_Mesh_static_createMesh(lua_State* state);
int lua_Mesh_static_createQuad(lua_State* state);
int lua_Mesh_static_createQuadFullscreen(lua_State* state);

void luaRegister_Mesh();

}

#endif
