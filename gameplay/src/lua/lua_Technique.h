#ifndef LUA_TECHNIQUE_H_
#define LUA_TECHNIQUE_H_

namespace gameplay
{

// Lua bindings for Technique.
int lua_Technique__gc(lua_State* state);
int lua_Technique_addRef(lua_State* state);
int lua_Technique_clearParameter(lua_State* state);
int lua_Technique_getId(lua_State* state);
int lua_Technique_getParameter(lua_State* state);
int lua_Technique_getPass(lua_State* state);
int lua_Technique_getPassByIndex(lua_State* state);
int lua_Technique_getPassCount(lua_State* state);
int lua_Technique_getRefCount(lua_State* state);
int lua_Technique_getStateBlock(lua_State* state);
int lua_Technique_release(lua_State* state);
int lua_Technique_setParameterAutoBinding(lua_State* state);
int lua_Technique_setStateBlock(lua_State* state);

void luaRegister_Technique();

}

#endif
