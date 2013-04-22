#ifndef LUA_MATERIALPARAMETER_H_
#define LUA_MATERIALPARAMETER_H_

namespace gameplay
{

// Lua bindings for MaterialParameter.
int lua_MaterialParameter__gc(lua_State* state);
int lua_MaterialParameter_addRef(lua_State* state);
int lua_MaterialParameter_bindValue(lua_State* state);
int lua_MaterialParameter_createAnimation(lua_State* state);
int lua_MaterialParameter_createAnimationFromBy(lua_State* state);
int lua_MaterialParameter_createAnimationFromTo(lua_State* state);
int lua_MaterialParameter_destroyAnimation(lua_State* state);
int lua_MaterialParameter_getAnimation(lua_State* state);
int lua_MaterialParameter_getAnimationPropertyComponentCount(lua_State* state);
int lua_MaterialParameter_getAnimationPropertyValue(lua_State* state);
int lua_MaterialParameter_getName(lua_State* state);
int lua_MaterialParameter_getRefCount(lua_State* state);
int lua_MaterialParameter_getSampler(lua_State* state);
int lua_MaterialParameter_release(lua_State* state);
int lua_MaterialParameter_setAnimationPropertyValue(lua_State* state);
int lua_MaterialParameter_setFloat(lua_State* state);
int lua_MaterialParameter_setFloatArray(lua_State* state);
int lua_MaterialParameter_setInt(lua_State* state);
int lua_MaterialParameter_setIntArray(lua_State* state);
int lua_MaterialParameter_setMatrix(lua_State* state);
int lua_MaterialParameter_setMatrixArray(lua_State* state);
int lua_MaterialParameter_setSampler(lua_State* state);
int lua_MaterialParameter_setValue(lua_State* state);
int lua_MaterialParameter_setVector2(lua_State* state);
int lua_MaterialParameter_setVector2Array(lua_State* state);
int lua_MaterialParameter_setVector3(lua_State* state);
int lua_MaterialParameter_setVector3Array(lua_State* state);
int lua_MaterialParameter_setVector4(lua_State* state);
int lua_MaterialParameter_setVector4Array(lua_State* state);
int lua_MaterialParameter_static_ANIMATE_UNIFORM(lua_State* state);

void luaRegister_MaterialParameter();

}

#endif
