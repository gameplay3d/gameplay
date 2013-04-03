#ifndef LUA_LIGHT_H_
#define LUA_LIGHT_H_

namespace gameplay
{

// Lua bindings for Light.
int lua_Light__gc(lua_State* state);
int lua_Light_addRef(lua_State* state);
int lua_Light_getColor(lua_State* state);
int lua_Light_getInnerAngle(lua_State* state);
int lua_Light_getInnerAngleCos(lua_State* state);
int lua_Light_getLightType(lua_State* state);
int lua_Light_getNode(lua_State* state);
int lua_Light_getOuterAngle(lua_State* state);
int lua_Light_getOuterAngleCos(lua_State* state);
int lua_Light_getRange(lua_State* state);
int lua_Light_getRangeInverse(lua_State* state);
int lua_Light_getRefCount(lua_State* state);
int lua_Light_release(lua_State* state);
int lua_Light_setColor(lua_State* state);
int lua_Light_setInnerAngle(lua_State* state);
int lua_Light_setOuterAngle(lua_State* state);
int lua_Light_setRange(lua_State* state);
int lua_Light_static_create(lua_State* state);
int lua_Light_static_createDirectional(lua_State* state);
int lua_Light_static_createPoint(lua_State* state);
int lua_Light_static_createSpot(lua_State* state);

void luaRegister_Light();

}

#endif
