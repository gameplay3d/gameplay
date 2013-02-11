#include "Base.h"
#include "ScriptController.h"
#include "lua_ControlListener.h"
#include "Animation.h"
#include "AnimationTarget.h"
#include "Base.h"
#include "Control.h"
#include "Game.h"
#include "Node.h"
#include "Ref.h"
#include "ScriptController.h"
#include "ScriptTarget.h"
#include "lua_ControlAlignment.h"
#include "lua_ControlListenerEventType.h"
#include "lua_ControlState.h"
#include "lua_CurveInterpolationType.h"
#include "lua_FontJustify.h"

namespace gameplay
{

void luaRegister_ControlListener()
{
    const luaL_Reg lua_members[] = 
    {
        {"controlEvent", lua_ControlListener_controlEvent},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("Control");

    gameplay::ScriptUtil::registerClass("ControlListener", lua_members, NULL, lua_ControlListener__gc, lua_statics, scopePath);
}

static Control::Listener* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "ControlListener");
    luaL_argcheck(state, userdata != NULL, 1, "'ControlListener' expected.");
    return (Control::Listener*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
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
            if ((lua_type(state, 1) == LUA_TUSERDATA))
            {
                void* userdata = luaL_checkudata(state, 1, "ControlListener");
                luaL_argcheck(state, userdata != NULL, 1, "'ControlListener' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    Control::Listener* instance = (Control::Listener*)object->instance;
                    SAFE_DELETE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_ControlListener__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_ControlListener_controlEvent(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 3:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TUSERDATA || lua_type(state, 2) == LUA_TTABLE || lua_type(state, 2) == LUA_TNIL) &&
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                bool param1Valid;
                gameplay::ScriptUtil::LuaArray<Control> param1 = gameplay::ScriptUtil::getObjectPointer<Control>(2, "Control", false, &param1Valid);
                if (!param1Valid)
                {
                    lua_pushstring(state, "Failed to convert parameter 1 to type 'Control'.");
                    lua_error(state);
                }

                // Get parameter 2 off the stack.
                Control::Listener::EventType param2 = (Control::Listener::EventType)lua_enumFromString_ControlListenerEventType(luaL_checkstring(state, 3));

                Control::Listener* instance = getInstance(state);
                instance->controlEvent(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_ControlListener_controlEvent - Failed to match the given parameters to a valid function signature.");
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

}
