#include "Base.h"
#include "ScriptController.h"
#include "Font.h"
#include "lua_Font.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_Font()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_Font_addRef},
        {"begin", lua_Font_begin},
        {"createText", lua_Font_createText},
        {"drawText", lua_Font_drawText},
        {"end", lua_Font_end},
        {"getIndexAtLocation", lua_Font_getIndexAtLocation},
        {"getLocationAtIndex", lua_Font_getLocationAtIndex},
        {"getRefCount", lua_Font_getRefCount},
        {"getSize", lua_Font_getSize},
        {"getSpriteBatch", lua_Font_getSpriteBatch},
        {"measureText", lua_Font_measureText},
        {"release", lua_Font_release},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_Font_static_create},
        {"getJustify", lua_Font_static_getJustify},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    sc->registerClass("Font", lua_members, NULL, lua_Font__gc, lua_statics, scopePath);
}

static Font* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Font");
    luaL_argcheck(state, userdata != NULL, 1, "'Font' expected.");
    return (Font*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_Font__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Font");
                luaL_argcheck(state, userdata != NULL, 1, "'Font' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    Font* instance = (Font*)object->instance;
                    SAFE_RELEASE(instance);
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

int lua_Font_addRef(lua_State* state)
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
                Font* instance = getInstance(state);
                instance->addRef();
                
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

int lua_Font_begin(lua_State* state)
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
                Font* instance = getInstance(state);
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

int lua_Font_createText(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                Font* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createText(param1, *param2, *param3);
                object->owns = false;
                luaL_getmetatable(state, "FontText");
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                Font* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createText(param1, *param2, *param3, param4);
                object->owns = false;
                luaL_getmetatable(state, "FontText");
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                Font* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createText(param1, *param2, *param3, param4, param5);
                object->owns = false;
                luaL_getmetatable(state, "FontText");
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = (luaL_checkint(state, 7) != 0);

                Font* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createText(param1, *param2, *param3, param4, param5, param6);
                object->owns = false;
                luaL_getmetatable(state, "FontText");
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
        case 8:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TBOOLEAN &&
                lua_type(state, 8) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = (luaL_checkint(state, 7) != 0);

                // Get parameter 7 off the stack.
                bool param7 = (luaL_checkint(state, 8) != 0);

                Font* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createText(param1, *param2, *param3, param4, param5, param6, param7);
                object->owns = false;
                luaL_getmetatable(state, "FontText");
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
        case 9:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TBOOLEAN &&
                lua_type(state, 8) == LUA_TBOOLEAN &&
                lua_type(state, 9) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = (luaL_checkint(state, 7) != 0);

                // Get parameter 7 off the stack.
                bool param7 = (luaL_checkint(state, 8) != 0);

                // Get parameter 8 off the stack.
                void* userdata9 = ScriptController::getInstance()->getObjectPointer(9, "Rectangle");
                if (!userdata9)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 9.");
                    lua_error(state);
                }
                Rectangle* param8 = (Rectangle*)((ScriptController::LuaObject*)userdata9)->instance;

                Font* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->createText(param1, *param2, *param3, param4, param5, param6, param7, param8);
                object->owns = false;
                luaL_getmetatable(state, "FontText");
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
            lua_pushstring(state, "Invalid number of parameters (expected 4, 5, 6, 7, 8 or 9).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Font_drawText(lua_State* state)
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
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "FontText");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Font::Text' for parameter 2.");
                    lua_error(state);
                }
                Font::Text* param1 = (Font::Text*)((ScriptController::LuaObject*)userdata2)->instance;

                Font* instance = getInstance(state);
                instance->drawText(param1);
                
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                Font* instance = getInstance(state);
                instance->drawText(param1, *param2, *param3);
                
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                int param3 = (int)luaL_checkint(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector4");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 5.");
                    lua_error(state);
                }
                Vector4* param4 = (Vector4*)((ScriptController::LuaObject*)userdata5)->instance;

                Font* instance = getInstance(state);
                instance->drawText(param1, param2, param3, *param4);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                Font* instance = getInstance(state);
                instance->drawText(param1, *param2, *param3, param4);
                
                return 0;
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                int param3 = (int)luaL_checkint(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector4");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 5.");
                    lua_error(state);
                }
                Vector4* param4 = (Vector4*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                Font* instance = getInstance(state);
                instance->drawText(param1, param2, param3, *param4, param5);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                Font* instance = getInstance(state);
                instance->drawText(param1, *param2, *param3, param4, param5);
                
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                int param3 = (int)luaL_checkint(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector4");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 5.");
                    lua_error(state);
                }
                Vector4* param4 = (Vector4*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                // Get parameter 6 off the stack.
                bool param6 = (luaL_checkint(state, 7) != 0);

                Font* instance = getInstance(state);
                instance->drawText(param1, param2, param3, *param4, param5, param6);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = (luaL_checkint(state, 7) != 0);

                Font* instance = getInstance(state);
                instance->drawText(param1, *param2, *param3, param4, param5, param6);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 8:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TBOOLEAN &&
                lua_type(state, 8) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = (luaL_checkint(state, 7) != 0);

                // Get parameter 7 off the stack.
                bool param7 = (luaL_checkint(state, 8) != 0);

                Font* instance = getInstance(state);
                instance->drawText(param1, *param2, *param3, param4, param5, param6, param7);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 9:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TBOOLEAN &&
                lua_type(state, 8) == LUA_TBOOLEAN &&
                lua_type(state, 9) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

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

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = (luaL_checkint(state, 7) != 0);

                // Get parameter 7 off the stack.
                bool param7 = (luaL_checkint(state, 8) != 0);

                // Get parameter 8 off the stack.
                void* userdata9 = ScriptController::getInstance()->getObjectPointer(9, "Rectangle");
                if (!userdata9)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 9.");
                    lua_error(state);
                }
                Rectangle* param8 = (Rectangle*)((ScriptController::LuaObject*)userdata9)->instance;

                Font* instance = getInstance(state);
                instance->drawText(param1, *param2, *param3, param4, param5, param6, param7, param8);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 2, 4, 5, 6, 7, 8 or 9).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Font_end(lua_State* state)
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
                Font* instance = getInstance(state);
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

int lua_Font_getIndexAtLocation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 6:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector2");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 5.");
                    lua_error(state);
                }
                Vector2* param4 = (Vector2*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Vector2");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 6.");
                    lua_error(state);
                }
                Vector2* param5 = (Vector2*)((ScriptController::LuaObject*)userdata6)->instance;

                Font* instance = getInstance(state);
                int result = instance->getIndexAtLocation(param1, *param2, param3, *param4, param5);

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TUSERDATA &&
                lua_type(state, 7) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector2");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 5.");
                    lua_error(state);
                }
                Vector2* param4 = (Vector2*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Vector2");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 6.");
                    lua_error(state);
                }
                Vector2* param5 = (Vector2*)((ScriptController::LuaObject*)userdata6)->instance;

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                Font* instance = getInstance(state);
                int result = instance->getIndexAtLocation(param1, *param2, param3, *param4, param5, param6);

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 8:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TUSERDATA &&
                lua_type(state, 7) == LUA_TSTRING &&
                lua_type(state, 8) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector2");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 5.");
                    lua_error(state);
                }
                Vector2* param4 = (Vector2*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Vector2");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 6.");
                    lua_error(state);
                }
                Vector2* param5 = (Vector2*)((ScriptController::LuaObject*)userdata6)->instance;

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                // Get parameter 7 off the stack.
                bool param7 = (luaL_checkint(state, 8) != 0);

                Font* instance = getInstance(state);
                int result = instance->getIndexAtLocation(param1, *param2, param3, *param4, param5, param6, param7);

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 9:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TUSERDATA &&
                lua_type(state, 7) == LUA_TSTRING &&
                lua_type(state, 8) == LUA_TBOOLEAN &&
                lua_type(state, 9) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector2");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 5.");
                    lua_error(state);
                }
                Vector2* param4 = (Vector2*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Vector2");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 6.");
                    lua_error(state);
                }
                Vector2* param5 = (Vector2*)((ScriptController::LuaObject*)userdata6)->instance;

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                // Get parameter 7 off the stack.
                bool param7 = (luaL_checkint(state, 8) != 0);

                // Get parameter 8 off the stack.
                bool param8 = (luaL_checkint(state, 9) != 0);

                Font* instance = getInstance(state);
                int result = instance->getIndexAtLocation(param1, *param2, param3, *param4, param5, param6, param7, param8);

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

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
            lua_pushstring(state, "Invalid number of parameters (expected 6, 7, 8 or 9).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Font_getLocationAtIndex(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 6:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector2");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 5.");
                    lua_error(state);
                }
                Vector2* param4 = (Vector2*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                Font* instance = getInstance(state);
                instance->getLocationAtIndex(param1, *param2, param3, param4, param5);
                
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector2");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 5.");
                    lua_error(state);
                }
                Vector2* param4 = (Vector2*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                Font* instance = getInstance(state);
                instance->getLocationAtIndex(param1, *param2, param3, param4, param5, param6);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 8:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TSTRING &&
                lua_type(state, 8) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector2");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 5.");
                    lua_error(state);
                }
                Vector2* param4 = (Vector2*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                // Get parameter 7 off the stack.
                bool param7 = (luaL_checkint(state, 8) != 0);

                Font* instance = getInstance(state);
                instance->getLocationAtIndex(param1, *param2, param3, param4, param5, param6, param7);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 9:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TNUMBER &&
                lua_type(state, 7) == LUA_TSTRING &&
                lua_type(state, 8) == LUA_TBOOLEAN &&
                lua_type(state, 9) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Vector2");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 5.");
                    lua_error(state);
                }
                Vector2* param4 = (Vector2*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                // Get parameter 7 off the stack.
                bool param7 = (luaL_checkint(state, 8) != 0);

                // Get parameter 8 off the stack.
                bool param8 = (luaL_checkint(state, 9) != 0);

                Font* instance = getInstance(state);
                instance->getLocationAtIndex(param1, *param2, param3, param4, param5, param6, param7, param8);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 6, 7, 8 or 9).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Font_getRefCount(lua_State* state)
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
                Font* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

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

int lua_Font_getSize(lua_State* state)
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
                Font* instance = getInstance(state);
                unsigned int result = instance->getSize();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

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

int lua_Font_getSpriteBatch(lua_State* state)
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
                Font* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->getSpriteBatch();
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
            lua_pushstring(state, "Invalid number of parameters (expected 1).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Font_measureText(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 5:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA))
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                unsigned int* param3 = ScriptController::getInstance()->getUnsignedIntPointer(4);

                // Get parameter 4 off the stack.
                unsigned int* param4 = ScriptController::getInstance()->getUnsignedIntPointer(5);

                Font* instance = getInstance(state);
                instance->measureText(param1, param2, param3, param4);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Rectangle");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 5.");
                    lua_error(state);
                }
                Rectangle* param4 = (Rectangle*)((ScriptController::LuaObject*)userdata5)->instance;

                Font* instance = getInstance(state);
                instance->measureText(param1, *param2, param3, param4);
                
                return 0;
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Rectangle");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 5.");
                    lua_error(state);
                }
                Rectangle* param4 = (Rectangle*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                Font* instance = getInstance(state);
                instance->measureText(param1, *param2, param3, param4, param5);
                
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
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Rectangle");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 5.");
                    lua_error(state);
                }
                Rectangle* param4 = (Rectangle*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = (luaL_checkint(state, 7) != 0);

                Font* instance = getInstance(state);
                instance->measureText(param1, *param2, param3, param4, param5, param6);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 8:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER &&
                lua_type(state, 5) == LUA_TUSERDATA &&
                lua_type(state, 6) == LUA_TSTRING &&
                lua_type(state, 7) == LUA_TBOOLEAN &&
                lua_type(state, 8) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Rectangle");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 3.");
                    lua_error(state);
                }
                Rectangle* param2 = (Rectangle*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                void* userdata5 = ScriptController::getInstance()->getObjectPointer(5, "Rectangle");
                if (!userdata5)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Rectangle' for parameter 5.");
                    lua_error(state);
                }
                Rectangle* param4 = (Rectangle*)((ScriptController::LuaObject*)userdata5)->instance;

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = (luaL_checkint(state, 7) != 0);

                // Get parameter 7 off the stack.
                bool param7 = (luaL_checkint(state, 8) != 0);

                Font* instance = getInstance(state);
                instance->measureText(param1, *param2, param3, param4, param5, param6, param7);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 5, 6, 7 or 8).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Font_release(lua_State* state)
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
                Font* instance = getInstance(state);
                instance->release();
                
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

int lua_Font_static_create(lua_State* state)
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
                object->instance = (void*)Font::create(param1);
                object->owns = false;
                luaL_getmetatable(state, "Font");
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
                lua_type(state, 2) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

                // Get parameter 2 off the stack.
                const char* param2 = luaL_checkstring(state, 2);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)Font::create(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "Font");
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
            if (lua_type(state, 1) == LUA_TSTRING &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TNUMBER &&
                lua_type(state, 4) == LUA_TUSERDATA &&
                lua_type(state, 5) == LUA_TNUMBER &&
                lua_type(state, 6) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

                // Get parameter 2 off the stack.
                Font::Style param2 = (Font::Style)lua_enumFromString_FontStyle(luaL_checkstring(state, 2));

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 4 off the stack.
                void* userdata4 = ScriptController::getInstance()->getObjectPointer(4, "FontGlyph");
                if (!userdata4)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Font::Glyph' for parameter 4.");
                    lua_error(state);
                }
                Font::Glyph* param4 = (Font::Glyph*)((ScriptController::LuaObject*)userdata4)->instance;

                // Get parameter 5 off the stack.
                int param5 = (int)luaL_checkint(state, 5);

                // Get parameter 6 off the stack.
                void* userdata6 = ScriptController::getInstance()->getObjectPointer(6, "Texture");
                if (!userdata6)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Texture' for parameter 6.");
                    lua_error(state);
                }
                Texture* param6 = (Texture*)((ScriptController::LuaObject*)userdata6)->instance;

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)Font::create(param1, param2, param3, param4, param5, param6);
                object->owns = false;
                luaL_getmetatable(state, "Font");
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
            lua_pushstring(state, "Invalid number of parameters (expected 1, 2 or 6).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Font_static_getJustify(lua_State* state)
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

                Font::Justify result = Font::getJustify(param1);

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_FontJustify(result).c_str());

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

}
