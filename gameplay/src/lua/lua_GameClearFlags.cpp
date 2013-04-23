#include "Base.h"
#include "lua_GameClearFlags.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_GameClearFlags_CLEAR_COLOR = "CLEAR_COLOR";
static const char* luaEnumString_GameClearFlags_CLEAR_DEPTH = "CLEAR_DEPTH";
static const char* luaEnumString_GameClearFlags_CLEAR_STENCIL = "CLEAR_STENCIL";
static const char* luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH = "CLEAR_COLOR_DEPTH";
static const char* luaEnumString_GameClearFlags_CLEAR_COLOR_STENCIL = "CLEAR_COLOR_STENCIL";
static const char* luaEnumString_GameClearFlags_CLEAR_DEPTH_STENCIL = "CLEAR_DEPTH_STENCIL";
static const char* luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH_STENCIL = "CLEAR_COLOR_DEPTH_STENCIL";

Game::ClearFlags lua_enumFromString_GameClearFlags(const char* s)
{
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_COLOR) == 0)
        return Game::CLEAR_COLOR;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_DEPTH) == 0)
        return Game::CLEAR_DEPTH;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_STENCIL) == 0)
        return Game::CLEAR_STENCIL;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH) == 0)
        return Game::CLEAR_COLOR_DEPTH;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_COLOR_STENCIL) == 0)
        return Game::CLEAR_COLOR_STENCIL;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_DEPTH_STENCIL) == 0)
        return Game::CLEAR_DEPTH_STENCIL;
    if (strcmp(s, luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH_STENCIL) == 0)
        return Game::CLEAR_COLOR_DEPTH_STENCIL;
    return Game::CLEAR_COLOR;
}

const char* lua_stringFromEnum_GameClearFlags(Game::ClearFlags e)
{
    if (e == Game::CLEAR_COLOR)
        return luaEnumString_GameClearFlags_CLEAR_COLOR;
    if (e == Game::CLEAR_DEPTH)
        return luaEnumString_GameClearFlags_CLEAR_DEPTH;
    if (e == Game::CLEAR_STENCIL)
        return luaEnumString_GameClearFlags_CLEAR_STENCIL;
    if (e == Game::CLEAR_COLOR_DEPTH)
        return luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH;
    if (e == Game::CLEAR_COLOR_STENCIL)
        return luaEnumString_GameClearFlags_CLEAR_COLOR_STENCIL;
    if (e == Game::CLEAR_DEPTH_STENCIL)
        return luaEnumString_GameClearFlags_CLEAR_DEPTH_STENCIL;
    if (e == Game::CLEAR_COLOR_DEPTH_STENCIL)
        return luaEnumString_GameClearFlags_CLEAR_COLOR_DEPTH_STENCIL;
    return enumStringEmpty;
}

}

