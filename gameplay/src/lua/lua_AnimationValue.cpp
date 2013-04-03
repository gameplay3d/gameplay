#include "Base.h"
#include "ScriptController.h"
#include "lua_AnimationValue.h"
#include "AnimationValue.h"
#include "Base.h"

namespace gameplay
{

void luaRegister_AnimationValue()
{
    const luaL_Reg lua_members[] = 
    {
        {"getFloat", lua_AnimationValue_getFloat},
        {"getFloats", lua_AnimationValue_getFloats},
        {"setFloat", lua_AnimationValue_setFloat},
        {"setFloats", lua_AnimationValue_setFloats},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("AnimationValue", lua_members, NULL, NULL, lua_statics, scopePath);
}

static AnimationValue* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "AnimationValue");
    luaL_argcheck(state, userdata != NULL, 1, "'AnimationValue' expected.");
    return (AnimationValue*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_AnimationValue_getFloat(lua_State* state)
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

                AnimationValue* instance = getInstance(state);
                float result = instance->getFloat(param1);

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_AnimationValue_getFloat - Failed to match the given parameters to a valid function signature.");
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

int lua_AnimationValue_getFloats(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                // Get parameter 2 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param2 = gameplay::ScriptUtil::getFloatPointer(3);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                AnimationValue* instance = getInstance(state);
                instance->getFloats(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_AnimationValue_getFloats - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_AnimationValue_setFloat(lua_State* state)
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
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                AnimationValue* instance = getInstance(state);
                instance->setFloat(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_AnimationValue_setFloat - Failed to match the given parameters to a valid function signature.");
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

int lua_AnimationValue_setFloats(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                // Get parameter 2 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param2 = gameplay::ScriptUtil::getFloatPointer(3);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                AnimationValue* instance = getInstance(state);
                instance->setFloats(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_AnimationValue_setFloats - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
