#include "Base.h"
#include "ScriptController.h"
#include "Effect.h"
#include "lua_Effect.h"
#include "lua_Global.h"

namespace gameplay
{

void luaRegister_Effect()
{
    ScriptController* sc = ScriptController::getInstance();

    const luaL_Reg lua_members[] = 
    {
        {"addRef", lua_Effect_addRef},
        {"bind", lua_Effect_bind},
        {"getId", lua_Effect_getId},
        {"getRefCount", lua_Effect_getRefCount},
        {"getUniform", lua_Effect_getUniform},
        {"getUniformCount", lua_Effect_getUniformCount},
        {"getVertexAttribute", lua_Effect_getVertexAttribute},
        {"release", lua_Effect_release},
        {"setValue", lua_Effect_setValue},
        {NULL, NULL}
    };
    const luaL_Reg lua_statics[] = 
    {
        {"createFromFile", lua_Effect_static_createFromFile},
        {"createFromSource", lua_Effect_static_createFromSource},
        {"getCurrentEffect", lua_Effect_static_getCurrentEffect},
        {NULL, NULL}
    };
    std::vector<std::string> scopePath;

    sc->registerClass("Effect", lua_members, NULL, lua_Effect__gc, lua_statics, scopePath);
}

static Effect* getInstance(lua_State* state)
{
    void* userdata = luaL_checkudata(state, 1, "Effect");
    luaL_argcheck(state, userdata != NULL, 1, "'Effect' expected.");
    return (Effect*)((ScriptController::LuaObject*)userdata)->instance;
}

int lua_Effect__gc(lua_State* state)
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
                void* userdata = luaL_checkudata(state, 1, "Effect");
                luaL_argcheck(state, userdata != NULL, 1, "'Effect' expected.");
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)userdata;
                if (object->owns)
                {
                    Effect* instance = (Effect*)object->instance;
                    SAFE_RELEASE(instance);
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

int lua_Effect_addRef(lua_State* state)
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
                Effect* instance = getInstance(state);
                instance->addRef();
                
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

int lua_Effect_bind(lua_State* state)
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
                Effect* instance = getInstance(state);
                instance->bind();
                
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

int lua_Effect_getId(lua_State* state)
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
                Effect* instance = getInstance(state);
                const char* result = instance->getId();

                // Push the return value onto the stack.
                lua_pushstring(state, result);

                return 1;
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

int lua_Effect_getRefCount(lua_State* state)
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
                Effect* instance = getInstance(state);
                unsigned int result = instance->getRefCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
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

int lua_Effect_getUniform(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                Effect* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->getUniform(param1);
                object->owns = false;
                luaL_getmetatable(state, "Uniform");
                lua_setmetatable(state, -2);

                return 1;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                unsigned int param1 = (unsigned int)luaL_checkunsigned(state, 2);

                Effect* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)instance->getUniform(param1);
                object->owns = false;
                luaL_getmetatable(state, "Uniform");
                lua_setmetatable(state, -2);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Effect_getUniformCount(lua_State* state)
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
                Effect* instance = getInstance(state);
                unsigned int result = instance->getUniformCount();

                // Push the return value onto the stack.
                lua_pushunsigned(state, result);

                return 1;
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

int lua_Effect_getVertexAttribute(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 2);

                Effect* instance = getInstance(state);
                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)new GLint(instance->getVertexAttribute(param1));
                object->owns = true;
                luaL_getmetatable(state, "GLint");
                lua_setmetatable(state, -2);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 2).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Effect_release(lua_State* state)
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
                Effect* instance = getInstance(state);
                instance->release();
                
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

int lua_Effect_setValue(lua_State* state)
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
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                float param2 = (float)luaL_checknumber(state, 3);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                (lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TLIGHTUSERDATA))
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                float* param2 = ScriptController::getInstance()->getFloatPointer(3);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                int param2 = (int)luaL_checkint(state, 3);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                (lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TLIGHTUSERDATA))
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                int* param2 = ScriptController::getInstance()->getIntPointer(3);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Matrix");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 3.");
                    lua_error(state);
                }
                Matrix* param2 = (Matrix*)((ScriptController::LuaObject*)userdata3)->instance;

                Effect* instance = getInstance(state);
                instance->setValue(param1, *param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Matrix");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 3.");
                    lua_error(state);
                }
                Matrix* param2 = (Matrix*)((ScriptController::LuaObject*)userdata3)->instance;

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector2");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 3.");
                    lua_error(state);
                }
                Vector2* param2 = (Vector2*)((ScriptController::LuaObject*)userdata3)->instance;

                Effect* instance = getInstance(state);
                instance->setValue(param1, *param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector2");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 3.");
                    lua_error(state);
                }
                Vector2* param2 = (Vector2*)((ScriptController::LuaObject*)userdata3)->instance;

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                Effect* instance = getInstance(state);
                instance->setValue(param1, *param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector4");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 3.");
                    lua_error(state);
                }
                Vector4* param2 = (Vector4*)((ScriptController::LuaObject*)userdata3)->instance;

                Effect* instance = getInstance(state);
                instance->setValue(param1, *param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector4");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 3.");
                    lua_error(state);
                }
                Vector4* param2 = (Vector4*)((ScriptController::LuaObject*)userdata3)->instance;

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "TextureSampler");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Texture::Sampler' for parameter 3.");
                    lua_error(state);
                }
                Texture::Sampler* param2 = (Texture::Sampler*)((ScriptController::LuaObject*)userdata3)->instance;

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2);
                
                return 0;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 4:
        {
            if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                (lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                float* param2 = ScriptController::getInstance()->getFloatPointer(3);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2, param3);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                (lua_type(state, 3) == LUA_TTABLE || lua_type(state, 3) == LUA_TLIGHTUSERDATA) &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                int* param2 = ScriptController::getInstance()->getIntPointer(3);

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2, param3);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Matrix");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Matrix' for parameter 3.");
                    lua_error(state);
                }
                Matrix* param2 = (Matrix*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2, param3);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector2");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector2' for parameter 3.");
                    lua_error(state);
                }
                Vector2* param2 = (Vector2*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2, param3);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector3");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector3' for parameter 3.");
                    lua_error(state);
                }
                Vector3* param2 = (Vector3*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2, param3);
                
                return 0;
            }
            else if (lua_type(state, 1) == LUA_TUSERDATA &&
                lua_type(state, 2) == LUA_TUSERDATA &&
                lua_type(state, 3) == LUA_TUSERDATA &&
                lua_type(state, 4) == LUA_TNUMBER)
            {
                // Get parameter 1 off the stack.
                void* userdata2 = ScriptController::getInstance()->getObjectPointer(2, "Uniform");
                if (!userdata2)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Uniform' for parameter 2.");
                    lua_error(state);
                }
                Uniform* param1 = (Uniform*)((ScriptController::LuaObject*)userdata2)->instance;

                // Get parameter 2 off the stack.
                void* userdata3 = ScriptController::getInstance()->getObjectPointer(3, "Vector4");
                if (!userdata3)
                {
                    lua_pushstring(state, "Failed to retrieve a valid object pointer of type 'Vector4' for parameter 3.");
                    lua_error(state);
                }
                Vector4* param2 = (Vector4*)((ScriptController::LuaObject*)userdata3)->instance;

                // Get parameter 3 off the stack.
                unsigned int param3 = (unsigned int)luaL_checkunsigned(state, 4);

                Effect* instance = getInstance(state);
                instance->setValue(param1, param2, param3);
                
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
            lua_pushstring(state, "Invalid number of parameters (expected 3 or 4).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Effect_static_createFromFile(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TSTRING &&
                lua_type(state, 2) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

                // Get parameter 2 off the stack.
                const char* param2 = luaL_checkstring(state, 2);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)Effect::createFromFile(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "Effect");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if (lua_type(state, 1) == LUA_TSTRING &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

                // Get parameter 2 off the stack.
                const char* param2 = luaL_checkstring(state, 2);

                // Get parameter 3 off the stack.
                const char* param3 = luaL_checkstring(state, 3);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)Effect::createFromFile(param1, param2, param3);
                object->owns = false;
                luaL_getmetatable(state, "Effect");
                lua_setmetatable(state, -2);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Effect_static_createFromSource(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 2:
        {
            if (lua_type(state, 1) == LUA_TSTRING &&
                lua_type(state, 2) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

                // Get parameter 2 off the stack.
                const char* param2 = luaL_checkstring(state, 2);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)Effect::createFromSource(param1, param2);
                object->owns = false;
                luaL_getmetatable(state, "Effect");
                lua_setmetatable(state, -2);

                return 1;
            }
            else
            {
                lua_pushstring(state, "Failed to match the given parameters to a valid function signature.");
                lua_error(state);
            }
            break;
        }
        case 3:
        {
            if (lua_type(state, 1) == LUA_TSTRING &&
                lua_type(state, 2) == LUA_TSTRING &&
                lua_type(state, 3) == LUA_TSTRING)
            {
                // Get parameter 1 off the stack.
                const char* param1 = luaL_checkstring(state, 1);

                // Get parameter 2 off the stack.
                const char* param2 = luaL_checkstring(state, 2);

                // Get parameter 3 off the stack.
                const char* param3 = luaL_checkstring(state, 3);

                ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
                object->instance = (void*)Effect::createFromSource(param1, param2, param3);
                object->owns = false;
                luaL_getmetatable(state, "Effect");
                lua_setmetatable(state, -2);

                return 1;
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
            lua_pushstring(state, "Invalid number of parameters (expected 2 or 3).");
            lua_error(state);
            break;
        }
    }
    return 0;
}

int lua_Effect_static_getCurrentEffect(lua_State* state)
{
    // Get the number of parameters.
    int paramCount = lua_gettop(state);

    // Attempt to match the parameters to a valid binding.
    switch (paramCount)
    {
        case 0:
        {
            ScriptController::LuaObject* object = (ScriptController::LuaObject*)lua_newuserdata(state, sizeof(ScriptController::LuaObject));
            object->instance = (void*)Effect::getCurrentEffect();
            object->owns = false;
            luaL_getmetatable(state, "Effect");
            lua_setmetatable(state, -2);

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
