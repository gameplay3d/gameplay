#ifndef LUA_LABEL_H_
#define LUA_LABEL_H_

namespace gameplay
{

// Lua bindings for Label.
int lua_Label__gc(lua_State* state);
int lua_Label_addListener(lua_State* state);
int lua_Label_addRef(lua_State* state);
int lua_Label_addScriptCallback(lua_State* state);
int lua_Label_createAnimation(lua_State* state);
int lua_Label_createAnimationFromBy(lua_State* state);
int lua_Label_createAnimationFromTo(lua_State* state);
int lua_Label_destroyAnimation(lua_State* state);
int lua_Label_getAbsoluteBounds(lua_State* state);
int lua_Label_getAlignment(lua_State* state);
int lua_Label_getAnimation(lua_State* state);
int lua_Label_getAnimationPropertyComponentCount(lua_State* state);
int lua_Label_getAnimationPropertyValue(lua_State* state);
int lua_Label_getAutoHeight(lua_State* state);
int lua_Label_getAutoWidth(lua_State* state);
int lua_Label_getBorder(lua_State* state);
int lua_Label_getBounds(lua_State* state);
int lua_Label_getClip(lua_State* state);
int lua_Label_getClipBounds(lua_State* state);
int lua_Label_getConsumeInputEvents(lua_State* state);
int lua_Label_getCursorColor(lua_State* state);
int lua_Label_getCursorRegion(lua_State* state);
int lua_Label_getCursorUVs(lua_State* state);
int lua_Label_getFocusIndex(lua_State* state);
int lua_Label_getFont(lua_State* state);
int lua_Label_getFontSize(lua_State* state);
int lua_Label_getHeight(lua_State* state);
int lua_Label_getId(lua_State* state);
int lua_Label_getImageColor(lua_State* state);
int lua_Label_getImageRegion(lua_State* state);
int lua_Label_getImageUVs(lua_State* state);
int lua_Label_getMargin(lua_State* state);
int lua_Label_getOpacity(lua_State* state);
int lua_Label_getPadding(lua_State* state);
int lua_Label_getRefCount(lua_State* state);
int lua_Label_getSkinColor(lua_State* state);
int lua_Label_getSkinRegion(lua_State* state);
int lua_Label_getState(lua_State* state);
int lua_Label_getStyle(lua_State* state);
int lua_Label_getText(lua_State* state);
int lua_Label_getTextAlignment(lua_State* state);
int lua_Label_getTextColor(lua_State* state);
int lua_Label_getTextRightToLeft(lua_State* state);
int lua_Label_getType(lua_State* state);
int lua_Label_getWidth(lua_State* state);
int lua_Label_getX(lua_State* state);
int lua_Label_getY(lua_State* state);
int lua_Label_getZIndex(lua_State* state);
int lua_Label_isContainer(lua_State* state);
int lua_Label_isEnabled(lua_State* state);
int lua_Label_isVisible(lua_State* state);
int lua_Label_release(lua_State* state);
int lua_Label_removeListener(lua_State* state);
int lua_Label_removeScriptCallback(lua_State* state);
int lua_Label_setAlignment(lua_State* state);
int lua_Label_setAnimationPropertyValue(lua_State* state);
int lua_Label_setAutoHeight(lua_State* state);
int lua_Label_setAutoWidth(lua_State* state);
int lua_Label_setBorder(lua_State* state);
int lua_Label_setBounds(lua_State* state);
int lua_Label_setConsumeInputEvents(lua_State* state);
int lua_Label_setCursorColor(lua_State* state);
int lua_Label_setCursorRegion(lua_State* state);
int lua_Label_setEnabled(lua_State* state);
int lua_Label_setFocusIndex(lua_State* state);
int lua_Label_setFont(lua_State* state);
int lua_Label_setFontSize(lua_State* state);
int lua_Label_setHeight(lua_State* state);
int lua_Label_setImageColor(lua_State* state);
int lua_Label_setImageRegion(lua_State* state);
int lua_Label_setMargin(lua_State* state);
int lua_Label_setOpacity(lua_State* state);
int lua_Label_setPadding(lua_State* state);
int lua_Label_setPosition(lua_State* state);
int lua_Label_setSize(lua_State* state);
int lua_Label_setSkinColor(lua_State* state);
int lua_Label_setSkinRegion(lua_State* state);
int lua_Label_setState(lua_State* state);
int lua_Label_setStyle(lua_State* state);
int lua_Label_setText(lua_State* state);
int lua_Label_setTextAlignment(lua_State* state);
int lua_Label_setTextColor(lua_State* state);
int lua_Label_setTextRightToLeft(lua_State* state);
int lua_Label_setVisible(lua_State* state);
int lua_Label_setWidth(lua_State* state);
int lua_Label_setZIndex(lua_State* state);
int lua_Label_static_ANIMATE_OPACITY(lua_State* state);
int lua_Label_static_ANIMATE_POSITION(lua_State* state);
int lua_Label_static_ANIMATE_POSITION_X(lua_State* state);
int lua_Label_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_Label_static_ANIMATE_SIZE(lua_State* state);
int lua_Label_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_Label_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_Label_static_create(lua_State* state);

void luaRegister_Label();

}

#endif
