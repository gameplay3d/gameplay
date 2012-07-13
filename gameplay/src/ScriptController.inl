#include "ScriptController.h"

namespace gameplay
{

template<typename T>T* ScriptUtil::getObjectPointer(int index, const char* type, bool nonNull)
{
    ScriptController* sc = Game::getInstance()->getScriptController();
    if (lua_type(sc->_lua, index) == LUA_TNIL)
    {
        if (nonNull)
        {
            GP_ERROR("Attempting to pass NULL for required non-NULL parameter at index %d (likely a reference or by-value parameter).", index);
        }

        return NULL;
    }
    else if (lua_type(sc->_lua, index) == LUA_TTABLE)
    {
        // Get the size of the array.
        lua_len(sc->_lua, index);
        int size = luaL_checkint(sc->_lua, -1);

        if (size <= 0)
            return NULL;

        // Create an array to store the values.
        T* values = (T*)malloc(sizeof(T)*size);
        
        // Push the first key.
        lua_pushnil(sc->_lua);
        int i = 0;
        for (; lua_next(sc->_lua, index) != 0 && i < size; i++)
        {
            void* p = lua_touserdata(sc->_lua, -1);
            if (p != NULL)
            {
                if (lua_getmetatable(sc->_lua, -1))
                {
                    // Check if it matches the type's metatable.
                    luaL_getmetatable(sc->_lua, type);
                    if (lua_rawequal(sc->_lua, -1, -2))
                    {
                        lua_pop(sc->_lua, 2);
                        T* ptr = (T*)((ScriptUtil::LuaObject*)p)->instance;
                        if (ptr)
                            memcpy((void*)&values[i], (void*)ptr, sizeof(T));
                        else
                            memset((void*)&values[i], 0, sizeof(T));

                        lua_pop(sc->_lua, 1);
                        continue;
                    }
                    lua_pop(sc->_lua, 1);

                    // Check if it matches any of the derived types' metatables.
                    const std::vector<std::string>& types = sc->_hierarchy[type];
                    for (unsigned int k = 0, count = types.size(); k < count; k++)
                    {
                        luaL_getmetatable(sc->_lua, types[k].c_str());
                        if (lua_rawequal(sc->_lua, -1, -2))
                        {
                            lua_pop(sc->_lua, 2);
                            T* ptr = (T*)((ScriptUtil::LuaObject*)p)->instance;
                            if (ptr)
                                memcpy((void*)&values[i], (void*)ptr, sizeof(T));
                            else
                                memset((void*)&values[i], 0, sizeof(T));

                            lua_pop(sc->_lua, 1);
                            continue;
                        }
                        lua_pop(sc->_lua, 1);
                    }
                    lua_pop(sc->_lua, 1);
                }
            }
        }
        
        return values;
    }
    else
    {
        void* p = lua_touserdata(sc->_lua, index);
        if (p != NULL)
        {
            if (lua_getmetatable(sc->_lua, index))
            {
                // Check if it matches the type's metatable.
                luaL_getmetatable(sc->_lua, type);
                if (lua_rawequal(sc->_lua, -1, -2))
                {
                    lua_pop(sc->_lua, 2);
                    T* ptr = (T*)((ScriptUtil::LuaObject*)p)->instance;
                    if (ptr == NULL && nonNull)
                    {
                        GP_ERROR("Attempting to pass NULL for required non-NULL parameter at index %d (likely a reference or by-value parameter).", index);
                    }
                    return ptr;
                }
                lua_pop(sc->_lua, 1);

                // Check if it matches any of the derived types' metatables.
                const std::vector<std::string>& types = sc->_hierarchy[type];
                for (unsigned int i = 0, count = types.size(); i < count; i++)
                {
                    luaL_getmetatable(sc->_lua, types[i].c_str());
                    if (lua_rawequal(sc->_lua, -1, -2))
                    {
                        lua_pop(sc->_lua, 2);
                        T* ptr = (T*)((ScriptUtil::LuaObject*)p)->instance;
                        if (ptr == NULL && nonNull)
                        {
                            GP_ERROR("Attempting to pass NULL for required non-NULL parameter at index %d (likely a reference or by-value parameter).", index);
                        }
                        return ptr;
                    }
                    lua_pop(sc->_lua, 1);
                }
            
                lua_pop(sc->_lua, 1);
            }
        }

        if (nonNull)
        {
            GP_ERROR("Failed to retrieve a valid object pointer of type '%s' for parameter %d.", type, index);
        }
        return NULL;
    }
}
    
template<typename T> T ScriptController::executeFunction(const char* func)
{
    executeFunctionHelper(1, func, NULL, NULL);
    T value = (T)((ScriptUtil::LuaObject*)lua_touserdata(_lua, -1))->instance;
    lua_pop(_lua, -1);
    return value;
}

template<typename T> T ScriptController::executeFunction(const char* func, const char* args, ...)
{
    va_list list;
    va_start(list, args);
    executeFunctionHelper(1, func, args, &list);

    T value = (T)((ScriptUtil::LuaObject*)lua_touserdata(_lua, -1))->instance;
    lua_pop(_lua, -1);
    va_end(list);
    return value;
}

template<typename T>T* ScriptController::getObjectPointer(const char* type, const char* name)
{
    lua_getglobal(_lua, name);
    void* userdata = luaL_checkudata(_lua, -1, type);
    std::string msg = std::string("'") + std::string(type) + std::string("' expected.");
    luaL_argcheck(_lua, userdata != NULL, 1, msg.c_str());
    return (T*)((ScriptUtil::LuaObject*)userdata)->instance;
}

template<typename T>void ScriptController::setObjectPointer(const char* type, const char* name, T* v)
{
    ScriptUtil::LuaObject* object = (ScriptUtil::LuaObject*)lua_newuserdata(_lua, sizeof(ScriptUtil::LuaObject));
    object->instance = (void*)v;
    object->owns = false;
    luaL_getmetatable(_lua, type);
    lua_setmetatable(_lua, -2);
    lua_setglobal(_lua, name);
}

}
