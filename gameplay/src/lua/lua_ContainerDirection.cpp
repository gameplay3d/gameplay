#include "Base.h"
#include "lua_ContainerDirection.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_ContainerDirection_UP = "UP";
static const char* luaEnumString_ContainerDirection_DOWN = "DOWN";
static const char* luaEnumString_ContainerDirection_LEFT = "LEFT";
static const char* luaEnumString_ContainerDirection_RIGHT = "RIGHT";
static const char* luaEnumString_ContainerDirection_NEXT = "NEXT";
static const char* luaEnumString_ContainerDirection_PREVIOUS = "PREVIOUS";

Container::Direction lua_enumFromString_ContainerDirection(const char* s)
{
    if (strcmp(s, luaEnumString_ContainerDirection_UP) == 0)
        return Container::UP;
    if (strcmp(s, luaEnumString_ContainerDirection_DOWN) == 0)
        return Container::DOWN;
    if (strcmp(s, luaEnumString_ContainerDirection_LEFT) == 0)
        return Container::LEFT;
    if (strcmp(s, luaEnumString_ContainerDirection_RIGHT) == 0)
        return Container::RIGHT;
    if (strcmp(s, luaEnumString_ContainerDirection_NEXT) == 0)
        return Container::NEXT;
    if (strcmp(s, luaEnumString_ContainerDirection_PREVIOUS) == 0)
        return Container::PREVIOUS;
    return Container::UP;
}

const char* lua_stringFromEnum_ContainerDirection(Container::Direction e)
{
    if (e == Container::UP)
        return luaEnumString_ContainerDirection_UP;
    if (e == Container::DOWN)
        return luaEnumString_ContainerDirection_DOWN;
    if (e == Container::LEFT)
        return luaEnumString_ContainerDirection_LEFT;
    if (e == Container::RIGHT)
        return luaEnumString_ContainerDirection_RIGHT;
    if (e == Container::NEXT)
        return luaEnumString_ContainerDirection_NEXT;
    if (e == Container::PREVIOUS)
        return luaEnumString_ContainerDirection_PREVIOUS;
    return enumStringEmpty;
}

}

