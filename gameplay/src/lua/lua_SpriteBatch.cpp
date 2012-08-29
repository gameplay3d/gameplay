#include "Base.h"
#include "ScriptController.h"
#include "lua_SpriteBatch.h"
#include "Base.h"
#include "Game.h"
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

    ScriptUtil::registerClass("SpriteBatch", lua_members, NULL, lua_SpriteBatch__gc, lua_statics, scopePath);
}

static SpriteBatch* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "SpriteBatch");
    luaL_argcheck(state, userdata != NULL, 1, "'SpriteBatch' expected.");
    return (SpriteBatch*)((ScriptUtil::LuaObject*)userdata)->instance;
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
                ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    SpriteBatch* instance = (SpriteBatch*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch__gc - Failed to match the given parameters to a valid function signature.");
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
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<Rectangle> param1 = ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Rectangle> param2 = ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<Rectangle> param1 = ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Rectangle> param2 = ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true);

                // Get parameter 3 off the stack.
                ScriptUtil::LuaArray<Vector4> param3 = ScriptUtil::getObjectPointer<Vector4>(4, "Vector4", true);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3);
                
                return 0;
            }
            else if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<Vector3> param1 = ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Rectangle> param2 = ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true);

                // Get parameter 3 off the stack.
                ScriptUtil::LuaArray<Vector2> param3 = ScriptUtil::getObjectPointer<Vector2>(4, "Vector2", true);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<Vector3> param1 = ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Rectangle> param2 = ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true);

                // Get parameter 3 off the stack.
                ScriptUtil::LuaArray<Vector2> param3 = ScriptUtil::getObjectPointer<Vector2>(4, "Vector2", true);

                // Get parameter 4 off the stack.
                ScriptUtil::LuaArray<Vector4> param4 = ScriptUtil::getObjectPointer<Vector4>(5, "Vector4", true);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3, *param4);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 7:
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
                ScriptUtil::LuaArray<Vector3> param1 = ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Rectangle> param2 = ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true);

                // Get parameter 3 off the stack.
                ScriptUtil::LuaArray<Vector2> param3 = ScriptUtil::getObjectPointer<Vector2>(4, "Vector2", true);

                // Get parameter 4 off the stack.
                ScriptUtil::LuaArray<Vector4> param4 = ScriptUtil::getObjectPointer<Vector4>(5, "Vector4", true);

                // Get parameter 5 off the stack.
                ScriptUtil::LuaArray<Vector2> param5 = ScriptUtil::getObjectPointer<Vector2>(6, "Vector2", true);

                // Get parameter 6 off the stack.
                float param6 = (float)luaL_checknumber(state, 7);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3, *param4, *param5, param6);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 10:
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
                ScriptUtil::LuaArray<Vector4> param9 = ScriptUtil::getObjectPointer<Vector4>(10, "Vector4", true);

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, *param9);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 11:
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
                ScriptUtil::LuaArray<Vector3> param1 = ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true);

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
                ScriptUtil::LuaArray<Vector4> param8 = ScriptUtil::getObjectPointer<Vector4>(9, "Vector4", true);

                // Get parameter 9 off the stack.
                ScriptUtil::LuaArray<Vector2> param9 = ScriptUtil::getObjectPointer<Vector2>(10, "Vector2", true);

                // Get parameter 10 off the stack.
                float param10 = (float)luaL_checknumber(state, 11);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, param2, param3, param4, param5, param6, param7, *param8, *param9, param10);
                
                return 0;
            }
            else if ((lua_type(state, 1) == LUA_TUSERDATA) &&
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
                ScriptUtil::LuaArray<Vector4> param9 = ScriptUtil::getObjectPointer<Vector4>(10, "Vector4", true);

                // Get parameter 10 off the stack.
                ScriptUtil::LuaArray<Rectangle> param10 = ScriptUtil::getObjectPointer<Rectangle>(11, "Rectangle", true);

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, *param9, *param10);
                
                return 0;
            }
            else if ((lua_type(state, 1) == LUA_TUSERDATA) &&
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
                ScriptUtil::LuaArray<Vector4> param10 = ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true);

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 12:
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
                ScriptUtil::LuaArray<Vector3> param1 = ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true);

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
                ScriptUtil::LuaArray<Vector4> param8 = ScriptUtil::getObjectPointer<Vector4>(9, "Vector4", true);

                // Get parameter 9 off the stack.
                ScriptUtil::LuaArray<Vector2> param9 = ScriptUtil::getObjectPointer<Vector2>(10, "Vector2", true);

                // Get parameter 10 off the stack.
                float param10 = (float)luaL_checknumber(state, 11);

                // Get parameter 11 off the stack.
                bool param11 = ScriptUtil::luaCheckBool(state, 12);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, param2, param3, param4, param5, param6, param7, *param8, *param9, param10, param11);
                
                return 0;
            }
            else if ((lua_type(state, 1) == LUA_TUSERDATA) &&
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
                ScriptUtil::LuaArray<Vector4> param10 = ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true);

                // Get parameter 11 off the stack.
                bool param11 = ScriptUtil::luaCheckBool(state, 12);

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10, param11);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 13:
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
                ScriptUtil::LuaArray<Vector4> param10 = ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true);

                // Get parameter 11 off the stack.
                ScriptUtil::LuaArray<Vector2> param11 = ScriptUtil::getObjectPointer<Vector2>(12, "Vector2", true);

                // Get parameter 12 off the stack.
                float param12 = (float)luaL_checknumber(state, 13);

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10, *param11, param12);
                
                return 0;
            }
            else if ((lua_type(state, 1) == LUA_TUSERDATA) &&
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
                ScriptUtil::LuaArray<Vector3> param1 = ScriptUtil::getObjectPointer<Vector3>(2, "Vector3", true);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Vector3> param2 = ScriptUtil::getObjectPointer<Vector3>(3, "Vector3", true);

                // Get parameter 3 off the stack.
                ScriptUtil::LuaArray<Vector3> param3 = ScriptUtil::getObjectPointer<Vector3>(4, "Vector3", true);

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
                ScriptUtil::LuaArray<Vector4> param10 = ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true);

                // Get parameter 11 off the stack.
                ScriptUtil::LuaArray<Vector2> param11 = ScriptUtil::getObjectPointer<Vector2>(12, "Vector2", true);

                // Get parameter 12 off the stack.
                float param12 = (float)luaL_checknumber(state, 13);

                SpriteBatch* instance = getInstance(state);
                instance->draw(*param1, *param2, *param3, param4, param5, param6, param7, param8, param9, *param10, *param11, param12);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 14:
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
                ScriptUtil::LuaArray<Vector4> param10 = ScriptUtil::getObjectPointer<Vector4>(11, "Vector4", true);

                // Get parameter 11 off the stack.
                ScriptUtil::LuaArray<Vector2> param11 = ScriptUtil::getObjectPointer<Vector2>(12, "Vector2", true);

                // Get parameter 12 off the stack.
                float param12 = (float)luaL_checknumber(state, 13);

                // Get parameter 13 off the stack.
                bool param13 = ScriptUtil::luaCheckBool(state, 14);

                SpriteBatch* instance = getInstance(state);
                instance->draw(param1, param2, param3, param4, param5, param6, param7, param8, param9, *param10, *param11, param12, param13);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_draw - Failed to match the given parameters to a valid function signature.");
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
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_finish - Failed to match the given parameters to a valid function signature.");
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                SpriteBatch* instance = getInstance(state);
                void* returnPtr = (void*)instance->getMaterial();
                if (returnPtr)
                {
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_getMaterial - Failed to match the given parameters to a valid function signature.");
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
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_getProjectionMatrix - Failed to match the given parameters to a valid function signature.");
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
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_getSampler - Failed to match the given parameters to a valid function signature.");
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                SpriteBatch* instance = getInstance(state);
                void* returnPtr = (void*)instance->getStateBlock();
                if (returnPtr)
                {
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_getStateBlock - Failed to match the given parameters to a valid function signature.");
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
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<Matrix> param1 = ScriptUtil::getObjectPointer<Matrix>(2, "Matrix", true);

                SpriteBatch* instance = getInstance(state);
                instance->setProjectionMatrix(*param1);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_setProjectionMatrix - Failed to match the given parameters to a valid function signature.");
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
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_start - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteBatch_static_create(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<const char> param1 = ScriptUtil::getString(1, false);

                void* returnPtr = (void*)SpriteBatch::create(param1);
                if (returnPtr)
                {
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<Texture> param1 = ScriptUtil::getObjectPointer<Texture>(1, "Texture", false);

                void* returnPtr = (void*)SpriteBatch::create(param1);
                if (returnPtr)
                {
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_static_create - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<const char> param1 = ScriptUtil::getString(1, false);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Effect> param2 = ScriptUtil::getObjectPointer<Effect>(2, "Effect", false);

                void* returnPtr = (void*)SpriteBatch::create(param1, param2);
                if (returnPtr)
                {
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<Texture> param1 = ScriptUtil::getObjectPointer<Texture>(1, "Texture", false);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Effect> param2 = ScriptUtil::getObjectPointer<Effect>(2, "Effect", false);

                void* returnPtr = (void*)SpriteBatch::create(param1, param2);
                if (returnPtr)
                {
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_static_create - Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<const char> param1 = ScriptUtil::getString(1, false);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Effect> param2 = ScriptUtil::getObjectPointer<Effect>(2, "Effect", false);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

                void* returnPtr = (void*)SpriteBatch::create(param1, param2, param3);
                if (returnPtr)
                {
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<Texture> param1 = ScriptUtil::getObjectPointer<Texture>(1, "Texture", false);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<Effect> param2 = ScriptUtil::getObjectPointer<Effect>(2, "Effect", false);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

                void* returnPtr = (void*)SpriteBatch::create(param1, param2, param3);
                if (returnPtr)
                {
                    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
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
            else
            {
                lua_pushstring(state, "lua_SpriteBatch_static_create - Failed to match the given parameters to a valid function signature.");
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
