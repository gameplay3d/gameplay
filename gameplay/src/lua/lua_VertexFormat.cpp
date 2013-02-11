#include "Base.h"
#include "ScriptController.h"
#include "lua_VertexFormat.h"
#include "Base.h"
#include "VertexFormat.h"
#include "lua_VertexFormatUsage.h"

namespace gameplay
{

void luaRegister_VertexFormat()
{
    const luaL_Reg lua_members[] = 
    {
        {"getElement", lua_VertexFormat_getElement},
        {"getElementCount", lua_VertexFormat_getElementCount},
        {"getVertexSize", lua_VertexFormat_getVertexSize},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"toString", lua_VertexFormat_static_toString},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("VertexFormat", lua_members, lua_VertexFormat__init, lua_VertexFormat__gc, lua_statics, scopePath);
}

static VertexFormat* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "VertexFormat");
    luaL_argcheck(state, userdata != NULL, 1, "'VertexFormat' expected.");
    return (VertexFormat*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_VertexFormat__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "VertexFormat");
                luaL_argcheck(state, userdata != NULL, 1, "'VertexFormat' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    VertexFormat* instance = (VertexFormat*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_VertexFormat__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_VertexFormat__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<VertexFormat::Element> param1 = gameplay::ScriptUtil::getObjectPointer<VertexFormat::Element>(1, "VertexFormatElement", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'VertexFormat::Element'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 2);

                void* returnPtr = (void*)new VertexFormat(param1, param2);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "VertexFormat");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_VertexFormat__init - Failed to match the given parameters to a valid function signature.");
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

int lua_VertexFormat_getElement(lua_State* state)
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

                VertexFormat* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getElement(param1));
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "VertexFormatElement");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_VertexFormat_getElement - Failed to match the given parameters to a valid function signature.");
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

int lua_VertexFormat_getElementCount(lua_State* state)
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
                VertexFormat* instance = getInstance(state);
                unsigned int result = instance->getElementCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_VertexFormat_getElementCount - Failed to match the given parameters to a valid function signature.");
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

int lua_VertexFormat_getVertexSize(lua_State* state)
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
                VertexFormat* instance = getInstance(state);
                unsigned int result = instance->getVertexSize();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_VertexFormat_getVertexSize - Failed to match the given parameters to a valid function signature.");
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

int lua_VertexFormat_static_toString(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                VertexFormat::Usage param1 = (VertexFormat::Usage)lua_enumFromString_VertexFormatUsage(luaL_checkstring(state, 1));

                const char* result = VertexFormat::toString(param1);

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_VertexFormat_static_toString - Failed to match the given parameters to a valid function signature.");
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
