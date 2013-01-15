#ifndef LUA_GAMEPADBUTTON_H_
#define LUA_GAMEPADBUTTON_H_

namespace gameplay
{

// Lua bindings for GamepadButton.
int lua_GamepadButton__gc(lua_State* state);
int lua_GamepadButton_addListener(lua_State* state);
int lua_GamepadButton_addRef(lua_State* state);
int lua_GamepadButton_addScriptCallback(lua_State* state);
int lua_GamepadButton_createAnimation(lua_State* state);
int lua_GamepadButton_createAnimationFromBy(lua_State* state);
int lua_GamepadButton_createAnimationFromTo(lua_State* state);
int lua_GamepadButton_destroyAnimation(lua_State* state);
int lua_GamepadButton_getAlignment(lua_State* state);
int lua_GamepadButton_getAnimation(lua_State* state);
int lua_GamepadButton_getAnimationPropertyComponentCount(lua_State* state);
int lua_GamepadButton_getAnimationPropertyValue(lua_State* state);
int lua_GamepadButton_getAutoHeight(lua_State* state);
int lua_GamepadButton_getAutoWidth(lua_State* state);
int lua_GamepadButton_getBorder(lua_State* state);
int lua_GamepadButton_getBounds(lua_State* state);
int lua_GamepadButton_getClip(lua_State* state);
int lua_GamepadButton_getClipBounds(lua_State* state);
int lua_GamepadButton_getConsumeInputEvents(lua_State* state);
int lua_GamepadButton_getCursorColor(lua_State* state);
int lua_GamepadButton_getCursorRegion(lua_State* state);
int lua_GamepadButton_getCursorUVs(lua_State* state);
int lua_GamepadButton_getFocusIndex(lua_State* state);
int lua_GamepadButton_getFont(lua_State* state);
int lua_GamepadButton_getFontSize(lua_State* state);
int lua_GamepadButton_getHeight(lua_State* state);
int lua_GamepadButton_getId(lua_State* state);
int lua_GamepadButton_getImageColor(lua_State* state);
int lua_GamepadButton_getImageRegion(lua_State* state);
int lua_GamepadButton_getImageUVs(lua_State* state);
int lua_GamepadButton_getMargin(lua_State* state);
int lua_GamepadButton_getOpacity(lua_State* state);
int lua_GamepadButton_getPadding(lua_State* state);
int lua_GamepadButton_getRefCount(lua_State* state);
int lua_GamepadButton_getSkinColor(lua_State* state);
int lua_GamepadButton_getSkinRegion(lua_State* state);
int lua_GamepadButton_getState(lua_State* state);
int lua_GamepadButton_getStyle(lua_State* state);
int lua_GamepadButton_getText(lua_State* state);
int lua_GamepadButton_getTextAlignment(lua_State* state);
int lua_GamepadButton_getTextColor(lua_State* state);
int lua_GamepadButton_getTextRightToLeft(lua_State* state);
int lua_GamepadButton_getWidth(lua_State* state);
int lua_GamepadButton_getX(lua_State* state);
int lua_GamepadButton_getY(lua_State* state);
int lua_GamepadButton_getZIndex(lua_State* state);
int lua_GamepadButton_isContainer(lua_State* state);
int lua_GamepadButton_isEnabled(lua_State* state);
int lua_GamepadButton_isVisible(lua_State* state);
int lua_GamepadButton_release(lua_State* state);
int lua_GamepadButton_removeListener(lua_State* state);
int lua_GamepadButton_removeScriptCallback(lua_State* state);
int lua_GamepadButton_setAlignment(lua_State* state);
int lua_GamepadButton_setAnimationPropertyValue(lua_State* state);
int lua_GamepadButton_setAutoHeight(lua_State* state);
int lua_GamepadButton_setAutoWidth(lua_State* state);
int lua_GamepadButton_setBorder(lua_State* state);
int lua_GamepadButton_setBounds(lua_State* state);
int lua_GamepadButton_setConsumeInputEvents(lua_State* state);
int lua_GamepadButton_setCursorColor(lua_State* state);
int lua_GamepadButton_setCursorRegion(lua_State* state);
int lua_GamepadButton_setEnabled(lua_State* state);
int lua_GamepadButton_setFocusIndex(lua_State* state);
int lua_GamepadButton_setFont(lua_State* state);
int lua_GamepadButton_setFontSize(lua_State* state);
int lua_GamepadButton_setHeight(lua_State* state);
int lua_GamepadButton_setImageColor(lua_State* state);
int lua_GamepadButton_setImageRegion(lua_State* state);
int lua_GamepadButton_setMargin(lua_State* state);
int lua_GamepadButton_setOpacity(lua_State* state);
int lua_GamepadButton_setPadding(lua_State* state);
int lua_GamepadButton_setPosition(lua_State* state);
int lua_GamepadButton_setSize(lua_State* state);
int lua_GamepadButton_setSkinColor(lua_State* state);
int lua_GamepadButton_setSkinRegion(lua_State* state);
int lua_GamepadButton_setState(lua_State* state);
int lua_GamepadButton_setStyle(lua_State* state);
int lua_GamepadButton_setText(lua_State* state);
int lua_GamepadButton_setTextAlignment(lua_State* state);
int lua_GamepadButton_setTextColor(lua_State* state);
int lua_GamepadButton_setTextRightToLeft(lua_State* state);
int lua_GamepadButton_setVisible(lua_State* state);
int lua_GamepadButton_setWidth(lua_State* state);
int lua_GamepadButton_setZIndex(lua_State* state);
int lua_GamepadButton_static_ANIMATE_OPACITY(lua_State* state);
int lua_GamepadButton_static_ANIMATE_POSITION(lua_State* state);
int lua_GamepadButton_static_ANIMATE_POSITION_X(lua_State* state);
int lua_GamepadButton_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_GamepadButton_static_ANIMATE_SIZE(lua_State* state);
int lua_GamepadButton_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_GamepadButton_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_GamepadButton_static_create(lua_State* state);

void luaRegister_GamepadButton();

}

#endif
