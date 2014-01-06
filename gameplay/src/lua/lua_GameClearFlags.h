#ifndef LUA_GAMECLEARFLAGS_H_
#define LUA_GAMECLEARFLAGS_H_

#include "Game.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Game::ClearFlags.
Game::ClearFlags lua_enumFromString_GameClearFlags(const char* s);
const char* lua_stringFromEnum_GameClearFlags(Game::ClearFlags e);

}

#endif
