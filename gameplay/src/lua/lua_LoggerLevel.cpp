#include "Base.h"
#include "lua_LoggerLevel.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_LoggerLevel_LEVEL_INFO = "LEVEL_INFO";
static const char* luaEnumString_LoggerLevel_LEVEL_WARN = "LEVEL_WARN";
static const char* luaEnumString_LoggerLevel_LEVEL_ERROR = "LEVEL_ERROR";

Logger::Level lua_enumFromString_LoggerLevel(const char* s)
{
    if (strcmp(s, luaEnumString_LoggerLevel_LEVEL_INFO) == 0)
        return Logger::LEVEL_INFO;
    if (strcmp(s, luaEnumString_LoggerLevel_LEVEL_WARN) == 0)
        return Logger::LEVEL_WARN;
    if (strcmp(s, luaEnumString_LoggerLevel_LEVEL_ERROR) == 0)
        return Logger::LEVEL_ERROR;
    return Logger::LEVEL_INFO;
}

const char* lua_stringFromEnum_LoggerLevel(Logger::Level e)
{
    if (e == Logger::LEVEL_INFO)
        return luaEnumString_LoggerLevel_LEVEL_INFO;
    if (e == Logger::LEVEL_WARN)
        return luaEnumString_LoggerLevel_LEVEL_WARN;
    if (e == Logger::LEVEL_ERROR)
        return luaEnumString_LoggerLevel_LEVEL_ERROR;
    return enumStringEmpty;
}

}

