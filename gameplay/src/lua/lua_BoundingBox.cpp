#include "Base.h"
#include "ScriptController.h"
#include "lua_BoundingBox.h"
#include "Base.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Plane.h"

namespace gameplay
{

void luaRegister_BoundingBox()
{
    const luaL_Reg lua_members[] = 
    {
        {"getCenter", lua_BoundingBox_getCenter},
        {"getCorners", lua_BoundingBox_getCorners},
        {"intersects", lua_BoundingBox_intersects},
        {"isEmpty", lua_BoundingBox_isEmpty},
        {"max", lua_BoundingBox_max},
        {"merge", lua_BoundingBox_merge},
        {"min", lua_BoundingBox_min},
        {"set", lua_BoundingBox_set},
        {"transform", lua_BoundingBox_transform},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"empty", lua_BoundingBox_static_empty},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("BoundingBox", lua_members, lua_BoundingBox__init, lua_BoundingBox__gc, lua_statics, scopePath);
}

static BoundingBox* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "BoundingBox");
    luaL_argcheck(state, userdata != NULL, 1, "'BoundingBox' expected.");
    return (BoundingBox*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_BoundingBox__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "BoundingBox");
                luaL_argcheck(state, userdata != NULL, 1, "'BoundingBox' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    BoundingBox* instance = (BoundingBox*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_BoundingBox__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_BoundingBox__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)new BoundingBox();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "BoundingBox");
                lua_setmetatable(state, -2);
            }
            else
            {
                lua_pushnil(state);
            }

            return 1;
            break;
        }
        case 1:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<BoundingBox> param1 = gameplay::ScriptUtil::getObjectPointer<BoundingBox>(1, "BoundingBox", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    void* returnPtr = (void*)new BoundingBox(*param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "BoundingBox");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(1, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    void* returnPtr = (void*)new BoundingBox(*param1, *param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "BoundingBox");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 6:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 1);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 3);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 4);

                    // Get parameter 5 off the stack.
                    float param5 = (float)luaL_checknumber(state, 5);

                    // Get parameter 6 off the stack.
                    float param6 = (float)luaL_checknumber(state, 6);

                    void* returnPtr = (void*)new BoundingBox(param1, param2, param3, param4, param5, param6);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "BoundingBox");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0, 1, 2 or 6).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_BoundingBox_getCenter(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA))
                {
                    BoundingBox* instance = getInstance(state);
                    void* returnPtr = (void*)new Vector3(instance->getCenter());
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Vector3");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox_getCenter - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    instance->getCenter(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox_getCenter - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_BoundingBox_getCorners(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Vector3'.");
                    lua_error(state);
                }

                BoundingBox* instance = getInstance(state);
                instance->getCorners(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_BoundingBox_getCorners - Failed to match the given parameters to a valid function signature.");
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

int lua_BoundingBox_intersects(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<BoundingBox> param1 = gameplay::ScriptUtil::getObjectPointer<BoundingBox>(2, "BoundingBox", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    bool result = instance->intersects(*param1);

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<BoundingSphere> param1 = gameplay::ScriptUtil::getObjectPointer<BoundingSphere>(2, "BoundingSphere", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    bool result = instance->intersects(*param1);

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Frustum> param1 = gameplay::ScriptUtil::getObjectPointer<Frustum>(2, "Frustum", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    bool result = instance->intersects(*param1);

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Plane> param1 = gameplay::ScriptUtil::getObjectPointer<Plane>(2, "Plane", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    float result = instance->intersects(*param1);

                    // Push the return value onto the stack.
                    lua_pushnumber(state, result);

                    return 1;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Ray> param1 = gameplay::ScriptUtil::getObjectPointer<Ray>(2, "Ray", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    float result = instance->intersects(*param1);

                    // Push the return value onto the stack.
                    lua_pushnumber(state, result);

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox_intersects - Failed to match the given parameters to a valid function signature.");
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

int lua_BoundingBox_isEmpty(lua_State* state)
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
                BoundingBox* instance = getInstance(state);
                bool result = instance->isEmpty();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_BoundingBox_isEmpty - Failed to match the given parameters to a valid function signature.");
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

int lua_BoundingBox_max(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    BoundingBox* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        bool param2Valid;
        gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
        if (!param2Valid)
        {
            lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector3'.");
            lua_error(state);
        }

        instance->max = *param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)new Vector3(instance->max);
        if (returnPtr)
        {
            gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
            object->instance = returnPtr;
            object->owns = true;
            luaL_getmetatable(state, "Vector3");
            lua_setmetatable(state, -2);
        }
        else
        {
            lua_pushnil(state);
        }

        return 1;
    }
}

int lua_BoundingBox_merge(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<BoundingSphere> param1 = gameplay::ScriptUtil::getObjectPointer<BoundingSphere>(2, "BoundingSphere", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    instance->merge(*param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<BoundingBox> param1 = gameplay::ScriptUtil::getObjectPointer<BoundingBox>(2, "BoundingBox", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    instance->merge(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox_merge - Failed to match the given parameters to a valid function signature.");
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

int lua_BoundingBox_min(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    BoundingBox* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        bool param2Valid;
        gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
        if (!param2Valid)
        {
            lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector3'.");
            lua_error(state);
        }

        instance->min = *param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)new Vector3(instance->min);
        if (returnPtr)
        {
            gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
            object->instance = returnPtr;
            object->owns = true;
            luaL_getmetatable(state, "Vector3");
            lua_setmetatable(state, -2);
        }
        else
        {
            lua_pushnil(state);
        }

        return 1;
    }
}

int lua_BoundingBox_set(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<BoundingBox> param1 = gameplay::ScriptUtil::getObjectPointer<BoundingBox>(2, "BoundingBox", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    instance->set(*param1);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<BoundingSphere> param1 = gameplay::ScriptUtil::getObjectPointer<BoundingSphere>(2, "BoundingSphere", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    instance->set(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox_set - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    BoundingBox* instance = getInstance(state);
                    instance->set(*param1, *param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox_set - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 3);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 4);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 5);

                    // Get parameter 5 off the stack.
                    float param5 = (float)luaL_checknumber(state, 6);

                    // Get parameter 6 off the stack.
                    float param6 = (float)luaL_checknumber(state, 7);

                    BoundingBox* instance = getInstance(state);
                    instance->set(param1, param2, param3, param4, param5, param6);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_BoundingBox_set - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3 or 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_BoundingBox_static_empty(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)&(BoundingBox::empty());
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = false;
                luaL_getmetatable(state, "BoundingBox");
                lua_setmetatable(state, -2);
            }
            else
            {
                lua_pushnil(state);
            }

            return 1;
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_BoundingBox_transform(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Matrix> param1 = gameplay::ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Matrix'.");
                    lua_error(state);
                }

                BoundingBox* instance = getInstance(state);
                instance->transform(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_BoundingBox_transform - Failed to match the given parameters to a valid function signature.");
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
