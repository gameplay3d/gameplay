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
    _data->value = (T*)new unsigned char[sizeof(T)* count];

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
            void* p = getUserDataObjectPointer(-1, type);
            arr.set(i, (T*)p);

            // Pop 'value' and key 'key' for lua_next.
            lua_pop(sc->_lua, 1);
        }

        return arr;
    }

    // Type is not nil and not a table, so it should be USERDATA.
    void* p = getUserDataObjectPointer(index, type);
    if (p == NULL && nonNull)
    {
        GP_WARN("Attempting to pass NULL for required non-NULL parameter at index %d (likely a reference or by-value parameter).", index);
        return LuaArray<T>((T*)NULL);
    }
    else
    {
        *success = true;
    }

    return LuaArray<T>((T*)p);
}

template<typename T> bool ScriptController::executeFunction(const char* func, T* out)
{
    return executeFunction<T>((Script*)NULL, func, out);
}

template<typename T> bool ScriptController::executeFunction(Script* script, const char* func, T* out)
{
    // Userdata / object type expected - all other return types have template specializations.
    // Non-userdata types will return NULL.
    int top = lua_gettop(_lua);
    bool success = executeFunctionHelper(1, func, NULL, NULL, script);
    if (out && success)
        *out = (T)((ScriptUtil::LuaObject*)lua_touserdata(_lua, -1))->instance;
    lua_settop(_lua, top);
    return success;
}

template<typename T> bool ScriptController::executeFunction(const char* func, const char* args, T* out, ...)
{
    va_list list;
    va_start(list, out);
    bool success = executeFunction<T>((Script*)NULL, func, args, out, list);
    va_end(list);
    return success;
}

template<typename T> bool ScriptController::executeFunction(Script* script, const char* func, const char* args, T* out, ...)
{
    va_list list;
    va_start(list, out);
    bool success = executeFunction<T>(script, func, args, out, list);
    va_end(list);
    return success;
}

template<typename T> bool ScriptController::executeFunction(const char* func, const char* args, T* out, va_list* list)
{
    // Userdata / object type expected - all other return types have template specializations.
    // Non-userdata types will return NULL.
    int top = lua_gettop(_lua);
    bool success = executeFunctionHelper(1, func, args, list, (Script*)NULL);
    if (out && success)
        *out = (T)((ScriptUtil::LuaObject*)lua_touserdata(_lua, -1))->instance;
    lua_settop(_lua, top);
    return success;
}

template<typename T> bool ScriptController::executeFunction(Script* script, const char* func, const char* args, T* out, va_list* list)
{
    // Userdata / object type expected - all other return types have template specializations.
    // Non-userdata types will return NULL.
    int top = lua_gettop(_lua);
    bool success = executeFunctionHelper(1, func, args, list, script);
    if (out && success)
        *out = (T)((ScriptUtil::LuaObject*)lua_touserdata(_lua, -1))->instance;
    lua_settop(_lua, top);
    return success;
}

}
