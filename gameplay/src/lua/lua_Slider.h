#ifndef LUA_SLIDER_H_
#define LUA_SLIDER_H_

namespace gameplay
{

// Lua bindings for Slider.
int lua_Slider__gc(lua_State* state);
int lua_Slider_addListener(lua_State* state);
int lua_Slider_addRef(lua_State* state);
int lua_Slider_addScriptCallback(lua_State* state);
int lua_Slider_createAnimation(lua_State* state);
int lua_Slider_createAnimationFromBy(lua_State* state);
int lua_Slider_createAnimationFromTo(lua_State* state);
int lua_Slider_destroyAnimation(lua_State* state);
int lua_Slider_getAbsoluteBounds(lua_State* state);
int lua_Slider_getAlignment(lua_State* state);
int lua_Slider_getAnimation(lua_State* state);
int lua_Slider_getAnimationPropertyComponentCount(lua_State* state);
int lua_Slider_getAnimationPropertyValue(lua_State* state);
int lua_Slider_getAutoHeight(lua_State* state);
int lua_Slider_getAutoWidth(lua_State* state);
int lua_Slider_getBorder(lua_State* state);
int lua_Slider_getBounds(lua_State* state);
int lua_Slider_getClip(lua_State* state);
int lua_Slider_getClipBounds(lua_State* state);
int lua_Slider_getConsumeInputEvents(lua_State* state);
int lua_Slider_getCursorColor(lua_State* state);
int lua_Slider_getCursorRegion(lua_State* state);
int lua_Slider_getCursorUVs(lua_State* state);
int lua_Slider_getFocusIndex(lua_State* state);
int lua_Slider_getFont(lua_State* state);
int lua_Slider_getFontSize(lua_State* state);
int lua_Slider_getHeight(lua_State* state);
int lua_Slider_getId(lua_State* state);
int lua_Slider_getImageColor(lua_State* state);
int lua_Slider_getImageRegion(lua_State* state);
int lua_Slider_getImageUVs(lua_State* state);
int lua_Slider_getMargin(lua_State* state);
int lua_Slider_getMax(lua_State* state);
int lua_Slider_getMin(lua_State* state);
int lua_Slider_getOpacity(lua_State* state);
int lua_Slider_getPadding(lua_State* state);
int lua_Slider_getRefCount(lua_State* state);
int lua_Slider_getSkinColor(lua_State* state);
int lua_Slider_getSkinRegion(lua_State* state);
int lua_Slider_getState(lua_State* state);
int lua_Slider_getStep(lua_State* state);
int lua_Slider_getStyle(lua_State* state);
int lua_Slider_getText(lua_State* state);
int lua_Slider_getTextAlignment(lua_State* state);
int lua_Slider_getTextColor(lua_State* state);
int lua_Slider_getTextRightToLeft(lua_State* state);
int lua_Slider_getType(lua_State* state);
int lua_Slider_getValue(lua_State* state);
int lua_Slider_getValueTextAlignment(lua_State* state);
int lua_Slider_getValueTextPrecision(lua_State* state);
int lua_Slider_getWidth(lua_State* state);
int lua_Slider_getX(lua_State* state);
int lua_Slider_getY(lua_State* state);
int lua_Slider_getZIndex(lua_State* state);
int lua_Slider_isContainer(lua_State* state);
int lua_Slider_isEnabled(lua_State* state);
int lua_Slider_isValueTextVisible(lua_State* state);
int lua_Slider_isVisible(lua_State* state);
int lua_Slider_release(lua_State* state);
int lua_Slider_removeListener(lua_State* state);
int lua_Slider_removeScriptCallback(lua_State* state);
int lua_Slider_setAlignment(lua_State* state);
int lua_Slider_setAnimationPropertyValue(lua_State* state);
int lua_Slider_setAutoHeight(lua_State* state);
int lua_Slider_setAutoWidth(lua_State* state);
int lua_Slider_setBorder(lua_State* state);
int lua_Slider_setBounds(lua_State* state);
int lua_Slider_setConsumeInputEvents(lua_State* state);
int lua_Slider_setCursorColor(lua_State* state);
int lua_Slider_setCursorRegion(lua_State* state);
int lua_Slider_setEnabled(lua_State* state);
int lua_Slider_setFocusIndex(lua_State* state);
int lua_Slider_setFont(lua_State* state);
int lua_Slider_setFontSize(lua_State* state);
int lua_Slider_setHeight(lua_State* state);
int lua_Slider_setImageColor(lua_State* state);
int lua_Slider_setImageRegion(lua_State* state);
int lua_Slider_setMargin(lua_State* state);
int lua_Slider_setMax(lua_State* state);
int lua_Slider_setMin(lua_State* state);
int lua_Slider_setOpacity(lua_State* state);
int lua_Slider_setPadding(lua_State* state);
int lua_Slider_setPosition(lua_State* state);
int lua_Slider_setSize(lua_State* state);
int lua_Slider_setSkinColor(lua_State* state);
int lua_Slider_setSkinRegion(lua_State* state);
int lua_Slider_setState(lua_State* state);
int lua_Slider_setStep(lua_State* state);
int lua_Slider_setStyle(lua_State* state);
int lua_Slider_setText(lua_State* state);
int lua_Slider_setTextAlignment(lua_State* state);
int lua_Slider_setTextColor(lua_State* state);
int lua_Slider_setTextRightToLeft(lua_State* state);
int lua_Slider_setValue(lua_State* state);
int lua_Slider_setValueTextAlignment(lua_State* state);
int lua_Slider_setValueTextPrecision(lua_State* state);
int lua_Slider_setValueTextVisible(lua_State* state);
int lua_Slider_setVisible(lua_State* state);
int lua_Slider_setWidth(lua_State* state);
int lua_Slider_setZIndex(lua_State* state);
int lua_Slider_static_ANIMATE_OPACITY(lua_State* state);
int lua_Slider_static_ANIMATE_POSITION(lua_State* state);
int lua_Slider_static_ANIMATE_POSITION_X(lua_State* state);
int lua_Slider_static_ANIMATE_POSITION_Y(lua_State* state);
int lua_Slider_static_ANIMATE_SIZE(lua_State* state);
int lua_Slider_static_ANIMATE_SIZE_HEIGHT(lua_State* state);
int lua_Slider_static_ANIMATE_SIZE_WIDTH(lua_State* state);
int lua_Slider_static_create(lua_State* state);

void luaRegister_Slider();

}

#endif
