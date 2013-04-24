#ifndef LUA_RADIOBUTTON_H_
#define LUA_RADIOBUTTON_H_

namespace gameplay
{

// Lua bindings for RadioButton.
int lua_RadioButton__gc(lua_State* state);
int lua_RadioButton_addListener(lua_State* state);
int lua_RadioButton_addRef(lua_State* state);
int lua_RadioButton_addScriptCallback(lua_State* state);
int lua_RadioButton_createAnimation(lua_State* state);
int lua_RadioButton_createAnimationFromBy(lua_State* state);
int lua_RadioButton_createAnimationFromTo(lua_State* state);
int lua_RadioButton_destroyAnimation(lua_State* state);
int lua_RadioButton_getAbsoluteBounds(lua_State* state);
int lua_RadioButton_getAlignment(lua_State* state);
int lua_RadioButton_getAnimation(lua_State* state);
int lua_RadioButton_getAnimationPropertyComponentCount(lua_State* state);
int lua_RadioButton_getAnimationPropertyValue(lua_State* state);
int lua_RadioButton_getAutoHeight(lua_State* state);
int lua_RadioButton_getAutoWidth(lua_State* state);
int lua_RadioButton_getBorder(lua_State* state);
int lua_RadioButton_getBounds(lua_State* state);
int lua_RadioButton_getClip(lua_State* state);
int lua_RadioButton_getClipBounds(lua_State* state);
int lua_RadioButton_getConsumeInputEvents(lua_State* state);
int lua_RadioButton_getCursorColor(lua_State* state);
int lua_RadioButton_getCursorRegion(lua_State* state);
int lua_RadioButton_getCursorUVs(lua_State* state);
int lua_RadioButton_getFocusIndex(lua_State* state);
int lua_RadioButton_getFont(lua_State* state);
int lua_RadioButton_getFontSize(lua_State* state);
int lua_RadioButton_getGroupId(lua_State* state);
int lua_RadioButton_getHeight(lua_State* state);
int lua_RadioButton_getId(lua_State* state);
int lua_RadioButton_getImageColor(lua_State* state);
int lua_RadioButton_getImageRegion(lua_State* state);
int lua_RadioButton_getImageSize(lua_State* state);
int lua_RadioButton_getImageUVs(lua_State* state);
int lua_RadioButton_getMargin(lua_State* state);
int lua_RadioButton_getOpacity(lua_State* state);
int lua_RadioButton_getPadding(lua_State* state);
int lua_RadioButton_getRefCount(lua_State* state);
int lua_RadioButton_getSkinColor(lua_State* state);
int lua_RadioButton_getSkinRegion(lua_State* state);
int lua_RadioButton_getState(lua_State* state);
int lua_RadioButton_getStyle(lua_State* state);
int lua_RadioButton_getText(lua_State* state);
int lua_RadioButton_getTextAlignment(lua_State* state);
int lua_RadioButton_getTextColor(lua_State* state);
int lua_RadioButton_getTextRightToLeft(lua_State* state);
int lua_RadioButton_getType(lua_State* state);
int lua_RadioButton_getWidth(lua_State* state);
int lua_RadioButton_getX(lua_State* state);
int lua_RadioButton_getY(lua_State* state);
int lua_RadioButton_getZIndex(lua_State* state);
int lua_RadioButton_isContainer(lua_State* state);
int lua_RadioButton_isEnabled(lua_State* state);
int lua_RadioButton_isSelected(lua_State* state);
int lua_RadioButton_isVisible(lua_State* state);
int lua_RadioButton_release(lua_State* state);
int lua_RadioButton_removeListener(lua_State* state);
int lua_RadioButton_removeScriptCallback(lua_State* state);
int lua_RadioButton_setAlignment(lua_State* state);
int lua_RadioButton_setAnimationPropertyValue(lua_State* state);
int lua_RadioButton_setAutoHeight(lua_State* state);
int lua_RadioButton_setAutoWidth(lua_State* state);
int lua_RadioButton_setBorder(lua_State* state);
int lua_RadioButton_setBounds(lua_State* state);
int lua_RadioButton_setConsumeInputEvents(lua_State* state);
int lua_RadioButton_setCursorColor(lua_State* state);
int lua_RadioButton_setCursorRegion(lua_State* state);
int lua_RadioButton_setEnabled(lua_State* state);
int lua_RadioButton_setFocusIndex(lua_State* state);
int lua_RadioButton_setFont(lua_State* state);
int lua_RadioButton_setFontSize(lua_State* state);
int lua_RadioButton_setGroupId(lua_State* state);
int lua_RadioButton_setHeight(lua_State* state);
int lua_RadioButton_setImageColor(lua_State* state);
int lua_RadioButton_setImageRegion(lua_State* state);
int lua_RadioButton_setImageSize(lua_State* state);
int lua_RadioButton_setMargin(lua_State* state);
int lua_RadioButton_setOpacity(lua_State* state);
int lua_RadioButton_setPadding(lua_State* state);
int lua_RadioButton_setPosition(lua_State* state);
int lua_RadioButton_setSelected(lua_State* state);
int lua_RadioButton_setSize(lua_State* state);
int lua_RadioButton_setSkinColor(lua_State* state);
int lua_RadioButton_setSkinRegion(lua_State* state);
int lua_RadioButton_setState(lua_State* state);
int lua_RadioButton_setStyle(lua_State* state);
int lua_RadioButton_setText(lua_State* state);
int lua_RadioButton_setTextAlignment(lua_State* state);
int lua_RadioButton_setTextColor(lua_State* state);
int lua_RadioButton_setTextRightToLeft(lua_State* state);
int lua_RadioButton_setVisible(lua_State* state);
int lua_RadioButton_setWidth(lua_State* state);
int lua_RadioButton_setZIndex(lua_State* state);
int lua_RadioButton_static_ANIMATE_OPACITY(lua_State* state);
int lua_RadioButton_static_ANIMATE_POSITION(lua_State* state);
int lua_RadioButton_static_ANIMATE_POSITION_X(lua_State* state);
int lua_RadioButton_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_RadioButton_static_ANIMATE_SIZE(lua_State* state);
int lua_RadioButton_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_RadioButton_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_RadioButton_static_create(lua_State* state);

void luaRegister_RadioButton();

}

#endif
