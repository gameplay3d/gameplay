#include "Base.h"
#include "lua_GameState.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_GameState_UNINITIALIZED = "UNINITIALIZED";
static const char* luaEnumString_GameState_RUNNING = "RUNNING";
static const char* luaEnumString_GameState_PAUSED = "PAUSED";

Game::State lua_enumFromString_GameState(const char* s)
{
    if (strcmp(s, luaEnumString_GameState_UNINITIALIZED) == 0)
        return Game::UNINITIALIZED;
    if (strcmp(s, luaEnumString_GameState_RUNNING) == 0)
        return Game::RUNNING;
    if (strcmp(s, luaEnumString_GameState_PAUSED) == 0)
        return Game::PAUSED;
    return Game::UNINITIALIZED;
}

const char* lua_stringFromEnum_GameState(Game::State e)
{
    if (e == Game::UNINITIALIZED)
        return luaEnumString_GameState_UNINITIALIZED;
    if (e == Game::RUNNING)
        return luaEnumString_GameState_RUNNING;
    if (e == Game::PAUSED)
        return luaEnumString_GameState_PAUSED;
    return enumStringEmpty;
}

}

