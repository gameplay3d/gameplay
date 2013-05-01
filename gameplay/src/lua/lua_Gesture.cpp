#include "Base.h"
#include "ScriptController.h"
#include "lua_Gesture.h"
#include "Gesture.h"

namespace gameplay
{

void luaRegister_Gesture()
{
    const luaL_Reg lua_members[] = 
    {
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"SWIPE_DIRECTION_DOWN", lua_Gesture_static_SWIPE_DIRECTION_DOWN},
        {"SWIPE_DIRECTION_LEFT", lua_Gesture_static_SWIPE_DIRECTION_LEFT},
        {"SWIPE_DIRECTION_RIGHT", lua_Gesture_static_SWIPE_DIRECTION_RIGHT},
        {"SWIPE_DIRECTION_UP", lua_Gesture_static_SWIPE_DIRECTION_UP},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Gesture", lua_members, NULL, lua_Gesture__gc, lua_statics, scopePath);
}

static Gesture* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Gesture");
    luaL_argcheck(state, userdata != NULL, 1, "'Gesture' expected.");
    return (Gesture*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Gesture__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Gesture");
                luaL_argcheck(state, userdata != NULL, 1, "'Gesture' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Gesture* instance = (Gesture*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_Gesture__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_Gesture_static_SWIPE_DIRECTION_DOWN(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Gesture::SWIPE_DIRECTION_DOWN;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Gesture_static_SWIPE_DIRECTION_LEFT(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Gesture::SWIPE_DIRECTION_LEFT;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Gesture_static_SWIPE_DIRECTION_RIGHT(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Gesture::SWIPE_DIRECTION_RIGHT;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_Gesture_static_SWIPE_DIRECTION_UP(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

    int result = Gesture::SWIPE_DIRECTION_UP;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

}
