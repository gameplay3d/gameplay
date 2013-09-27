#include "Base.h"
#include "ScriptController.h"
#include "lua_MeshPart.h"
#include "Base.h"
#include "MeshPart.h"
#include "lua_MeshIndexFormat.h"
#include "lua_MeshPrimitiveType.h"

namespace gameplay
{

void luaRegister_MeshPart()
{
    const luaL_Reg lua_members[] = 
    {
        {"getIndexBuffer", lua_MeshPart_getIndexBuffer},
        {"getIndexCount", lua_MeshPart_getIndexCount},
        {"getIndexFormat", lua_MeshPart_getIndexFormat},
        {"getMeshIndex", lua_MeshPart_getMeshIndex},
        {"getPrimitiveType", lua_MeshPart_getPrimitiveType},
        {"isDynamic", lua_MeshPart_isDynamic},
        {"setIndexData16", lua_MeshPart_setIndexData16},
        {"setIndexData32", lua_MeshPart_setIndexData32},
        {"setIndexData8", lua_MeshPart_setIndexData8},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("MeshPart", lua_members, NULL, lua_MeshPart__gc, lua_statics, scopePath);
}

static MeshPart* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "MeshPart");
    luaL_argcheck(state, userdata != NULL, 1, "'MeshPart' expected.");
    return (MeshPart*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_MeshPart__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "MeshPart");
                luaL_argcheck(state, userdata != NULL, 1, "'MeshPart' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    MeshPart* instance = (MeshPart*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshPart__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshPart_getIndexBuffer(lua_State* state)
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
                MeshPart* instance = getInstance(state);
                void* returnPtr = (void*)new GLuint(instance->getIndexBuffer());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "GLuint");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_MeshPart_getIndexBuffer - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshPart_getIndexCount(lua_State* state)
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
                MeshPart* instance = getInstance(state);
                unsigned int result = instance->getIndexCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_MeshPart_getIndexCount - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshPart_getIndexFormat(lua_State* state)
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
                MeshPart* instance = getInstance(state);
                Mesh::IndexFormat result = instance->getIndexFormat();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_MeshIndexFormat(result));

                return 1;
            }

            lua_pushstring(state, "lua_MeshPart_getIndexFormat - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshPart_getMeshIndex(lua_State* state)
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
                MeshPart* instance = getInstance(state);
                unsigned int result = instance->getMeshIndex();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_MeshPart_getMeshIndex - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshPart_getPrimitiveType(lua_State* state)
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
                MeshPart* instance = getInstance(state);
                Mesh::PrimitiveType result = instance->getPrimitiveType();

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_MeshPrimitiveType(result));

                return 1;
            }

            lua_pushstring(state, "lua_MeshPart_getPrimitiveType - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshPart_isDynamic(lua_State* state)
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
                MeshPart* instance = getInstance(state);
                bool result = instance->isDynamic();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_MeshPart_isDynamic - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshPart_setIndexData16(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<unsigned short> param1 = gameplay::ScriptUtil::getUnsignedShortPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                MeshPart* instance = getInstance(state);
                instance->setIndexData16(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshPart_setIndexData16 - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshPart_setIndexData32(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<unsigned long> param1 = gameplay::ScriptUtil::getUnsignedLongPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                MeshPart* instance = getInstance(state);
                instance->setIndexData32(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshPart_setIndexData32 - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshPart_setIndexData8(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<unsigned char> param1 = gameplay::ScriptUtil::getUnsignedCharPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                MeshPart* instance = getInstance(state);
                instance->setIndexData8(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshPart_setIndexData8 - Failed to match the given parameters to a valid function signature.");
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
