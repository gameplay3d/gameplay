#include "Base.h"
#include "ScriptController.h"
#include "lua_TerrainListener.h"
#include "Animation.h"
#include "AnimationTarget.h"
#include "Base.h"
#include "FileSystem.h"
#include "Game.h"
#include "Node.h"
#include "Ref.h"
#include "ScriptController.h"
#include "ScriptTarget.h"
#include "Terrain.h"
#include "TerrainPatch.h"
#include "Transform.h"
#include "lua_CurveInterpolationType.h"
#include "lua_TerrainFlags.h"

namespace gameplay
{

void luaRegister_TerrainListener()
{
    const luaL_Reg lua_members[] = 
    {
        {"materialUpdated", lua_TerrainListener_materialUpdated},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("Terrain");

    gameplay::ScriptUtil::registerClass("TerrainListener", lua_members, NULL, lua_TerrainListener__gc, lua_statics, scopePath);
}

static Terrain::Listener* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "TerrainListener");
    luaL_argcheck(state, userdata != NULL, 1, "'TerrainListener' expected.");
    return (Terrain::Listener*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_TerrainListener__gc(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                void* userdata = luaL_checkudata(state, 1, "TerrainListener");
                luaL_argcheck(state, userdata != NULL, 1, "'TerrainListener' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Terrain::Listener* instance = (Terrain::Listener*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_TerrainListener__gc - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_TerrainListener_materialUpdated(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Terrain> param1 = gameplay::ScriptUtil::getObjectPointer<Terrain>(2, "Terrain", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Terrain'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Material> param2 = gameplay::ScriptUtil::getObjectPointer<Material>(3, "Material", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Material'.");
                    lua_error(state);
                }

                Terrain::Listener* instance = getInstance(state);
                instance->materialUpdated(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_TerrainListener_materialUpdated - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
