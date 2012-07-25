#ifndef LUA_SCENEDEBUGFLAGS_H_
#define LUA_SCENEDEBUGFLAGS_H_

#include "Scene.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for Scene::DebugFlags.
Scene::DebugFlags lua_enumFromString_SceneDebugFlags(const char* s);
const char* lua_stringFromEnum_SceneDebugFlags(Scene::DebugFlags e);

}

#endif
