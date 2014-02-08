#ifndef LUA_EFFECT_H_
#define LUA_EFFECT_H_

namespace gameplay
{

// Lua bindings for Effect.
int lua_Effect__gc(lua_State* state);
int lua_Effect_addRef(lua_State* state);
int lua_Effect_bind(lua_State* state);
int lua_Effect_getId(lua_State* state);
int lua_Effect_getRefCount(lua_State* state);
int lua_Effect_getUniform(lua_State* state);
int lua_Effect_getUniformCount(lua_State* state);
int lua_Effect_getVertexAttribute(lua_State* state);
int lua_Effect_release(lua_State* state);
int lua_Effect_setValue(lua_State* state);
int lua_Effect_static_createFromFile(lua_State* state);
int lua_Effect_static_createFromSource(lua_State* state);
int lua_Effect_static_getCurrentEffect(lua_State* state);

void luaRegister_Effect();

}

#endif
