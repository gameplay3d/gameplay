#include "Base.h"
#include "ScriptController.h"
#include "SpriteBatch.h"
#include "lua_SpriteBatch.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_SpriteBatch()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"begin", lua_SpriteBatch_begin},
        {"draw", lua_SpriteBatch_draw},
        {"end", lua_SpriteBatch_end},
        {"getMaterial", lua_SpriteBatch_getMaterial},
        {"getStateBlock", lua_SpriteBatch_getStateBlock},
        {"setProjectionMatrix", lua_SpriteBatch_setProjectionMatrix},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_SpriteBatch_static_create},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    sc->registerClass("SpriteBatch", lua_members, NULL, lua_SpriteBatch__gc, lua_statics, scopePath);
}

static SpriteBatch* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "SpriteBatch");
    luaL_argcheck(state, userdata != NULL, 1, "'SpriteBatch' expected.");
    return (SpriteBatch*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_SpriteBatch__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "SpriteBatch");
                luaL_argcheck(state, userdata != NULL, 1, "'SpriteBatch' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    SpriteBatch* instance = (SpriteBatch*)object->instance;
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

int lua_SpriteBatch_begin(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
                instance->begin();
                
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

int lua_SpriteBatch_draw(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Rectangle");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 2.");
                    lua_error(state);
                }
                Rectangle* param1 = (Rectangle*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2);
                
                return 0;
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Rectangle");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 2.");
                    lua_error(state);
                }
                Rectangle* param1 = (Rectangle*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector4");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 4.");
                    lua_error(state);
                }
                Vector4* param3 = (Vector4*)((ScriptController::LuaObject*)userdata4)->instance;

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA)
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
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector2");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 4.");
                    lua_error(state);
                }
                Vector2* param3 = (Vector2*)((ScriptController::LuaObject*)userdata4)->instance;

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3);
                
                return 0;
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Vector3");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 2.");
                    lua_error(state);
                }
                Vector3* param1 = (Vector3*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector2");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 4.");
                    lua_error(state);
                }
                Vector2* param3 = (Vector2*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector4");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 5.");
                    lua_error(state);
                }
                Vector4* param4 = (Vector4*)((ScriptController::LuaObject*)userdata5)->instance;

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3, *param4);
                
                return 0;
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
                lua_type(state, 7) == LUA_TNUMBER)
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
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector2");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 4.");
                    lua_error(state);
                }
                Vector2* param3 = (Vector2*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector4");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 5.");
                    lua_error(state);
                }
                Vector4* param4 = (Vector4*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Vector2");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 6.");
                    lua_error(state);
                }
                Vector2* param5 = (Vector2*)((ScriptController::LuaObject*)userdata6)->instance;

                // Get parameter 6 off the stack.
                float param6 = (float)luaL_checknumber(state, 7);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3, *param4, *param5, param6);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 10:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TNUMBER &&
                lua_type(state, 8) == LUA_TNUMBER &&
                lua_type(state, 9) == LUA_TNUMBER &&
                lua_type(state, 10) == LUA_TUSERDATA)
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

                // Get parameter 7 off the stack.
                float param7 = (float)luaL_checknumber(state, 8);

                // Get parameter 8 off the stack.
                float param8 = (float)luaL_checknumber(state, 9);

                // Get parameter 9 off the stack.
                void* userdata10 = ScriptController::getInstance()->getObjectPointer(10, "Vector4");
                if (!userdata10)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 10.");
                    lua_error(state);
                }
                Vector4* param9 = (Vector4*)((ScriptController::LuaObject*)userdata10)->instance;

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, *param9);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 11:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TNUMBER &&
                lua_type(state, 8) == LUA_TNUMBER &&
                lua_type(state, 9) == LUA_TUSERDATA &&
                lua_type(state, 10) == LUA_TUSERDATA &&
                lua_type(state, 11) == LUA_TNUMBER)
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

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                // Get parameter 5 off the stack.
                float param5 = (float)luaL_checknumber(state, 6);

                // Get parameter 6 off the stack.
                float param6 = (float)luaL_checknumber(state, 7);

                // Get parameter 7 off the stack.
                float param7 = (float)luaL_checknumber(state, 8);

                // Get parameter 8 off the stack.
                void* userdata9 = ScriptController::getInstance()->getObjectPointer(9, "Vector4");
                if (!userdata9)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 9.");
                    lua_error(state);
                }
                Vector4* param8 = (Vector4*)((ScriptController::LuaObject*)userdata9)->instance;

                // Get parameter 9 off the stack.
                void* userdata10 = ScriptController::getInstance()->getObjectPointer(10, "Vector2");
                if (!userdata10)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 10.");
                    lua_error(state);
                }
                Vector2* param9 = (Vector2*)((ScriptController::LuaObject*)userdata10)->instance;

                // Get parameter 10 off the stack.
                float param10 = (float)luaL_checknumber(state, 11);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, param2, param3, param4, param5, param6, param7, *param8, *param9, param10);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TNUMBER &&
                lua_type(state, 8) == LUA_TNUMBER &&
                lua_type(state, 9) == LUA_TNUMBER &&
                lua_type(state, 10) == LUA_TUSERDATA &&
                lua_type(state, 11) == LUA_TUSERDATA)
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

                // Get parameter 7 off the stack.
                float param7 = (float)luaL_checknumber(state, 8);

                // Get parameter 8 off the stack.
                float param8 = (float)luaL_checknumber(state, 9);

                // Get parameter 9 off the stack.
                void* userdata10 = ScriptController::getInstance()->getObjectPointer(10, "Vector4");
                if (!userdata10)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 10.");
                    lua_error(state);
                }
                Vector4* param9 = (Vector4*)((ScriptController::LuaObject*)userdata10)->instance;

                // Get parameter 10 off the stack.
                void* userdata11 = ScriptController::getInstance()->getObjectPointer(11, "Rectangle");
                if (!userdata11)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 11.");
                    lua_error(state);
                }
                Rectangle* param10 = (Rectangle*)((ScriptController::LuaObject*)userdata11)->instance;

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, *param9, *param10);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TNUMBER &&
                lua_type(state, 8) == LUA_TNUMBER &&
                lua_type(state, 9) == LUA_TNUMBER &&
                lua_type(state, 10) == LUA_TNUMBER &&
                lua_type(state, 11) == LUA_TUSERDATA)
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

                // Get parameter 7 off the stack.
                float param7 = (float)luaL_checknumber(state, 8);

                // Get parameter 8 off the stack.
                float param8 = (float)luaL_checknumber(state, 9);

                // Get parameter 9 off the stack.
                float param9 = (float)luaL_checknumber(state, 10);

                // Get parameter 10 off the stack.
                void* userdata11 = ScriptController::getInstance()->getObjectPointer(11, "Vector4");
                if (!userdata11)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 11.");
                    lua_error(state);
                }
                Vector4* param10 = (Vector4*)((ScriptController::LuaObject*)userdata11)->instance;

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 12:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TNUMBER &&
                lua_type(state, 8) == LUA_TNUMBER &&
                lua_type(state, 9) == LUA_TUSERDATA &&
                lua_type(state, 10) == LUA_TUSERDATA &&
                lua_type(state, 11) == LUA_TNUMBER &&
                lua_type(state, 12) == LUA_TBOOLEAN)
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

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                // Get parameter 5 off the stack.
                float param5 = (float)luaL_checknumber(state, 6);

                // Get parameter 6 off the stack.
                float param6 = (float)luaL_checknumber(state, 7);

                // Get parameter 7 off the stack.
                float param7 = (float)luaL_checknumber(state, 8);

                // Get parameter 8 off the stack.
                void* userdata9 = ScriptController::getInstance()->getObjectPointer(9, "Vector4");
                if (!userdata9)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 9.");
                    lua_error(state);
                }
                Vector4* param8 = (Vector4*)((ScriptController::LuaObject*)userdata9)->instance;

                // Get parameter 9 off the stack.
                void* userdata10 = ScriptController::getInstance()->getObjectPointer(10, "Vector2");
                if (!userdata10)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 10.");
                    lua_error(state);
                }
                Vector2* param9 = (Vector2*)((ScriptController::LuaObject*)userdata10)->instance;

                // Get parameter 10 off the stack.
                float param10 = (float)luaL_checknumber(state, 11);

                // Get parameter 11 off the stack.
                bool param11 = (luaL_checkint(state, 12) != 0);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, param2, param3, param4, param5, param6, param7, *param8, *param9, param10, param11);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TNUMBER &&
                lua_type(state, 8) == LUA_TNUMBER &&
                lua_type(state, 9) == LUA_TNUMBER &&
                lua_type(state, 10) == LUA_TNUMBER &&
                lua_type(state, 11) == LUA_TUSERDATA &&
                lua_type(state, 12) == LUA_TBOOLEAN)
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

                // Get parameter 7 off the stack.
                float param7 = (float)luaL_checknumber(state, 8);

                // Get parameter 8 off the stack.
                float param8 = (float)luaL_checknumber(state, 9);

                // Get parameter 9 off the stack.
                float param9 = (float)luaL_checknumber(state, 10);

                // Get parameter 10 off the stack.
                void* userdata11 = ScriptController::getInstance()->getObjectPointer(11, "Vector4");
                if (!userdata11)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 11.");
                    lua_error(state);
                }
                Vector4* param10 = (Vector4*)((ScriptController::LuaObject*)userdata11)->instance;

                // Get parameter 11 off the stack.
                bool param11 = (luaL_checkint(state, 12) != 0);

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10, param11);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 13:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TNUMBER &&
                lua_type(state, 8) == LUA_TNUMBER &&
                lua_type(state, 9) == LUA_TNUMBER &&
                lua_type(state, 10) == LUA_TNUMBER &&
                lua_type(state, 11) == LUA_TUSERDATA &&
                lua_type(state, 12) == LUA_TUSERDATA &&
                lua_type(state, 13) == LUA_TNUMBER)
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

                // Get parameter 7 off the stack.
                float param7 = (float)luaL_checknumber(state, 8);

                // Get parameter 8 off the stack.
                float param8 = (float)luaL_checknumber(state, 9);

                // Get parameter 9 off the stack.
                float param9 = (float)luaL_checknumber(state, 10);

                // Get parameter 10 off the stack.
                void* userdata11 = ScriptController::getInstance()->getObjectPointer(11, "Vector4");
                if (!userdata11)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 11.");
                    lua_error(state);
                }
                Vector4* param10 = (Vector4*)((ScriptController::LuaObject*)userdata11)->instance;

                // Get parameter 11 off the stack.
                void* userdata12 = ScriptController::getInstance()->getObjectPointer(12, "Vector2");
                if (!userdata12)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 12.");
                    lua_error(state);
                }
                Vector2* param11 = (Vector2*)((ScriptController::LuaObject*)userdata12)->instance;

                // Get parameter 12 off the stack.
                float param12 = (float)luaL_checknumber(state, 13);

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10, *param11, param12);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TNUMBER &&
                lua_type(state, 8) == LUA_TNUMBER &&
                lua_type(state, 9) == LUA_TNUMBER &&
                lua_type(state, 10) == LUA_TNUMBER &&
                lua_type(state, 11) == LUA_TUSERDATA &&
                lua_type(state, 12) == LUA_TUSERDATA &&
                lua_type(state, 13) == LUA_TNUMBER)
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

                // Get parameter 3 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "Vector3");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 4.");
                    lua_error(state);
                }
                Vector3* param3 = (Vector3*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 4 off the stack.
                float param4 = (float)luaL_checknumber(state, 5);

                // Get parameter 5 off the stack.
                float param5 = (float)luaL_checknumber(state, 6);

                // Get parameter 6 off the stack.
                float param6 = (float)luaL_checknumber(state, 7);

                // Get parameter 7 off the stack.
                float param7 = (float)luaL_checknumber(state, 8);

                // Get parameter 8 off the stack.
                float param8 = (float)luaL_checknumber(state, 9);

                // Get parameter 9 off the stack.
                float param9 = (float)luaL_checknumber(state, 10);

                // Get parameter 10 off the stack.
                void* userdata11 = ScriptController::getInstance()->getObjectPointer(11, "Vector4");
                if (!userdata11)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 11.");
                    lua_error(state);
                }
                Vector4* param10 = (Vector4*)((ScriptController::LuaObject*)userdata11)->instance;

                // Get parameter 11 off the stack.
                void* userdata12 = ScriptController::getInstance()->getObjectPointer(12, "Vector2");
                if (!userdata12)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 12.");
                    lua_error(state);
                }
                Vector2* param11 = (Vector2*)((ScriptController::LuaObject*)userdata12)->instance;

                // Get parameter 12 off the stack.
                float param12 = (float)luaL_checknumber(state, 13);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3, param4, param5, param6, param7, param8, param9, *param10, *param11, param12);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 14:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TNUMBER &&
                lua_type(state, 8) == LUA_TNUMBER &&
                lua_type(state, 9) == LUA_TNUMBER &&
                lua_type(state, 10) == LUA_TNUMBER &&
                lua_type(state, 11) == LUA_TUSERDATA &&
                lua_type(state, 12) == LUA_TUSERDATA &&
                lua_type(state, 13) == LUA_TNUMBER &&
                lua_type(state, 14) == LUA_TBOOLEAN)
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

                // Get parameter 7 off the stack.
                float param7 = (float)luaL_checknumber(state, 8);

                // Get parameter 8 off the stack.
                float param8 = (float)luaL_checknumber(state, 9);

                // Get parameter 9 off the stack.
                float param9 = (float)luaL_checknumber(state, 10);

                // Get parameter 10 off the stack.
                void* userdata11 = ScriptController::getInstance()->getObjectPointer(11, "Vector4");
                if (!userdata11)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 11.");
                    lua_error(state);
                }
                Vector4* param10 = (Vector4*)((ScriptController::LuaObject*)userdata11)->instance;

                // Get parameter 11 off the stack.
                void* userdata12 = ScriptController::getInstance()->getObjectPointer(12, "Vector2");
                if (!userdata12)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 12.");
                    lua_error(state);
                }
                Vector2* param11 = (Vector2*)((ScriptController::LuaObject*)userdata12)->instance;

                // Get parameter 12 off the stack.
                float param12 = (float)luaL_checknumber(state, 13);

                // Get parameter 13 off the stack.
                bool param13 = (luaL_checkint(state, 14) != 0);

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10, *param11, param12, param13);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 3, 4, 5, 7, 10, 11, 12, 13 or 14).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_SpriteBatch_end(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
                instance->end();
                
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

int lua_SpriteBatch_getMaterial(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->getMaterial();
                object->owns = false;
                luaL_getmetatable(state, "Material");
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

int lua_SpriteBatch_getStateBlock(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->getStateBlock();
                object->owns = false;
                luaL_getmetatable(state, "RenderStateStateBlock");
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

int lua_SpriteBatch_setProjectionMatrix(lua_State* state)
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

                SpriteBatch* instance = getInstance(state);
                instance->setProjectionMatrix(*param1);
                
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

int lua_SpriteBatch_static_create(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)SpriteBatch::create(param1);
                object->owns = false;
                luaL_getmetatable(state, "SpriteBatch");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Texture");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Texture' for parameter 1.");
                    lua_error(state);
                }
                Texture* param1 = (Texture*)((ScriptController::LuaObject*)userdata1)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)SpriteBatch::create(param1);
                object->owns = false;
                luaL_getmetatable(state, "SpriteBatch");
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
            if (lua_type(state, 1) == LUA_TSTRING &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Effect");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Effect' for parameter 2.");
                    lua_error(state);
                }
                Effect* param2 = (Effect*)((ScriptController::LuaObject*)userdata2)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)SpriteBatch::create(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "SpriteBatch");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Texture");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Texture' for parameter 1.");
                    lua_error(state);
                }
                Texture* param1 = (Texture*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Effect");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Effect' for parameter 2.");
                    lua_error(state);
                }
                Effect* param2 = (Effect*)((ScriptController::LuaObject*)userdata2)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)SpriteBatch::create(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "SpriteBatch");
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
            if (lua_type(state, 1) == LUA_TSTRING &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Effect");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Effect' for parameter 2.");
                    lua_error(state);
                }
                Effect* param2 = (Effect*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)SpriteBatch::create(param1, param2, param3);
                object->owns = false;
                luaL_getmetatable(state, "SpriteBatch");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata1 = ScriptController::getInstance()->getObjectPointer(1, "Texture");
                if (!userdata1)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Texture' for parameter 1.");
                    lua_error(state);
                }
                Texture* param1 = (Texture*)((ScriptController::LuaObject*)userdata1)->instance;

                // Get parameter 2 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Effect");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Effect' for parameter 2.");
                    lua_error(state);
                }
                Effect* param2 = (Effect*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)SpriteBatch::create(param1, param2, param3);
                object->owns = false;
                luaL_getmetatable(state, "SpriteBatch");
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
            lua_pushstring(state, "Invalid number of parameters (expected 1, 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
