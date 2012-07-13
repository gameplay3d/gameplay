#include "Base.h"
#include "ScriptController.h"
#include "lua_FontGlyph.h"
#include "Base.h"
#include "Bundle.h"
#include "FileSystem.h"
#include "Font.h"
#include "Game.h"
#include "Ref.h"
#include "lua_FontJustify.h"
#include "lua_FontStyle.h"

namespace gameplay
{

void luaRegister_FontGlyph()
{
    const luaL_Reg lua_members[] = 
    {
        {"code", lua_FontGlyph_code},
        {"uvs", lua_FontGlyph_uvs},
        {"width", lua_FontGlyph_width},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("Font");

    ScriptUtil::registerClass("FontGlyph", lua_members, lua_FontGlyph__init, lua_FontGlyph__gc, lua_statics, scopePath);
}

static Font::Glyph* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "FontGlyph");
    luaL_argcheck(state, userdata != NULL, 1, "'FontGlyph' expected.");
    return (Font::Glyph*)((ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_FontGlyph__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "FontGlyph");
                luaL_argcheck(state, userdata != NULL, 1, "'FontGlyph' expected.");
                ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Font::Glyph* instance = (Font::Glyph*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "lua_FontGlyph__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_FontGlyph__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)new Font::Glyph();
            if (returnPtr)
            {
                ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "FontGlyph");
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

int lua_FontGlyph_code(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Font::Glyph* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 2);

        instance->code = param2;
        return 0;
    }
    else
    {
        unsigned int result = instance->code;

        // Push the return value onto the stack.
        lua_pushunsigned(state, result);

        return 1;
    }
}

int lua_FontGlyph_uvs(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Font::Glyph* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float* param2 = ScriptUtil::getFloatPointer(2);

        memcpy(instance->uvs, param2, sizeof(float) * 4);
        return 0;
    }
    else
    {
        float* result = instance->uvs;

        // Push the return value onto the stack.
        lua_pushlightuserdata(state, result);
        return 1;
    }
}

int lua_FontGlyph_width(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Font::Glyph* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 2);

        instance->width = param2;
        return 0;
    }
    else
    {
        unsigned int result = instance->width;

        // Push the return value onto the stack.
        lua_pushunsigned(state, result);

        return 1;
    }
}

}
