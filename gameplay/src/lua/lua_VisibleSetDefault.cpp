#include "Base.h"
#include "ScriptController.h"
#include "lua_VisibleSetDefault.h"
#include "Base.h"
#include "VisibleSetDefault.h"

namespace gameplay
{

void luaRegister_VisibleSetDefault()
{
    const luaL_Reg lua_members[] = 
    {
        {"getNext", lua_VisibleSetDefault_getNext},
        {"getScene", lua_VisibleSetDefault_getScene},
        {"reset", lua_VisibleSetDefault_reset},
        {"setScene", lua_VisibleSetDefault_setScene},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_VisibleSetDefault_static_create},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("VisibleSetDefault", lua_members, NULL, NULL, lua_statics, scopePath);
}

static VisibleSetDefault* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "VisibleSetDefault");
    luaL_argcheck(state, userdata != NULL, 1, "'VisibleSetDefault' expected.");
    return (VisibleSetDefault*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_VisibleSetDefault_getNext(lua_State* state)
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
                VisibleSetDefault* instance = getInstance(state);
                void* returnPtr = (void*)instance->getNext();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Node");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_VisibleSetDefault_getNext - Failed to match the given parameters to a valid function signature.");
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

int lua_VisibleSetDefault_getScene(lua_State* state)
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
                VisibleSetDefault* instance = getInstance(state);
                void* returnPtr = (void*)instance->getScene();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Scene");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_VisibleSetDefault_getScene - Failed to match the given parameters to a valid function signature.");
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

int lua_VisibleSetDefault_reset(lua_State* state)
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
                VisibleSetDefault* instance = getInstance(state);
                instance->reset();
                
                return 0;
            }

            lua_pushstring(state, "lua_VisibleSetDefault_reset - Failed to match the given parameters to a valid function signature.");
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

int lua_VisibleSetDefault_setScene(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Scene> param1 = gameplay::ScriptUtil::getObjectPointer<Scene>(2, "Scene", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Scene'.");
                    lua_error(state);
                }

                VisibleSetDefault* instance = getInstance(state);
                instance->setScene(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_VisibleSetDefault_setScene - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_VisibleSetDefault_static_create(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Scene> param1 = gameplay::ScriptUtil::getObjectPointer<Scene>(1, "Scene", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Scene'.");
                    lua_error(state);
                }

                void* returnPtr = (void*)VisibleSetDefault::create(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "VisibleSetDefault");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_VisibleSetDefault_static_create - Failed to match the given parameters to a valid function signature.");
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

}
