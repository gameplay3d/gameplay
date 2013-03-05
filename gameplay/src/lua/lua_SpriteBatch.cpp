#include "Base.h"
#include "ScriptController.h"
#include "lua_SpriteBatch.h"
#include "Base.h"
#include "Game.h"
#include "Material.h"
#include "SpriteBatch.h"

namespace gameplay
{

void luaRegister_SpriteBatch()
{
    const luaL_Reg lua_members[] = 
    {
        {"draw", lua_SpriteBatch_draw},
        {"finish", lua_SpriteBatch_finish},
        {"getMaterial", lua_SpriteBatch_getMaterial},
        {"getProjectionMatrix", lua_SpriteBatch_getProjectionMatrix},
        {"getSampler", lua_SpriteBatch_getSampler},
        {"getStateBlock", lua_SpriteBatch_getStateBlock},
        {"setProjectionMatrix", lua_SpriteBatch_setProjectionMatrix},
        {"start", lua_SpriteBatch_start},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_SpriteBatch_static_create},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("SpriteBatch", lua_members, NULL, lua_SpriteBatch__gc, lua_statics, scopePath);
}

static SpriteBatch* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "SpriteBatch");
    luaL_argcheck(state, userdata != NULL, 1, "'SpriteBatch' expected.");
    return (SpriteBatch*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                void* userdata = luaL_checkudata(state, 1, "SpriteBatch");
                luaL_argcheck(state, userdata != NULL, 1, "'SpriteBatch' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    SpriteBatch* instance = (SpriteBatch*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteBatch__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteBatch_draw(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
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
                    gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(*param1, *param2);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param3 = gameplay::ScriptUtil::getObjectPointer<Vector4>(4, "Vector4", true, &param3Valid);
                    if (!param3Valid)
                        break;

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(*param1, *param2, *param3);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param3 = gameplay::ScriptUtil::getObjectPointer<Vector2>(4, "Vector2", true, &param3Valid);
                    if (!param3Valid)
                        break;

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(*param1, *param2, *param3);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param3 = gameplay::ScriptUtil::getObjectPointer<Vector2>(4, "Vector2", true, &param3Valid);
                    if (!param3Valid)
                        break;

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param4 = gameplay::ScriptUtil::getObjectPointer<Vector4>(5, "Vector4", true, &param4Valid);
                    if (!param4Valid)
                        break;

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(*param1, *param2, *param3, *param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                    (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TNIL) &&
                    lua_type(state, 7) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param3 = gameplay::ScriptUtil::getObjectPointer<Vector2>(4, "Vector2", true, &param3Valid);
                    if (!param3Valid)
                        break;

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param4 = gameplay::ScriptUtil::getObjectPointer<Vector4>(5, "Vector4", true, &param4Valid);
                    if (!param4Valid)
                        break;

                    // Get parameter 5 off the stack.
                    bool param5Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param5 = gameplay::ScriptUtil::getObjectPointer<Vector2>(6, "Vector2", true, &param5Valid);
                    if (!param5Valid)
                        break;

                    // Get parameter 6 off the stack.
                    float param6 = (float)luaL_checknumber(state, 7);

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(*param1, *param2, *param3, *param4, *param5, param6);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 10:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    (lua_type(state, 10) == LUA_TUSERDATA || lua_type(state, 10) == LUA_TNIL))
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
                    bool param9Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param9 = gameplay::ScriptUtil::getObjectPointer<Vector4>(10, "Vector4", true, &param9Valid);
                    if (!param9Valid)
                        break;

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, *param9);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 11:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    (lua_type(state, 9) == LUA_TUSERDATA || lua_type(state, 9) == LUA_TNIL) &&
                    (lua_type(state, 10) == LUA_TUSERDATA || lua_type(state, 10) == LUA_TNIL) &&
                    lua_type(state, 11) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

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
                    bool param8Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param8 = gameplay::ScriptUtil::getObjectPointer<Vector4>(9, "Vector4", true, &param8Valid);
                    if (!param8Valid)
                        break;

                    // Get parameter 9 off the stack.
                    bool param9Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param9 = gameplay::ScriptUtil::getObjectPointer<Vector2>(10, "Vector2", true, &param9Valid);
                    if (!param9Valid)
                        break;

                    // Get parameter 10 off the stack.
                    float param10 = (float)luaL_checknumber(state, 11);

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(*param1, param2, param3, param4, param5, param6, param7, *param8, *param9, param10);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    (lua_type(state, 10) == LUA_TUSERDATA || lua_type(state, 10) == LUA_TNIL) &&
                    (lua_type(state, 11) == LUA_TUSERDATA || lua_type(state, 11) == LUA_TNIL))
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
                    bool param9Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param9 = gameplay::ScriptUtil::getObjectPointer<Vector4>(10, "Vector4", true, &param9Valid);
                    if (!param9Valid)
                        break;

                    // Get parameter 10 off the stack.
                    bool param10Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param10 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(11, "Rectangle", true, &param10Valid);
                    if (!param10Valid)
                        break;

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, *param9, *param10);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    lua_type(state, 10) == LUA_TNUMBER &&
                    (lua_type(state, 11) == LUA_TUSERDATA || lua_type(state, 11) == LUA_TNIL))
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
                    bool param10Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param10 = gameplay::ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true, &param10Valid);
                    if (!param10Valid)
                        break;

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 12:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    (lua_type(state, 9) == LUA_TUSERDATA || lua_type(state, 9) == LUA_TNIL) &&
                    (lua_type(state, 10) == LUA_TUSERDATA || lua_type(state, 10) == LUA_TNIL) &&
                    lua_type(state, 11) == LUA_TNUMBER &&
                    lua_type(state, 12) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param1 = gameplay::ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true, &param1Valid);
                    if (!param1Valid)
                        break;

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
                    bool param8Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param8 = gameplay::ScriptUtil::getObjectPointer<Vector4>(9, "Vector4", true, &param8Valid);
                    if (!param8Valid)
                        break;

                    // Get parameter 9 off the stack.
                    bool param9Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param9 = gameplay::ScriptUtil::getObjectPointer<Vector2>(10, "Vector2", true, &param9Valid);
                    if (!param9Valid)
                        break;

                    // Get parameter 10 off the stack.
                    float param10 = (float)luaL_checknumber(state, 11);

                    // Get parameter 11 off the stack.
                    bool param11 = gameplay::ScriptUtil::luaCheckBool(state, 12);

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(*param1, param2, param3, param4, param5, param6, param7, *param8, *param9, param10, param11);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    lua_type(state, 10) == LUA_TNUMBER &&
                    (lua_type(state, 11) == LUA_TUSERDATA || lua_type(state, 11) == LUA_TNIL) &&
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
                    bool param10Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param10 = gameplay::ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true, &param10Valid);
                    if (!param10Valid)
                        break;

                    // Get parameter 11 off the stack.
                    bool param11 = gameplay::ScriptUtil::luaCheckBool(state, 12);

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10, param11);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 13:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    lua_type(state, 10) == LUA_TNUMBER &&
                    (lua_type(state, 11) == LUA_TUSERDATA || lua_type(state, 11) == LUA_TNIL) &&
                    (lua_type(state, 12) == LUA_TUSERDATA || lua_type(state, 12) == LUA_TNIL) &&
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
                    bool param10Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param10 = gameplay::ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true, &param10Valid);
                    if (!param10Valid)
                        break;

                    // Get parameter 11 off the stack.
                    bool param11Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param11 = gameplay::ScriptUtil::getObjectPointer<Vector2>(12, "Vector2", true, &param11Valid);
                    if (!param11Valid)
                        break;

                    // Get parameter 12 off the stack.
                    float param12 = (float)luaL_checknumber(state, 13);

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10, *param11, param12);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    lua_type(state, 10) == LUA_TNUMBER &&
                    (lua_type(state, 11) == LUA_TUSERDATA || lua_type(state, 11) == LUA_TNIL) &&
                    (lua_type(state, 12) == LUA_TUSERDATA || lua_type(state, 12) == LUA_TNIL) &&
                    lua_type(state, 13) == LUA_TNUMBER)
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

                    // Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Vector3> param3 = gameplay::ScriptUtil::getObjectPointer<Vector3>(4, "Vector3", true, &param3Valid);
                    if (!param3Valid)
                        break;

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
                    bool param10Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param10 = gameplay::ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true, &param10Valid);
                    if (!param10Valid)
                        break;

                    // Get parameter 11 off the stack.
                    bool param11Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param11 = gameplay::ScriptUtil::getObjectPointer<Vector2>(12, "Vector2", true, &param11Valid);
                    if (!param11Valid)
                        break;

                    // Get parameter 12 off the stack.
                    float param12 = (float)luaL_checknumber(state, 13);

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(*param1, *param2, *param3, param4, param5, param6, param7, param8, param9, *param10, *param11, param12);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 14:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    lua_type(state, 10) == LUA_TNUMBER &&
                    (lua_type(state, 11) == LUA_TUSERDATA || lua_type(state, 11) == LUA_TNIL) &&
                    (lua_type(state, 12) == LUA_TUSERDATA || lua_type(state, 12) == LUA_TNIL) &&
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
                    bool param10Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param10 = gameplay::ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true, &param10Valid);
                    if (!param10Valid)
                        break;

                    // Get parameter 11 off the stack.
                    bool param11Valid;
                    gameplay::ScriptUtil::LuaArray<Vector2> param11 = gameplay::ScriptUtil::getObjectPointer<Vector2>(12, "Vector2", true, &param11Valid);
                    if (!param11Valid)
                        break;

                    // Get parameter 12 off the stack.
                    float param12 = (float)luaL_checknumber(state, 13);

                    // Get parameter 13 off the stack.
                    bool param13 = gameplay::ScriptUtil::luaCheckBool(state, 14);

                    SpriteBatch* instance = getInstance(state);
                    instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10, *param11, param12, param13);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_SpriteBatch_finish(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
                instance->finish();
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteBatch_finish - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteBatch_getMaterial(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
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

            lua_pushstring(state, "lua_SpriteBatch_getMaterial - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteBatch_getProjectionMatrix(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
                void* returnPtr = (void*)&(instance->getProjectionMatrix());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Matrix");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteBatch_getProjectionMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteBatch_getSampler(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
                void* returnPtr = (void*)instance->getSampler();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "TextureSampler");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteBatch_getSampler - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteBatch_getStateBlock(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
                void* returnPtr = (void*)instance->getStateBlock();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "RenderStateStateBlock");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteBatch_getStateBlock - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteBatch_setProjectionMatrix(lua_State* state)
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

                SpriteBatch* instance = getInstance(state);
                instance->setProjectionMatrix(*param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteBatch_setProjectionMatrix - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteBatch_start(lua_State* state)
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
                SpriteBatch* instance = getInstance(state);
                instance->start();
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteBatch_start - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteBatch_static_create(lua_State* state)
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
                if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(1, false);

                    void* returnPtr = (void*)SpriteBatch::create(param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "SpriteBatch");
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Texture> param1 = gameplay::ScriptUtil::getObjectPointer<Texture>(1, "Texture", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    void* returnPtr = (void*)SpriteBatch::create(param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "SpriteBatch");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(1, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Effect> param2 = gameplay::ScriptUtil::getObjectPointer<Effect>(2, "Effect", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    void* returnPtr = (void*)SpriteBatch::create(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "SpriteBatch");
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Texture> param1 = gameplay::ScriptUtil::getObjectPointer<Texture>(1, "Texture", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Effect> param2 = gameplay::ScriptUtil::getObjectPointer<Effect>(2, "Effect", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    void* returnPtr = (void*)SpriteBatch::create(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "SpriteBatch");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(1, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Effect> param2 = gameplay::ScriptUtil::getObjectPointer<Effect>(2, "Effect", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

                    void* returnPtr = (void*)SpriteBatch::create(param1, param2, param3);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "SpriteBatch");
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
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Texture> param1 = gameplay::ScriptUtil::getObjectPointer<Texture>(1, "Texture", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Effect> param2 = gameplay::ScriptUtil::getObjectPointer<Effect>(2, "Effect", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

                    void* returnPtr = (void*)SpriteBatch::create(param1, param2, param3);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "SpriteBatch");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteBatch_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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
