#ifndef LUA_LOGGER_H_
#define LUA_LOGGER_H_

namespace gameplay
{

// Lua bindings for Logger.
int lua_Logger_static_isEnabled(lua_State* state);
int lua_Logger_static_log(lua_State* state);
int lua_Logger_static_set(lua_State* state);
int lua_Logger_static_setEnabled(lua_State* state);

void luaRegister_Logger();

}

#endif
