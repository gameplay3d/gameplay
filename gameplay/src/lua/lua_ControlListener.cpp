#include "Base.h"
#include "ScriptController.h"
#include "Control.h"
#include "lua_ControlListener.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_ControlListener()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"controlEvent", lua_ControlListener_controlEvent},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("Control");

    sc->registerClass("ControlListener", lua_members, NULL, lua_ControlListener__gc, lua_statics, scopePath);
}

static Control::Listener* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ControlListener");
    luaL_argcheck(state, userdata != NULL, 1, "'ControlListener' expected.");
    return (Control::Listener*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_ControlListener__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "ControlListener");
                luaL_argcheck(state, userdata != NULL, 1, "'ControlListener' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    Control::Listener* instance = (Control::Listener*)object->instance;
                    SAFE_DELETE(instance);
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

int lua_ControlListener_controlEvent(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Control");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Control' for parameter 2.");
                    lua_error(state);
                }
                Control* param1 = (Control*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                Control::Listener::EventType param2 = (Control::Listener::EventType)lua_enumFromString_ControlListenerEventType(luaL_checkstring(state, 3));

                Control::Listener* instance = getInstance(state);
                instance->controlEvent(param1, param2);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

}
