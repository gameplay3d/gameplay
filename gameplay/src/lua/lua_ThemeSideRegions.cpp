#include "Base.h"
#include "ScriptController.h"
#include "lua_ThemeSideRegions.h"
#include "Base.h"
#include "Game.h"
#include "Ref.h"
#include "Theme.h"
#include "ThemeStyle.h"

namespace gameplay
{

void luaRegister_ThemeSideRegions()
{
    const luaL_Reg lua_members[] = 
    {
        {"bottom", lua_ThemeSideRegions_bottom},
        {"left", lua_ThemeSideRegions_left},
        {"right", lua_ThemeSideRegions_right},
        {"top", lua_ThemeSideRegions_top},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"empty", lua_ThemeSideRegions_static_empty},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;
    scopePath.push_back("Theme");

    gameplay::ScriptUtil::registerClass("ThemeSideRegions", lua_members, lua_ThemeSideRegions__init, lua_ThemeSideRegions__gc, lua_statics, scopePath);
}

static Theme::SideRegions* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ThemeSideRegions");
    luaL_argcheck(state, userdata != NULL, 1, "'ThemeSideRegions' expected.");
    return (Theme::SideRegions*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_ThemeSideRegions__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "ThemeSideRegions");
                luaL_argcheck(state, userdata != NULL, 1, "'ThemeSideRegions' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Theme::SideRegions* instance = (Theme::SideRegions*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_ThemeSideRegions__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_ThemeSideRegions__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)new Theme::SideRegions();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "ThemeSideRegions");
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

int lua_ThemeSideRegions_bottom(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Theme::SideRegions* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->bottom = param2;
        return 0;
    }
    else
    {
        float result = instance->bottom;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_ThemeSideRegions_left(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Theme::SideRegions* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->left = param2;
        return 0;
    }
    else
    {
        float result = instance->left;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_ThemeSideRegions_right(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Theme::SideRegions* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->right = param2;
        return 0;
    }
    else
    {
        float result = instance->right;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_ThemeSideRegions_static_empty(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)&(Theme::SideRegions::empty());
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = false;
                luaL_getmetatable(state, "ThemeSideRegions");
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

int lua_ThemeSideRegions_top(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Theme::SideRegions* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->top = param2;
        return 0;
    }
    else
    {
        float result = instance->top;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

}
