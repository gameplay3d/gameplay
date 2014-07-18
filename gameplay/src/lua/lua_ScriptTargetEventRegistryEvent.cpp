#include "Base.h"
#include "ScriptController.h"
#include "lua_ScriptTargetEventRegistryEvent.h"
#include "Base.h"
#include "ScriptController.h"
#include "ScriptTarget.h"

namespace gameplay
{

void luaRegister_ScriptTargetEventRegistryEvent()
{
    const luaL_Reg lua_members[] = 
    {
        {"args", lua_ScriptTargetEventRegistryEvent_args},
        {"name", lua_ScriptTargetEventRegistryEvent_name},
        {"registry", lua_ScriptTargetEventRegistryEvent_registry},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("ScriptTarget");
    scopePath.push_back("EventRegistry");

    gameplay::ScriptUtil::registerClass("ScriptTargetEventRegistryEvent", lua_members, lua_ScriptTargetEventRegistryEvent__init, lua_ScriptTargetEventRegistryEvent__gc, lua_statics, scopePath);
}

static ScriptTarget::EventRegistry::Event* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ScriptTargetEventRegistryEvent");
    luaL_argcheck(state, userdata != NULL, 1, "'ScriptTargetEventRegistryEvent' expected.");
    return (ScriptTarget::EventRegistry::Event*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_ScriptTargetEventRegistryEvent__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "ScriptTargetEventRegistryEvent");
                luaL_argcheck(state, userdata != NULL, 1, "'ScriptTargetEventRegistryEvent' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    ScriptTarget::EventRegistry::Event* instance = (ScriptTarget::EventRegistry::Event*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_ScriptTargetEventRegistryEvent__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_ScriptTargetEventRegistryEvent__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)new ScriptTarget::EventRegistry::Event();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "ScriptTargetEventRegistryEvent");
                lua_setmetatable(state, -2);
            }
            else
            {
                lua_pushnil(state);
            }

            return 1;
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_ScriptTargetEventRegistryEvent_args(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    ScriptTarget::EventRegistry::Event* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        std::string param2 = gameplay::ScriptUtil::getString(2, true);

        instance->args = param2;
        return 0;
    }
    else
    {
        std::string result = instance->args;

        // Push the return value onto the stack.
        lua_pushstring(state, result.c_str());

        return 1;
    }
}

int lua_ScriptTargetEventRegistryEvent_name(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    ScriptTarget::EventRegistry::Event* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        std::string param2 = gameplay::ScriptUtil::getString(2, true);

        instance->name = param2;
        return 0;
    }
    else
    {
        std::string result = instance->name;

        // Push the return value onto the stack.
        lua_pushstring(state, result.c_str());

        return 1;
    }
}

int lua_ScriptTargetEventRegistryEvent_registry(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    ScriptTarget::EventRegistry::Event* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        GP_WARN("Attempting to get parameter 2 with unrecognized type classgameplay_1_1_script_target_1_1_event_registry as an unsigned integer.");
        classgameplay_1_1_script_target_1_1_event_registry* param2 = (classgameplay_1_1_script_target_1_1_event_registry)luaL_checkunsigned(state, 2);

        memcpy(instance->registry, param2, sizeof() * classgameplay_1_1_script_target_1_1_event_registry);
        return 0;
    }
    else
    {
        classgameplay_1_1_script_target_1_1_event_registry* result = instance->registry;

        // Push the return value onto the stack.
        lua_pushlightuserdata(state, result);
        return 1;
    }
}

}
