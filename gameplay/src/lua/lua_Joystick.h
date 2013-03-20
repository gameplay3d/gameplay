#ifndef LUA_JOYSTICK_H_
#define LUA_JOYSTICK_H_

namespace gameplay
{

// Lua bindings for Joystick.
int lua_Joystick__gc(lua_State* state);
int lua_Joystick_addListener(lua_State* state);
int lua_Joystick_addRef(lua_State* state);
int lua_Joystick_addScriptCallback(lua_State* state);
int lua_Joystick_createAnimation(lua_State* state);
int lua_Joystick_createAnimationFromBy(lua_State* state);
int lua_Joystick_createAnimationFromTo(lua_State* state);
int lua_Joystick_destroyAnimation(lua_State* state);
int lua_Joystick_getAbsoluteBounds(lua_State* state);
int lua_Joystick_getAlignment(lua_State* state);
int lua_Joystick_getAnimation(lua_State* state);
int lua_Joystick_getAnimationPropertyComponentCount(lua_State* state);
int lua_Joystick_getAnimationPropertyValue(lua_State* state);
int lua_Joystick_getAutoHeight(lua_State* state);
int lua_Joystick_getAutoWidth(lua_State* state);
int lua_Joystick_getBorder(lua_State* state);
int lua_Joystick_getBounds(lua_State* state);
int lua_Joystick_getClip(lua_State* state);
int lua_Joystick_getClipBounds(lua_State* state);
int lua_Joystick_getConsumeInputEvents(lua_State* state);
int lua_Joystick_getCursorColor(lua_State* state);
int lua_Joystick_getCursorRegion(lua_State* state);
int lua_Joystick_getCursorUVs(lua_State* state);
int lua_Joystick_getFocusIndex(lua_State* state);
int lua_Joystick_getFont(lua_State* state);
int lua_Joystick_getFontSize(lua_State* state);
int lua_Joystick_getHeight(lua_State* state);
int lua_Joystick_getId(lua_State* state);
int lua_Joystick_getImageColor(lua_State* state);
int lua_Joystick_getImageRegion(lua_State* state);
int lua_Joystick_getImageUVs(lua_State* state);
int lua_Joystick_getIndex(lua_State* state);
int lua_Joystick_getInnerRegionSize(lua_State* state);
int lua_Joystick_getMargin(lua_State* state);
int lua_Joystick_getOpacity(lua_State* state);
int lua_Joystick_getOuterRegionSize(lua_State* state);
int lua_Joystick_getPadding(lua_State* state);
int lua_Joystick_getRefCount(lua_State* state);
int lua_Joystick_getSkinColor(lua_State* state);
int lua_Joystick_getSkinRegion(lua_State* state);
int lua_Joystick_getState(lua_State* state);
int lua_Joystick_getStyle(lua_State* state);
int lua_Joystick_getTextAlignment(lua_State* state);
int lua_Joystick_getTextColor(lua_State* state);
int lua_Joystick_getTextRightToLeft(lua_State* state);
int lua_Joystick_getType(lua_State* state);
int lua_Joystick_getValue(lua_State* state);
int lua_Joystick_getWidth(lua_State* state);
int lua_Joystick_getX(lua_State* state);
int lua_Joystick_getY(lua_State* state);
int lua_Joystick_getZIndex(lua_State* state);
int lua_Joystick_isContainer(lua_State* state);
int lua_Joystick_isEnabled(lua_State* state);
int lua_Joystick_isRelative(lua_State* state);
int lua_Joystick_isVisible(lua_State* state);
int lua_Joystick_release(lua_State* state);
int lua_Joystick_removeListener(lua_State* state);
int lua_Joystick_removeScriptCallback(lua_State* state);
int lua_Joystick_setAlignment(lua_State* state);
int lua_Joystick_setAnimationPropertyValue(lua_State* state);
int lua_Joystick_setAutoHeight(lua_State* state);
int lua_Joystick_setAutoWidth(lua_State* state);
int lua_Joystick_setBorder(lua_State* state);
int lua_Joystick_setBounds(lua_State* state);
int lua_Joystick_setConsumeInputEvents(lua_State* state);
int lua_Joystick_setCursorColor(lua_State* state);
int lua_Joystick_setCursorRegion(lua_State* state);
int lua_Joystick_setEnabled(lua_State* state);
int lua_Joystick_setFocusIndex(lua_State* state);
int lua_Joystick_setFont(lua_State* state);
int lua_Joystick_setFontSize(lua_State* state);
int lua_Joystick_setHeight(lua_State* state);
int lua_Joystick_setImageColor(lua_State* state);
int lua_Joystick_setImageRegion(lua_State* state);
int lua_Joystick_setInnerRegionSize(lua_State* state);
int lua_Joystick_setMargin(lua_State* state);
int lua_Joystick_setOpacity(lua_State* state);
int lua_Joystick_setOuterRegionSize(lua_State* state);
int lua_Joystick_setPadding(lua_State* state);
int lua_Joystick_setPosition(lua_State* state);
int lua_Joystick_setRelative(lua_State* state);
int lua_Joystick_setSize(lua_State* state);
int lua_Joystick_setSkinColor(lua_State* state);
int lua_Joystick_setSkinRegion(lua_State* state);
int lua_Joystick_setState(lua_State* state);
int lua_Joystick_setStyle(lua_State* state);
int lua_Joystick_setTextAlignment(lua_State* state);
int lua_Joystick_setTextColor(lua_State* state);
int lua_Joystick_setTextRightToLeft(lua_State* state);
int lua_Joystick_setVisible(lua_State* state);
int lua_Joystick_setWidth(lua_State* state);
int lua_Joystick_setZIndex(lua_State* state);
int lua_Joystick_static_ANIMATE_OPACITY(lua_State* state);
int lua_Joystick_static_ANIMATE_POSITION(lua_State* state);
int lua_Joystick_static_ANIMATE_POSITION_X(lua_State* state);
int lua_Joystick_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_Joystick_static_ANIMATE_SIZE(lua_State* state);
int lua_Joystick_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_Joystick_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_Joystick_static_create(lua_State* state);

void luaRegister_Joystick();

}

#endif
