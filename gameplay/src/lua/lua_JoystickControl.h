#ifndef LUA_JOYSTICKCONTROL_H_
#define LUA_JOYSTICKCONTROL_H_

namespace gameplay
{

// Lua bindings for JoystickControl.
int lua_JoystickControl__gc(lua_State* state);
int lua_JoystickControl_addListener(lua_State* state);
int lua_JoystickControl_addRef(lua_State* state);
int lua_JoystickControl_addScriptCallback(lua_State* state);
int lua_JoystickControl_canFocus(lua_State* state);
int lua_JoystickControl_createAnimation(lua_State* state);
int lua_JoystickControl_createAnimationFromBy(lua_State* state);
int lua_JoystickControl_createAnimationFromTo(lua_State* state);
int lua_JoystickControl_destroyAnimation(lua_State* state);
int lua_JoystickControl_getAbsoluteBounds(lua_State* state);
int lua_JoystickControl_getAlignment(lua_State* state);
int lua_JoystickControl_getAnimation(lua_State* state);
int lua_JoystickControl_getAnimationPropertyComponentCount(lua_State* state);
int lua_JoystickControl_getAnimationPropertyValue(lua_State* state);
int lua_JoystickControl_getAutoSize(lua_State* state);
int lua_JoystickControl_getBorder(lua_State* state);
int lua_JoystickControl_getBounds(lua_State* state);
int lua_JoystickControl_getClip(lua_State* state);
int lua_JoystickControl_getClipBounds(lua_State* state);
int lua_JoystickControl_getConsumeInputEvents(lua_State* state);
int lua_JoystickControl_getCursorColor(lua_State* state);
int lua_JoystickControl_getCursorRegion(lua_State* state);
int lua_JoystickControl_getCursorUVs(lua_State* state);
int lua_JoystickControl_getFocusIndex(lua_State* state);
int lua_JoystickControl_getFont(lua_State* state);
int lua_JoystickControl_getFontSize(lua_State* state);
int lua_JoystickControl_getHeight(lua_State* state);
int lua_JoystickControl_getId(lua_State* state);
int lua_JoystickControl_getImageColor(lua_State* state);
int lua_JoystickControl_getImageRegion(lua_State* state);
int lua_JoystickControl_getImageUVs(lua_State* state);
int lua_JoystickControl_getIndex(lua_State* state);
int lua_JoystickControl_getInnerRegionSize(lua_State* state);
int lua_JoystickControl_getMargin(lua_State* state);
int lua_JoystickControl_getOpacity(lua_State* state);
int lua_JoystickControl_getOuterRegionSize(lua_State* state);
int lua_JoystickControl_getPadding(lua_State* state);
int lua_JoystickControl_getParent(lua_State* state);
int lua_JoystickControl_getRefCount(lua_State* state);
int lua_JoystickControl_getSkinColor(lua_State* state);
int lua_JoystickControl_getSkinRegion(lua_State* state);
int lua_JoystickControl_getState(lua_State* state);
int lua_JoystickControl_getStyle(lua_State* state);
int lua_JoystickControl_getTextAlignment(lua_State* state);
int lua_JoystickControl_getTextColor(lua_State* state);
int lua_JoystickControl_getTextRightToLeft(lua_State* state);
int lua_JoystickControl_getTheme(lua_State* state);
int lua_JoystickControl_getTopLevelForm(lua_State* state);
int lua_JoystickControl_getType(lua_State* state);
int lua_JoystickControl_getValue(lua_State* state);
int lua_JoystickControl_getWidth(lua_State* state);
int lua_JoystickControl_getX(lua_State* state);
int lua_JoystickControl_getY(lua_State* state);
int lua_JoystickControl_getZIndex(lua_State* state);
int lua_JoystickControl_hasFocus(lua_State* state);
int lua_JoystickControl_isChild(lua_State* state);
int lua_JoystickControl_isContainer(lua_State* state);
int lua_JoystickControl_isEnabled(lua_State* state);
int lua_JoystickControl_isEnabledInHierarchy(lua_State* state);
int lua_JoystickControl_isHeightPercentage(lua_State* state);
int lua_JoystickControl_isRelative(lua_State* state);
int lua_JoystickControl_isVisible(lua_State* state);
int lua_JoystickControl_isVisibleInHierarchy(lua_State* state);
int lua_JoystickControl_isWidthPercentage(lua_State* state);
int lua_JoystickControl_isXPercentage(lua_State* state);
int lua_JoystickControl_isYPercentage(lua_State* state);
int lua_JoystickControl_release(lua_State* state);
int lua_JoystickControl_removeListener(lua_State* state);
int lua_JoystickControl_removeScriptCallback(lua_State* state);
int lua_JoystickControl_setAlignment(lua_State* state);
int lua_JoystickControl_setAnimationPropertyValue(lua_State* state);
int lua_JoystickControl_setAutoSize(lua_State* state);
int lua_JoystickControl_setBorder(lua_State* state);
int lua_JoystickControl_setBounds(lua_State* state);
int lua_JoystickControl_setCanFocus(lua_State* state);
int lua_JoystickControl_setConsumeInputEvents(lua_State* state);
int lua_JoystickControl_setCursorColor(lua_State* state);
int lua_JoystickControl_setCursorRegion(lua_State* state);
int lua_JoystickControl_setEnabled(lua_State* state);
int lua_JoystickControl_setFocus(lua_State* state);
int lua_JoystickControl_setFocusIndex(lua_State* state);
int lua_JoystickControl_setFont(lua_State* state);
int lua_JoystickControl_setFontSize(lua_State* state);
int lua_JoystickControl_setHeight(lua_State* state);
int lua_JoystickControl_setId(lua_State* state);
int lua_JoystickControl_setImageColor(lua_State* state);
int lua_JoystickControl_setImageRegion(lua_State* state);
int lua_JoystickControl_setInnerRegionSize(lua_State* state);
int lua_JoystickControl_setMargin(lua_State* state);
int lua_JoystickControl_setOpacity(lua_State* state);
int lua_JoystickControl_setOuterRegionSize(lua_State* state);
int lua_JoystickControl_setPadding(lua_State* state);
int lua_JoystickControl_setPosition(lua_State* state);
int lua_JoystickControl_setRelative(lua_State* state);
int lua_JoystickControl_setSize(lua_State* state);
int lua_JoystickControl_setSkinColor(lua_State* state);
int lua_JoystickControl_setSkinRegion(lua_State* state);
int lua_JoystickControl_setStyle(lua_State* state);
int lua_JoystickControl_setTextAlignment(lua_State* state);
int lua_JoystickControl_setTextColor(lua_State* state);
int lua_JoystickControl_setTextRightToLeft(lua_State* state);
int lua_JoystickControl_setVisible(lua_State* state);
int lua_JoystickControl_setWidth(lua_State* state);
int lua_JoystickControl_setX(lua_State* state);
int lua_JoystickControl_setY(lua_State* state);
int lua_JoystickControl_setZIndex(lua_State* state);
int lua_JoystickControl_static_ANIMATE_OPACITY(lua_State* state);
int lua_JoystickControl_static_ANIMATE_POSITION(lua_State* state);
int lua_JoystickControl_static_ANIMATE_POSITION_X(lua_State* state);
int lua_JoystickControl_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_JoystickControl_static_ANIMATE_SIZE(lua_State* state);
int lua_JoystickControl_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_JoystickControl_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_JoystickControl_static_create(lua_State* state);

void luaRegister_JoystickControl();

}

#endif
