#include "Base.h"
#include "ScriptController.h"
#include "PhysicsController.h"
#include "lua_PhysicsControllerHitResult.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_PhysicsControllerHitResult()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"fraction", lua_PhysicsControllerHitResult_fraction},
        {"normal", lua_PhysicsControllerHitResult_normal},
        {"object", lua_PhysicsControllerHitResult_object},
        {"point", lua_PhysicsControllerHitResult_point},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("PhysicsController");

    sc->registerClass("PhysicsControllerHitResult", lua_members, lua_PhysicsControllerHitResult__init, lua_PhysicsControllerHitResult__gc, lua_statics, scopePath);
}

static PhysicsController::HitResult* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsControllerHitResult");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsControllerHitResult' expected.");
    return (PhysicsController::HitResult*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_PhysicsControllerHitResult__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "PhysicsControllerHitResult");
                luaL_argcheck(state, userdata != NULL, 1, "'PhysicsControllerHitResult' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    PhysicsController::HitResult* instance = (PhysicsController::HitResult*)object->instance;
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

int lua_PhysicsControllerHitResult__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)new PhysicsController::HitResult();
            object->owns = true;
            luaL_getmetatable(state, "PhysicsControllerHitResult");
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

int lua_PhysicsControllerHitResult_fraction(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsController::HitResult* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->fraction = param2;
        return 0;
    }
    else
    {
        float result = instance->fraction;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_PhysicsControllerHitResult_normal(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsController::HitResult* instance = getInstance(state);
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

        instance->normal = *param2;
        return 0;
    }
    else
    {
        ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
        object->instance = (void*)new Vector3(instance->normal);
        object->owns = true;
        luaL_getmetatable(state, "Vector3");
        lua_setmetatable(state, -2);

        return 1;
    }
}

int lua_PhysicsControllerHitResult_object(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsController::HitResult* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsCollisionObject");
        if (!userdata2)
        {
            lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsCollisionObject' for parameter 2.");
            lua_error(state);
        }
        PhysicsCollisionObject* param2 = (PhysicsCollisionObject*)((ScriptController::LuaObject*)userdata2)->instance;

        instance->object = param2;
        return 0;
    }
    else
    {
        ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
        object->instance = (void*)instance->object;
        object->owns = false;
        luaL_getmetatable(state, "PhysicsCollisionObject");
        lua_setmetatable(state, -2);

        return 1;
    }
}

int lua_PhysicsControllerHitResult_point(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    PhysicsController::HitResult* instance = getInstance(state);
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

        instance->point = *param2;
        return 0;
    }
    else
    {
        ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
        object->instance = (void*)new Vector3(instance->point);
        object->owns = true;
        luaL_getmetatable(state, "Vector3");
        lua_setmetatable(state, -2);

        return 1;
    }
}

}
