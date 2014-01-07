#ifndef LUA_PROPERTIES_H_
#define LUA_PROPERTIES_H_

namespace gameplay
{

// Lua bindings for Properties.
int lua_Properties__gc(lua_State* state);
int lua_Properties_exists(lua_State* state);
int lua_Properties_getBool(lua_State* state);
int lua_Properties_getColor(lua_State* state);
int lua_Properties_getFloat(lua_State* state);
int lua_Properties_getId(lua_State* state);
int lua_Properties_getInt(lua_State* state);
int lua_Properties_getLong(lua_State* state);
int lua_Properties_getMatrix(lua_State* state);
int lua_Properties_getNamespace(lua_State* state);
int lua_Properties_getNextNamespace(lua_State* state);
int lua_Properties_getNextProperty(lua_State* state);
int lua_Properties_getQuaternionFromAxisAngle(lua_State* state);
int lua_Properties_getString(lua_State* state);
int lua_Properties_getType(lua_State* state);
int lua_Properties_getVariable(lua_State* state);
int lua_Properties_getVector2(lua_State* state);
int lua_Properties_getVector3(lua_State* state);
int lua_Properties_getVector4(lua_State* state);
int lua_Properties_rewind(lua_State* state);
int lua_Properties_setString(lua_State* state);
int lua_Properties_setVariable(lua_State* state);
int lua_Properties_static_create(lua_State* state);
int lua_Properties_static_parseAxisAngle(lua_State* state);
int lua_Properties_static_parseColor(lua_State* state);
int lua_Properties_static_parseVector2(lua_State* state);
int lua_Properties_static_parseVector3(lua_State* state);
int lua_Properties_static_parseVector4(lua_State* state);

void luaRegister_Properties();

}

#endif
