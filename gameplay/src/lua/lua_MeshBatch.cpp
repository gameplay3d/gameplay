#include "Base.h"
#include "ScriptController.h"
#include "lua_MeshBatch.h"
#include "Base.h"
#include "Material.h"
#include "MeshBatch.h"
#include "lua_MeshPrimitiveType.h"

namespace gameplay
{

void luaRegister_MeshBatch()
{
    const luaL_Reg lua_members[] = 
    {
        {"add", lua_MeshBatch_add},
        {"draw", lua_MeshBatch_draw},
        {"finish", lua_MeshBatch_finish},
        {"getCapacity", lua_MeshBatch_getCapacity},
        {"getMaterial", lua_MeshBatch_getMaterial},
        {"setCapacity", lua_MeshBatch_setCapacity},
        {"start", lua_MeshBatch_start},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_MeshBatch_static_create},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("MeshBatch", lua_members, NULL, lua_MeshBatch__gc, lua_statics, scopePath);
}

static MeshBatch* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "MeshBatch");
    luaL_argcheck(state, userdata != NULL, 1, "'MeshBatch' expected.");
    return (MeshBatch*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_MeshBatch__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "MeshBatch");
                luaL_argcheck(state, userdata != NULL, 1, "'MeshBatch' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    MeshBatch* instance = (MeshBatch*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshBatch__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshBatch_add(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                MeshBatch* instance = getInstance(state);
                instance->add(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshBatch_add - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA))
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                gameplay::ScriptUtil::LuaArray<unsigned short> param3 = gameplay::ScriptUtil::getUnsignedShortPointer(4);

                MeshBatch* instance = getInstance(state);
                instance->add(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshBatch_add - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param1 = gameplay::ScriptUtil::getFloatPointer(2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                gameplay::ScriptUtil::LuaArray<unsigned short> param3 = gameplay::ScriptUtil::getUnsignedShortPointer(4);

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                MeshBatch* instance = getInstance(state);
                instance->add(param1, param2, param3, param4);
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshBatch_add - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_MeshBatch_draw(lua_State* state)
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
                MeshBatch* instance = getInstance(state);
                instance->draw();
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshBatch_draw - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshBatch_finish(lua_State* state)
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
                MeshBatch* instance = getInstance(state);
                instance->finish();
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshBatch_finish - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshBatch_getCapacity(lua_State* state)
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
                MeshBatch* instance = getInstance(state);
                unsigned int result = instance->getCapacity();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_MeshBatch_getCapacity - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshBatch_getMaterial(lua_State* state)
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
                MeshBatch* instance = getInstance(state);
                void* returnPtr = (void*)instance->getMaterial();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Material");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_MeshBatch_getMaterial - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshBatch_setCapacity(lua_State* state)
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

                MeshBatch* instance = getInstance(state);
                instance->setCapacity(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshBatch_setCapacity - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshBatch_start(lua_State* state)
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
                MeshBatch* instance = getInstance(state);
                instance->start();
                
                return 0;
            }

            lua_pushstring(state, "lua_MeshBatch_start - Failed to match the given parameters to a valid function signature.");
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

int lua_MeshBatch_static_create(lua_State* state)
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<VertexFormat> param1 = gameplay::ScriptUtil::getObjectPointer<VertexFormat>(1, "VertexFormat", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    Mesh::PrimitiveType param2 = (Mesh::PrimitiveType)lua_enumFromString_MeshPrimitiveType(luaL_checkstring(state, 2));

                    // Get parameter 3 off the stack.
                    const char* param3 = gameplay::ScriptUtil::getString(3, false);

                    // Get parameter 4 off the stack.
                    bool param4 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                    void* returnPtr = (void*)MeshBatch::create(*param1, param2, param3, param4);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "MeshBatch");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<VertexFormat> param1 = gameplay::ScriptUtil::getObjectPointer<VertexFormat>(1, "VertexFormat", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    Mesh::PrimitiveType param2 = (Mesh::PrimitiveType)lua_enumFromString_MeshPrimitiveType(luaL_checkstring(state, 2));

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Material> param3 = gameplay::ScriptUtil::getObjectPointer<Material>(3, "Material", false, &param3Valid);
                    if (!param3Valid)
                        break;

                    // Get parameter 4 off the stack.
                    bool param4 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                    void* returnPtr = (void*)MeshBatch::create(*param1, param2, param3, param4);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "MeshBatch");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_MeshBatch_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TBOOLEAN &&
                    lua_type(state, 5) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<VertexFormat> param1 = gameplay::ScriptUtil::getObjectPointer<VertexFormat>(1, "VertexFormat", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    Mesh::PrimitiveType param2 = (Mesh::PrimitiveType)lua_enumFromString_MeshPrimitiveType(luaL_checkstring(state, 2));

                    // Get parameter 3 off the stack.
                    const char* param3 = gameplay::ScriptUtil::getString(3, false);

                    // Get parameter 4 off the stack.
                    bool param4 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                    // Get parameter 5 off the stack.
                    unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 5);

                    void* returnPtr = (void*)MeshBatch::create(*param1, param2, param3, param4, param5);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "MeshBatch");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TBOOLEAN &&
                    lua_type(state, 5) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<VertexFormat> param1 = gameplay::ScriptUtil::getObjectPointer<VertexFormat>(1, "VertexFormat", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    Mesh::PrimitiveType param2 = (Mesh::PrimitiveType)lua_enumFromString_MeshPrimitiveType(luaL_checkstring(state, 2));

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Material> param3 = gameplay::ScriptUtil::getObjectPointer<Material>(3, "Material", false, &param3Valid);
                    if (!param3Valid)
                        break;

                    // Get parameter 4 off the stack.
                    bool param4 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                    // Get parameter 5 off the stack.
                    unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 5);

                    void* returnPtr = (void*)MeshBatch::create(*param1, param2, param3, param4, param5);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "MeshBatch");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_MeshBatch_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 6:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TBOOLEAN &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<VertexFormat> param1 = gameplay::ScriptUtil::getObjectPointer<VertexFormat>(1, "VertexFormat", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    Mesh::PrimitiveType param2 = (Mesh::PrimitiveType)lua_enumFromString_MeshPrimitiveType(luaL_checkstring(state, 2));

                    // Get parameter 3 off the stack.
                    const char* param3 = gameplay::ScriptUtil::getString(3, false);

                    // Get parameter 4 off the stack.
                    bool param4 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                    // Get parameter 5 off the stack.
                    unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 5);

                    // Get parameter 6 off the stack.
                    unsigned int param6 = (unsigned int)luaL_checkunsigned(state, 6);

                    void* returnPtr = (void*)MeshBatch::create(*param1, param2, param3, param4, param5, param6);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "MeshBatch");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TBOOLEAN &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<VertexFormat> param1 = gameplay::ScriptUtil::getObjectPointer<VertexFormat>(1, "VertexFormat", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    Mesh::PrimitiveType param2 = (Mesh::PrimitiveType)lua_enumFromString_MeshPrimitiveType(luaL_checkstring(state, 2));

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Material> param3 = gameplay::ScriptUtil::getObjectPointer<Material>(3, "Material", false, &param3Valid);
                    if (!param3Valid)
                        break;

                    // Get parameter 4 off the stack.
                    bool param4 = gameplay::ScriptUtil::luaCheckBool(state, 4);

                    // Get parameter 5 off the stack.
                    unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 5);

                    // Get parameter 6 off the stack.
                    unsigned int param6 = (unsigned int)luaL_checkunsigned(state, 6);

                    void* returnPtr = (void*)MeshBatch::create(*param1, param2, param3, param4, param5, param6);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "MeshBatch");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_MeshBatch_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4, 5 or 6).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
