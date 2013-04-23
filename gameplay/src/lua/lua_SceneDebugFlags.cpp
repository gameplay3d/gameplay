#include "Base.h"
#include "lua_SceneDebugFlags.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_SceneDebugFlags_DEBUG_BOXES = "DEBUG_BOXES";
static const char* luaEnumString_SceneDebugFlags_DEBUG_SPHERES = "DEBUG_SPHERES";

Scene::DebugFlags lua_enumFromString_SceneDebugFlags(const char* s)
{
    if (strcmp(s, luaEnumString_SceneDebugFlags_DEBUG_BOXES) == 0)
        return Scene::DEBUG_BOXES;
    if (strcmp(s, luaEnumString_SceneDebugFlags_DEBUG_SPHERES) == 0)
        return Scene::DEBUG_SPHERES;
    return Scene::DEBUG_BOXES;
}

const char* lua_stringFromEnum_SceneDebugFlags(Scene::DebugFlags e)
{
    if (e == Scene::DEBUG_BOXES)
        return luaEnumString_SceneDebugFlags_DEBUG_BOXES;
    if (e == Scene::DEBUG_SPHERES)
        return luaEnumString_SceneDebugFlags_DEBUG_SPHERES;
    return enumStringEmpty;
}

}

