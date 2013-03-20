#ifndef LUA_CONTROL_H_
#define LUA_CONTROL_H_

namespace gameplay
{

// Lua bindings for Control.
int lua_Control__gc(lua_State* state);
int lua_Control_addListener(lua_State* state);
int lua_Control_addRef(lua_State* state);
int lua_Control_addScriptCallback(lua_State* state);
int lua_Control_createAnimation(lua_State* state);
int lua_Control_createAnimationFromBy(lua_State* state);
int lua_Control_createAnimationFromTo(lua_State* state);
int lua_Control_destroyAnimation(lua_State* state);
int lua_Control_getAbsoluteBounds(lua_State* state);
int lua_Control_getAlignment(lua_State* state);
int lua_Control_getAnimation(lua_State* state);
int lua_Control_getAnimationPropertyComponentCount(lua_State* state);
int lua_Control_getAnimationPropertyValue(lua_State* state);
int lua_Control_getAutoHeight(lua_State* state);
int lua_Control_getAutoWidth(lua_State* state);
int lua_Control_getBorder(lua_State* state);
int lua_Control_getBounds(lua_State* state);
int lua_Control_getClip(lua_State* state);
int lua_Control_getClipBounds(lua_State* state);
int lua_Control_getConsumeInputEvents(lua_State* state);
int lua_Control_getCursorColor(lua_State* state);
int lua_Control_getCursorRegion(lua_State* state);
int lua_Control_getCursorUVs(lua_State* state);
int lua_Control_getFocusIndex(lua_State* state);
int lua_Control_getFont(lua_State* state);
int lua_Control_getFontSize(lua_State* state);
int lua_Control_getHeight(lua_State* state);
int lua_Control_getId(lua_State* state);
int lua_Control_getImageColor(lua_State* state);
int lua_Control_getImageRegion(lua_State* state);
int lua_Control_getImageUVs(lua_State* state);
int lua_Control_getMargin(lua_State* state);
int lua_Control_getOpacity(lua_State* state);
int lua_Control_getPadding(lua_State* state);
int lua_Control_getRefCount(lua_State* state);
int lua_Control_getSkinColor(lua_State* state);
int lua_Control_getSkinRegion(lua_State* state);
int lua_Control_getState(lua_State* state);
int lua_Control_getStyle(lua_State* state);
int lua_Control_getTextAlignment(lua_State* state);
int lua_Control_getTextColor(lua_State* state);
int lua_Control_getTextRightToLeft(lua_State* state);
int lua_Control_getType(lua_State* state);
int lua_Control_getWidth(lua_State* state);
int lua_Control_getX(lua_State* state);
int lua_Control_getY(lua_State* state);
int lua_Control_getZIndex(lua_State* state);
int lua_Control_isContainer(lua_State* state);
int lua_Control_isEnabled(lua_State* state);
int lua_Control_isVisible(lua_State* state);
int lua_Control_release(lua_State* state);
int lua_Control_removeListener(lua_State* state);
int lua_Control_removeScriptCallback(lua_State* state);
int lua_Control_setAlignment(lua_State* state);
int lua_Control_setAnimationPropertyValue(lua_State* state);
int lua_Control_setAutoHeight(lua_State* state);
int lua_Control_setAutoWidth(lua_State* state);
int lua_Control_setBorder(lua_State* state);
int lua_Control_setBounds(lua_State* state);
int lua_Control_setConsumeInputEvents(lua_State* state);
int lua_Control_setCursorColor(lua_State* state);
int lua_Control_setCursorRegion(lua_State* state);
int lua_Control_setEnabled(lua_State* state);
int lua_Control_setFocusIndex(lua_State* state);
int lua_Control_setFont(lua_State* state);
int lua_Control_setFontSize(lua_State* state);
int lua_Control_setHeight(lua_State* state);
int lua_Control_setImageColor(lua_State* state);
int lua_Control_setImageRegion(lua_State* state);
int lua_Control_setMargin(lua_State* state);
int lua_Control_setOpacity(lua_State* state);
int lua_Control_setPadding(lua_State* state);
int lua_Control_setPosition(lua_State* state);
int lua_Control_setSize(lua_State* state);
int lua_Control_setSkinColor(lua_State* state);
int lua_Control_setSkinRegion(lua_State* state);
int lua_Control_setState(lua_State* state);
int lua_Control_setStyle(lua_State* state);
int lua_Control_setTextAlignment(lua_State* state);
int lua_Control_setTextColor(lua_State* state);
int lua_Control_setTextRightToLeft(lua_State* state);
int lua_Control_setVisible(lua_State* state);
int lua_Control_setWidth(lua_State* state);
int lua_Control_setZIndex(lua_State* state);
int lua_Control_static_ANIMATE_OPACITY(lua_State* state);
int lua_Control_static_ANIMATE_POSITION(lua_State* state);
int lua_Control_static_ANIMATE_POSITION_X(lua_State* state);
int lua_Control_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_Control_static_ANIMATE_SIZE(lua_State* state);
int lua_Control_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_Control_static_ANIMATE_SIZE_WIDTH(lua_State* state);

void luaRegister_Control();

}

#endif
