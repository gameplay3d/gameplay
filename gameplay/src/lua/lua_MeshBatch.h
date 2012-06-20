#ifndef LUA_MESHBATCH_H_
#define LUA_MESHBATCH_H_

namespace gameplay
{

// Lua bindings for MeshBatch.
int lua_MeshBatch__gc(lua_State* state);
int lua_MeshBatch_begin(lua_State* state);
int lua_MeshBatch_draw(lua_State* state);
int lua_MeshBatch_end(lua_State* state);
int lua_MeshBatch_getCapacity(lua_State* state);
int lua_MeshBatch_getMaterial(lua_State* state);
int lua_MeshBatch_setCapacity(lua_State* state);
int lua_MeshBatch_static_create(lua_State* state);

void luaRegister_MeshBatch();

}

#endif
