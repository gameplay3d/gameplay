#include "Base.h"
#include "ScriptController.h"
#include "lua_Gamepad.h"
#include "Base.h"
#include "Button.h"
#include "Form.h"
#include "Game.h"
#include "Gamepad.h"
#include "Joystick.h"
#include "Platform.h"
#include "lua_GamepadButtonMapping.h"

namespace gameplay
{

void luaRegister_Gamepad()
{
    const luaL_Reg lua_members[] = 
    {
        {"draw", lua_Gamepad_draw},
        {"getButtonCount", lua_Gamepad_getButtonCount},
        {"getForm", lua_Gamepad_getForm},
        {"getJoystickCount", lua_Gamepad_getJoystickCount},
        {"getJoystickValues", lua_Gamepad_getJoystickValues},
        {"getProductId", lua_Gamepad_getProductId},
        {"getProductString", lua_Gamepad_getProductString},
        {"getTriggerCount", lua_Gamepad_getTriggerCount},
        {"getTriggerValue", lua_Gamepad_getTriggerValue},
        {"getVendorId", lua_Gamepad_getVendorId},
        {"getVendorString", lua_Gamepad_getVendorString},
        {"isButtonDown", lua_Gamepad_isButtonDown},
        {"isVirtual", lua_Gamepad_isVirtual},
        {"update", lua_Gamepad_update},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Gamepad", lua_members, NULL, NULL, lua_statics, scopePath);
}

static Gamepad* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Gamepad");
    luaL_argcheck(state, userdata != NULL, 1, "'Gamepad' expected.");
    return (Gamepad*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Gamepad_draw(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                instance->draw();
                
                return 0;
            }

            lua_pushstring(state, "lua_Gamepad_draw - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getButtonCount(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                unsigned int result = instance->getButtonCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_getButtonCount - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getForm(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                void* returnPtr = (void*)instance->getForm();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Form");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_getForm - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getJoystickCount(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                unsigned int result = instance->getJoystickCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_getJoystickCount - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getJoystickValues(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param2 = gameplay::ScriptUtil::getObjectPointer<Vector2>(3, "Vector2", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector2'.");
                    lua_error(state);
                }

                Gamepad* instance = getInstance(state);
                instance->getJoystickValues(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Gamepad_getJoystickValues - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getProductId(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                unsigned int result = instance->getProductId();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_getProductId - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getProductString(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                const char* result = instance->getProductString();

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_getProductString - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getTriggerCount(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                unsigned int result = instance->getTriggerCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_getTriggerCount - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getTriggerValue(lua_State* state)
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
                unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                Gamepad* instance = getInstance(state);
                float result = instance->getTriggerValue(param1);

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_getTriggerValue - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getVendorId(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                unsigned int result = instance->getVendorId();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_getVendorId - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_getVendorString(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                const char* result = instance->getVendorString();

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_getVendorString - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_isButtonDown(lua_State* state)
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
                Gamepad::ButtonMapping param1 = (Gamepad::ButtonMapping)lua_enumFromString_GamepadButtonMapping(luaL_checkstring(state, 2));

                Gamepad* instance = getInstance(state);
                bool result = instance->isButtonDown(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_isButtonDown - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_isVirtual(lua_State* state)
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
                Gamepad* instance = getInstance(state);
                bool result = instance->isVirtual();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Gamepad_isVirtual - Failed to match the given parameters to a valid function signature.");
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

int lua_Gamepad_update(lua_State* state)
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
                float param1 = (float)luaL_checknumber(state, 2);

                Gamepad* instance = getInstance(state);
                instance->update(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Gamepad_update - Failed to match the given parameters to a valid function signature.");
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

}
