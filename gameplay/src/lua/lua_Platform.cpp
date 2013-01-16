#include "Base.h"
#include "ScriptController.h"
#include "lua_Platform.h"
#include "Platform.h"
#include "lua_GestureGestureEvent.h"
#include "lua_KeyboardKeyEvent.h"
#include "lua_MouseMouseEvent.h"
#include "lua_TouchTouchEvent.h"

namespace gameplay
{

void luaRegister_Platform()
{
    const luaL_Reg lua_members[] = 
    {
        {"enterMessagePump", lua_Platform_enterMessagePump},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"canExit", lua_Platform_static_canExit},
        {"displayKeyboard", lua_Platform_static_displayKeyboard},
        {"getAbsoluteTime", lua_Platform_static_getAbsoluteTime},
        {"getAccelerometerValues", lua_Platform_static_getAccelerometerValues},
        {"getDisplayHeight", lua_Platform_static_getDisplayHeight},
        {"getDisplayWidth", lua_Platform_static_getDisplayWidth},
        {"hasMouse", lua_Platform_static_hasMouse},
        {"isCursorVisible", lua_Platform_static_isCursorVisible},
        {"isGestureRegistered", lua_Platform_static_isGestureRegistered},
        {"isGestureSupported", lua_Platform_static_isGestureSupported},
        {"isMouseCaptured", lua_Platform_static_isMouseCaptured},
        {"isMultiTouch", lua_Platform_static_isMultiTouch},
        {"isVsync", lua_Platform_static_isVsync},
        {"keyEventInternal", lua_Platform_static_keyEventInternal},
        {"launchURL", lua_Platform_static_launchURL},
        {"mouseEventInternal", lua_Platform_static_mouseEventInternal},
        {"pollGamepadState", lua_Platform_static_pollGamepadState},
        {"registerGesture", lua_Platform_static_registerGesture},
        {"setAbsoluteTime", lua_Platform_static_setAbsoluteTime},
        {"setCursorVisible", lua_Platform_static_setCursorVisible},
        {"setMouseCaptured", lua_Platform_static_setMouseCaptured},
        {"setMultiTouch", lua_Platform_static_setMultiTouch},
        {"setVsync", lua_Platform_static_setVsync},
        {"signalShutdown", lua_Platform_static_signalShutdown},
        {"sleep", lua_Platform_static_sleep},
        {"swapBuffers", lua_Platform_static_swapBuffers},
        {"touchEventInternal", lua_Platform_static_touchEventInternal},
        {"unregisterGesture", lua_Platform_static_unregisterGesture},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    ScriptUtil::registerClass("Platform", lua_members, NULL, lua_Platform__gc, lua_statics, scopePath);
}

static Platform* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Platform");
    luaL_argcheck(state, userdata != NULL, 1, "'Platform' expected.");
    return (Platform*)((ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_Platform__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Platform");
                luaL_argcheck(state, userdata != NULL, 1, "'Platform' expected.");
                ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Platform* instance = (Platform*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_enterMessagePump(lua_State* state)
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
                Platform* instance = getInstance(state);
                int result = instance->enterMessagePump();

                // Push the return value onto the stack.
                lua_pushinteger(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Platform_enterMessagePump - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_canExit(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            bool result = Platform::canExit();

            // Push the return value onto the stack.
            lua_pushboolean(state, result);

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

int lua_Platform_static_displayKeyboard(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = ScriptUtil::luaCheckBool(state, 1);

                Platform::displayKeyboard(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_displayKeyboard - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_getAbsoluteTime(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            double result = Platform::getAbsoluteTime();

            // Push the return value onto the stack.
            lua_pushnumber(state, result);

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

int lua_Platform_static_getAccelerometerValues(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TLIGHTUSERDATA) &&
                (lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TLIGHTUSERDATA))
            {
                // Get parameter 1 off the stack.
                ScriptUtil::LuaArray<float> param1 = ScriptUtil::getFloatPointer(1);

                // Get parameter 2 off the stack.
                ScriptUtil::LuaArray<float> param2 = ScriptUtil::getFloatPointer(2);

                Platform::getAccelerometerValues(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_getAccelerometerValues - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_getDisplayHeight(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            unsigned int result = Platform::getDisplayHeight();

            // Push the return value onto the stack.
            lua_pushunsigned(state, result);

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

int lua_Platform_static_getDisplayWidth(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            unsigned int result = Platform::getDisplayWidth();

            // Push the return value onto the stack.
            lua_pushunsigned(state, result);

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

int lua_Platform_static_hasMouse(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            bool result = Platform::hasMouse();

            // Push the return value onto the stack.
            lua_pushboolean(state, result);

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

int lua_Platform_static_isCursorVisible(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            bool result = Platform::isCursorVisible();

            // Push the return value onto the stack.
            lua_pushboolean(state, result);

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

int lua_Platform_static_isGestureRegistered(lua_State* state)
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
                Gesture::GestureEvent param1 = (Gesture::GestureEvent)lua_enumFromString_GestureGestureEvent(luaL_checkstring(state, 1));

                bool result = Platform::isGestureRegistered(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Platform_static_isGestureRegistered - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_isGestureSupported(lua_State* state)
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
                Gesture::GestureEvent param1 = (Gesture::GestureEvent)lua_enumFromString_GestureGestureEvent(luaL_checkstring(state, 1));

                bool result = Platform::isGestureSupported(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Platform_static_isGestureSupported - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_isMouseCaptured(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            bool result = Platform::isMouseCaptured();

            // Push the return value onto the stack.
            lua_pushboolean(state, result);

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

int lua_Platform_static_isMultiTouch(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            bool result = Platform::isMultiTouch();

            // Push the return value onto the stack.
            lua_pushboolean(state, result);

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

int lua_Platform_static_isVsync(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            bool result = Platform::isVsync();

            // Push the return value onto the stack.
            lua_pushboolean(state, result);

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

int lua_Platform_static_keyEventInternal(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TSTRING || lua_type(state, 1) == LUA_TNIL) &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                Keyboard::KeyEvent param1 = (Keyboard::KeyEvent)lua_enumFromString_KeyboardKeyEvent(luaL_checkstring(state, 1));

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 2);

                Platform::keyEventInternal(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_keyEventInternal - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_launchURL(lua_State* state)
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
                const char* param1 = ScriptUtil::getString(1, false);

                bool result = Platform::launchURL(param1);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Platform_static_launchURL - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_mouseEventInternal(lua_State* state)
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
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                Mouse::MouseEvent param1 = (Mouse::MouseEvent)lua_enumFromString_MouseMouseEvent(luaL_checkstring(state, 1));

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 2);

                // Get parameter 3 off the stack.
                int param3 = (int)luaL_checkint(state, 3);

                // Get parameter 4 off the stack.
                int param4 = (int)luaL_checkint(state, 4);

                bool result = Platform::mouseEventInternal(param1, param2, param3, param4);

                // Push the return value onto the stack.
                lua_pushboolean(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_Platform_static_mouseEventInternal - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_pollGamepadState(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TTABLE || lua_type(state, 1) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                ScriptUtil::LuaArray<Gamepad> param1 = ScriptUtil::getObjectPointer<Gamepad>(1, "Gamepad", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Gamepad'.");
                    lua_error(state);
                }

                Platform::pollGamepadState(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_pollGamepadState - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_registerGesture(lua_State* state)
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
                Gesture::GestureEvent param1 = (Gesture::GestureEvent)lua_enumFromString_GestureGestureEvent(luaL_checkstring(state, 1));

                Platform::registerGesture(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_registerGesture - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_setAbsoluteTime(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                double param1 = (double)luaL_checknumber(state, 1);

                Platform::setAbsoluteTime(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_setAbsoluteTime - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_setCursorVisible(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = ScriptUtil::luaCheckBool(state, 1);

                Platform::setCursorVisible(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_setCursorVisible - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_setMouseCaptured(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = ScriptUtil::luaCheckBool(state, 1);

                Platform::setMouseCaptured(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_setMouseCaptured - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_setMultiTouch(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = ScriptUtil::luaCheckBool(state, 1);

                Platform::setMultiTouch(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_setMultiTouch - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_setVsync(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = ScriptUtil::luaCheckBool(state, 1);

                Platform::setVsync(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_setVsync - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_signalShutdown(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            Platform::signalShutdown();
            
            return 0;
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

int lua_Platform_static_sleep(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if (lua_type(state, 1) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                long param1 = (long)luaL_checklong(state, 1);

                Platform::sleep(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_sleep - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_swapBuffers(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            Platform::swapBuffers();
            
            return 0;
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

int lua_Platform_static_touchEventInternal(lua_State* state)
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
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                Touch::TouchEvent param1 = (Touch::TouchEvent)lua_enumFromString_TouchTouchEvent(luaL_checkstring(state, 1));

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 2);

                // Get parameter 3 off the stack.
                int param3 = (int)luaL_checkint(state, 3);

                // Get parameter 4 off the stack.
                unsigned int param4 = (unsigned int)luaL_checkunsigned(state, 4);

                Platform::touchEventInternal(param1, param2, param3, param4);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_touchEventInternal - Failed to match the given parameters to a valid function signature.");
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

int lua_Platform_static_unregisterGesture(lua_State* state)
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
                Gesture::GestureEvent param1 = (Gesture::GestureEvent)lua_enumFromString_GestureGestureEvent(luaL_checkstring(state, 1));

                Platform::unregisterGesture(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_Platform_static_unregisterGesture - Failed to match the given parameters to a valid function signature.");
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
