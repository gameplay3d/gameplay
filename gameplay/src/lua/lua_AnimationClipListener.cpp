#include "Base.h"
#include "ScriptController.h"
#include "AnimationClip.h"
#include "lua_AnimationClipListener.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_AnimationClipListener()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"animationEvent", lua_AnimationClipListener_animationEvent},
        {NULL, NULL}
    };
    const luaL_Reg* lua_statics = NULL;
    std::vector<std::string> scopePath;
    scopePath.push_back("AnimationClip");

    sc->registerClass("AnimationClipListener", lua_members, NULL, lua_AnimationClipListener__gc, lua_statics, scopePath);
}

static AnimationClip::Listener* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "AnimationClipListener");
    luaL_argcheck(state, userdata != NULL, 1, "'AnimationClipListener' expected.");
    return (AnimationClip::Listener*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_AnimationClipListener__gc(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 1:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA || lua_type(state, 1) == LUA_TNIL))
            {
                void* userdata = luaL_checkudata(state, 1, "AnimationClipListener");
                luaL_argcheck(state, userdata != NULL, 1, "'AnimationClipListener' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    AnimationClip::Listener* instance = (AnimationClip::Listener*)object->instance;
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

int lua_AnimationClipListener_animationEvent(lua_State* state)
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
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                AnimationClip* param1 = ScriptController::getInstance()->getObjectPointer<AnimationClip>(2, "AnimationClip", false);

                // Get parameter 2 off the stack.
                AnimationClip::Listener::EventType param2 = (AnimationClip::Listener::EventType)lua_enumFromString_AnimationClipListenerEventType(luaL_checkstring(state, 3));

                AnimationClip::Listener* instance = getInstance(state);
                instance->animationEvent(param1, param2);
                
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
