#include "Base.h"
#include "ScriptController.h"
#include "BoundingSphere.h"
#include "lua_BoundingSphere.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_BoundingSphere()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"center", lua_BoundingSphere_center},
        {"intersects", lua_BoundingSphere_intersects},
        {"isEmpty", lua_BoundingSphere_isEmpty},
        {"merge", lua_BoundingSphere_merge},
        {"radius", lua_BoundingSphere_radius},
        {"set", lua_BoundingSphere_set},
        {"transform", lua_BoundingSphere_transform},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"empty", lua_BoundingSphere_static_empty},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    sc->registerClass("BoundingSphere", lua_members, lua_BoundingSphere__init, lua_BoundingSphere__gc, lua_statics, scopePath);
}

static BoundingSphere* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "BoundingSphere");
    luaL_argcheck(state, userdata != NULL, 1, "'BoundingSphere' expected.");
    return (BoundingSphere*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_BoundingSphere__gc(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                void* userdata = luaL_checkudata(state, 1, "BoundingSphere");
                luaL_argcheck(state, userdata != NULL, 1, "'BoundingSphere' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    BoundingSphere* instance = (BoundingSphere*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_BoundingSphere__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)new BoundingSphere();
            object->owns = true;
            luaL_getmetatable(state, "BoundingSphere");
            lua_setmetatable(state, -2);

            return 1;
            break;
        }
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "BoundingSphere");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'BoundingSphere' for parameter 1.");
                    lua_error(state);
                }
                BoundingSphere* param1 = (BoundingSphere*)((ScriptController::LuaObject*)userdata1)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new BoundingSphere(*param1);
                object->owns = true;
                luaL_getmetatable(state, "BoundingSphere");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Vector3");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 1.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 2);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new BoundingSphere(*param1, param2);
                object->owns = true;
                luaL_getmetatable(state, "BoundingSphere");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0, 1 or 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_BoundingSphere_center(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    BoundingSphere* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
        if (!userdata2)
        {
            lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
            lua_error(state);
        }
        Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

        instance->center = *param2;
        return 0;
    }
    else
    {
        ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
        object->instance = (void*)new Vector3(instance->center);
        object->owns = true;
        luaL_getmetatable(state, "Vector3");
        lua_setmetatable(state, -2);

        return 1;
    }
}

int lua_BoundingSphere_intersects(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "BoundingSphere");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'BoundingSphere' for parameter 2.");
                    lua_error(state);
                }
                BoundingSphere* param1 = (BoundingSphere*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                bool result = instance->intersects(*param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "BoundingBox");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'BoundingBox' for parameter 2.");
                    lua_error(state);
                }
                BoundingBox* param1 = (BoundingBox*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                bool result = instance->intersects(*param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Frustum");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Frustum' for parameter 2.");
                    lua_error(state);
                }
                Frustum* param1 = (Frustum*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                bool result = instance->intersects(*param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Plane");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Plane' for parameter 2.");
                    lua_error(state);
                }
                Plane* param1 = (Plane*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                float result = instance->intersects(*param1);

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Ray");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Ray' for parameter 2.");
                    lua_error(state);
                }
                Ray* param1 = (Ray*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                float result = instance->intersects(*param1);

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_BoundingSphere_isEmpty(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                BoundingSphere* instance = getInstance(state);
                bool result = instance->isEmpty();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_BoundingSphere_merge(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "BoundingSphere");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'BoundingSphere' for parameter 2.");
                    lua_error(state);
                }
                BoundingSphere* param1 = (BoundingSphere*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                instance->merge(*param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "BoundingBox");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'BoundingBox' for parameter 2.");
                    lua_error(state);
                }
                BoundingBox* param1 = (BoundingBox*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                instance->merge(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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

int lua_BoundingSphere_radius(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    BoundingSphere* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->radius = param2;
        return 0;
    }
    else
    {
        float result = instance->radius;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_BoundingSphere_set(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "BoundingSphere");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'BoundingSphere' for parameter 2.");
                    lua_error(state);
                }
                BoundingSphere* param1 = (BoundingSphere*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                instance->set(*param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "BoundingBox");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'BoundingBox' for parameter 2.");
                    lua_error(state);
                }
                BoundingBox* param1 = (BoundingBox*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                instance->set(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                BoundingSphere* instance = getInstance(state);
                instance->set(*param1, param2);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_BoundingSphere_static_empty(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)&(BoundingSphere::empty());
            object->owns = false;
            luaL_getmetatable(state, "BoundingSphere");
            lua_setmetatable(state, -2);

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

int lua_BoundingSphere_transform(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Matrix");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 2.");
                    lua_error(state);
                }
                Matrix* param1 = (Matrix*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingSphere* instance = getInstance(state);
                instance->transform(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
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
