#ifndef LUA_FILESYSTEM_H_
#define LUA_FILESYSTEM_H_

namespace gameplay
{

// Lua bindings for FileSystem.
int lua_FileSystem__gc(lua_State* state);
int lua_FileSystem_static_createFileFromAsset(lua_State* state);
int lua_FileSystem_static_fileExists(lua_State* state);
int lua_FileSystem_static_getDirectoryName(lua_State* state);
int lua_FileSystem_static_getExtension(lua_State* state);
int lua_FileSystem_static_getResourcePath(lua_State* state);
int lua_FileSystem_static_isAbsolutePath(lua_State* state);
int lua_FileSystem_static_loadResourceAliases(lua_State* state);
int lua_FileSystem_static_readAll(lua_State* state);
int lua_FileSystem_static_resolvePath(lua_State* state);
int lua_FileSystem_static_setResourcePath(lua_State* state);

void luaRegister_FileSystem();

}

#endif
