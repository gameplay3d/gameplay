#ifndef LUA_SPRITE_H_
#define LUA_SPRITE_H_

namespace gameplay
{

// Lua bindings for Sprite.
int lua_Sprite__gc(lua_State* state);
int lua_Sprite_addRef(lua_State* state);
int lua_Sprite_getRefCount(lua_State* state);
int lua_Sprite_getId(lua_State* state);
int lua_Sprite_getNode(lua_State* state);
int lua_Sprite_getTileSheet(lua_State* state);
int lua_Sprite_release(lua_State* state);
int lua_Sprite_setNode(lua_State* state);
int lua_Sprite_static_create(lua_State* state);

//TODO: need to add AnimationTarget functions

void luaRegister_Sprite();

}

#endif
