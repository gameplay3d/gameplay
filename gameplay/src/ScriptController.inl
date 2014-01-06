#include "ScriptController.h"

namespace gameplay
{

template <typename T>
ScriptUtil::LuaArray<T>::LuaArray(T* param)
{
    _data = new ScriptUtil::LuaArray<T>::Data();
    _data->value = param;

    // Initial ref count of zero means no memory management
    _data->refCount = 0;
}

template <typename T>
ScriptUtil::LuaArray<T>::LuaArray(int count)
{
    _data = new ScriptUtil::LuaArray<T>::Data();

    // Allocate a chunk of memory to store 'count' number of T.
    // Use new instead of malloc since we track memory allocations
    // int DebugMem configurations.
    _data->value = (T*)new unsigned char[sizeof(T) * count];

    // Positive ref count means we automatically cleanup memory
    _data->refCount = 1;
}

template <typename T>
ScriptUtil::LuaArray<T>::LuaArray(const ScriptUtil::LuaArray<T>& copy)
{
    _data = copy._data;
    ++_data->refCount;
}

template <typename T>
ScriptUtil::LuaArray<T>::~LuaArray()
{
    if ((--_data->refCount) <= 0)
    {
        // Non managed arrays/pointers start with ref count zero, so only delete data if
        // the decremented ref count == 0 (otherwise it will be -1).
        if (_data->refCount == 0)
        {
            unsigned char* value = (unsigned char*)_data->value;
            SAFE_DELETE_ARRAY(value);
        }

        SAFE_DELETE(_data);
    }
}

template <typename T>
ScriptUtil::LuaArray<T>& ScriptUtil::LuaArray<T>::operator = (const ScriptUtil::LuaArray<T>& p)
{
    _data = p._data;
    ++_data->refCount;
}

template <typename T>
void ScriptUtil::LuaArray<T>::set(unsigned int index, const T* itemPtr)
{
    // WARNING: The following code will only work properly for arrays of pointers
    // to objects (i.e. T**) or for simple structs that are being passed
    // in as read-only. Since the memory is directly copied, any member data that
    // is modified with the object that is copied, will not modify the original object.
    // What is even scarier is that if an array of objects that contain virtual functions
    // is copied here, then the vtables are copied directly, meaning the new object
    // contains a copy of a vtable that points to functions in the old object. Calling
    // virtual fucntions on the new object would then call the functions on the old object.
    // If the old object is deleted, the vtable on the new object would point to addressess
    // for functions that no longer exist.
    if (itemPtr)
        memcpy((void*)&_data->value[index], (void*)itemPtr, sizeof(T));
    else
        memset((void*)&_data->value[index], 0, sizeof(T));
}

template <typename T>
ScriptUtil::LuaArray<T>::operator T* () const
{
    return _data->value;
}

template <typename T>
T& ScriptUtil::LuaArray<T>::operator[] (int index)
{
    return _data->value[index];
}

template<typename T>
ScriptUtil::LuaArray<T> ScriptUtil::getObjectPointer(int index, const char* type, bool nonNull, bool* success)
{
    *success = false;

    ScriptController* sc = Game::getInstance()->getScriptController();

    // Was 'nil' passed?
    if (lua_type(sc->_lua, index) == LUA_TNIL)
    {
        if (nonNull)
        {
            GP_WARN("Attempting to pass NULL for required non-NULL parameter at index %d (likely a reference or by-value parameter).", index);
        }
        else
        {
            // NULL values allowed
            *success = true;
        }

        return LuaArray<T>((T*)NULL);
    }

    // Was a Lua table passed?
    if (lua_type(sc->_lua, index) == LUA_TTABLE)
    {
        // Array type - assume success unless we encounter a problem below.
        *success = true;

        // Get the size of the array.
        lua_len(sc->_lua, index);
        int size = luaL_checkint(sc->_lua, -1);
        lua_pop(sc->_lua, 1);

        if (size <= 0)
        {
            // Array of size zero
            return LuaArray<T>((T*)NULL);
        }

        LuaArray<T> arr(size);

        // Loop through the lua table/array.
        int i = 0;
        lua_pushnil(sc->_lua);
        for (; lua_next(sc->_lua, index) != 0 && i < size; i++)
        {
            // Process each item in the array - make sure they are of valid types.
            void* p = lua_touserdata(sc->_lua, -1);
            if (p == NULL)
            {
                arr.set(i, (T*)NULL);
            }
            else
            {
                bool foundMatch = false;

                // Push array element metatable.
                if (lua_getmetatable(sc->_lua, -1))
                {
                    // Push param type metatable.
                    luaL_getmetatable(sc->_lua, type);
                    if (lua_rawequal(sc->_lua, -1, -2))
                    {
                        // Pop param type metatable.
                        lua_pop(sc->_lua, 1);

                        // Matched the declared parameter type.
                        arr.set(i, (T*)((ScriptUtil::LuaObject*)p)->instance);
                        foundMatch = true;
                    }
                    else
                    {
                        // Pop param type metatable.
                        lua_pop(sc->_lua, 1);

                        // Check if it matches any of the derived types' metatables.
                        const std::vector<std::string>& types = sc->_hierarchy[type];
                        for (size_t k = 0, count = types.size(); k < count; k++)
                        {
                            // Push dervied type metatable.
                            luaL_getmetatable(sc->_lua, types[k].c_str());
                            if (lua_rawequal(sc->_lua, -1, -2))
                            {
                                // Pop derived type metatable.
                                lua_pop(sc->_lua, 1);

                                // Matched a derived type.
                                arr.set(i, (T*)((ScriptUtil::LuaObject*)p)->instance);
                                foundMatch = true;
                                break;
                            }
                            // Pop derived type metatable
                            lua_pop(sc->_lua, 1);
                        }
                    }

                    // Pop array element metatable.
                    lua_pop(sc->_lua, 1);
                }

                if (!foundMatch)
                {
                    GP_WARN("Invalid type passed for an array element for parameter index %d.", index);
                    arr.set(i, (T*)NULL);
                    *success = false;
                }
            }

            // Pop 'value' and key 'key' for lua_next.
            lua_pop(sc->_lua, 1);
        }

        return arr;
    }

    // Type is not nil and not a table, so it should be USERDATA.
    void* p = lua_touserdata(sc->_lua, index);
    if (p != NULL)
    {
        // Push object metatable.
        if (lua_getmetatable(sc->_lua, index))
        {
            // Push param type metatable.
            luaL_getmetatable(sc->_lua, type);
            if (lua_rawequal(sc->_lua, -1, -2))
            {
                // Pop both metatables.
                lua_pop(sc->_lua, 2);

                T* ptr = (T*)((ScriptUtil::LuaObject*)p)->instance;
                if (ptr == NULL && nonNull)
                {
                    GP_WARN("Attempting to pass NULL for required non-NULL parameter at index %d (likely a reference or by-value parameter).", index);
                    return LuaArray<T>((T*)NULL);
                }

                // Type is valid.
                *success = true;
                return LuaArray<T>(ptr);
            }
            // Pop param type metable.
            lua_pop(sc->_lua, 1);

            // Check if it matches any of the derived types' metatables.
            const std::vector<std::string>& types = sc->_hierarchy[type];
            for (size_t i = 0, count = types.size(); i < count; i++)
            {
                // Push derived type metatable.
                luaL_getmetatable(sc->_lua, types[i].c_str());
                if (lua_rawequal(sc->_lua, -1, -2))
                {
                    // Pop both metatables.
                    lua_pop(sc->_lua, 2);

                    T* ptr = (T*)((ScriptUtil::LuaObject*)p)->instance;
                    if (ptr == NULL && nonNull)
                    {
                        GP_WARN("Attempting to pass NULL for required non-NULL parameter at index %d (likely a reference or by-value parameter).", index);
                        return LuaArray<T>((T*)NULL);
                    }

                    // Type is valid (matches a derived type).
                    *success = true;
                    return LuaArray<T>(ptr);
                }
                // Pop derived type metatable.
                lua_pop(sc->_lua, 1);
            }

            // Pop object metatable.
            lua_pop(sc->_lua, 1);
        }
    }

    // If we made it here, type was not nil, and it could not be mapped to a valid object pointer.
    //GP_WARN("Failed to retrieve a valid object pointer of type '%s' for parameter %d.", type, index);

    return LuaArray<T>((T*)NULL);
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

template<typename T> T ScriptController::executeFunction(const char* func, const char* args, va_list* list)
{
    executeFunctionHelper(1, func, args, list);

    T value = (T)((ScriptUtil::LuaObject*)lua_touserdata(_lua, -1))->instance;
    lua_pop(_lua, -1);
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
