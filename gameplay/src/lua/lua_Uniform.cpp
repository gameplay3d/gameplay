#include "Base.h"
#include "ScriptController.h"
#include "lua_Uniform.h"
#include "Base.h"
#include "Effect.h"
#include "FileSystem.h"
#include "Game.h"
#include "Ref.h"

namespace gameplay
{

void luaRegister_Uniform()
{
    const luaL_Reg lua_members[] = 
    {
        {"getEffect", lua_Uniform_getEffect},
        {"getName", lua_Uniform_getName},
        {"getType", lua_Uniform_getType},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Uniform", lua_members, NULL, NULL, lua_statics, scopePath);
}

static Uniform* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Uniform");
    luaL_argcheck(state, userdata != NULL, 1, "'Uniform' expected.");
    return (Uniform*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Uniform_getEffect(lua_State* state)
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
                Uniform* instance = getInstance(state);
                void* returnPtr = (void*)instance->getEffect();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Effect");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Uniform_getEffect - Failed to match the given parameters to a valid function signature.");
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

int lua_Uniform_getName(lua_State* state)
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
                Uniform* instance = getInstance(state);
                const char* result = instance->getName();

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Uniform_getName - Failed to match the given parameters to a valid function signature.");
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

int lua_Uniform_getType(lua_State* state)
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
                Uniform* instance = getInstance(state);
                GLenum result = instance->getType();

                // Push the return value onto the stack.
                GP_WARN("Attempting to return value with unrecognized type GLenum as an unsigned integer.");
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Uniform_getType - Failed to match the given parameters to a valid function signature.");
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
