#ifndef LUA_MATRIX_H_
#define LUA_MATRIX_H_

namespace gameplay
{

// Lua bindings for Matrix.
int lua_Matrix__gc(lua_State* state);
int lua_Matrix__init(lua_State* state);
int lua_Matrix_add(lua_State* state);
int lua_Matrix_decompose(lua_State* state);
int lua_Matrix_determinant(lua_State* state);
int lua_Matrix_getBackVector(lua_State* state);
int lua_Matrix_getDownVector(lua_State* state);
int lua_Matrix_getForwardVector(lua_State* state);
int lua_Matrix_getLeftVector(lua_State* state);
int lua_Matrix_getRightVector(lua_State* state);
int lua_Matrix_getRotation(lua_State* state);
int lua_Matrix_getScale(lua_State* state);
int lua_Matrix_getTranslation(lua_State* state);
int lua_Matrix_getUpVector(lua_State* state);
int lua_Matrix_invert(lua_State* state);
int lua_Matrix_isIdentity(lua_State* state);
int lua_Matrix_m(lua_State* state);
int lua_Matrix_multiply(lua_State* state);
int lua_Matrix_negate(lua_State* state);
int lua_Matrix_rotate(lua_State* state);
int lua_Matrix_rotateX(lua_State* state);
int lua_Matrix_rotateY(lua_State* state);
int lua_Matrix_rotateZ(lua_State* state);
int lua_Matrix_scale(lua_State* state);
int lua_Matrix_set(lua_State* state);
int lua_Matrix_setIdentity(lua_State* state);
int lua_Matrix_setZero(lua_State* state);
int lua_Matrix_static_add(lua_State* state);
int lua_Matrix_static_createBillboard(lua_State* state);
int lua_Matrix_static_createLookAt(lua_State* state);
int lua_Matrix_static_createOrthographic(lua_State* state);
int lua_Matrix_static_createOrthographicOffCenter(lua_State* state);
int lua_Matrix_static_createPerspective(lua_State* state);
int lua_Matrix_static_createReflection(lua_State* state);
int lua_Matrix_static_createRotation(lua_State* state);
int lua_Matrix_static_createRotationX(lua_State* state);
int lua_Matrix_static_createRotationY(lua_State* state);
int lua_Matrix_static_createRotationZ(lua_State* state);
int lua_Matrix_static_createScale(lua_State* state);
int lua_Matrix_static_createTranslation(lua_State* state);
int lua_Matrix_static_identity(lua_State* state);
int lua_Matrix_static_multiply(lua_State* state);
int lua_Matrix_static_subtract(lua_State* state);
int lua_Matrix_static_zero(lua_State* state);
int lua_Matrix_subtract(lua_State* state);
int lua_Matrix_transformPoint(lua_State* state);
int lua_Matrix_transformVector(lua_State* state);
int lua_Matrix_translate(lua_State* state);
int lua_Matrix_transpose(lua_State* state);

void luaRegister_Matrix();

}

#endif
