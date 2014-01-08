#ifndef LUA_ANIMATIONVALUE_H_
#define LUA_ANIMATIONVALUE_H_

namespace gameplay
{

// Lua bindings for AnimationValue.
int lua_AnimationValue_getFloat(lua_State* state);
int lua_AnimationValue_getFloats(lua_State* state);
int lua_AnimationValue_setFloat(lua_State* state);
int lua_AnimationValue_setFloats(lua_State* state);

void luaRegister_AnimationValue();

}

#endif
