#ifndef LUA_SPRITEBATCH_H_
#define LUA_SPRITEBATCH_H_

namespace gameplay
{

// Lua bindings for SpriteBatch.
int lua_SpriteBatch__gc(lua_State* state);
int lua_SpriteBatch_begin(lua_State* state);
int lua_SpriteBatch_draw(lua_State* state);
int lua_SpriteBatch_end(lua_State* state);
int lua_SpriteBatch_getMaterial(lua_State* state);
int lua_SpriteBatch_getStateBlock(lua_State* state);
int lua_SpriteBatch_setProjectionMatrix(lua_State* state);
int lua_SpriteBatch_static_create(lua_State* state);

void luaRegister_SpriteBatch();

}

#endif
