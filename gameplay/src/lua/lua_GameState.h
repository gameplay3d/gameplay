#ifndef LUA_GAMESTATE_H_
#define LUA_GAMESTATE_H_

#include "Game.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Game::State.
Game::State lua_enumFromString_GameState(const char* s);
const char* lua_stringFromEnum_GameState(Game::State e);

}

#endif
