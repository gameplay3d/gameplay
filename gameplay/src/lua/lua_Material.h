#ifndef LUA_MATERIAL_H_
#define LUA_MATERIAL_H_

namespace gameplay
{

// Lua bindings for Material.
int lua_Material__gc(lua_State* state);
int lua_Material_addRef(lua_State* state);
int lua_Material_clearParameter(lua_State* state);
int lua_Material_getParameter(lua_State* state);
int lua_Material_getRefCount(lua_State* state);
int lua_Material_getStateBlock(lua_State* state);
int lua_Material_getTechnique(lua_State* state);
int lua_Material_getTechniqueByIndex(lua_State* state);
int lua_Material_getTechniqueCount(lua_State* state);
int lua_Material_release(lua_State* state);
int lua_Material_setParameterAutoBinding(lua_State* state);
int lua_Material_setStateBlock(lua_State* state);
int lua_Material_setTechnique(lua_State* state);
int lua_Material_static_create(lua_State* state);

void luaRegister_Material();

}

#endif
