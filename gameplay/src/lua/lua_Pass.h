#ifndef LUA_PASS_H_
#define LUA_PASS_H_

namespace gameplay
{

// Lua bindings for Pass.
int lua_Pass__gc(lua_State* state);
int lua_Pass_addRef(lua_State* state);
int lua_Pass_bind(lua_State* state);
int lua_Pass_clearParameter(lua_State* state);
int lua_Pass_getEffect(lua_State* state);
int lua_Pass_getId(lua_State* state);
int lua_Pass_getParameter(lua_State* state);
int lua_Pass_getRefCount(lua_State* state);
int lua_Pass_getStateBlock(lua_State* state);
int lua_Pass_getVertexAttributeBinding(lua_State* state);
int lua_Pass_release(lua_State* state);
int lua_Pass_setParameterAutoBinding(lua_State* state);
int lua_Pass_setStateBlock(lua_State* state);
int lua_Pass_setVertexAttributeBinding(lua_State* state);
int lua_Pass_unbind(lua_State* state);

void luaRegister_Pass();

}

#endif
