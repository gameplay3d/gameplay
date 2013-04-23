#include "Base.h"
#include "lua_PropertiesType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_PropertiesType_NONE = "NONE";
static const char* luaEnumString_PropertiesType_STRING = "STRING";
static const char* luaEnumString_PropertiesType_NUMBER = "NUMBER";
static const char* luaEnumString_PropertiesType_VECTOR2 = "VECTOR2";
static const char* luaEnumString_PropertiesType_VECTOR3 = "VECTOR3";
static const char* luaEnumString_PropertiesType_VECTOR4 = "VECTOR4";
static const char* luaEnumString_PropertiesType_MATRIX = "MATRIX";

Properties::Type lua_enumFromString_PropertiesType(const char* s)
{
    if (strcmp(s, luaEnumString_PropertiesType_NONE) == 0)
        return Properties::NONE;
    if (strcmp(s, luaEnumString_PropertiesType_STRING) == 0)
        return Properties::STRING;
    if (strcmp(s, luaEnumString_PropertiesType_NUMBER) == 0)
        return Properties::NUMBER;
    if (strcmp(s, luaEnumString_PropertiesType_VECTOR2) == 0)
        return Properties::VECTOR2;
    if (strcmp(s, luaEnumString_PropertiesType_VECTOR3) == 0)
        return Properties::VECTOR3;
    if (strcmp(s, luaEnumString_PropertiesType_VECTOR4) == 0)
        return Properties::VECTOR4;
    if (strcmp(s, luaEnumString_PropertiesType_MATRIX) == 0)
        return Properties::MATRIX;
    return Properties::NONE;
}

const char* lua_stringFromEnum_PropertiesType(Properties::Type e)
{
    if (e == Properties::NONE)
        return luaEnumString_PropertiesType_NONE;
    if (e == Properties::STRING)
        return luaEnumString_PropertiesType_STRING;
    if (e == Properties::NUMBER)
        return luaEnumString_PropertiesType_NUMBER;
    if (e == Properties::VECTOR2)
        return luaEnumString_PropertiesType_VECTOR2;
    if (e == Properties::VECTOR3)
        return luaEnumString_PropertiesType_VECTOR3;
    if (e == Properties::VECTOR4)
        return luaEnumString_PropertiesType_VECTOR4;
    if (e == Properties::MATRIX)
        return luaEnumString_PropertiesType_MATRIX;
    return enumStringEmpty;
}

}

