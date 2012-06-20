#include "Base.h"
#include "ScriptController.h"
#include "PhysicsController.h"
#include "lua_PhysicsControllerHitFilter.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_PhysicsControllerHitFilter()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"filter", lua_PhysicsControllerHitFilter_filter},
        {"hit", lua_PhysicsControllerHitFilter_hit},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("PhysicsController");

    sc->registerClass("PhysicsControllerHitFilter", lua_members, lua_PhysicsControllerHitFilter__init, lua_PhysicsControllerHitFilter__gc, lua_statics, scopePath);
}

static PhysicsController::HitFilter* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsControllerHitFilter");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsControllerHitFilter' expected.");
    return (PhysicsController::HitFilter*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_PhysicsControllerHitFilter__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "PhysicsControllerHitFilter");
                luaL_argcheck(state, userdata != NULL, 1, "'PhysicsControllerHitFilter' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    PhysicsController::HitFilter* instance = (PhysicsController::HitFilter*)object->instance;
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

int lua_PhysicsControllerHitFilter__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)new PhysicsController::HitFilter();
            object->owns = true;
            luaL_getmetatable(state, "PhysicsControllerHitFilter");
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

int lua_PhysicsControllerHitFilter_filter(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsCollisionObject");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsCollisionObject' for parameter 2.");
                    lua_error(state);
                }
                PhysicsCollisionObject* param1 = (PhysicsCollisionObject*)((ScriptController::LuaObject*)userdata2)->instance;

                PhysicsController::HitFilter* instance = getInstance(state);
                bool result = instance->filter(param1);

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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsControllerHitFilter_hit(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsControllerHitResult");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsController::HitResult' for parameter 2.");
                    lua_error(state);
                }
                PhysicsController::HitResult* param1 = (PhysicsController::HitResult*)((ScriptController::LuaObject*)userdata2)->instance;

                PhysicsController::HitFilter* instance = getInstance(state);
                bool result = instance->hit(*param1);

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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
