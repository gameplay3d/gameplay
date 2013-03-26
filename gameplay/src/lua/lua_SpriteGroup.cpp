#include "Base.h"
#include "ScriptController.h"
#include "lua_SpriteGroup.h"
#include "SpriteGroup.h"
#include "Node.h"
#include "lua_CurveInterpolationType.h"

namespace gameplay
{

void luaRegister_SpriteGroup()
{
    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_SpriteGroup_addRef},
		{"createAnimation", lua_SpriteGroup_createAnimation},
		{"createAnimationFromBy", lua_SpriteGroup_createAnimationFromBy},
		{"createAnimationFromTo", lua_SpriteGroup_createAnimationFromTo},
		{"destroyAnimation", lua_SpriteGroup_destroyAnimation},
		{"draw", lua_SpriteGroup_draw},
		{"getAnimation", lua_SpriteGroup_getAnimation},
		{"getAnimationPropertyComponentCount", lua_SpriteGroup_getAnimationPropertyComponentCount},
		{"getAnimationPropertyValue", lua_SpriteGroup_getAnimationPropertyValue},
		{"getDefaultTile", lua_SpriteGroup_getDefaultTile},
		{"getFlip", lua_SpriteGroup_getFlip},
		{"getId", lua_SpriteGroup_getId},
		{"getNode", lua_SpriteGroup_getNode},
		{"getRefCount", lua_SpriteGroup_getRefCount},
		{"getGroupHeight", lua_SpriteGroup_getGroupHeight},
		{"getGroupWidth", lua_SpriteGroup_getGroupWidth},
		{"getHeight", lua_SpriteGroup_getHeight},
		{"getHorzGap", lua_SpriteGroup_getHorzGap},
		{"getOffset", lua_SpriteGroup_getOffset},
		{"getOffsetX", lua_SpriteGroup_getOffsetX},
		{"getOffsetY", lua_SpriteGroup_getOffsetY},
		{"getSize", lua_SpriteGroup_getSize},
		{"getSprite", lua_SpriteGroup_getSprite},
		{"getSpriteType", lua_SpriteGroup_getSpriteType},
		{"getWidth", lua_SpriteGroup_getWidth},
		{"getTileSheet", lua_SpriteGroup_getTileSheet},
		{"getTint", lua_SpriteGroup_getTint},
		{"getVertGap", lua_SpriteGroup_getVertGap},
        {"release", lua_SpriteGroup_release},
		{"setAnimationPropertyValue", lua_SpriteGroup_setAnimationPropertyValue},
		{"setDefaultTile", lua_SpriteGroup_setDefaultTile},
		{"setFlip", lua_SpriteGroup_setFlip},
		{"setHorzGap", lua_SpriteGroup_setHorzGap},
		{"setNode", lua_SpriteGroup_setNode},
		{"setOffset", lua_SpriteGroup_setOffset},
		{"setOffsetX", lua_SpriteGroup_setOffsetX},
		{"setOffsetY", lua_SpriteGroup_setOffsetY},
		{"setSize", lua_SpriteGroup_setSize},
		{"setSprite", lua_SpriteGroup_setSprite},
		{"setSpriteType", lua_SpriteGroup_setSpriteType},
		{"setTint", lua_SpriteGroup_setTint},
		{"setVertGap", lua_SpriteGroup_setVertGap},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
		{"ANIMATE_FRAME_INDEX", lua_SpriteGroup_static_ANIMATE_FRAME_INDEX},
		{"ANIMATE_FRAME_SPECIFIC", lua_SpriteGroup_static_ANIMATE_FRAME_SPECIFIC},
		{"ANIMATE_OFFSET", lua_SpriteGroup_static_ANIMATE_OFFSET},
        {"ANIMATE_SIZE", lua_SpriteGroup_static_ANIMATE_SIZE},
		{"ANIMATE_TINT", lua_SpriteGroup_static_ANIMATE_TINT},
		{"create", lua_SpriteGroup_static_create},
		{"createFrom", lua_SpriteGroup_static_createFrom},
		{"FLIP_HORZ", lua_SpriteGroup_static_FLIP_HORZ},
		{"FLIP_NONE", lua_SpriteGroup_static_FLIP_NONE},
		{"FLIP_VERT", lua_SpriteGroup_static_FLIP_VERT},
		{"getDefaultSpriteOffset", lua_SpriteGroup_static_getDefaultSpriteOffset},
		{"setDefaultSpriteOffset", lua_SpriteGroup_static_setDefaultSpriteOffset},
		{"getSpriteGroup", lua_SpriteGroup_static_getSpriteGroup},
		{"TYPE_CHILD", lua_SpriteGroup_static_TYPE_CHILD},
		{"TYPE_DEFAULT", lua_SpriteGroup_static_TYPE_DEFAULT},
		{"TYPE_TRANSPARENT", lua_SpriteGroup_static_TYPE_TRANSPARENT},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("SpriteGroup", lua_members, NULL, lua_SpriteGroup__gc, lua_statics, scopePath);
}

static SpriteGroup* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "SpriteGroup");
    luaL_argcheck(state, userdata != NULL, 1, "'SpriteGroup' expected.");
    return (SpriteGroup*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_SpriteGroup__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "SpriteGroup");
                luaL_argcheck(state, userdata != NULL, 1, "'SpriteGroup' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    SpriteGroup* instance = (SpriteGroup*)object->instance;
                    SAFE_RELEASE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_addRef(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
                instance->addRef();
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_addRef - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_createAnimation(lua_State* state)
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
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    const char* param2 = gameplay::ScriptUtil::getString(3, false);

                    SpriteGroup* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
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
                if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                    (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                    (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    bool param2Valid;
                    gameplay::ScriptUtil::LuaArray<Properties> param2 = gameplay::ScriptUtil::getObjectPointer<Properties>(3, "Properties", false, &param2Valid);
                    if (!param2Valid)
                        break;

                    SpriteGroup* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteGroup_createAnimation - Failed to match the given parameters to a valid function signature.");
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
                    (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 7) == LUA_TSTRING || lua_type(state, 7) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    gameplay::ScriptUtil::LuaArray<unsigned int> param4 = gameplay::ScriptUtil::getUnsignedIntPointer(5);

                    // Get parameter 5 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param5 = gameplay::ScriptUtil::getFloatPointer(6);

                    // Get parameter 6 off the stack.
                    Curve::InterpolationType param6 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 7));

                    SpriteGroup* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2, param3, param4, param5, param6);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteGroup_createAnimation - Failed to match the given parameters to a valid function signature.");
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
                    (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 6) == LUA_TTABLE || lua_type(state, 6) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 7) == LUA_TTABLE || lua_type(state, 7) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 8) == LUA_TTABLE || lua_type(state, 8) == LUA_TLIGHTUSERDATA) &&
                    (lua_type(state, 9) == LUA_TSTRING || lua_type(state, 9) == LUA_TNIL))
                {
                    // Get parameter 1 off the stack.
                    const char* param1 = gameplay::ScriptUtil::getString(2, false);

                    // Get parameter 2 off the stack.
                    int param2 = (int)luaL_checkint(state, 3);

                    // Get parameter 3 off the stack.
                    unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                    // Get parameter 4 off the stack.
                    gameplay::ScriptUtil::LuaArray<unsigned int> param4 = gameplay::ScriptUtil::getUnsignedIntPointer(5);

                    // Get parameter 5 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param5 = gameplay::ScriptUtil::getFloatPointer(6);

                    // Get parameter 6 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param6 = gameplay::ScriptUtil::getFloatPointer(7);

                    // Get parameter 7 off the stack.
                    gameplay::ScriptUtil::LuaArray<float> param7 = gameplay::ScriptUtil::getFloatPointer(8);

                    // Get parameter 8 off the stack.
                    Curve::InterpolationType param8 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 9));

                    SpriteGroup* instance = getInstance(state);
                    void* returnPtr = (void*)instance->createAnimation(param1, param2, param3, param4, param5, param6, param7, param8);
                    if (returnPtr)
                    {
                        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                        object->instance = returnPtr;
                        object->owns = false;
                        luaL_getmetatable(state, "Animation");
                        lua_setmetatable(state, -2);
                    }
                    else
                    {
                        lua_pushnil(state);
                    }

                    return 1;
                }
            } while (0);

            lua_pushstring(state, "lua_SpriteGroup_createAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3, 7 or 9).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_SpriteGroup_createAnimationFromBy(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                lua_type(state, 7) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param3 = gameplay::ScriptUtil::getFloatPointer(4);

                // Get parameter 4 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param4 = gameplay::ScriptUtil::getFloatPointer(5);

                // Get parameter 5 off the stack.
                Curve::InterpolationType param5 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                unsigned long param6 = (unsigned long)luaL_checkunsigned(state, 7);

                SpriteGroup* instance = getInstance(state);
                void* returnPtr = (void*)instance->createAnimationFromBy(param1, param2, param3, param4, param5, param6);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_createAnimationFromBy - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_SpriteGroup_createAnimationFromTo(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 7:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER &&
                (lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 5) == LUA_TTABLE || lua_type(state, 5) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 6) == LUA_TSTRING || lua_type(state, 6) == LUA_TNIL) &&
                lua_type(state, 7) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                // Get parameter 3 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param3 = gameplay::ScriptUtil::getFloatPointer(4);

                // Get parameter 4 off the stack.
                gameplay::ScriptUtil::LuaArray<float> param4 = gameplay::ScriptUtil::getFloatPointer(5);

                // Get parameter 5 off the stack.
                Curve::InterpolationType param5 = (Curve::InterpolationType)lua_enumFromString_CurveInterpolationType(luaL_checkstring(state, 6));

                // Get parameter 6 off the stack.
                unsigned long param6 = (unsigned long)luaL_checkunsigned(state, 7);

                SpriteGroup* instance = getInstance(state);
                void* returnPtr = (void*)instance->createAnimationFromTo(param1, param2, param3, param4, param5, param6);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_createAnimationFromTo - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 7).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_SpriteGroup_destroyAnimation(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
                instance->destroyAnimation();
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_destroyAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                SpriteGroup* instance = getInstance(state);
                instance->destroyAnimation(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_destroyAnimation - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_draw(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				instance->draw();

                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_draw - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
		case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				(lua_type(state, 2) == LUA_TBOOLEAN))
            {
				// Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                SpriteGroup* instance = getInstance(state);
				instance->draw(param1);

                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_draw - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getAnimation(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
                void* returnPtr = (void*)instance->getAnimation();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getAnimation - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                SpriteGroup* instance = getInstance(state);
                void* returnPtr = (void*)instance->getAnimation(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Animation");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getAnimation - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getAnimationPropertyComponentCount(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                SpriteGroup* instance = getInstance(state);
                unsigned int result = instance->getAnimationPropertyComponentCount(param1);

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getAnimationPropertyComponentCount - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getAnimationPropertyValue(lua_State* state)
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
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<AnimationValue> param2 = gameplay::ScriptUtil::getObjectPointer<AnimationValue>(3, "AnimationValue", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'AnimationValue'.");
                    lua_error(state);
                }

                SpriteGroup* instance = getInstance(state);
                instance->getAnimationPropertyValue(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_getAnimationPropertyValue - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getDefaultTile(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				void* returnPtr = (void*)&(instance->getDefaultTile());
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
            }

            lua_pushstring(state, "lua_SpriteGroup_getDefaultTile - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getFlip(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				int result = instance->getFlip();

                // Push the return value onto the stack.
				lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getFlip - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getId(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
                const char* result = instance->getId();

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getId - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getNode(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
                void* returnPtr = (void*)instance->getNode();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Node");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getNode - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getRefCount(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getRefCount - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getGroupHeight(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				unsigned int result = instance->getGroupHeight();

                // Push the return value onto the stack.
				lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getGroupHeight - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getGroupWidth(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				unsigned int result = instance->getGroupWidth();

                // Push the return value onto the stack.
				lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getGroupWidth - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getHeight(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				float result = instance->getHeight();

                // Push the return value onto the stack.
				lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getHeight - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getHorzGap(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				float result = instance->getHorzGap();

                // Push the return value onto the stack.
				lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getHorzGap - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getOffset(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				void* returnPtr = (void*)new Vector2(instance->getOffset());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "Vector2");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getOffset - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getOffsetX(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				float result = instance->getOffsetX();

                // Push the return value onto the stack.
				lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getOffsetX - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getOffsetY(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				float result = instance->getOffsetY();

                // Push the return value onto the stack.
				lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getOffsetY - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getSize(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				void* returnPtr = (void*)new Vector2(instance->getSize());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "Vector2");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getSize - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getSprite(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				(lua_type(state, 2) == LUA_TNUMBER) &&
				(lua_type(state, 3) == LUA_TNUMBER))
            {
				// Get parameter 1 off the stack.
				unsigned int param1 = (int)luaL_checkunsigned(state, 2);

				// Get parameter 2 off the stack.
				unsigned int param2 = (int)luaL_checkunsigned(state, 3);

                SpriteGroup* instance = getInstance(state);
				void* returnPtr = (void*)instance->getSprite(param1, param2);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Sprite");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getSprite - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getSpriteType(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				(lua_type(state, 2) == LUA_TNUMBER) &&
				(lua_type(state, 3) == LUA_TNUMBER))
            {
				// Get parameter 1 off the stack.
				unsigned int param1 = (int)luaL_checkunsigned(state, 2);

				// Get parameter 2 off the stack.
				unsigned int param2 = (int)luaL_checkunsigned(state, 3);

                SpriteGroup* instance = getInstance(state);
				int result = instance->getSpriteType(param1, param2);

				// Push the return value onto the stack.
				lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getSpriteType - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getWidth(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				float result = instance->getWidth();

                // Push the return value onto the stack.
				lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getWidth - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getTileSheet(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
                void* returnPtr = (void*)instance->getTileSheet();
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "TileSheet");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getTileSheet - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getTint(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				void* returnPtr = (void*)&(instance->getTint());
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "Vector4");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getTint - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_getVertGap(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
				float result = instance->getVertGap();

                // Push the return value onto the stack.
				lua_pushnumber(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_getVertGap - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_release(lua_State* state)
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
                SpriteGroup* instance = getInstance(state);
                instance->release();
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_release - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_setAnimationPropertyValue(lua_State* state)
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
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<AnimationValue> param2 = gameplay::ScriptUtil::getObjectPointer<AnimationValue>(3, "AnimationValue", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'AnimationValue'.");
                    lua_error(state);
                }

                SpriteGroup* instance = getInstance(state);
                instance->setAnimationPropertyValue(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setAnimationPropertyValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TNUMBER &&
                (lua_type(state, 3) == LUA_TUSERDATA || lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TNIL) &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                int param1 = (int)luaL_checkint(state, 2);

                // Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<AnimationValue> param2 = gameplay::ScriptUtil::getObjectPointer<AnimationValue>(3, "AnimationValue", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'AnimationValue'.");
                    lua_error(state);
                }

                // Get parameter 3 off the stack.
                float param3 = (float)luaL_checknumber(state, 4);

                SpriteGroup* instance = getInstance(state);
                instance->setAnimationPropertyValue(param1, param2, param3);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setAnimationPropertyValue - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 3 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_SpriteGroup_setDefaultTile(lua_State* state)
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

					SpriteGroup* instance = getInstance(state);
					instance->setDefaultTile(*param1);
                
					return 0;
				}
			} while (0);

            lua_pushstring(state, "lua_SpriteGroup_setDefaultTile - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_setFlip(lua_State* state)
{
	// Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
				int param1 = (int)luaL_checkint(state, 2);

                SpriteGroup* instance = getInstance(state);
				instance->setFlip(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setFlip - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_setHorzGap(lua_State* state)
{
	// Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
				float param1 = (float)luaL_checknumber(state, 2);

                SpriteGroup* instance = getInstance(state);
				instance->setHorzGap(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setHorzGap - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_setNode(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Node> param1 = gameplay::ScriptUtil::getObjectPointer<Node>(2, "Node", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Node'.");
                    lua_error(state);
                }

                SpriteGroup* instance = getInstance(state);
                instance->setNode(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setNode - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_setOffset(lua_State* state)
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
					gameplay::ScriptUtil::LuaArray<Vector2> param1 = gameplay::ScriptUtil::getObjectPointer<Vector2>(2, "Vector2", true, &param1Valid);
					if (!param1Valid)
						break;

					SpriteGroup* instance = getInstance(state);
					instance->setOffset(*param1);
                
					return 0;
				}
			} while (0);

            lua_pushstring(state, "lua_SpriteGroup_setOffset - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
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

                SpriteGroup* instance = getInstance(state);
				instance->setOffset(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setOffset - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_SpriteGroup_setOffsetX(lua_State* state)
{
	// Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
				float param1 = (float)luaL_checknumber(state, 2);

                SpriteGroup* instance = getInstance(state);
				instance->setOffsetX(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setOffsetX - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_setOffsetY(lua_State* state)
{
	// Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
				float param1 = (float)luaL_checknumber(state, 2);

                SpriteGroup* instance = getInstance(state);
				instance->setOffsetY(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setOffsetY - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_setSize(lua_State* state)
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
					gameplay::ScriptUtil::LuaArray<Vector2> param1 = gameplay::ScriptUtil::getObjectPointer<Vector2>(2, "Vector2", true, &param1Valid);
					if (!param1Valid)
						break;

					SpriteGroup* instance = getInstance(state);
					instance->setSize(*param1);
                
					return 0;
				}
			} while (0);

            lua_pushstring(state, "lua_SpriteGroup_setSize - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
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

                SpriteGroup* instance = getInstance(state);
				instance->setSize(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setSize - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
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

int lua_SpriteGroup_setSprite(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				(lua_type(state, 2) == LUA_TNUMBER) &&
				(lua_type(state, 3) == LUA_TNUMBER) &&
                (lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
            {
				// Get parameter 1 off the stack.
				unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

				// Get parameter 2 off the stack.
				unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                // Get parameter 3 off the stack.
                bool param3Valid;
                gameplay::ScriptUtil::LuaArray<Sprite> param3 = gameplay::ScriptUtil::getObjectPointer<Sprite>(4, "Sprite", false, &param3Valid);
                if (!param3Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 3 to type 'Sprite'.");
                    lua_error(state);
                }

                SpriteGroup* instance = getInstance(state);
				bool result = instance->setSprite(param1, param2, param3);

				// Push the return value onto the stack.
				lua_pushboolean(state, result);
                
                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_setSprite - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_SpriteGroup_setSpriteType(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				(lua_type(state, 2) == LUA_TNUMBER) &&
				(lua_type(state, 3) == LUA_TNUMBER) &&
                (lua_type(state, 4) == LUA_TNUMBER))
            {
				// Get parameter 1 off the stack.
				unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

				// Get parameter 2 off the stack.
				unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

				// Get parameter 3 off the stack.
				int param3 = (int)luaL_checkinteger(state, 4);

                SpriteGroup* instance = getInstance(state);
				bool result = instance->setSpriteType(param1, param2, param3);

				// Push the return value onto the stack.
				lua_pushboolean(state, result);
                
                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_setSpriteType - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_SpriteGroup_setTint(lua_State* state)
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
					gameplay::ScriptUtil::LuaArray<Vector4> param1 = gameplay::ScriptUtil::getObjectPointer<Vector4>(2, "Vector4", true, &param1Valid);
					if (!param1Valid)
						break;

					SpriteGroup* instance = getInstance(state);
					instance->setTint(*param1);
                
					return 0;
				}
			} while (0);

            lua_pushstring(state, "lua_SpriteGroup_setTint - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_setVertGap(lua_State* state)
{
	// Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
				float param1 = (float)luaL_checknumber(state, 2);

                SpriteGroup* instance = getInstance(state);
				instance->setVertGap(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_SpriteGroup_setVertGap - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_static_ANIMATE_FRAME_INDEX(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::ANIMATE_FRAME_INDEX;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_ANIMATE_FRAME_SPECIFIC(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::ANIMATE_FRAME_SPECIFIC;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_ANIMATE_OFFSET(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::ANIMATE_OFFSET;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_ANIMATE_SIZE(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::ANIMATE_SIZE;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_ANIMATE_TINT(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::ANIMATE_TINT;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_create(lua_State* state)
{
	// Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
				lua_type(state, 2) == LUA_TNUMBER &&
				lua_type(state, 3) == LUA_TNUMBER &&
				(lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(1, false);

				// Get parameter 2 off the stack.
				unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 2);

				// Get parameter 3 off the stack.
				unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

				// Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<TileSheet> param4 = gameplay::ScriptUtil::getObjectPointer<TileSheet>(4, "TileSheet", false, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'TileSheet'.");
                    lua_error(state);
                }

				void* returnPtr = (void*)SpriteGroup::create(param1, param2, param3, param4);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "SpriteGroup");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_SpriteGroup_static_createFrom(lua_State* state)
{
	// Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
				lua_type(state, 2) == LUA_TNUMBER &&
				lua_type(state, 3) == LUA_TNUMBER &&
				(lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(1, false);

				// Get parameter 2 off the stack.
				unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 2);

				// Get parameter 3 off the stack.
				unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

				// Get parameter 4 off the stack.
                bool param4Valid;
                gameplay::ScriptUtil::LuaArray<Sprite> param4 = gameplay::ScriptUtil::getObjectPointer<Sprite>(4, "Sprite", false, &param4Valid);
                if (!param4Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 4 to type 'Sprite'.");
                    lua_error(state);
                }

				void* returnPtr = (void*)SpriteGroup::createFrom(param1, param2, param3, param4);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "SpriteGroup");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_static_createFrom - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_SpriteGroup_static_FLIP_HORZ(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::FLIP_HORZ;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_FLIP_NONE(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::FLIP_NONE;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_FLIP_VERT(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::FLIP_VERT;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_getDefaultSpriteOffset(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	// Push the return value onto the stack.
	void* returnPtr = (void*)&(SpriteGroup::getDefaultSpriteOffset());
    if (returnPtr)
    {
        gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
        object->instance = returnPtr;
        object->owns = false;
        luaL_getmetatable(state, "Vector2");
        lua_setmetatable(state, -2);
    }
    else
    {
        lua_pushnil(state);
    }

    return 1;
}

int lua_SpriteGroup_static_setDefaultSpriteOffset(lua_State* state)
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
				if ((lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TNIL))
				{
					// Get parameter 1 off the stack.
					bool param1Valid;
					gameplay::ScriptUtil::LuaArray<Vector2> param1 = gameplay::ScriptUtil::getObjectPointer<Vector2>(2, "Vector2", true, &param1Valid);
					if (!param1Valid)
						break;

					SpriteGroup::setDefaultSpriteOffset(*param1);
                
					return 0;
				}
			} while (0);

            lua_pushstring(state, "lua_SpriteGroup_static_setDefaultSpriteOffset - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_static_getSpriteGroup(lua_State* state)
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

				void* returnPtr = (void*)SpriteGroup::getSpriteGroup(param1);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = false;
                    luaL_getmetatable(state, "SpriteGroup");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_SpriteGroup_static_getSpriteGroup - Failed to match the given parameters to a valid function signature.");
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

int lua_SpriteGroup_static_TYPE_CHILD(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::TYPE_CHILD;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_TYPE_DEFAULT(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::TYPE_DEFAULT;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

int lua_SpriteGroup_static_TYPE_TRANSPARENT(lua_State* state)
{
	// Validate the number of parameters.
    if (lua_gettop(state) > 0)
    {
        lua_pushstring(state, "Invalid number of parameters (expected 0).");
        lua_error(state);
    }

	int result = SpriteGroup::TYPE_TRANSPARENT;

    // Push the return value onto the stack.
    lua_pushinteger(state, result);

    return 1;
}

}