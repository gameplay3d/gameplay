#ifndef LUA_SPRITE_H_
#define LUA_SPRITE_H_

namespace gameplay
{

// Lua bindings for Sprite.
int lua_Sprite__gc(lua_State* state);
int lua_Sprite_addRef(lua_State* state);
int lua_Sprite_createAnimation(lua_State* state);
int lua_Sprite_createAnimationFromBy(lua_State* state);
int lua_Sprite_createAnimationFromTo(lua_State* state);
int lua_Sprite_destroyAnimation(lua_State* state);
int lua_Sprite_getAnimation(lua_State* state);
int lua_Sprite_getAnimationPropertyComponentCount(lua_State* state);
int lua_Sprite_getAnimationPropertyValue(lua_State* state);
int lua_Sprite_getRefCount(lua_State* state);
int lua_Sprite_getId(lua_State* state);
int lua_Sprite_getNode(lua_State* state);
int lua_Sprite_getTileSheet(lua_State* state);
int lua_Sprite_release(lua_State* state);
int lua_Sprite_setAnimationPropertyValue(lua_State* state);
int lua_Sprite_setNode(lua_State* state);
int lua_Sprite_static_create(lua_State* state);

void luaRegister_Sprite();

}

#endif
