#include "Base.h"
#include "ScriptController.h"
#include "lua_Rectangle.h"
#include "Base.h"
#include "Rectangle.h"

namespace gameplay
{

void luaRegister_Rectangle()
{
    const luaL_Reg lua_members[] = 
    {
        {"bottom", lua_Rectangle_bottom},
        {"contains", lua_Rectangle_contains},
        {"height", lua_Rectangle_height},
        {"inflate", lua_Rectangle_inflate},
        {"intersects", lua_Rectangle_intersects},
        {"isEmpty", lua_Rectangle_isEmpty},
        {"left", lua_Rectangle_left},
        {"right", lua_Rectangle_right},
        {"set", lua_Rectangle_set},
        {"setPosition", lua_Rectangle_setPosition},
        {"top", lua_Rectangle_top},
        {"width", lua_Rectangle_width},
        {"x", lua_Rectangle_x},
        {"y", lua_Rectangle_y},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"combine", lua_Rectangle_static_combine},
        {"empty", lua_Rectangle_static_empty},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("Rectangle", lua_members, lua_Rectangle__init, lua_Rectangle__gc, lua_statics, scopePath);
}

static Rectangle* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Rectangle");
    luaL_argcheck(state, userdata != NULL, 1, "'Rectangle' expected.");
    return (Rectangle*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Rectangle__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Rectangle");
                luaL_argcheck(state, userdata != NULL, 1, "'Rectangle' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Rectangle* instance = (Rectangle*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_Rectangle__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_Rectangle__init(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)new Rectangle();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "Rectangle");
                lua_setmetatable(state, -2);
            }
            else
            {
                lua_pushnil(state);
            }

            return 1;
            break;
        }
        case 1:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(1, "Rectangle", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    void* returnPtr = (void*)new Rectangle(*param1);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Rectangle");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 1);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    void* returnPtr = (void*)new Rectangle(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Rectangle");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            do
            {
                if (lua_type(state, 1) == LUA_TNUMBER &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 1);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 2);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 3);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 4);

                    void* returnPtr = (void*)new Rectangle(param1, param2, param3, param4);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = true;
                        luaL_getmetatable(state, "Rectangle");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle__init - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 0, 1, 2 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Rectangle_bottom(lua_State* state)
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
                Rectangle* instance = getInstance(state);
                float result = instance->bottom();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Rectangle_bottom - Failed to match the given parameters to a valid function signature.");
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

int lua_Rectangle_contains(lua_State* state)
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
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    Rectangle* instance = getInstance(state);
                    bool result = instance->contains(*param1);

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle_contains - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 3:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 3);

                    Rectangle* instance = getInstance(state);
                    bool result = instance->contains(param1, param2);

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle_contains - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 3);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 4);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 5);

                    Rectangle* instance = getInstance(state);
                    bool result = instance->contains(param1, param2, param3, param4);

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle_contains - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2, 3 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Rectangle_height(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Rectangle* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->height = param2;
        return 0;
    }
    else
    {
        float result = instance->height;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_Rectangle_inflate(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                Rectangle* instance = getInstance(state);
                instance->inflate(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Rectangle_inflate - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Rectangle_intersects(lua_State* state)
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
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    Rectangle* instance = getInstance(state);
                    bool result = instance->intersects(*param1);

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle_intersects - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 3);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 4);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 5);

                    Rectangle* instance = getInstance(state);
                    bool result = instance->intersects(param1, param2, param3, param4);

                    // Push the return value onto the stack.
                    lua_pushboolean(state, result);

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle_intersects - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Rectangle_isEmpty(lua_State* state)
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
                Rectangle* instance = getInstance(state);
                bool result = instance->isEmpty();

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Rectangle_isEmpty - Failed to match the given parameters to a valid function signature.");
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

int lua_Rectangle_left(lua_State* state)
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
                Rectangle* instance = getInstance(state);
                float result = instance->left();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Rectangle_left - Failed to match the given parameters to a valid function signature.");
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

int lua_Rectangle_right(lua_State* state)
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
                Rectangle* instance = getInstance(state);
                float result = instance->right();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Rectangle_right - Failed to match the given parameters to a valid function signature.");
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

int lua_Rectangle_set(lua_State* state)
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
                    (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    bool param1Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param1Valid);
                    if (!param1Valid)
                        break;

                    Rectangle* instance = getInstance(state);
                    instance->set(*param1);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle_set - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 5:
        {
            do
            {
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    lua_type(state, 2) == LUA_TNUMBER &&
                    lua_type(state, 3) == LUA_TNUMBER &&
                    lua_type(state, 4) == LUA_TNUMBER &&
                    lua_type(state, 5) == LUA_TNUMBER)
                {
                    // Get parameter 1 off the stack.
                    float param1 = (float)luaL_checknumber(state, 2);

                    // Get parameter 2 off the stack.
                    float param2 = (float)luaL_checknumber(state, 3);

                    // Get parameter 3 off the stack.
                    float param3 = (float)luaL_checknumber(state, 4);

                    // Get parameter 4 off the stack.
                    float param4 = (float)luaL_checknumber(state, 5);

                    Rectangle* instance = getInstance(state);
                    instance->set(param1, param2, param3, param4);
                    
                    return 0;
                }
            } while (0);

            lua_pushstring(state, "lua_Rectangle_set - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Rectangle_setPosition(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                float param1 = (float)luaL_checknumber(state, 2);

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                Rectangle* instance = getInstance(state);
                instance->setPosition(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Rectangle_setPosition - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Rectangle_static_combine(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param1 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(1, "Rectangle", true, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param2 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(2, "Rectangle", true, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Rectangle'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Rectangle> param3 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(3, "Rectangle", false, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Rectangle'.");
                    lua_error(state);
                }

                Rectangle::combine(*param1, *param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_Rectangle_static_combine - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Rectangle_static_empty(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)&(Rectangle::empty());
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = false;
                luaL_getmetatable(state, "Rectangle");
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

int lua_Rectangle_top(lua_State* state)
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
                Rectangle* instance = getInstance(state);
                float result = instance->top();

                // Push the return value onto the stack.
                lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Rectangle_top - Failed to match the given parameters to a valid function signature.");
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

int lua_Rectangle_width(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Rectangle* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->width = param2;
        return 0;
    }
    else
    {
        float result = instance->width;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_Rectangle_x(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Rectangle* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->x = param2;
        return 0;
    }
    else
    {
        float result = instance->x;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

int lua_Rectangle_y(lua_State* state)
{
    // Validate the number of parameters.
    if (lua_gettop(state) > 2)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 1 or 2).");
        lua_error(state);
    }

    Rectangle* instance = getInstance(state);
    if (lua_gettop(state) == 2)
    {
        // Get parameter 2 off the stack.
        float param2 = (float)luaL_checknumber(state, 2);

        instance->y = param2;
        return 0;
    }
    else
    {
        float result = instance->y;

        // Push the return value onto the stack.
        lua_pushnumber(state, result);

        return 1;
    }
}

}
