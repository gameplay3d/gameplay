#ifndef LUA_TEXTURESAMPLER_H_
#define LUA_TEXTURESAMPLER_H_

namespace gameplay
{

// Lua bindings for Texture::Sampler.
int lua_TextureSampler__gc(lua_State* state);
int lua_TextureSampler_addRef(lua_State* state);
int lua_TextureSampler_bind(lua_State* state);
int lua_TextureSampler_getRefCount(lua_State* state);
int lua_TextureSampler_getTexture(lua_State* state);
int lua_TextureSampler_release(lua_State* state);
int lua_TextureSampler_setFilterMode(lua_State* state);
int lua_TextureSampler_setWrapMode(lua_State* state);
int lua_TextureSampler_static_create(lua_State* state);

void luaRegister_TextureSampler();

}

#endif
