#include "Base.h"
#include "ScriptController.h"
#include "lua_RenderStateStateBlock.h"
#include "Base.h"
#include "Game.h"
#include "Node.h"
#include "Pass.h"
#include "Ref.h"
#include "RenderState.h"
#include "Scene.h"
#include "Technique.h"
#include "lua_RenderStateAutoBinding.h"
#include "lua_RenderStateBlend.h"
#include "lua_RenderStateCullFaceSide.h"
#include "lua_RenderStateDepthFunction.h"

namespace gameplay
{

void luaRegister_RenderStateStateBlock()
{
    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_RenderStateStateBlock_addRef},
        {"bind", lua_RenderStateStateBlock_bind},
        {"getRefCount", lua_RenderStateStateBlock_getRefCount},
        {"release", lua_RenderStateStateBlock_release},
        {"setBlend", lua_RenderStateStateBlock_setBlend},
        {"setBlendDst", lua_RenderStateStateBlock_setBlendDst},
        {"setBlendSrc", lua_RenderStateStateBlock_setBlendSrc},
        {"setCullFace", lua_RenderStateStateBlock_setCullFace},
        {"setCullFaceSide", lua_RenderStateStateBlock_setCullFaceSide},
        {"setDepthFunction", lua_RenderStateStateBlock_setDepthFunction},
        {"setDepthTest", lua_RenderStateStateBlock_setDepthTest},
        {"setDepthWrite", lua_RenderStateStateBlock_setDepthWrite},
        {"setState", lua_RenderStateStateBlock_setState},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"create", lua_RenderStateStateBlock_static_create},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;
    scopePath.push_back("RenderState");

    gameplay::ScriptUtil::registerClass("RenderStateStateBlock", lua_members, NULL, lua_RenderStateStateBlock__gc, lua_statics, scopePath);
}

static RenderState::StateBlock* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "RenderStateStateBlock");
    luaL_argcheck(state, userdata != NULL, 1, "'RenderStateStateBlock' expected.");
    return (RenderState::StateBlock*)((gameplay::ScriptUtil::LuaObject*)userdata)->instance;
}

int lua_RenderStateStateBlock__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "RenderStateStateBlock");
                luaL_argcheck(state, userdata != NULL, 1, "'RenderStateStateBlock' expected.");
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)userdata;
                if (object->owns)
                {
                    RenderState::StateBlock* instance = (RenderState::StateBlock*)object->instance;
                    SAFE_RELEASE(instance);
                }
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock__gc - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_addRef(lua_State* state)
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
                RenderState::StateBlock* instance = getInstance(state);
                instance->addRef();
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_addRef - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_bind(lua_State* state)
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
                RenderState::StateBlock* instance = getInstance(state);
                instance->bind();
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_bind - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_getRefCount(lua_State* state)
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
                RenderState::StateBlock* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_getRefCount - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_release(lua_State* state)
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
                RenderState::StateBlock* instance = getInstance(state);
                instance->release();
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_release - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_setBlend(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                RenderState::StateBlock* instance = getInstance(state);
                instance->setBlend(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_setBlend - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_setBlendDst(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                RenderState::Blend param1 = (RenderState::Blend)lua_enumFromString_RenderStateBlend(luaL_checkstring(state, 2));

                RenderState::StateBlock* instance = getInstance(state);
                instance->setBlendDst(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_setBlendDst - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_setBlendSrc(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                RenderState::Blend param1 = (RenderState::Blend)lua_enumFromString_RenderStateBlend(luaL_checkstring(state, 2));

                RenderState::StateBlock* instance = getInstance(state);
                instance->setBlendSrc(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_setBlendSrc - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_setCullFace(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                RenderState::StateBlock* instance = getInstance(state);
                instance->setCullFace(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_setCullFace - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_setCullFaceSide(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                RenderState::CullFaceSide param1 = (RenderState::CullFaceSide)lua_enumFromString_RenderStateCullFaceSide(luaL_checkstring(state, 2));

                RenderState::StateBlock* instance = getInstance(state);
                instance->setCullFaceSide(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_setCullFaceSide - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_setDepthFunction(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                (lua_type(state, 2) == LUA_TSTRING || lua_type(state, 2) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                RenderState::DepthFunction param1 = (RenderState::DepthFunction)lua_enumFromString_RenderStateDepthFunction(luaL_checkstring(state, 2));

                RenderState::StateBlock* instance = getInstance(state);
                instance->setDepthFunction(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_setDepthFunction - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_setDepthTest(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                RenderState::StateBlock* instance = getInstance(state);
                instance->setDepthTest(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_setDepthTest - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_setDepthWrite(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if ((lua_type(state, 1) == LUA_TUSERDATA) &&
                lua_type(state, 2) == LUA_TBOOLEAN)
            {
                // Get parameter 1 off the stack.
                bool param1 = gameplay::ScriptUtil::luaCheckBool(state, 2);

                RenderState::StateBlock* instance = getInstance(state);
                instance->setDepthWrite(param1);
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_setDepthWrite - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_setState(lua_State* state)
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
                (lua_type(state, 3) == LUA_TSTRING || lua_type(state, 3) == LUA_TNIL))
            {
                // Get parameter 1 off the stack.
                const char* param1 = gameplay::ScriptUtil::getString(2, false);

                // Get parameter 2 off the stack.
                const char* param2 = gameplay::ScriptUtil::getString(3, false);

                RenderState::StateBlock* instance = getInstance(state);
                instance->setState(param1, param2);
                
                return 0;
            }

            lua_pushstring(state, "lua_RenderStateStateBlock_setState - Failed to match the given parameters to a valid function signature.");
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

int lua_RenderStateStateBlock_static_create(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            void* returnPtr = (void*)RenderState::StateBlock::create();
            if (returnPtr)
            {
                gameplay::ScriptUtil::LuaObject* object = (gameplay::ScriptUtil::LuaObject*)lua_newuserdata(state, sizeof(gameplay::ScriptUtil::LuaObject));
                object->instance = returnPtr;
                object->owns = true;
                luaL_getmetatable(state, "RenderStateStateBlock");
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

}
