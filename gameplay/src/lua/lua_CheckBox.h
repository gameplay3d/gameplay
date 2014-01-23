#ifndef LUA_CHECKBOX_H_
#define LUA_CHECKBOX_H_

namespace gameplay
{

// Lua bindings for CheckBox.
int lua_CheckBox__gc(lua_State* state);
int lua_CheckBox_addListener(lua_State* state);
int lua_CheckBox_addRef(lua_State* state);
int lua_CheckBox_addScriptCallback(lua_State* state);
int lua_CheckBox_canFocus(lua_State* state);
int lua_CheckBox_createAnimation(lua_State* state);
int lua_CheckBox_createAnimationFromBy(lua_State* state);
int lua_CheckBox_createAnimationFromTo(lua_State* state);
int lua_CheckBox_destroyAnimation(lua_State* state);
int lua_CheckBox_getAbsoluteBounds(lua_State* state);
int lua_CheckBox_getAlignment(lua_State* state);
int lua_CheckBox_getAnimation(lua_State* state);
int lua_CheckBox_getAnimationPropertyComponentCount(lua_State* state);
int lua_CheckBox_getAnimationPropertyValue(lua_State* state);
int lua_CheckBox_getAutoSize(lua_State* state);
int lua_CheckBox_getBorder(lua_State* state);
int lua_CheckBox_getBounds(lua_State* state);
int lua_CheckBox_getClip(lua_State* state);
int lua_CheckBox_getClipBounds(lua_State* state);
int lua_CheckBox_getConsumeInputEvents(lua_State* state);
int lua_CheckBox_getCursorColor(lua_State* state);
int lua_CheckBox_getCursorRegion(lua_State* state);
int lua_CheckBox_getCursorUVs(lua_State* state);
int lua_CheckBox_getFocusIndex(lua_State* state);
int lua_CheckBox_getFont(lua_State* state);
int lua_CheckBox_getFontSize(lua_State* state);
int lua_CheckBox_getHeight(lua_State* state);
int lua_CheckBox_getId(lua_State* state);
int lua_CheckBox_getImageColor(lua_State* state);
int lua_CheckBox_getImageRegion(lua_State* state);
int lua_CheckBox_getImageUVs(lua_State* state);
int lua_CheckBox_getMargin(lua_State* state);
int lua_CheckBox_getOpacity(lua_State* state);
int lua_CheckBox_getPadding(lua_State* state);
int lua_CheckBox_getParent(lua_State* state);
int lua_CheckBox_getRefCount(lua_State* state);
int lua_CheckBox_getSkinColor(lua_State* state);
int lua_CheckBox_getSkinRegion(lua_State* state);
int lua_CheckBox_getState(lua_State* state);
int lua_CheckBox_getStyle(lua_State* state);
int lua_CheckBox_getText(lua_State* state);
int lua_CheckBox_getTextAlignment(lua_State* state);
int lua_CheckBox_getTextColor(lua_State* state);
int lua_CheckBox_getTextRightToLeft(lua_State* state);
int lua_CheckBox_getTheme(lua_State* state);
int lua_CheckBox_getTopLevelForm(lua_State* state);
int lua_CheckBox_getType(lua_State* state);
int lua_CheckBox_getWidth(lua_State* state);
int lua_CheckBox_getX(lua_State* state);
int lua_CheckBox_getY(lua_State* state);
int lua_CheckBox_getZIndex(lua_State* state);
int lua_CheckBox_hasFocus(lua_State* state);
int lua_CheckBox_isChecked(lua_State* state);
int lua_CheckBox_isChild(lua_State* state);
int lua_CheckBox_isContainer(lua_State* state);
int lua_CheckBox_isEnabled(lua_State* state);
int lua_CheckBox_isEnabledInHierarchy(lua_State* state);
int lua_CheckBox_isHeightPercentage(lua_State* state);
int lua_CheckBox_isVisible(lua_State* state);
int lua_CheckBox_isVisibleInHierarchy(lua_State* state);
int lua_CheckBox_isWidthPercentage(lua_State* state);
int lua_CheckBox_isXPercentage(lua_State* state);
int lua_CheckBox_isYPercentage(lua_State* state);
int lua_CheckBox_release(lua_State* state);
int lua_CheckBox_removeListener(lua_State* state);
int lua_CheckBox_removeScriptCallback(lua_State* state);
int lua_CheckBox_setAlignment(lua_State* state);
int lua_CheckBox_setAnimationPropertyValue(lua_State* state);
int lua_CheckBox_setAutoSize(lua_State* state);
int lua_CheckBox_setBorder(lua_State* state);
int lua_CheckBox_setBounds(lua_State* state);
int lua_CheckBox_setCanFocus(lua_State* state);
int lua_CheckBox_setChecked(lua_State* state);
int lua_CheckBox_setConsumeInputEvents(lua_State* state);
int lua_CheckBox_setCursorColor(lua_State* state);
int lua_CheckBox_setCursorRegion(lua_State* state);
int lua_CheckBox_setEnabled(lua_State* state);
int lua_CheckBox_setFocus(lua_State* state);
int lua_CheckBox_setFocusIndex(lua_State* state);
int lua_CheckBox_setFont(lua_State* state);
int lua_CheckBox_setFontSize(lua_State* state);
int lua_CheckBox_setHeight(lua_State* state);
int lua_CheckBox_setId(lua_State* state);
int lua_CheckBox_setImageColor(lua_State* state);
int lua_CheckBox_setImageRegion(lua_State* state);
int lua_CheckBox_setMargin(lua_State* state);
int lua_CheckBox_setOpacity(lua_State* state);
int lua_CheckBox_setPadding(lua_State* state);
int lua_CheckBox_setPosition(lua_State* state);
int lua_CheckBox_setSize(lua_State* state);
int lua_CheckBox_setSkinColor(lua_State* state);
int lua_CheckBox_setSkinRegion(lua_State* state);
int lua_CheckBox_setStyle(lua_State* state);
int lua_CheckBox_setText(lua_State* state);
int lua_CheckBox_setTextAlignment(lua_State* state);
int lua_CheckBox_setTextColor(lua_State* state);
int lua_CheckBox_setTextRightToLeft(lua_State* state);
int lua_CheckBox_setVisible(lua_State* state);
int lua_CheckBox_setWidth(lua_State* state);
int lua_CheckBox_setX(lua_State* state);
int lua_CheckBox_setY(lua_State* state);
int lua_CheckBox_setZIndex(lua_State* state);
int lua_CheckBox_static_ANIMATE_OPACITY(lua_State* state);
int lua_CheckBox_static_ANIMATE_POSITION(lua_State* state);
int lua_CheckBox_static_ANIMATE_POSITION_X(lua_State* state);
int lua_CheckBox_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_CheckBox_static_ANIMATE_SIZE(lua_State* state);
int lua_CheckBox_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_CheckBox_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_CheckBox_static_create(lua_State* state);

void luaRegister_CheckBox();

}

#endif
