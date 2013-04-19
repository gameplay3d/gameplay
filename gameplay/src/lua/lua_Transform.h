#ifndef LUA_TRANSFORM_H_
#define LUA_TRANSFORM_H_

namespace gameplay
{

// Lua bindings for Transform.
int lua_Transform__gc(lua_State* state);
int lua_Transform__init(lua_State* state);
int lua_Transform_addListener(lua_State* state);
int lua_Transform_addScriptCallback(lua_State* state);
int lua_Transform_createAnimation(lua_State* state);
int lua_Transform_createAnimationFromBy(lua_State* state);
int lua_Transform_createAnimationFromTo(lua_State* state);
int lua_Transform_destroyAnimation(lua_State* state);
int lua_Transform_getAnimation(lua_State* state);
int lua_Transform_getAnimationPropertyComponentCount(lua_State* state);
int lua_Transform_getAnimationPropertyValue(lua_State* state);
int lua_Transform_getBackVector(lua_State* state);
int lua_Transform_getDownVector(lua_State* state);
int lua_Transform_getForwardVector(lua_State* state);
int lua_Transform_getLeftVector(lua_State* state);
int lua_Transform_getMatrix(lua_State* state);
int lua_Transform_getRightVector(lua_State* state);
int lua_Transform_getRotation(lua_State* state);
int lua_Transform_getScale(lua_State* state);
int lua_Transform_getScaleX(lua_State* state);
int lua_Transform_getScaleY(lua_State* state);
int lua_Transform_getScaleZ(lua_State* state);
int lua_Transform_getTranslation(lua_State* state);
int lua_Transform_getTranslationX(lua_State* state);
int lua_Transform_getTranslationY(lua_State* state);
int lua_Transform_getTranslationZ(lua_State* state);
int lua_Transform_getUpVector(lua_State* state);
int lua_Transform_isStatic(lua_State* state);
int lua_Transform_removeListener(lua_State* state);
int lua_Transform_removeScriptCallback(lua_State* state);
int lua_Transform_rotate(lua_State* state);
int lua_Transform_rotateX(lua_State* state);
int lua_Transform_rotateY(lua_State* state);
int lua_Transform_rotateZ(lua_State* state);
int lua_Transform_scale(lua_State* state);
int lua_Transform_scaleX(lua_State* state);
int lua_Transform_scaleY(lua_State* state);
int lua_Transform_scaleZ(lua_State* state);
int lua_Transform_set(lua_State* state);
int lua_Transform_setAnimationPropertyValue(lua_State* state);
int lua_Transform_setIdentity(lua_State* state);
int lua_Transform_setRotation(lua_State* state);
int lua_Transform_setScale(lua_State* state);
int lua_Transform_setScaleX(lua_State* state);
int lua_Transform_setScaleY(lua_State* state);
int lua_Transform_setScaleZ(lua_State* state);
int lua_Transform_setTranslation(lua_State* state);
int lua_Transform_setTranslationX(lua_State* state);
int lua_Transform_setTranslationY(lua_State* state);
int lua_Transform_setTranslationZ(lua_State* state);
int lua_Transform_static_ANIMATE_ROTATE(lua_State* state);
int lua_Transform_static_ANIMATE_ROTATE_TRANSLATE(lua_State* state);
int lua_Transform_static_ANIMATE_SCALE(lua_State* state);
int lua_Transform_static_ANIMATE_SCALE_ROTATE(lua_State* state);
int lua_Transform_static_ANIMATE_SCALE_ROTATE_TRANSLATE(lua_State* state);
int lua_Transform_static_ANIMATE_SCALE_TRANSLATE(lua_State* state);
int lua_Transform_static_ANIMATE_SCALE_UNIT(lua_State* state);
int lua_Transform_static_ANIMATE_SCALE_X(lua_State* state);
int lua_Transform_static_ANIMATE_SCALE_Y(lua_State* state);
int lua_Transform_static_ANIMATE_SCALE_Z(lua_State* state);
int lua_Transform_static_ANIMATE_TRANSLATE(lua_State* state);
int lua_Transform_static_ANIMATE_TRANSLATE_X(lua_State* state);
int lua_Transform_static_ANIMATE_TRANSLATE_Y(lua_State* state);
int lua_Transform_static_ANIMATE_TRANSLATE_Z(lua_State* state);
int lua_Transform_static_isTransformChangedSuspended(lua_State* state);
int lua_Transform_static_resumeTransformChanged(lua_State* state);
int lua_Transform_static_suspendTransformChanged(lua_State* state);
int lua_Transform_transformPoint(lua_State* state);
int lua_Transform_transformVector(lua_State* state);
int lua_Transform_translate(lua_State* state);
int lua_Transform_translateForward(lua_State* state);
int lua_Transform_translateLeft(lua_State* state);
int lua_Transform_translateSmooth(lua_State* state);
int lua_Transform_translateUp(lua_State* state);
int lua_Transform_translateX(lua_State* state);
int lua_Transform_translateY(lua_State* state);
int lua_Transform_translateZ(lua_State* state);

void luaRegister_Transform();

}

#endif
