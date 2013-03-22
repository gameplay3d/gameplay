#include "Base.h"
#include "ScriptController.h"
#include "lua_TileSheet.h"
#include "TileSheet.h"
#include "Camera.h"

namespace gameplay
{

void luaRegister_TileSheet()
{
    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_TileSheet_addRef},
		{"addStrip", lua_TileSheet_addStrip},
		{"finishBatch", lua_TileSheet_finishBatch},
        {"getRefCount", lua_TileSheet_getRefCount},
		{"getId", lua_TileSheet_getId},
		{"getSpriteBatch", lua_TileSheet_getSpriteBatch},
		{"getStripCount", lua_TileSheet_getStripCount},
		{"getStripFrame", lua_TileSheet_getStripFrame},
		{"getStripFrames", lua_TileSheet_getStripFrames},
		{"getStripFrameCount", lua_TileSheet_getStripFrameCount},
		{"getStripId", lua_TileSheet_getStripId},
        {"release", lua_TileSheet_release},
		{"removeStrip", lua_TileSheet_removeStrip},
		{"setStripFrame", lua_TileSheet_setStripFrame},
		{"setStripFrames", lua_TileSheet_setStripFrames},
		{"startBatch", lua_TileSheet_startBatch},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_TileSheet_static_create},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    gameplay::ScriptUtil::registerClass("TileSheet", lua_members, NULL, lua_TileSheet__gc, lua_statics, scopePath);
}

static TileSheet* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "TileSheet");
    luaL_argcheck(state, userdata != NULL, 1, "'TileSheet' expected.");
    return (TileSheet*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_TileSheet__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "TileSheet");
                luaL_argcheck(state, userdata != NULL, 1, "'TileSheet' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    TileSheet* instance = (TileSheet*)object->instance;
                    SAFE_RELEASE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_TileSheet__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_addRef(lua_State* state)
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
                TileSheet* instance = getInstance(state);
                instance->addRef();
                
                return 0;
            }

            lua_pushstring(state, "lua_TileSheet_addRef - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_addStrip(lua_State* state)
{
	// Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				(lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
				// Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
				unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

                TileSheet* instance = getInstance(state);
				unsigned int result = instance->addStrip(param1, param2);

				// Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_TileSheet_addStrip - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_finishBatch(lua_State* state)
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
                TileSheet* instance = getInstance(state);
				instance->finishBatch();
                
                return 0;
            }

            lua_pushstring(state, "lua_TileSheet_finishBatch - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_getRefCount(lua_State* state)
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
                TileSheet* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_TileSheet_getRefCount - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_getId(lua_State* state)
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
                TileSheet* instance = getInstance(state);
                const char* result = instance->getId();

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_TileSheet_getId - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_getSpriteBatch(lua_State* state)
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
                TileSheet* instance = getInstance(state);
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

            lua_pushstring(state, "lua_TileSheet_getSpriteBatch - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_getStripCount(lua_State* state)
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
                TileSheet* instance = getInstance(state);
				unsigned int result = instance->getStripCount();

                // Push the return value onto the stack.
				lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_TileSheet_getStripCount - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_getStripFrame(lua_State* state)
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
					lua_type(state, 3) == LUA_TNUMBER)
				{
					// Get parameter 1 off the stack.
					const char* param1 = gameplay::ScriptUtil::getString(2, false);

					// Get parameter 2 off the stack.
					unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

					TileSheet* instance = getInstance(state);
					void* returnPtr = (void*)&(instance->getStripFrame(param1, param2));
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
			} while (0);

			do
			{
				if ((lua_type(state, 1) == LUA_TUSERDATA) &&
					lua_type(state, 2) == LUA_TNUMBER &&
					lua_type(state, 3) == LUA_TNUMBER)
				{
					// Get parameter 1 off the stack.
					unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

					// Get parameter 2 off the stack.
					unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

					TileSheet* instance = getInstance(state);
					void* returnPtr = (void*)&(instance->getStripFrame(param1, param2));
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
			} while (0);

            lua_pushstring(state, "lua_TileSheet_getStripFrame - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_getStripFrames(lua_State* state)
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
					(lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL) &&
					lua_type(state, 5) == LUA_TNUMBER)
				{
					// Get parameter 1 off the stack.
					const char* param1 = gameplay::ScriptUtil::getString(2, false);

					// Get parameter 2 off the stack.
					unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

					// Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param3 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(4, "Rectangle", false, &param3Valid);
                    if (!param3Valid)
					{
						lua_pushstring(state, "Failed to convert parameter 3 to type 'Rectangle'.");
						lua_error(state);
                        break;
					}

					// Get parameter 4 off the stack.
					unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

					TileSheet* instance = getInstance(state);
					instance->getStripFrames(param1, param2, param3, param4);
                
					return 0;
				}
			} while (0);

			do
			{
				if ((lua_type(state, 1) == LUA_TUSERDATA) &&
					lua_type(state, 2) == LUA_TNUMBER &&
					lua_type(state, 3) == LUA_TNUMBER &&
					(lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL) &&
					lua_type(state, 5) == LUA_TNUMBER)
				{
					// Get parameter 1 off the stack.
					unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

					// Get parameter 2 off the stack.
					unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

					// Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param3 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(4, "Rectangle", false, &param3Valid);
                    if (!param3Valid)
					{
						lua_pushstring(state, "Failed to convert parameter 3 to type 'Rectangle'.");
						lua_error(state);
                        break;
					}

					// Get parameter 4 off the stack.
					unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

					TileSheet* instance = getInstance(state);
					instance->getStripFrames(param1, param2, param3, param4);
                
					return 0;
				}
			} while (0);

            lua_pushstring(state, "lua_TileSheet_getStripFrames - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_TileSheet_getStripFrameCount(lua_State* state)
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
					(lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
				{
					// Get parameter 1 off the stack.
					const char* param1 = gameplay::ScriptUtil::getString(2, false);

					TileSheet* instance = getInstance(state);
					unsigned int result = instance->getStripFrameCount(param1);

					// Push the return value onto the stack.
					lua_pushunsigned(state, result);
                
					return 1;
				}
			} while (0);

			do
			{
				if ((lua_type(state, 1) == LUA_TUSERDATA) &&
					lua_type(state, 2) == LUA_TNUMBER)
				{
					// Get parameter 1 off the stack.
					unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

					TileSheet* instance = getInstance(state);
					unsigned int result = instance->getStripFrameCount(param1);

					// Push the return value onto the stack.
					lua_pushunsigned(state, result);
                
					return 1;
				}
			} while (0);

            lua_pushstring(state, "lua_TileSheet_getStripFrameCount - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_getStripId(lua_State* state)
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
				unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

				TileSheet* instance = getInstance(state);
				const char* result = instance->getStripId(param1);

				// Push the return value onto the stack.
				lua_pushstring(state, result);
                
				return 1;
			}

            lua_pushstring(state, "lua_TileSheet_getStripId - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_release(lua_State* state)
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
                TileSheet* instance = getInstance(state);
                instance->release();
                
                return 0;
            }

            lua_pushstring(state, "lua_TileSheet_release - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_removeStrip(lua_State* state)
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
					(lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
				{
					// Get parameter 1 off the stack.
					const char* param1 = gameplay::ScriptUtil::getString(2, false);

					TileSheet* instance = getInstance(state);
					bool result = instance->removeStrip(param1);

					// Push the return value onto the stack.
					lua_pushboolean(state, result);
                
					return 1;
				}
			} while (0);

			do
			{
				if ((lua_type(state, 1) == LUA_TUSERDATA) &&
					lua_type(state, 2) == LUA_TNUMBER)
				{
					// Get parameter 1 off the stack.
					unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

					TileSheet* instance = getInstance(state);
					bool result = instance->removeStrip(param1);

					// Push the return value onto the stack.
					lua_pushboolean(state, result);
                
					return 1;
				}
			} while (0);

            lua_pushstring(state, "lua_TileSheet_removeStrip - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_setStripFrame(lua_State* state)
{
	// Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 4:
        {
			do
			{
				if ((lua_type(state, 1) == LUA_TUSERDATA) &&
					(lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL) &&
					lua_type(state, 3) == LUA_TNUMBER &&
					(lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
				{
					// Get parameter 1 off the stack.
					const char* param1 = gameplay::ScriptUtil::getString(2, false);

					// Get parameter 2 off the stack.
					unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

					// Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param3 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(4, "Rectangle", true, &param3Valid);
                    if (!param3Valid)
                        break;

					TileSheet* instance = getInstance(state);
					instance->setStripFrame(param1, param2, *param3);
                
					return 0;
				}
			} while (0);

			do
			{
				if ((lua_type(state, 1) == LUA_TUSERDATA) &&
					lua_type(state, 2) == LUA_TNUMBER &&
					lua_type(state, 3) == LUA_TNUMBER &&
					(lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TNIL))
				{
					// Get parameter 1 off the stack.
					unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

					// Get parameter 2 off the stack.
					unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

					// Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param3 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(4, "Rectangle", true, &param3Valid);
                    if (!param3Valid)
                        break;

					TileSheet* instance = getInstance(state);
					instance->setStripFrame(param1, param2, *param3);
                
					return 0;
				}
			} while (0);

            lua_pushstring(state, "lua_TileSheet_setStripFrame - Failed to match the given parameters to a valid function signature.");
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

int lua_TileSheet_setStripFrames(lua_State* state)
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
					(lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL) &&
					lua_type(state, 5) == LUA_TNUMBER)
				{
					// Get parameter 1 off the stack.
					const char* param1 = gameplay::ScriptUtil::getString(2, false);

					// Get parameter 2 off the stack.
					unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

					// Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param3 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(4, "Rectangle", false, &param3Valid);
                    if (!param3Valid)
					{
						lua_pushstring(state, "Failed to convert parameter 3 to type 'Rectangle'.");
						lua_error(state);
                        break;
					}

					// Get parameter 4 off the stack.
					unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

					TileSheet* instance = getInstance(state);
					instance->setStripFrames(param1, param2, param3, param4);
                
					return 0;
				}
			} while (0);

			do
			{
				if ((lua_type(state, 1) == LUA_TUSERDATA) &&
					lua_type(state, 2) == LUA_TNUMBER &&
					lua_type(state, 3) == LUA_TNUMBER &&
					(lua_type(state, 4) == LUA_TUSERDATA || lua_type(state, 4) == LUA_TTABLE || lua_type(state, 4) == LUA_TNIL) &&
					lua_type(state, 5) == LUA_TNUMBER)
				{
					// Get parameter 1 off the stack.
					unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

					// Get parameter 2 off the stack.
					unsigned int param2 = (unsigned int)luaL_checkunsigned(state, 3);

					// Get parameter 3 off the stack.
                    bool param3Valid;
                    gameplay::ScriptUtil::LuaArray<Rectangle> param3 = gameplay::ScriptUtil::getObjectPointer<Rectangle>(4, "Rectangle", false, &param3Valid);
                    if (!param3Valid)
					{
						lua_pushstring(state, "Failed to convert parameter 3 to type 'Rectangle'.");
						lua_error(state);
                        break;
					}

					// Get parameter 4 off the stack.
					unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 5);

					TileSheet* instance = getInstance(state);
					instance->setStripFrames(param1, param2, param3, param4);
                
					return 0;
				}
			} while (0);

            lua_pushstring(state, "lua_TileSheet_setStripFrames - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 5).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_TileSheet_startBatch(lua_State* state)
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
				TileSheet* instance = getInstance(state);
				instance->startBatch();
                
				return 0;
			}

            lua_pushstring(state, "lua_TileSheet_startBatch - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
		case 2:
        {
			if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				(lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
			{
				// Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Camera> param1 = gameplay::ScriptUtil::getObjectPointer<Camera>(2, "Camera", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Camera'.");
                    lua_error(state);
                }

				TileSheet* instance = getInstance(state);
				instance->startBatch(param1);
                
				return 0;
			}

            lua_pushstring(state, "lua_TileSheet_startBatch - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
		case 3:
        {
			if ((lua_type(state, 1) == LUA_TUSERDATA) &&
				(lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
				(lua_type(state, 3) == LUA_TBOOLEAN))
			{
				// Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Camera> param1 = gameplay::ScriptUtil::getObjectPointer<Camera>(2, "Camera", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Camera'.");
                    lua_error(state);
                }

				// Get parameter 2 off the stack.
				bool param2 = gameplay::ScriptUtil::luaCheckBool(state, 3);

				TileSheet* instance = getInstance(state);
				instance->startBatch(param1, param2);
                
				return 0;
			}

            lua_pushstring(state, "lua_TileSheet_startBatch - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
        default:
        {
            lua_pushstring(state, "Invalid number of parameters (expected 1 or 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_TileSheet_static_create(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
				(lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(1, false);

				// Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Texture> param2 = gameplay::ScriptUtil::getObjectPointer<Texture>(2, "Texture", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Texture'.");
                    lua_error(state);
                }

                void* returnPtr = (void*)TileSheet::create(param1, param2);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "TileSheet");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_TileSheet_static_create - Failed to match the given parameters to a valid function signature.");
            lua_error(state);
            break;
        }
		case 3:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
				(lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
				(lua_type(state, 3) == LUA_TNUMBER))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(1, false);

				// Get parameter 2 off the stack.
                bool param2Valid;
                gameplay::ScriptUtil::LuaArray<Texture> param2 = gameplay::ScriptUtil::getObjectPointer<Texture>(2, "Texture", false, &param2Valid);
                if (!param2Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 2 to type 'Texture'.");
                    lua_error(state);
                }

				// Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 3);

                void* returnPtr = (void*)TileSheet::create(param1, param2, param3);
                if (returnPtr)
                {
                    gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                    object->instance = returnPtr;
                    object->owns = true;
                    luaL_getmetatable(state, "TileSheet");
                    lua_setmetatable(state, -2);
                }
                else
                {
                    lua_pushnil(state);
                }

                return 1;
            }

            lua_pushstring(state, "lua_TileSheet_static_create - Failed to match the given parameters to a valid function signature.");
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

}