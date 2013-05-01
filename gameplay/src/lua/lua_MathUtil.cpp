#include "Base.h"
#include "ScriptController.h"
#include "lua_MathUtil.h"
#include "Base.h"
#include "MathUtil.h"

namespace gameplay
{

void luaRegister_MathUtil()
{
    const luaL_Reg lua_members[] = 
    {
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"smooth", lua_MathUtil_static_smooth},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("MathUtil", lua_members, NULL, lua_MathUtil__gc, lua_statics, scopePath);
}

static MathUtil* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "MathUtil");
    luaL_argcheck(state, userdata != NULL, 1, "'MathUtil' expected.");
    return (MathUtil*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_MathUtil__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "MathUtil");
                luaL_argcheck(state, userdata != NULL, 1, "'MathUtil' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    MathUtil* instance = (MathUtil*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_MathUtil__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_MathUtil_static_smooth(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TLIGHTUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(1);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 3);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 4);

                    MathUtil::smooth(param1, param2, param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_MathUtil_static_smooth - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TLIGHTUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(1);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 3);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 4);

                    // Get parameter 5 off the stack.
                    float param5 = (float)luaL_checknumber(state, 5);

                    MathUtil::smooth(param1, param2, param3, param4, param5);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_MathUtil_static_smooth - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
