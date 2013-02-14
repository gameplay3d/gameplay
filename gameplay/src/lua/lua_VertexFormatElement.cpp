#include "Base.h"
#include "ScriptController.h"
#include "lua_VertexFormatElement.h"
#include "Base.h"
#include "VertexFormat.h"
#include "lua_VertexFormatUsage.h"

namespace gameplay
{

void luaRegister_VertexFormatElement()
{
    const luaL_Reg lua_members[] = 
    {
        {"size", lua_VertexFormatElement_size},
        {"usage", lua_VertexFormatElement_usage},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("VertexFormat");

    gameplay::ScriptUtil::registerClass("VertexFormatElement", lua_members, lua_VertexFormatElement__init, lua_VertexFormatElement__gc, lua_statics, scopePath);
}

static VertexFormat::Element* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "VertexFormatElement");
    luaL_argcheck(state, userdata != NULL, 1, "'VertexFormatElement' expected.");
    return (VertexFormat::Element*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_VertexFormatElement__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "VertexFormatElement");
                luaL_argcheck(state, userdata != NULL, 1, "'VertexFormatElement' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    VertexFormat::Element* instance = (VertexFormat::Element*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_VertexFormatElement__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_VertexFormatElement__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)new VertexFormat::Element();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "VertexFormatElement");
                lua_setmetatable(state, -2);
            }
            else
            {
                lua_pushnil(state);
            }

            return 1;
            break;
        }
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                    lua_type(state, 2) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    VertexFormat::Usage param1 = (VertexFormat::Usage)lua_enumFromString_VertexFormatUsage(luaL_checkstring(state, 1));

                    // Get parameter 2 off the stack.
                    unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 2);

                    void* returnPtr = (void*)new VertexFormat::Element(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "VertexFormatElement");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_VertexFormatElement__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_VertexFormatElement_size(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    VertexFormat::Element* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 2);

        instance->size = param2;
        return 0;
    }
    else
    {
        unsigned int result = instance->size;

        // Push the return value onto the stack.
        lua_pushunsigned(state, result);

        return 1;
    }
}

int lua_VertexFormatElement_usage(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    VertexFormat::Element* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        VertexFormat::Usage param2 = (VertexFormat::Usage)lua_enumFromString_VertexFormatUsage(luaL_checkstring(state, 2));

        instance->usage = param2;
        return 0;
    }
    else
    {
        VertexFormat::Usage result = instance->usage;

        // Push the return value onto the stack.
        lua_pushstring(state, lua_stringFromEnum_VertexFormatUsage(result));

        return 1;
    }
}

}
