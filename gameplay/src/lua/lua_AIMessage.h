#ifndef LUA_AIMESSAGE_H_
#define LUA_AIMESSAGE_H_

namespace gameplay
{

// Lua bindings for AIMessage.
int lua_AIMessage_getBoolean(lua_State* state);
int lua_AIMessage_getDouble(lua_State* state);
int lua_AIMessage_getFloat(lua_State* state);
int lua_AIMessage_getId(lua_State* state);
int lua_AIMessage_getInt(lua_State* state);
int lua_AIMessage_getLong(lua_State* state);
int lua_AIMessage_getParameterCount(lua_State* state);
int lua_AIMessage_getParameterType(lua_State* state);
int lua_AIMessage_getReceiver(lua_State* state);
int lua_AIMessage_getSender(lua_State* state);
int lua_AIMessage_getString(lua_State* state);
int lua_AIMessage_setBoolean(lua_State* state);
int lua_AIMessage_setDouble(lua_State* state);
int lua_AIMessage_setFloat(lua_State* state);
int lua_AIMessage_setInt(lua_State* state);
int lua_AIMessage_setLong(lua_State* state);
int lua_AIMessage_setString(lua_State* state);
int lua_AIMessage_static_create(lua_State* state);
int lua_AIMessage_static_destroy(lua_State* state);

void luaRegister_AIMessage();

}

#endif
