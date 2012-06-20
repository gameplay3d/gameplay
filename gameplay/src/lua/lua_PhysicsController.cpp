#include "Base.h"
#include "ScriptController.h"
#include "PhysicsController.h"
#include "lua_PhysicsController.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_PhysicsController()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"addStatusListener", lua_PhysicsController_addStatusListener},
        {"createFixedConstraint", lua_PhysicsController_createFixedConstraint},
        {"createGenericConstraint", lua_PhysicsController_createGenericConstraint},
        {"createHingeConstraint", lua_PhysicsController_createHingeConstraint},
        {"createSocketConstraint", lua_PhysicsController_createSocketConstraint},
        {"createSpringConstraint", lua_PhysicsController_createSpringConstraint},
        {"drawDebug", lua_PhysicsController_drawDebug},
        {"getGravity", lua_PhysicsController_getGravity},
        {"rayTest", lua_PhysicsController_rayTest},
        {"removeStatusListener", lua_PhysicsController_removeStatusListener},
        {"setGravity", lua_PhysicsController_setGravity},
        {"sweepTest", lua_PhysicsController_sweepTest},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;

    sc->registerClass("PhysicsController", lua_members, NULL, NULL, lua_statics, scopePath);
}

static PhysicsController* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "PhysicsController");
    luaL_argcheck(state, userdata != NULL, 1, "'PhysicsController' expected.");
    return (PhysicsController*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_PhysicsController_addStatusListener(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsControllerListener");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsController::Listener' for parameter 2.");
                    lua_error(state);
                }
                PhysicsController::Listener* param1 = (PhysicsController::Listener*)((ScriptController::LuaObject*)userdata2)->instance;

                PhysicsController* instance = getInstance(state);
                instance->addStatusListener(param1);
                
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

int lua_PhysicsController_createFixedConstraint(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createFixedConstraint(param1);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsFixedConstraint");
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
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "PhysicsRigidBody");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 3.");
                    lua_error(state);
                }
                PhysicsRigidBody* param2 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata3)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createFixedConstraint(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsFixedConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_createGenericConstraint(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createGenericConstraint(param1);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "PhysicsRigidBody");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 3.");
                    lua_error(state);
                }
                PhysicsRigidBody* param2 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata3)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createGenericConstraint(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createGenericConstraint(param1, *param2, *param3);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 5:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "PhysicsRigidBody");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 5.");
                    lua_error(state);
                }
                PhysicsRigidBody* param4 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata5)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createGenericConstraint(param1, *param2, *param3, param4);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 6:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "PhysicsRigidBody");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 5.");
                    lua_error(state);
                }
                PhysicsRigidBody* param4 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Quaternion");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 6.");
                    lua_error(state);
                }
                Quaternion* param5 = (Quaternion*)((ScriptController::LuaObject*)userdata6)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createGenericConstraint(param1, *param2, *param3, param4, *param5);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsGenericConstraint");
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
        case 7:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TUSERDATA &&
                lua_type(state, 7) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "PhysicsRigidBody");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 5.");
                    lua_error(state);
                }
                PhysicsRigidBody* param4 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Quaternion");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 6.");
                    lua_error(state);
                }
                Quaternion* param5 = (Quaternion*)((ScriptController::LuaObject*)userdata6)->instance;

                // Get parameter 6 off the stack.
                void* userdata7 = ScriptController::getInstance()->getObjectPointer(7, "Vector3");
                if (!userdata7)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 7.");
                    lua_error(state);
                }
                Vector3* param6 = (Vector3*)((ScriptController::LuaObject*)userdata7)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createGenericConstraint(param1, *param2, *param3, param4, *param5, *param6);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsGenericConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3, 4, 5, 6 or 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_createHingeConstraint(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createHingeConstraint(param1, *param2, *param3);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsHingeConstraint");
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
        case 5:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "PhysicsRigidBody");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 5.");
                    lua_error(state);
                }
                PhysicsRigidBody* param4 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata5)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createHingeConstraint(param1, *param2, *param3, param4);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsHingeConstraint");
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
        case 6:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "PhysicsRigidBody");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 5.");
                    lua_error(state);
                }
                PhysicsRigidBody* param4 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Quaternion");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 6.");
                    lua_error(state);
                }
                Quaternion* param5 = (Quaternion*)((ScriptController::LuaObject*)userdata6)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createHingeConstraint(param1, *param2, *param3, param4, *param5);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsHingeConstraint");
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
        case 7:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TUSERDATA &&
                lua_type(state, 7) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "PhysicsRigidBody");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 5.");
                    lua_error(state);
                }
                PhysicsRigidBody* param4 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Quaternion");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 6.");
                    lua_error(state);
                }
                Quaternion* param5 = (Quaternion*)((ScriptController::LuaObject*)userdata6)->instance;

                // Get parameter 6 off the stack.
                void* userdata7 = ScriptController::getInstance()->getObjectPointer(7, "Vector3");
                if (!userdata7)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 7.");
                    lua_error(state);
                }
                Vector3* param6 = (Vector3*)((ScriptController::LuaObject*)userdata7)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createHingeConstraint(param1, *param2, *param3, param4, *param5, *param6);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsHingeConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 4, 5, 6 or 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_createSocketConstraint(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createSocketConstraint(param1);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsSocketConstraint");
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
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "PhysicsRigidBody");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 3.");
                    lua_error(state);
                }
                PhysicsRigidBody* param2 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata3)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createSocketConstraint(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsSocketConstraint");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createSocketConstraint(param1, *param2);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsSocketConstraint");
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
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "PhysicsRigidBody");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 4.");
                    lua_error(state);
                }
                PhysicsRigidBody* param3 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata4)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createSocketConstraint(param1, *param2, param3);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsSocketConstraint");
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
        case 5:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "PhysicsRigidBody");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 4.");
                    lua_error(state);
                }
                PhysicsRigidBody* param3 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector3");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 5.");
                    lua_error(state);
                }
                Vector3* param4 = (Vector3*)((ScriptController::LuaObject*)userdata5)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createSocketConstraint(param1, *param2, param3, *param4);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsSocketConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_createSpringConstraint(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "PhysicsRigidBody");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 3.");
                    lua_error(state);
                }
                PhysicsRigidBody* param2 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata3)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createSpringConstraint(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsSpringConstraint");
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
        case 7:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TUSERDATA &&
                lua_type(state, 7) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsRigidBody");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 2.");
                    lua_error(state);
                }
                PhysicsRigidBody* param1 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Quaternion");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 3.");
                    lua_error(state);
                }
                Quaternion* param2 = (Quaternion*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "PhysicsRigidBody");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsRigidBody' for parameter 5.");
                    lua_error(state);
                }
                PhysicsRigidBody* param4 = (PhysicsRigidBody*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Quaternion");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Quaternion' for parameter 6.");
                    lua_error(state);
                }
                Quaternion* param5 = (Quaternion*)((ScriptController::LuaObject*)userdata6)->instance;

                // Get parameter 6 off the stack.
                void* userdata7 = ScriptController::getInstance()->getObjectPointer(7, "Vector3");
                if (!userdata7)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 7.");
                    lua_error(state);
                }
                Vector3* param6 = (Vector3*)((ScriptController::LuaObject*)userdata7)->instance;

                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createSpringConstraint(param1, *param2, *param3, param4, *param5, *param6);
                object->owns = false;
                luaL_getmetatable(state, "PhysicsSpringConstraint");
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
            lua_pushstring(state, "Invalid number of parameters (expected 3 or 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_drawDebug(lua_State* state)
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

                PhysicsController* instance = getInstance(state);
                instance->drawDebug(*param1);
                
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

int lua_PhysicsController_getGravity(lua_State* state)
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
                PhysicsController* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)&(instance->getGravity());
                object->owns = false;
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
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_rayTest(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Ray");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Ray' for parameter 2.");
                    lua_error(state);
                }
                Ray* param1 = (Ray*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                PhysicsController* instance = getInstance(state);
                bool result = instance->rayTest(*param1, param2);

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
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Ray");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Ray' for parameter 2.");
                    lua_error(state);
                }
                Ray* param1 = (Ray*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "PhysicsControllerHitResult");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsController::HitResult' for parameter 4.");
                    lua_error(state);
                }
                PhysicsController::HitResult* param3 = (PhysicsController::HitResult*)((ScriptController::LuaObject*)userdata4)->instance;

                PhysicsController* instance = getInstance(state);
                bool result = instance->rayTest(*param1, param2, param3);

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
        case 5:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Ray");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Ray' for parameter 2.");
                    lua_error(state);
                }
                Ray* param1 = (Ray*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "PhysicsControllerHitResult");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsController::HitResult' for parameter 4.");
                    lua_error(state);
                }
                PhysicsController::HitResult* param3 = (PhysicsController::HitResult*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "PhysicsControllerHitFilter");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsController::HitFilter' for parameter 5.");
                    lua_error(state);
                }
                PhysicsController::HitFilter* param4 = (PhysicsController::HitFilter*)((ScriptController::LuaObject*)userdata5)->instance;

                PhysicsController* instance = getInstance(state);
                bool result = instance->rayTest(*param1, param2, param3, param4);

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
            lua_pushstring(state, "Invalid number of parameters (expected 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_PhysicsController_removeStatusListener(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsControllerListener");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsController::Listener' for parameter 2.");
                    lua_error(state);
                }
                PhysicsController::Listener* param1 = (PhysicsController::Listener*)((ScriptController::LuaObject*)userdata2)->instance;

                PhysicsController* instance = getInstance(state);
                instance->removeStatusListener(param1);
                
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

int lua_PhysicsController_setGravity(lua_State* state)
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

                PhysicsController* instance = getInstance(state);
                instance->setGravity(*param1);
                
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

int lua_PhysicsController_sweepTest(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsCollisionObject");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsCollisionObject' for parameter 2.");
                    lua_error(state);
                }
                PhysicsCollisionObject* param1 = (PhysicsCollisionObject*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                PhysicsController* instance = getInstance(state);
                bool result = instance->sweepTest(param1, *param2);

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
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsCollisionObject");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsCollisionObject' for parameter 2.");
                    lua_error(state);
                }
                PhysicsCollisionObject* param1 = (PhysicsCollisionObject*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "PhysicsControllerHitResult");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsController::HitResult' for parameter 4.");
                    lua_error(state);
                }
                PhysicsController::HitResult* param3 = (PhysicsController::HitResult*)((ScriptController::LuaObject*)userdata4)->instance;

                PhysicsController* instance = getInstance(state);
                bool result = instance->sweepTest(param1, *param2, param3);

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
        case 5:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "PhysicsCollisionObject");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsCollisionObject' for parameter 2.");
                    lua_error(state);
                }
                PhysicsCollisionObject* param1 = (PhysicsCollisionObject*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "PhysicsControllerHitResult");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsController::HitResult' for parameter 4.");
                    lua_error(state);
                }
                PhysicsController::HitResult* param3 = (PhysicsController::HitResult*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "PhysicsControllerHitFilter");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'PhysicsController::HitFilter' for parameter 5.");
                    lua_error(state);
                }
                PhysicsController::HitFilter* param4 = (PhysicsController::HitFilter*)((ScriptController::LuaObject*)userdata5)->instance;

                PhysicsController* instance = getInstance(state);
                bool result = instance->sweepTest(param1, *param2, param3, param4);

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
            lua_pushstring(state, "Invalid number of parameters (expected 3, 4 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
