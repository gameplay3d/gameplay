#ifndef LUA_LOGGERLEVEL_H_
#define LUA_LOGGERLEVEL_H_

#include "Logger.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Logger::Level.
Logger::Level lua_enumFromString_LoggerLevel(const char* s);
const char* lua_stringFromEnum_LoggerLevel(Logger::Level e);

}

#endif
