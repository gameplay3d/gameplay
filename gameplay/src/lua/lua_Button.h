#ifndef LUA_BUTTON_H_
#define LUA_BUTTON_H_

namespace gameplay
{

// Lua bindings for Button.
int lua_Button__gc(lua_State* state);
int lua_Button_addListener(lua_State* state);
int lua_Button_addRef(lua_State* state);
int lua_Button_addScriptCallback(lua_State* state);
int lua_Button_createAnimation(lua_State* state);
int lua_Button_createAnimationFromBy(lua_State* state);
int lua_Button_createAnimationFromTo(lua_State* state);
int lua_Button_destroyAnimation(lua_State* state);
int lua_Button_getAbsoluteBounds(lua_State* state);
int lua_Button_getAlignment(lua_State* state);
int lua_Button_getAnimation(lua_State* state);
int lua_Button_getAnimationPropertyComponentCount(lua_State* state);
int lua_Button_getAnimationPropertyValue(lua_State* state);
int lua_Button_getAutoHeight(lua_State* state);
int lua_Button_getAutoWidth(lua_State* state);
int lua_Button_getBorder(lua_State* state);
int lua_Button_getBounds(lua_State* state);
int lua_Button_getClip(lua_State* state);
int lua_Button_getClipBounds(lua_State* state);
int lua_Button_getConsumeInputEvents(lua_State* state);
int lua_Button_getCursorColor(lua_State* state);
int lua_Button_getCursorRegion(lua_State* state);
int lua_Button_getCursorUVs(lua_State* state);
int lua_Button_getFocusIndex(lua_State* state);
int lua_Button_getFont(lua_State* state);
int lua_Button_getFontSize(lua_State* state);
int lua_Button_getHeight(lua_State* state);
int lua_Button_getId(lua_State* state);
int lua_Button_getImageColor(lua_State* state);
int lua_Button_getImageRegion(lua_State* state);
int lua_Button_getImageUVs(lua_State* state);
int lua_Button_getMargin(lua_State* state);
int lua_Button_getOpacity(lua_State* state);
int lua_Button_getPadding(lua_State* state);
int lua_Button_getRefCount(lua_State* state);
int lua_Button_getSkinColor(lua_State* state);
int lua_Button_getSkinRegion(lua_State* state);
int lua_Button_getState(lua_State* state);
int lua_Button_getStyle(lua_State* state);
int lua_Button_getText(lua_State* state);
int lua_Button_getTextAlignment(lua_State* state);
int lua_Button_getTextColor(lua_State* state);
int lua_Button_getTextRightToLeft(lua_State* state);
int lua_Button_getWidth(lua_State* state);
int lua_Button_getX(lua_State* state);
int lua_Button_getY(lua_State* state);
int lua_Button_getZIndex(lua_State* state);
int lua_Button_isContainer(lua_State* state);
int lua_Button_isEnabled(lua_State* state);
int lua_Button_isVisible(lua_State* state);
int lua_Button_release(lua_State* state);
int lua_Button_removeListener(lua_State* state);
int lua_Button_removeScriptCallback(lua_State* state);
int lua_Button_setAlignment(lua_State* state);
int lua_Button_setAnimationPropertyValue(lua_State* state);
int lua_Button_setAutoHeight(lua_State* state);
int lua_Button_setAutoWidth(lua_State* state);
int lua_Button_setBorder(lua_State* state);
int lua_Button_setBounds(lua_State* state);
int lua_Button_setConsumeInputEvents(lua_State* state);
int lua_Button_setCursorColor(lua_State* state);
int lua_Button_setCursorRegion(lua_State* state);
int lua_Button_setEnabled(lua_State* state);
int lua_Button_setFocusIndex(lua_State* state);
int lua_Button_setFont(lua_State* state);
int lua_Button_setFontSize(lua_State* state);
int lua_Button_setHeight(lua_State* state);
int lua_Button_setImageColor(lua_State* state);
int lua_Button_setImageRegion(lua_State* state);
int lua_Button_setMargin(lua_State* state);
int lua_Button_setOpacity(lua_State* state);
int lua_Button_setPadding(lua_State* state);
int lua_Button_setPosition(lua_State* state);
int lua_Button_setSize(lua_State* state);
int lua_Button_setSkinColor(lua_State* state);
int lua_Button_setSkinRegion(lua_State* state);
int lua_Button_setState(lua_State* state);
int lua_Button_setStyle(lua_State* state);
int lua_Button_setText(lua_State* state);
int lua_Button_setTextAlignment(lua_State* state);
int lua_Button_setTextColor(lua_State* state);
int lua_Button_setTextRightToLeft(lua_State* state);
int lua_Button_setVisible(lua_State* state);
int lua_Button_setWidth(lua_State* state);
int lua_Button_setZIndex(lua_State* state);
int lua_Button_static_ANIMATE_OPACITY(lua_State* state);
int lua_Button_static_ANIMATE_POSITION(lua_State* state);
int lua_Button_static_ANIMATE_POSITION_X(lua_State* state);
int lua_Button_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_Button_static_ANIMATE_SIZE(lua_State* state);
int lua_Button_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_Button_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_Button_static_create(lua_State* state);

void luaRegister_Button();

}

#endif
