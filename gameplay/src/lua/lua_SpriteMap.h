#ifndef LUA_SPRITEMAP_H_
#define LUA_SPRITEMAP_H_

namespace gameplay
{

// Lua bindings for SpriteMap.
int lua_SpriteMap__gc(lua_State* state);
int lua_SpriteMap_addRef(lua_State* state);
int lua_SpriteMap_createAnimation(lua_State* state);
int lua_SpriteMap_createAnimationFromBy(lua_State* state);
int lua_SpriteMap_createAnimationFromTo(lua_State* state);
int lua_SpriteMap_destroyAnimation(lua_State* state);
int lua_SpriteMap_draw(lua_State* state);
int lua_SpriteMap_getAnimation(lua_State* state);
int lua_SpriteMap_getAnimationPropertyComponentCount(lua_State* state);
int lua_SpriteMap_getAnimationPropertyValue(lua_State* state);
int lua_SpriteMap_getDefaultTile(lua_State* state);
int lua_SpriteMap_getFlip(lua_State* state);
int lua_SpriteMap_getId(lua_State* state);
int lua_SpriteMap_getNode(lua_State* state);
int lua_SpriteMap_getRefCount(lua_State* state);
int lua_SpriteMap_getHeight(lua_State* state);
int lua_SpriteMap_getOffset(lua_State* state);
int lua_SpriteMap_getOffsetX(lua_State* state);
int lua_SpriteMap_getOffsetY(lua_State* state);
int lua_SpriteMap_getSize(lua_State* state);
int lua_SpriteMap_getWidth(lua_State* state);
int lua_SpriteMap_getTileSheet(lua_State* state);
int lua_SpriteMap_getTint(lua_State* state);
int lua_SpriteMap_release(lua_State* state);
int lua_SpriteMap_setAnimationPropertyValue(lua_State* state);
int lua_SpriteMap_setDefaultTile(lua_State* state);
int lua_SpriteMap_setFlip(lua_State* state);
int lua_SpriteMap_setNode(lua_State* state);
int lua_SpriteMap_setOffset(lua_State* state);
int lua_SpriteMap_setOffsetX(lua_State* state);
int lua_SpriteMap_setOffsetY(lua_State* state);
int lua_SpriteMap_setSize(lua_State* state);
int lua_SpriteMap_setTint(lua_State* state);
int lua_SpriteMap_static_ANIMATE_FRAME_INDEX(lua_State* state);
int lua_SpriteMap_static_ANIMATE_FRAME_SPECIFIC(lua_State* state);
int lua_SpriteMap_static_ANIMATE_OFFSET(lua_State* state);
int lua_SpriteMap_static_ANIMATE_SIZE(lua_State* state);
int lua_SpriteMap_static_ANIMATE_TINT(lua_State* state);
int lua_SpriteMap_static_create(lua_State* state);
int lua_SpriteMap_static_FLIP_HORZ(lua_State* state);
int lua_SpriteMap_static_FLIP_NONE(lua_State* state);
int lua_SpriteMap_static_FLIP_VERT(lua_State* state);
int lua_SpriteMap_static_getDefaultSpriteOffset(lua_State* state);
int lua_SpriteMap_static_setDefaultSpriteOffset(lua_State* state);
int lua_SpriteMap_static_getSpriteMap(lua_State* state);

void luaRegister_SpriteMap();

}

#endif
