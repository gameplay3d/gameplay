#ifndef LUA_TEXTBOX_H_
#define LUA_TEXTBOX_H_

namespace gameplay
{

// Lua bindings for TextBox.
int lua_TextBox__gc(lua_State* state);
int lua_TextBox_addListener(lua_State* state);
int lua_TextBox_addRef(lua_State* state);
int lua_TextBox_addScriptCallback(lua_State* state);
int lua_TextBox_createAnimation(lua_State* state);
int lua_TextBox_createAnimationFromBy(lua_State* state);
int lua_TextBox_createAnimationFromTo(lua_State* state);
int lua_TextBox_destroyAnimation(lua_State* state);
int lua_TextBox_getAbsoluteBounds(lua_State* state);
int lua_TextBox_getAlignment(lua_State* state);
int lua_TextBox_getAnimation(lua_State* state);
int lua_TextBox_getAnimationPropertyComponentCount(lua_State* state);
int lua_TextBox_getAnimationPropertyValue(lua_State* state);
int lua_TextBox_getAutoHeight(lua_State* state);
int lua_TextBox_getAutoWidth(lua_State* state);
int lua_TextBox_getBorder(lua_State* state);
int lua_TextBox_getBounds(lua_State* state);
int lua_TextBox_getClip(lua_State* state);
int lua_TextBox_getClipBounds(lua_State* state);
int lua_TextBox_getConsumeInputEvents(lua_State* state);
int lua_TextBox_getCursorColor(lua_State* state);
int lua_TextBox_getCursorRegion(lua_State* state);
int lua_TextBox_getCursorUVs(lua_State* state);
int lua_TextBox_getFocusIndex(lua_State* state);
int lua_TextBox_getFont(lua_State* state);
int lua_TextBox_getFontSize(lua_State* state);
int lua_TextBox_getHeight(lua_State* state);
int lua_TextBox_getId(lua_State* state);
int lua_TextBox_getImageColor(lua_State* state);
int lua_TextBox_getImageRegion(lua_State* state);
int lua_TextBox_getImageUVs(lua_State* state);
int lua_TextBox_getLastKeypress(lua_State* state);
int lua_TextBox_getMargin(lua_State* state);
int lua_TextBox_getOpacity(lua_State* state);
int lua_TextBox_getPadding(lua_State* state);
int lua_TextBox_getRefCount(lua_State* state);
int lua_TextBox_getSkinColor(lua_State* state);
int lua_TextBox_getSkinRegion(lua_State* state);
int lua_TextBox_getState(lua_State* state);
int lua_TextBox_getStyle(lua_State* state);
int lua_TextBox_getText(lua_State* state);
int lua_TextBox_getTextAlignment(lua_State* state);
int lua_TextBox_getTextColor(lua_State* state);
int lua_TextBox_getTextRightToLeft(lua_State* state);
int lua_TextBox_getType(lua_State* state);
int lua_TextBox_getWidth(lua_State* state);
int lua_TextBox_getX(lua_State* state);
int lua_TextBox_getY(lua_State* state);
int lua_TextBox_getZIndex(lua_State* state);
int lua_TextBox_isContainer(lua_State* state);
int lua_TextBox_isEnabled(lua_State* state);
int lua_TextBox_isVisible(lua_State* state);
int lua_TextBox_release(lua_State* state);
int lua_TextBox_removeListener(lua_State* state);
int lua_TextBox_removeScriptCallback(lua_State* state);
int lua_TextBox_setAlignment(lua_State* state);
int lua_TextBox_setAnimationPropertyValue(lua_State* state);
int lua_TextBox_setAutoHeight(lua_State* state);
int lua_TextBox_setAutoWidth(lua_State* state);
int lua_TextBox_setBorder(lua_State* state);
int lua_TextBox_setBounds(lua_State* state);
int lua_TextBox_setConsumeInputEvents(lua_State* state);
int lua_TextBox_setCursorColor(lua_State* state);
int lua_TextBox_setCursorRegion(lua_State* state);
int lua_TextBox_setEnabled(lua_State* state);
int lua_TextBox_setFocusIndex(lua_State* state);
int lua_TextBox_setFont(lua_State* state);
int lua_TextBox_setFontSize(lua_State* state);
int lua_TextBox_setHeight(lua_State* state);
int lua_TextBox_setImageColor(lua_State* state);
int lua_TextBox_setImageRegion(lua_State* state);
int lua_TextBox_setMargin(lua_State* state);
int lua_TextBox_setOpacity(lua_State* state);
int lua_TextBox_setPadding(lua_State* state);
int lua_TextBox_setPosition(lua_State* state);
int lua_TextBox_setSize(lua_State* state);
int lua_TextBox_setSkinColor(lua_State* state);
int lua_TextBox_setSkinRegion(lua_State* state);
int lua_TextBox_setState(lua_State* state);
int lua_TextBox_setStyle(lua_State* state);
int lua_TextBox_setText(lua_State* state);
int lua_TextBox_setTextAlignment(lua_State* state);
int lua_TextBox_setTextColor(lua_State* state);
int lua_TextBox_setTextRightToLeft(lua_State* state);
int lua_TextBox_setVisible(lua_State* state);
int lua_TextBox_setWidth(lua_State* state);
int lua_TextBox_setZIndex(lua_State* state);
int lua_TextBox_static_ANIMATE_OPACITY(lua_State* state);
int lua_TextBox_static_ANIMATE_POSITION(lua_State* state);
int lua_TextBox_static_ANIMATE_POSITION_X(lua_State* state);
int lua_TextBox_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_TextBox_static_ANIMATE_SIZE(lua_State* state);
int lua_TextBox_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_TextBox_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_TextBox_static_create(lua_State* state);

void luaRegister_TextBox();

}

#endif
