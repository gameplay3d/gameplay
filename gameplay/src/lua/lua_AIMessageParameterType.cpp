#include "Base.h"
#include "lua_AIMessageParameterType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_AIMessageParameterType_UNDEFINED = "UNDEFINED";
static const char* luaEnumString_AIMessageParameterType_INTEGER = "INTEGER";
static const char* luaEnumString_AIMessageParameterType_LONG = "LONG";
static const char* luaEnumString_AIMessageParameterType_FLOAT = "FLOAT";
static const char* luaEnumString_AIMessageParameterType_DOUBLE = "DOUBLE";
static const char* luaEnumString_AIMessageParameterType_BOOLEAN = "BOOLEAN";
static const char* luaEnumString_AIMessageParameterType_STRING = "STRING";

AIMessage::ParameterType lua_enumFromString_AIMessageParameterType(const char* s)
{
    if (strcmp(s, luaEnumString_AIMessageParameterType_UNDEFINED) == 0)
        return AIMessage::UNDEFINED;
    if (strcmp(s, luaEnumString_AIMessageParameterType_INTEGER) == 0)
        return AIMessage::INTEGER;
    if (strcmp(s, luaEnumString_AIMessageParameterType_LONG) == 0)
        return AIMessage::LONG;
    if (strcmp(s, luaEnumString_AIMessageParameterType_FLOAT) == 0)
        return AIMessage::FLOAT;
    if (strcmp(s, luaEnumString_AIMessageParameterType_DOUBLE) == 0)
        return AIMessage::DOUBLE;
    if (strcmp(s, luaEnumString_AIMessageParameterType_BOOLEAN) == 0)
        return AIMessage::BOOLEAN;
    if (strcmp(s, luaEnumString_AIMessageParameterType_STRING) == 0)
        return AIMessage::STRING;
    return AIMessage::UNDEFINED;
}

const char* lua_stringFromEnum_AIMessageParameterType(AIMessage::ParameterType e)
{
    if (e == AIMessage::UNDEFINED)
        return luaEnumString_AIMessageParameterType_UNDEFINED;
    if (e == AIMessage::INTEGER)
        return luaEnumString_AIMessageParameterType_INTEGER;
    if (e == AIMessage::LONG)
        return luaEnumString_AIMessageParameterType_LONG;
    if (e == AIMessage::FLOAT)
        return luaEnumString_AIMessageParameterType_FLOAT;
    if (e == AIMessage::DOUBLE)
        return luaEnumString_AIMessageParameterType_DOUBLE;
    if (e == AIMessage::BOOLEAN)
        return luaEnumString_AIMessageParameterType_BOOLEAN;
    if (e == AIMessage::STRING)
        return luaEnumString_AIMessageParameterType_STRING;
    return enumStringEmpty;
}

}

