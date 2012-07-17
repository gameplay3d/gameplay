#ifndef LUA_SCRIPTTARGET_H_
#define LUA_SCRIPTTARGET_H_

namespace gameplay
{

// Lua bindings for ScriptTarget.
int lua_ScriptTarget__gc(lua_State* state);
int lua_ScriptTarget_addCallback(lua_State* state);
int lua_ScriptTarget_removeCallback(lua_State* state);

void luaRegister_ScriptTarget();

}

#endif
