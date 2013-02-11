#include "Base.h"
#include "ScriptController.h"
#include "lua_PhysicsControllerHitResult.h"
#include "Base.h"
#include "Bundle.h"
#include "Game.h"
#include "MeshPart.h"
#include "PhysicsCharacter.h"
#include "PhysicsController.h"
#include "PhysicsRigidBody.h"
#include "ScriptController.h"
#include "ScriptTarget.h"
#include "Terrain.h"
#include "lua_PhysicsControllerListenerEventType.h"

namespace gameplay
{

void luaRegister_PhysicsControllerHitResult()
{
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

    gameplay::ScriptUtil::registerClass("PhysicsControllerHitResult", lua_members, lua_PhysicsControllerHitResult__init, lua_PhysicsControllerHitResult__gc, lua_statics, scopePath);
}

static PhysicsController::HitResult* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsControllerHitResult");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsControllerHitResult' expected.");
    return (PhysicsController::HitResult*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                void* userdata = luaL_checkudata(state, 1, "PhysicsControllerHitResult");
                luaL_argcheck(state, userdata != NULL, 1, "'PhysicsControllerHitResult' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    PhysicsController::HitResult* instance = (PhysicsController::HitResult*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_PhysicsControllerHitResult__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_PhysicsControllerHitResult__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)new PhysicsController::HitResult();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "PhysicsControllerHitResult");
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
        bool param2Valid;
        gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
        if (!param2Valid)
        {
            lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector3'.");
            lua_error(state);
        }

        instance->normal = *param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)new Vector3(instance->normal);
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
        bool param2Valid;
        gameplay::ScriptUtil::LuaArray<PhysicsCollisionObject> param2 = gameplay::ScriptUtil::getObjectPointer<PhysicsCollisionObject>(2, "PhysicsCollisionObject", false, &param2Valid);
        if (!param2Valid)
        {
            lua_pushstring(state, "Failed to convert parameter 2 to type 'PhysicsCollisionObject'.");
            lua_error(state);
        }

        instance->object = param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)instance->object;
        if (returnPtr)
        {
            gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
            object->instance = returnPtr;
            object->owns = false;
            luaL_getmetatable(state, "PhysicsCollisionObject");
            lua_setmetatable(state, -2);
        }
        else
        {
            lua_pushnil(state);
        }

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
        bool param2Valid;
        gameplay::ScriptUtil::LuaArray<Vector3> param2 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param2Valid);
        if (!param2Valid)
        {
            lua_pushstring(state, "Failed to convert parameter 2 to type 'Vector3'.");
            lua_error(state);
        }

        instance->point = *param2;
        return 0;
    }
    else
    {
        void* returnPtr = (void*)new Vector3(instance->point);
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

}
