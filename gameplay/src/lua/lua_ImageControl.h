#ifndef LUA_IMAGECONTROL_H_
#define LUA_IMAGECONTROL_H_

namespace gameplay
{

// Lua bindings for ImageControl.
int lua_ImageControl__gc(lua_State* state);
int lua_ImageControl_addListener(lua_State* state);
int lua_ImageControl_addRef(lua_State* state);
int lua_ImageControl_addScriptCallback(lua_State* state);
int lua_ImageControl_canFocus(lua_State* state);
int lua_ImageControl_createAnimation(lua_State* state);
int lua_ImageControl_createAnimationFromBy(lua_State* state);
int lua_ImageControl_createAnimationFromTo(lua_State* state);
int lua_ImageControl_destroyAnimation(lua_State* state);
int lua_ImageControl_getAbsoluteBounds(lua_State* state);
int lua_ImageControl_getAlignment(lua_State* state);
int lua_ImageControl_getAnimation(lua_State* state);
int lua_ImageControl_getAnimationPropertyComponentCount(lua_State* state);
int lua_ImageControl_getAnimationPropertyValue(lua_State* state);
int lua_ImageControl_getAutoSize(lua_State* state);
int lua_ImageControl_getBorder(lua_State* state);
int lua_ImageControl_getBounds(lua_State* state);
int lua_ImageControl_getClip(lua_State* state);
int lua_ImageControl_getClipBounds(lua_State* state);
int lua_ImageControl_getConsumeInputEvents(lua_State* state);
int lua_ImageControl_getCursorColor(lua_State* state);
int lua_ImageControl_getCursorRegion(lua_State* state);
int lua_ImageControl_getCursorUVs(lua_State* state);
int lua_ImageControl_getFocusIndex(lua_State* state);
int lua_ImageControl_getFont(lua_State* state);
int lua_ImageControl_getFontSize(lua_State* state);
int lua_ImageControl_getHeight(lua_State* state);
int lua_ImageControl_getId(lua_State* state);
int lua_ImageControl_getImageColor(lua_State* state);
int lua_ImageControl_getImageRegion(lua_State* state);
int lua_ImageControl_getImageUVs(lua_State* state);
int lua_ImageControl_getMargin(lua_State* state);
int lua_ImageControl_getOpacity(lua_State* state);
int lua_ImageControl_getPadding(lua_State* state);
int lua_ImageControl_getParent(lua_State* state);
int lua_ImageControl_getRefCount(lua_State* state);
int lua_ImageControl_getRegionDst(lua_State* state);
int lua_ImageControl_getRegionSrc(lua_State* state);
int lua_ImageControl_getSkinColor(lua_State* state);
int lua_ImageControl_getSkinRegion(lua_State* state);
int lua_ImageControl_getState(lua_State* state);
int lua_ImageControl_getStyle(lua_State* state);
int lua_ImageControl_getTextAlignment(lua_State* state);
int lua_ImageControl_getTextColor(lua_State* state);
int lua_ImageControl_getTextRightToLeft(lua_State* state);
int lua_ImageControl_getTheme(lua_State* state);
int lua_ImageControl_getTopLevelForm(lua_State* state);
int lua_ImageControl_getType(lua_State* state);
int lua_ImageControl_getWidth(lua_State* state);
int lua_ImageControl_getX(lua_State* state);
int lua_ImageControl_getY(lua_State* state);
int lua_ImageControl_getZIndex(lua_State* state);
int lua_ImageControl_hasFocus(lua_State* state);
int lua_ImageControl_isChild(lua_State* state);
int lua_ImageControl_isContainer(lua_State* state);
int lua_ImageControl_isEnabled(lua_State* state);
int lua_ImageControl_isEnabledInHierarchy(lua_State* state);
int lua_ImageControl_isHeightPercentage(lua_State* state);
int lua_ImageControl_isVisible(lua_State* state);
int lua_ImageControl_isVisibleInHierarchy(lua_State* state);
int lua_ImageControl_isWidthPercentage(lua_State* state);
int lua_ImageControl_isXPercentage(lua_State* state);
int lua_ImageControl_isYPercentage(lua_State* state);
int lua_ImageControl_release(lua_State* state);
int lua_ImageControl_removeListener(lua_State* state);
int lua_ImageControl_removeScriptCallback(lua_State* state);
int lua_ImageControl_setAlignment(lua_State* state);
int lua_ImageControl_setAnimationPropertyValue(lua_State* state);
int lua_ImageControl_setAutoSize(lua_State* state);
int lua_ImageControl_setBorder(lua_State* state);
int lua_ImageControl_setBounds(lua_State* state);
int lua_ImageControl_setCanFocus(lua_State* state);
int lua_ImageControl_setConsumeInputEvents(lua_State* state);
int lua_ImageControl_setCursorColor(lua_State* state);
int lua_ImageControl_setCursorRegion(lua_State* state);
int lua_ImageControl_setEnabled(lua_State* state);
int lua_ImageControl_setFocus(lua_State* state);
int lua_ImageControl_setFocusIndex(lua_State* state);
int lua_ImageControl_setFont(lua_State* state);
int lua_ImageControl_setFontSize(lua_State* state);
int lua_ImageControl_setHeight(lua_State* state);
int lua_ImageControl_setId(lua_State* state);
int lua_ImageControl_setImage(lua_State* state);
int lua_ImageControl_setImageColor(lua_State* state);
int lua_ImageControl_setImageRegion(lua_State* state);
int lua_ImageControl_setMargin(lua_State* state);
int lua_ImageControl_setOpacity(lua_State* state);
int lua_ImageControl_setPadding(lua_State* state);
int lua_ImageControl_setPosition(lua_State* state);
int lua_ImageControl_setRegionDst(lua_State* state);
int lua_ImageControl_setRegionSrc(lua_State* state);
int lua_ImageControl_setSize(lua_State* state);
int lua_ImageControl_setSkinColor(lua_State* state);
int lua_ImageControl_setSkinRegion(lua_State* state);
int lua_ImageControl_setStyle(lua_State* state);
int lua_ImageControl_setTextAlignment(lua_State* state);
int lua_ImageControl_setTextColor(lua_State* state);
int lua_ImageControl_setTextRightToLeft(lua_State* state);
int lua_ImageControl_setVisible(lua_State* state);
int lua_ImageControl_setWidth(lua_State* state);
int lua_ImageControl_setX(lua_State* state);
int lua_ImageControl_setY(lua_State* state);
int lua_ImageControl_setZIndex(lua_State* state);
int lua_ImageControl_static_ANIMATE_OPACITY(lua_State* state);
int lua_ImageControl_static_ANIMATE_POSITION(lua_State* state);
int lua_ImageControl_static_ANIMATE_POSITION_X(lua_State* state);
int lua_ImageControl_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_ImageControl_static_ANIMATE_SIZE(lua_State* state);
int lua_ImageControl_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_ImageControl_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_ImageControl_static_create(lua_State* state);

void luaRegister_ImageControl();

}

#endif
