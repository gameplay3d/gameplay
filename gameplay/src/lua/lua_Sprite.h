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
int lua_Sprite_draw(lua_State* state);
int lua_Sprite_getAnimation(lua_State* state);
int lua_Sprite_getAnimationPropertyComponentCount(lua_State* state);
int lua_Sprite_getAnimationPropertyValue(lua_State* state);
int lua_Sprite_getDefaultTile(lua_State* state);
int lua_Sprite_getFlip(lua_State* state);
int lua_Sprite_getId(lua_State* state);
int lua_Sprite_getNode(lua_State* state);
int lua_Sprite_getRefCount(lua_State* state);
int lua_Sprite_getSpriteHeight(lua_State* state);
int lua_Sprite_getSpriteOffset(lua_State* state);
int lua_Sprite_getSpriteOffsetX(lua_State* state);
int lua_Sprite_getSpriteOffsetY(lua_State* state);
int lua_Sprite_getSpriteSize(lua_State* state);
int lua_Sprite_getSpriteWidth(lua_State* state);
int lua_Sprite_getTileSheet(lua_State* state);
int lua_Sprite_release(lua_State* state);
int lua_Sprite_setAnimationPropertyValue(lua_State* state);
int lua_Sprite_setDefaultTile(lua_State* state);
int lua_Sprite_setFlip(lua_State* state);
int lua_Sprite_setNode(lua_State* state);
int lua_Sprite_setSpriteOffset(lua_State* state);
int lua_Sprite_setSpriteOffsetX(lua_State* state);
int lua_Sprite_setSpriteOffsetY(lua_State* state);
int lua_Sprite_setSpriteSize(lua_State* state);
int lua_Sprite_static_create(lua_State* state);
int lua_Sprite_static_FLIP_HORZ(lua_State* state);
int lua_Sprite_static_FLIP_NONE(lua_State* state);
int lua_Sprite_static_FLIP_VERT(lua_State* state);

void luaRegister_Sprite();

}

#endif
