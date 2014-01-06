#ifndef LUA_IMAGE_H_
#define LUA_IMAGE_H_

namespace gameplay
{

// Lua bindings for Image.
int lua_Image__gc(lua_State* state);
int lua_Image_addRef(lua_State* state);
int lua_Image_getFormat(lua_State* state);
int lua_Image_getHeight(lua_State* state);
int lua_Image_getRefCount(lua_State* state);
int lua_Image_getWidth(lua_State* state);
int lua_Image_release(lua_State* state);
int lua_Image_static_create(lua_State* state);

void luaRegister_Image();

}

#endif
