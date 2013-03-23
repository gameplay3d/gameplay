#ifndef LUA_TEXTURE_H_
#define LUA_TEXTURE_H_

namespace gameplay
{

// Lua bindings for Texture.
int lua_Texture__gc(lua_State* state);
int lua_Texture_addRef(lua_State* state);
int lua_Texture_generateMipmaps(lua_State* state);
int lua_Texture_getFormat(lua_State* state);
int lua_Texture_getHandle(lua_State* state);
int lua_Texture_getHeight(lua_State* state);
int lua_Texture_getPath(lua_State* state);
int lua_Texture_getRefCount(lua_State* state);
int lua_Texture_getWidth(lua_State* state);
int lua_Texture_isCompressed(lua_State* state);
int lua_Texture_isMipmapped(lua_State* state);
int lua_Texture_release(lua_State* state);
int lua_Texture_static_create(lua_State* state);

void luaRegister_Texture();

}

#endif
