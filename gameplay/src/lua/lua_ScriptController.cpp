#include "Base.h"
#include "ScriptController.h"
#include "lua_ScriptController.h"
#include "Base.h"
#include "FileSystem.h"
#include "ScriptController.h"

namespace gameplay
{

void luaRegister_ScriptController()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"getBoolPointer", lua_ScriptController_getBoolPointer},
        {"getDoublePointer", lua_ScriptController_getDoublePointer},
        {"getFloatPointer", lua_ScriptController_getFloatPointer},
        {"getIntPointer", lua_ScriptController_getIntPointer},
        {"getLongPointer", lua_ScriptController_getLongPointer},
        {"getShortPointer", lua_ScriptController_getShortPointer},
        {"getUnsignedCharPointer", lua_ScriptController_getUnsignedCharPointer},
        {"getUnsignedIntPointer", lua_ScriptController_getUnsignedIntPointer},
        {"getUnsignedLongPointer", lua_ScriptController_getUnsignedLongPointer},
        {"getUnsignedShortPointer", lua_ScriptController_getUnsignedShortPointer},
        {"loadScript", lua_ScriptController_loadScript},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"getInstance", lua_ScriptController_static_getInstance},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    sc->registerClass("ScriptController", lua_members, NULL, NULL, lua_statics, scopePath);
}

static ScriptController* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ScriptController");
    luaL_argcheck(state, userdata != NULL, 1, "'ScriptController' expected.");
    return (ScriptController*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_ScriptController_getBoolPointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                bool* result = instance->getBoolPointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getBoolPointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_getDoublePointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                double* result = instance->getDoublePointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getDoublePointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_getFloatPointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                float* result = instance->getFloatPointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getFloatPointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_getIntPointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                int* result = instance->getIntPointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getIntPointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_getLongPointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                long* result = instance->getLongPointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getLongPointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_getShortPointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                short* result = instance->getShortPointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getShortPointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_getUnsignedCharPointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                unsigned char* result = instance->getUnsignedCharPointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getUnsignedCharPointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_getUnsignedIntPointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                unsigned int* result = instance->getUnsignedIntPointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getUnsignedIntPointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_getUnsignedLongPointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                unsigned long* result = instance->getUnsignedLongPointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getUnsignedLongPointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_getUnsignedShortPointer(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                ScriptController* instance = getInstance(state);
                unsigned short* result = instance->getUnsignedShortPointer(param1);

                // Push the return value onto the stack.
                lua_pushlightuserdata(state, result);
                return 1;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_getUnsignedShortPointer - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_loadScript(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = ScriptController::getInstance()->getString(2, false);

                ScriptController* instance = getInstance(state);
                instance->loadScript(param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_ScriptController_loadScript - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_ScriptController_static_getInstance(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)ScriptController::getInstance();
            if (returnPtr)
            {
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = returnPtr;
                object->owns = false;
                luaL_getmetatable(state, "ScriptController");
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

}
