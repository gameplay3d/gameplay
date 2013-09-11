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
		{"setIndexData", lua_MeshPart_setIndexData},
        {"isDynamic", lua_MeshPart_isDynamic},
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

int lua_MeshPart_setIndexData(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA))
			{
                MeshPart* instance = getInstance(state);

				Mesh::IndexFormat fmt = instance->getIndexFormat();

				switch ( fmt ) {
					case Mesh::INDEX8: {
						gameplay::ScriptUtil::LuaArray<unsigned char> param1 = gameplay::ScriptUtil::getUnsignedCharPointer(2);
						instance->setIndexData( param1, 0, instance->getIndexCount() );
		    			return 0;
					}
					case Mesh::INDEX16: {
						gameplay::ScriptUtil::LuaArray<unsigned short> param1 = gameplay::ScriptUtil::getUnsignedShortPointer(2);
						instance->setIndexData( param1, 0, instance->getIndexCount() );
		    			return 0;
					}
					case Mesh::INDEX32: {
						gameplay::ScriptUtil::LuaArray<unsigned long> param1 = gameplay::ScriptUtil::getUnsignedLongPointer(2);
						instance->setIndexData( param1, 0, instance->getIndexCount() );
		    			return 0;
					}
					default: {
						lua_pushstring(state, "lua_MeshPart_setIndexData - Unknown index format.");
						lua_error(state);
						break;
					 }
				}
            }

            lua_pushstring(state, "lua_MeshPart_setIndexData - Failed to match the given parameters to a valid function signature.");
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

}
