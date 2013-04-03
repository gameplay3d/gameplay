#include "Base.h"
#include "ScriptController.h"
#include "lua_Font.h"
#include "Base.h"
#include "Bundle.h"
#include "FileSystem.h"
#include "Font.h"
#include "Game.h"
#include "Ref.h"
#include "lua_FontJustify.h"

namespace gameplay
{

void luaRegister_Font()
{
    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_Font_addRef},
        {"createText", lua_Font_createText},
        {"drawText", lua_Font_drawText},
        {"finish", lua_Font_finish},
        {"getIndexAtLocation", lua_Font_getIndexAtLocation},
        {"getLocationAtIndex", lua_Font_getLocationAtIndex},
        {"getRefCount", lua_Font_getRefCount},
        {"getSize", lua_Font_getSize},
        {"getSpriteBatch", lua_Font_getSpriteBatch},
        {"measureText", lua_Font_measureText},
        {"release", lua_Font_release},
        {"start", lua_Font_start},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_Font_static_create},
        {"getJustify", lua_Font_static_getJustify},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Font", lua_members, NULL, lua_Font__gc, lua_statics, scopePath);
}

static Font* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Font");
    luaL_argcheck(state, userdata != NULL, 1, "'Font' expected.");
    return (Font*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                void* userdata = luaL_checkudata(state, 1, "Font");
                luaL_argcheck(state, userdata != NULL, 1, "'Font' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Font* instance = (Font*)object->instance;
                    SAFE_RELEASE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_Font__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_Font_addRef(lua_State* state)
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
                Font* instance = getInstance(state);
                instance->addRef();
                
                return 0;
            }

            lua_pushstring(state, "lua_Font_addRef - Failed to match the given parameters to a valid function signature.");
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

int lua_Font_createText(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param3 = gameplay::ScriptUtil::getObjectPointer<Vector4>(4, "Vector4", true, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Vector4'.");
                    lua_error(state);
                }

                Font* instance = getInstance(state);
                void* returnPtr = (void*)instance->createText(param1, *param2, *param3);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "FontText");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Font_createText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                lua_type(state, 5) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param3 = gameplay::ScriptUtil::getObjectPointer<Vector4>(4, "Vector4", true, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                Font* instance = getInstance(state);
                void* returnPtr = (void*)instance->createText(param1, *param2, *param3, param4);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "FontText");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Font_createText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 6:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                lua_type(state, 5) == LUA_TNUMBER &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param3 = gameplay::ScriptUtil::getObjectPointer<Vector4>(4, "Vector4", true, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                Font* instance = getInstance(state);
                void* returnPtr = (void*)instance->createText(param1, *param2, *param3, param4, param5);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "FontText");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Font_createText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                lua_type(state, 5) == LUA_TNUMBER &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                lua_type(state, 7) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param3 = gameplay::ScriptUtil::getObjectPointer<Vector4>(4, "Vector4", true, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 7);

                Font* instance = getInstance(state);
                void* returnPtr = (void*)instance->createText(param1, *param2, *param3, param4, param5, param6);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "FontText");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Font_createText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 8:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                lua_type(state, 5) == LUA_TNUMBER &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                lua_type(state, 7) == LUA_TBOOLEAN &&
                lua_type(state, 8) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param3 = gameplay::ScriptUtil::getObjectPointer<Vector4>(4, "Vector4", true, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 7);

                // Get parameter 7 off the stack.
                bool param7 = gameplay::ScriptUtil::luaCheckBool(state, 8);

                Font* instance = getInstance(state);
                void* returnPtr = (void*)instance->createText(param1, *param2, *param3, param4, param5, param6, param7);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "FontText");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Font_createText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 9:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                lua_type(state, 5) == LUA_TNUMBER &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                lua_type(state, 7) == LUA_TBOOLEAN &&
                lua_type(state, 8) == LUA_TBOOLEAN &&
                (lua_type(state, 9) == LUA_TUSERDATA || lua_type(state, 9) == LUA_TTABLE || lua_type(state, 9) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Vector4> param3 = gameplay::ScriptUtil::getObjectPointer<Vector4>(4, "Vector4", true, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Vector4'.");
                    lua_error(state);
                }

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                // Get parameter 5 off the stack.
                Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 7);

                // Get parameter 7 off the stack.
                bool param7 = gameplay::ScriptUtil::luaCheckBool(state, 8);

                // Get parameter 8 off the stack.
                bool param8Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param8 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(9, "Rectangle", false, &param8Valid);
                if (!param8Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 8 to type 'Rectangle'.");
                    lua_error(state);
                }

                Font* instance = getInstance(state);
                void* returnPtr = (void*)instance->createText(param1, *param2, *param3, param4, param5, param6, param7, param8);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "FontText");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Font_createText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Font::Text> param1 = gameplay::ScriptUtil::getObjectPointer<Font::Text>(2, "FontText", false, &param1Valid);
                    if (!param1Valid)
                        break;

                    Font* instance = getInstance(state);
                    instance->drawText(param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_drawText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

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

                    Font* instance = getInstance(state);
                    instance->drawText(param1, *param2, *param3);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_drawText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    int param3 = (int)luaL_checkint(state, 4);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param4 = gameplay::ScriptUtil::getObjectPointer<Vector4>(5, "Vector4", true, &param4Valid);
                    if (!param4Valid)
                        break;

                    Font* instance = getInstance(state);
                    instance->drawText(param1, param2, param3, *param4);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                    lua_type(state, 5) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

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

                    // Get parameter 4 off the stack.
                    unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                    Font* instance = getInstance(state);
                    instance->drawText(param1, *param2, *param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_drawText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 6:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                    lua_type(state, 6) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    int param3 = (int)luaL_checkint(state, 4);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param4 = gameplay::ScriptUtil::getObjectPointer<Vector4>(5, "Vector4", true, &param4Valid);
                    if (!param4Valid)
                        break;

                    // Get parameter 5 off the stack.
                    unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                    Font* instance = getInstance(state);
                    instance->drawText(param1, param2, param3, *param4, param5);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

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

                    // Get parameter 4 off the stack.
                    unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                    // Get parameter 5 off the stack.
                    Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                    Font* instance = getInstance(state);
                    instance->drawText(param1, *param2, *param3, param4, param5);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_drawText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    int param3 = (int)luaL_checkint(state, 4);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Vector4> param4 = gameplay::ScriptUtil::getObjectPointer<Vector4>(5, "Vector4", true, &param4Valid);
                    if (!param4Valid)
                        break;

                    // Get parameter 5 off the stack.
                    unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                    // Get parameter 6 off the stack.
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 7);

                    Font* instance = getInstance(state);
                    instance->drawText(param1, param2, param3, *param4, param5, param6);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                    lua_type(state, 7) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

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

                    // Get parameter 4 off the stack.
                    unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                    // Get parameter 5 off the stack.
                    Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                    // Get parameter 6 off the stack.
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 7);

                    Font* instance = getInstance(state);
                    instance->drawText(param1, *param2, *param3, param4, param5, param6);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_drawText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 8:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    int param3 = (int)luaL_checkint(state, 4);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 5);

                    // Get parameter 5 off the stack.
                    float param5 = (float)luaL_checknumber(state, 6);

                    // Get parameter 6 off the stack.
                    float param6 = (float)luaL_checknumber(state, 7);

                    // Get parameter 7 off the stack.
                    float param7 = (float)luaL_checknumber(state, 8);

                    Font* instance = getInstance(state);
                    instance->drawText(param1, param2, param3, param4, param5, param6, param7);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                    lua_type(state, 7) == LUA_TBOOLEAN &&
                    lua_type(state, 8) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

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

                    // Get parameter 4 off the stack.
                    unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                    // Get parameter 5 off the stack.
                    Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                    // Get parameter 6 off the stack.
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 7);

                    // Get parameter 7 off the stack.
                    bool param7 = gameplay::ScriptUtil::luaCheckBool(state, 8);

                    Font* instance = getInstance(state);
                    instance->drawText(param1, *param2, *param3, param4, param5, param6, param7);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_drawText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 9:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    int param3 = (int)luaL_checkint(state, 4);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 5);

                    // Get parameter 5 off the stack.
                    float param5 = (float)luaL_checknumber(state, 6);

                    // Get parameter 6 off the stack.
                    float param6 = (float)luaL_checknumber(state, 7);

                    // Get parameter 7 off the stack.
                    float param7 = (float)luaL_checknumber(state, 8);

                    // Get parameter 8 off the stack.
                    unsigned int param8 = (unsigned int)luaL_checkunsigned(state, 9);

                    Font* instance = getInstance(state);
                    instance->drawText(param1, param2, param3, param4, param5, param6, param7, param8);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL) &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                    lua_type(state, 7) == LUA_TBOOLEAN &&
                    lua_type(state, 8) == LUA_TBOOLEAN &&
                    (lua_type(state, 9) == LUA_TUSERDATA || lua_type(state, 9) == LUA_TTABLE || lua_type(state, 9) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

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

                    // Get parameter 4 off the stack.
                    unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

                    // Get parameter 5 off the stack.
                    Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                    // Get parameter 6 off the stack.
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 7);

                    // Get parameter 7 off the stack.
                    bool param7 = gameplay::ScriptUtil::luaCheckBool(state, 8);

                    // Get parameter 8 off the stack.
                    bool param8Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param8 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(9, "Rectangle", false, &param8Valid);
                    if (!param8Valid)
                        break;

                    Font* instance = getInstance(state);
                    instance->drawText(param1, *param2, *param3, param4, param5, param6, param7, param8);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_drawText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 10:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER &&
                    lua_type(state, 6) == LUA_TNUMBER &&
                    lua_type(state, 7) == LUA_TNUMBER &&
                    lua_type(state, 8) == LUA_TNUMBER &&
                    lua_type(state, 9) == LUA_TNUMBER &&
                    lua_type(state, 10) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    int param3 = (int)luaL_checkint(state, 4);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 5);

                    // Get parameter 5 off the stack.
                    float param5 = (float)luaL_checknumber(state, 6);

                    // Get parameter 6 off the stack.
                    float param6 = (float)luaL_checknumber(state, 7);

                    // Get parameter 7 off the stack.
                    float param7 = (float)luaL_checknumber(state, 8);

                    // Get parameter 8 off the stack.
                    unsigned int param8 = (unsigned int)luaL_checkunsigned(state, 9);

                    // Get parameter 9 off the stack.
                    bool param9 = gameplay::ScriptUtil::luaCheckBool(state, 10);

                    Font* instance = getInstance(state);
                    instance->drawText(param1, param2, param3, param4, param5, param6, param7, param8, param9);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_drawText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2, 4, 5, 6, 7, 8, 9 or 10).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Font_finish(lua_State* state)
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
                Font* instance = getInstance(state);
                instance->finish();
                
                return 0;
            }

            lua_pushstring(state, "lua_Font_finish - Failed to match the given parameters to a valid function signature.");
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

int lua_Font_getIndexAtLocation(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 6:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param4 = gameplay::ScriptUtil::getObjectPointer<Vector2>(5, "Vector2", true, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 5 off the stack.
                bool param5Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param5 = gameplay::ScriptUtil::getObjectPointer<Vector2>(6, "Vector2", false, &param5Valid);
                if (!param5Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 5 to type 'Vector2'.");
                    lua_error(state);
                }

                Font* instance = getInstance(state);
                int result = instance->getIndexAtLocation(param1, *param2, param3, *param4, param5);

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Font_getIndexAtLocation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TNIL) &&
                (lua_type(state, 7) == LUA_TSTRING || lua_type(state, 7) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param4 = gameplay::ScriptUtil::getObjectPointer<Vector2>(5, "Vector2", true, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 5 off the stack.
                bool param5Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param5 = gameplay::ScriptUtil::getObjectPointer<Vector2>(6, "Vector2", false, &param5Valid);
                if (!param5Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 5 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                Font* instance = getInstance(state);
                int result = instance->getIndexAtLocation(param1, *param2, param3, *param4, param5, param6);

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Font_getIndexAtLocation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 8:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TNIL) &&
                (lua_type(state, 7) == LUA_TSTRING || lua_type(state, 7) == LUA_TNIL) &&
                lua_type(state, 8) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param4 = gameplay::ScriptUtil::getObjectPointer<Vector2>(5, "Vector2", true, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 5 off the stack.
                bool param5Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param5 = gameplay::ScriptUtil::getObjectPointer<Vector2>(6, "Vector2", false, &param5Valid);
                if (!param5Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 5 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                // Get parameter 7 off the stack.
                bool param7 = gameplay::ScriptUtil::luaCheckBool(state, 8);

                Font* instance = getInstance(state);
                int result = instance->getIndexAtLocation(param1, *param2, param3, *param4, param5, param6, param7);

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Font_getIndexAtLocation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 9:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TNIL) &&
                (lua_type(state, 6) == LUA_TUSERDATA || lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TNIL) &&
                (lua_type(state, 7) == LUA_TSTRING || lua_type(state, 7) == LUA_TNIL) &&
                lua_type(state, 8) == LUA_TBOOLEAN &&
                lua_type(state, 9) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param4 = gameplay::ScriptUtil::getObjectPointer<Vector2>(5, "Vector2", true, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 5 off the stack.
                bool param5Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param5 = gameplay::ScriptUtil::getObjectPointer<Vector2>(6, "Vector2", false, &param5Valid);
                if (!param5Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 5 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                // Get parameter 7 off the stack.
                bool param7 = gameplay::ScriptUtil::luaCheckBool(state, 8);

                // Get parameter 8 off the stack.
                bool param8 = gameplay::ScriptUtil::luaCheckBool(state, 9);

                Font* instance = getInstance(state);
                int result = instance->getIndexAtLocation(param1, *param2, param3, *param4, param5, param6, param7, param8);

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Font_getIndexAtLocation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL) &&
                lua_type(state, 6) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param4 = gameplay::ScriptUtil::getObjectPointer<Vector2>(5, "Vector2", false, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                Font* instance = getInstance(state);
                instance->getLocationAtIndex(param1, *param2, param3, param4, param5);
                
                return 0;
            }

            lua_pushstring(state, "lua_Font_getLocationAtIndex - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL) &&
                lua_type(state, 6) == LUA_TNUMBER &&
                (lua_type(state, 7) == LUA_TSTRING || lua_type(state, 7) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param4 = gameplay::ScriptUtil::getObjectPointer<Vector2>(5, "Vector2", false, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                Font* instance = getInstance(state);
                instance->getLocationAtIndex(param1, *param2, param3, param4, param5, param6);
                
                return 0;
            }

            lua_pushstring(state, "lua_Font_getLocationAtIndex - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 8:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL) &&
                lua_type(state, 6) == LUA_TNUMBER &&
                (lua_type(state, 7) == LUA_TSTRING || lua_type(state, 7) == LUA_TNIL) &&
                lua_type(state, 8) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param4 = gameplay::ScriptUtil::getObjectPointer<Vector2>(5, "Vector2", false, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                // Get parameter 7 off the stack.
                bool param7 = gameplay::ScriptUtil::luaCheckBool(state, 8);

                Font* instance = getInstance(state);
                instance->getLocationAtIndex(param1, *param2, param3, param4, param5, param6, param7);
                
                return 0;
            }

            lua_pushstring(state, "lua_Font_getLocationAtIndex - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 9:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER &&
                (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL) &&
                lua_type(state, 6) == LUA_TNUMBER &&
                (lua_type(state, 7) == LUA_TSTRING || lua_type(state, 7) == LUA_TNIL) &&
                lua_type(state, 8) == LUA_TBOOLEAN &&
                lua_type(state, 9) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                // Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Vector2> param4 = gameplay::ScriptUtil::getObjectPointer<Vector2>(5, "Vector2", false, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Vector2'.");
                    lua_error(state);
                }

                // Get parameter 5 off the stack.
                unsigned int param5 = (unsigned int)luaL_checkunsigned(state, 6);

                // Get parameter 6 off the stack.
                Font::Justify param6 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 7));

                // Get parameter 7 off the stack.
                bool param7 = gameplay::ScriptUtil::luaCheckBool(state, 8);

                // Get parameter 8 off the stack.
                bool param8 = gameplay::ScriptUtil::luaCheckBool(state, 9);

                Font* instance = getInstance(state);
                instance->getLocationAtIndex(param1, *param2, param3, param4, param5, param6, param7, param8);
                
                return 0;
            }

            lua_pushstring(state, "lua_Font_getLocationAtIndex - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Font* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Font_getRefCount - Failed to match the given parameters to a valid function signature.");
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

int lua_Font_getSize(lua_State* state)
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
                Font* instance = getInstance(state);
                unsigned int result = instance->getSize();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Font_getSize - Failed to match the given parameters to a valid function signature.");
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

int lua_Font_getSpriteBatch(lua_State* state)
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
                Font* instance = getInstance(state);
                void* returnPtr = (void*)instance->getSpriteBatch();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "SpriteBatch");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Font_getSpriteBatch - Failed to match the given parameters to a valid function signature.");
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

int lua_Font_measureText(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                    // Get parameter 3 off the stack.
                    gameplay::ScriptUtil::LuaArray<unsigned int> param3 = gameplay::ScriptUtil::getUnsignedIntPointer(4);

                    // Get parameter 4 off the stack.
                    gameplay::ScriptUtil::LuaArray<unsigned int> param4 = gameplay::ScriptUtil::getUnsignedIntPointer(5);

                    Font* instance = getInstance(state);
                    instance->measureText(param1, param2, param3, param4);
                    
                    return 0;
                }
            } while (0);

            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param4 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(5, "Rectangle", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    Font* instance = getInstance(state);
                    instance->measureText(param1, *param2, param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_measureText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 6:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL) &&
                    (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param4 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(5, "Rectangle", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    // Get parameter 5 off the stack.
                    Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                    Font* instance = getInstance(state);
                    instance->measureText(param1, *param2, param3, param4, param5);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_measureText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 7:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL) &&
                    (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                    lua_type(state, 7) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param4 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(5, "Rectangle", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    // Get parameter 5 off the stack.
                    Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                    // Get parameter 6 off the stack.
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 7);

                    Font* instance = getInstance(state);
                    instance->measureText(param1, *param2, param3, param4, param5, param6);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_measureText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 8:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TNIL) &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    (lua_type(state, 5) == LUA_TUSERDATA || lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TNIL) &&
                    (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                    lua_type(state, 7) == LUA_TBOOLEAN &&
                    lua_type(state, 8) == LUA_TBOOLEAN)
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", true, &param2Valid);
                    if (!param2Valid)
                        break;

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    bool param4Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param4 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(5, "Rectangle", false, &param4Valid);
                    if (!param4Valid)
                        break;

                    // Get parameter 5 off the stack.
                    Font::Justify param5 = (Font::Justify)lua_enumFromString_FontJustify(luaL_checkstring(state, 6));

                    // Get parameter 6 off the stack.
                    bool param6 = gameplay::ScriptUtil::luaCheckBool(state, 7);

                    // Get parameter 7 off the stack.
                    bool param7 = gameplay::ScriptUtil::luaCheckBool(state, 8);

                    Font* instance = getInstance(state);
                    instance->measureText(param1, *param2, param3, param4, param5, param6, param7);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Font_measureText - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                Font* instance = getInstance(state);
                instance->release();
                
                return 0;
            }

            lua_pushstring(state, "lua_Font_release - Failed to match the given parameters to a valid function signature.");
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

int lua_Font_start(lua_State* state)
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
                Font* instance = getInstance(state);
                instance->start();
                
                return 0;
            }

            lua_pushstring(state, "lua_Font_start - Failed to match the given parameters to a valid function signature.");
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

int lua_Font_static_create(lua_State* state)
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
                const char* param1 = gameplay::ScriptUtil::getString(1, false);

                void* returnPtr = (void*)Font::create(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "Font");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Font_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(1, false);

                // Get parameter 2 off the stack.
                const char* param2 = gameplay::ScriptUtil::getString(2, false);

                void* returnPtr = (void*)Font::create(param1, param2);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "Font");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_Font_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_Font_static_getJustify(lua_State* state)
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
                const char* param1 = gameplay::ScriptUtil::getString(1, false);

                Font::Justify result = Font::getJustify(param1);

                // Push the return value onto the stack.
                lua_pushstring(state, lua_stringFromEnum_FontJustify(result));

                return 1;
            }

            lua_pushstring(state, "lua_Font_static_getJustify - Failed to match the given parameters to a valid function signature.");
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

}
