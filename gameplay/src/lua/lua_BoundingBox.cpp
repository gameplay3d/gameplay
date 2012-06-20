#include "Base.h"
#include "ScriptController.h"
#include "BoundingBox.h"
#include "lua_BoundingBox.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_BoundingBox()
{
    ScriptController* sc = ScriptController::getInstance();

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

    sc->registerClass("BoundingBox", lua_members, lua_BoundingBox__init, lua_BoundingBox__gc, lua_statics, scopePath);
}

static BoundingBox* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "BoundingBox");
    luaL_argcheck(state, userdata != NULL, 1, "'BoundingBox' expected.");
    return (BoundingBox*)((ScriptController::LuaObject*)userdata)->instance;
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
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                void* userdata = luaL_checkudata(state, 1, "BoundingBox");
                luaL_argcheck(state, userdata != NULL, 1, "'BoundingBox' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    BoundingBox* instance = (BoundingBox*)object->instance;
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

int lua_BoundingBox__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)new BoundingBox();
            object->owns = true;
            luaL_getmetatable(state, "BoundingBox");
            lua_setmetatable(state, -2);

            return 1;
            break;
        }
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "BoundingBox");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'BoundingBox' for parameter 1.");
                    lua_error(state);
                }
                BoundingBox* param1 = (BoundingBox*)((ScriptController::LuaObject*)userdata1)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new BoundingBox(*param1);
                object->owns = true;
                luaL_getmetatable(state, "BoundingBox");
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
                lua_type(state, 2) == LUA_TUSERDATA)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new BoundingBox(*param1, *param2);
                object->owns = true;
                luaL_getmetatable(state, "BoundingBox");
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

int lua_BoundingBox_getCenter(lua_State* state)
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
                BoundingBox* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Vector3(instance->getCenter());
                object->owns = true;
                luaL_getmetatable(state, "Vector3");
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
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingBox* instance = getInstance(state);
                instance->getCenter(param1);
                
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
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingBox* instance = getInstance(state);
                instance->getCorners(param1);
                
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

int lua_BoundingBox_intersects(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "BoundingBox");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'BoundingBox' for parameter 2.");
                    lua_error(state);
                }
                BoundingBox* param1 = (BoundingBox*)((ScriptController::LuaObject*)userdata2)->instance;

                BoundingBox* instance = getInstance(state);
                bool result = instance->intersects(*param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
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

                BoundingBox* instance = getInstance(state);
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

                BoundingBox* instance = getInstance(state);
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

                BoundingBox* instance = getInstance(state);
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

                BoundingBox* instance = getInstance(state);
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

int lua_BoundingBox_isEmpty(lua_State* state)
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
                BoundingBox* instance = getInstance(state);
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
        void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
        if (!userdata2)
        {
            lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
            lua_error(state);
        }
        Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

        instance->max = *param2;
        return 0;
    }
    else
    {
        ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
        object->instance = (void*)new Vector3(instance->max);
        object->owns = true;
        luaL_getmetatable(state, "Vector3");
        lua_setmetatable(state, -2);

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

                BoundingBox* instance = getInstance(state);
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

                BoundingBox* instance = getInstance(state);
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
        void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
        if (!userdata2)
        {
            lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
            lua_error(state);
        }
        Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

        instance->min = *param2;
        return 0;
    }
    else
    {
        ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
        object->instance = (void*)new Vector3(instance->min);
        object->owns = true;
        luaL_getmetatable(state, "Vector3");
        lua_setmetatable(state, -2);

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
            if (lua_type(state, 1) == LUA_TUSERDATA &&
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

                BoundingBox* instance = getInstance(state);
                instance->set(*param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
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

                BoundingBox* instance = getInstance(state);
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
                lua_type(state, 3) == LUA_TUSERDATA)
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
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                BoundingBox* instance = getInstance(state);
                instance->set(*param1, *param2);
                
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

int lua_BoundingBox_static_empty(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)&(BoundingBox::empty());
            object->owns = false;
            luaL_getmetatable(state, "BoundingBox");
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

int lua_BoundingBox_transform(lua_State* state)
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

                BoundingBox* instance = getInstance(state);
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
