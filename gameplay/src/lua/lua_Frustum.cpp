#include "Base.h"
#include "ScriptController.h"
#include "Frustum.h"
#include "lua_Frustum.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_Frustum()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"getBottom", lua_Frustum_getBottom},
        {"getCorners", lua_Frustum_getCorners},
        {"getFar", lua_Frustum_getFar},
        {"getLeft", lua_Frustum_getLeft},
        {"getMatrix", lua_Frustum_getMatrix},
        {"getNear", lua_Frustum_getNear},
        {"getRight", lua_Frustum_getRight},
        {"getTop", lua_Frustum_getTop},
        {"intersects", lua_Frustum_intersects},
        {"set", lua_Frustum_set},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    sc->registerClass("Frustum", lua_members, lua_Frustum__init, lua_Frustum__gc, lua_statics, scopePath);
}

static Frustum* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Frustum");
    luaL_argcheck(state, userdata != NULL, 1, "'Frustum' expected.");
    return (Frustum*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_Frustum__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Frustum");
                luaL_argcheck(state, userdata != NULL, 1, "'Frustum' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    Frustum* instance = (Frustum*)object->instance;
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

int lua_Frustum__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)new Frustum();
            object->owns = true;
            luaL_getmetatable(state, "Frustum");
            lua_setmetatable(state, -2);

            return 1;
            break;
        }
        case 1:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Matrix");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 1.");
                    lua_error(state);
                }
                Matrix* param1 = (Matrix*)((ScriptController::LuaObject*)userdata1)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Frustum(*param1);
                object->owns = true;
                luaL_getmetatable(state, "Frustum");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Frustum");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Frustum' for parameter 1.");
                    lua_error(state);
                }
                Frustum* param1 = (Frustum*)((ScriptController::LuaObject*)userdata1)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new Frustum(*param1);
                object->owns = true;
                luaL_getmetatable(state, "Frustum");
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
            lua_pushstring(state, "Invalid number of parameters (expected 0 or 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Frustum_getBottom(lua_State* state)
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
                Frustum* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getBottom());
                object->owns = false;
                luaL_getmetatable(state, "Plane");
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
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Frustum_getCorners(lua_State* state)
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

                Frustum* instance = getInstance(state);
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

int lua_Frustum_getFar(lua_State* state)
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
                Frustum* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getFar());
                object->owns = false;
                luaL_getmetatable(state, "Plane");
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
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Frustum_getLeft(lua_State* state)
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
                Frustum* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getLeft());
                object->owns = false;
                luaL_getmetatable(state, "Plane");
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
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Frustum_getMatrix(lua_State* state)
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

                Frustum* instance = getInstance(state);
                instance->getMatrix(param1);
                
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

int lua_Frustum_getNear(lua_State* state)
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
                Frustum* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getNear());
                object->owns = false;
                luaL_getmetatable(state, "Plane");
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
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Frustum_getRight(lua_State* state)
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
                Frustum* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getRight());
                object->owns = false;
                luaL_getmetatable(state, "Plane");
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
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Frustum_getTop(lua_State* state)
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
                Frustum* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getTop());
                object->owns = false;
                luaL_getmetatable(state, "Plane");
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
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Frustum_intersects(lua_State* state)
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

                Frustum* instance = getInstance(state);
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

                Frustum* instance = getInstance(state);
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

                Frustum* instance = getInstance(state);
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

                Frustum* instance = getInstance(state);
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

                Frustum* instance = getInstance(state);
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

int lua_Frustum_set(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Frustum");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Frustum' for parameter 2.");
                    lua_error(state);
                }
                Frustum* param1 = (Frustum*)((ScriptController::LuaObject*)userdata2)->instance;

                Frustum* instance = getInstance(state);
                instance->set(*param1);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
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

                Frustum* instance = getInstance(state);
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
