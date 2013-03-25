#ifndef LUA_SPRITEGROUP_H_
#define LUA_SPRITEGROUP_H_

namespace gameplay
{

// Lua bindings for SpriteGroup.
int lua_SpriteGroup__gc(lua_State* state);
int lua_SpriteGroup_addRef(lua_State* state);
int lua_SpriteGroup_createAnimation(lua_State* state);
int lua_SpriteGroup_createAnimationFromBy(lua_State* state);
int lua_SpriteGroup_createAnimationFromTo(lua_State* state);
int lua_SpriteGroup_destroyAnimation(lua_State* state);
int lua_SpriteGroup_draw(lua_State* state);
int lua_SpriteGroup_getAnimation(lua_State* state);
int lua_SpriteGroup_getAnimationPropertyComponentCount(lua_State* state);
int lua_SpriteGroup_getAnimationPropertyValue(lua_State* state);
int lua_SpriteGroup_getDefaultTile(lua_State* state);
int lua_SpriteGroup_getFlip(lua_State* state);
int lua_SpriteGroup_getId(lua_State* state);
int lua_SpriteGroup_getNode(lua_State* state);
int lua_SpriteGroup_getRefCount(lua_State* state);
int lua_SpriteGroup_getHeight(lua_State* state);
int lua_SpriteGroup_getOffset(lua_State* state);
int lua_SpriteGroup_getOffsetX(lua_State* state);
int lua_SpriteGroup_getOffsetY(lua_State* state);
int lua_SpriteGroup_getSize(lua_State* state);
int lua_SpriteGroup_getWidth(lua_State* state);
int lua_SpriteGroup_getTileSheet(lua_State* state);
int lua_SpriteGroup_getTint(lua_State* state);
int lua_SpriteGroup_release(lua_State* state);
int lua_SpriteGroup_setAnimationPropertyValue(lua_State* state);
int lua_SpriteGroup_setDefaultTile(lua_State* state);
int lua_SpriteGroup_setFlip(lua_State* state);
int lua_SpriteGroup_setNode(lua_State* state);
int lua_SpriteGroup_setOffset(lua_State* state);
int lua_SpriteGroup_setOffsetX(lua_State* state);
int lua_SpriteGroup_setOffsetY(lua_State* state);
int lua_SpriteGroup_setSize(lua_State* state);
int lua_SpriteGroup_setTint(lua_State* state);
int lua_SpriteGroup_static_ANIMATE_FRAME_INDEX(lua_State* state);
int lua_SpriteGroup_static_ANIMATE_FRAME_SPECIFIC(lua_State* state);
int lua_SpriteGroup_static_ANIMATE_OFFSET(lua_State* state);
int lua_SpriteGroup_static_ANIMATE_SIZE(lua_State* state);
int lua_SpriteGroup_static_ANIMATE_TINT(lua_State* state);
int lua_SpriteGroup_static_create(lua_State* state);
int lua_SpriteGroup_static_FLIP_HORZ(lua_State* state);
int lua_SpriteGroup_static_FLIP_NONE(lua_State* state);
int lua_SpriteGroup_static_FLIP_VERT(lua_State* state);
int lua_SpriteGroup_static_getDefaultSpriteOffset(lua_State* state);
int lua_SpriteGroup_static_setDefaultSpriteOffset(lua_State* state);
int lua_SpriteGroup_static_getSpriteGroup(lua_State* state);

void luaRegister_SpriteGroup();

}

#endif
