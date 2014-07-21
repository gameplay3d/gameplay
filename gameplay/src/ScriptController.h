#ifndef SCRIPTCONTROLLER_H_
#define SCRIPTCONTROLLER_H_

#include "Base.h"
#include "Game.h"
#include "Control.h"

namespace gameplay
{

/**
 * Functions and structures used by the generated Lua script bindings.
 */
namespace ScriptUtil
{

/**
 * Represents a C++ object from within Lua.
 * 
 * @script{ignore}
 */
struct LuaObject
{
    /** The actual object instance. */
    void* instance;
    /** Whether object is owned by Lua. */
    bool owns;
};

/**
 * Stores a Lua parameter of an array/pointer type that is passed from Lua to C.
 * Handles automatic cleanup of any temporary memory associated with the array.
 * 
 * @script{ignore}
 */
template <typename T>
class LuaArray
{
public:

    /**
     * Creates a LuaArray to store a single pointer value.
     */
    LuaArray(T* param);

    /**
     * Allocates a LuaArray to store an array of values.
     *
     * Individual items in the array can be set using the 
     * set(unsigned int, const T&) method.
     * 
     * @param count Number of elements to store in the parameter.
     */
    LuaArray(int count);

    /**
     * Copy constructor.
     */
    LuaArray(const LuaArray<T>& copy);

    /**
     * Destructor.
     */
    ~LuaArray();

    /**
     * Assignment operator.
     */
    LuaArray<T>& operator = (const LuaArray<T>& p);

    /**
     * Copies the value of the object pointed to by itemPtr into the specified
     * index of this LuaArray's array.
     */
    void set(unsigned int index, const T* itemPtr);

    /**
     * Conversion operator from LuaArray to T*.
     */
    operator T* () const;

    /**
     * Overloads [] operator to get/set item value at index.
     */
    T& operator[] (int index);

private:

    struct Data
    {
        Data() : value(NULL), refCount(0) { }
        T* value;
        int refCount;
    };

    Data* _data;
};

/**
 * Registers the given library with Lua.
 * 
 * @param name The name of the library from within Lua.
 * @param functions The library function mapping (Lua function names to C++ functions).
 * 
 * @script{ignore}
 */
void registerLibrary(const char* name, const luaL_Reg* functions);

/**
 * Registers the given boolean constant as valid for the given scope path.
 * 
 * @param name The name of the constant (what the user would use from Lua).
 * @param value The constant's value.
 * @param scopePath The list of containing classes, going inward from the most outer class.
 * 
 * @script{ignore}
 */
void registerConstantBool(const std::string& name, bool value, const std::vector<std::string>& scopePath);

/**
 * Registers the given number constant as valid for the given scope path.
 * 
 * @param name The name of the constant (what the user would use from Lua).
 * @param value The constant's value.
 * @param scopePath The list of containing classes, going inward from the most outer class.
 * 
 * @script{ignore}
 */
void registerConstantNumber(const std::string& name, double value, const std::vector<std::string>& scopePath);

/**
 * Registers the given string constant as valid for the given scope path.
 * 
 * @param name The name of the constant (what the user would use from Lua).
 * @param value The constant's value.
 * @param scopePath The list of containing classes, going inward from the most outer class.
 * 
 * @script{ignore}
 */
void registerConstantString(const std::string& name, const std::string& value, const std::vector<std::string>& scopePath);

/**
 * Registers the given enum value as valid for the given scope path.
 *
 * @param enumValue The enumeration value, expressed as an integer.
 * @param enumValueString The string representation of the enum value (what the user would use from Lua).
 * @param scopePath The list of containing classes, going inward from the most outer class.
 *
 * @script{ignore}
 */
void registerEnumValue(int enumValue, const std::string& enumValueString, const std::vector<std::string>& scopePath);

/**
 * Registers the given class type with Lua.
 * 
 * @param name The name of the class from within Lua.
 * @param members The library function mapping for all the member functions (Lua function names to C++ functions).
 * @param newFunction The function to call that creates an instance of the class.
 * @param deleteFunction The function to call that destroys an instance of the class.
 * @param statics The library function mapping for all the static functions (Lua function names to C++ functions).
 * @param scopePath For an inner class, this is a list of its containing classes, going inward from the most outer class.
 * 
 * @script{ignore}
 */
void registerClass(const char* name, const luaL_Reg* members, lua_CFunction newFunction, lua_CFunction deleteFunction, const luaL_Reg* statics,
                   const std::vector<std::string>& scopePath);

/**
 * Register a function with Lua.
 * 
 * @param luaFunction The name of the function from within Lua.
 * @param cppFunction The C++ function pointer.
 * 
 * @script{ignore}
 */
void registerFunction(const char* luaFunction, lua_CFunction cppFunction);

/**
 * Sets an inheritance pair within the global inheritance hierarchy (base, derived).
 * 
 * @param base The base class of the inheritance pair.
 * @param derived The derived class of the inheritance pair.
 * 
 * @script{ignore}
 */
void setGlobalHierarchyPair(const std::string& base, const std::string& derived);

/**
 * Gets a pointer to a bool (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<bool> getBoolPointer(int index);

/**
 * Gets a pointer to a short (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<short> getShortPointer(int index);

/**
 * Gets a pointer to an int (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<int> getIntPointer(int index);

/**
 * Gets a pointer to a long (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<long> getLongPointer(int index);

/**
 * Gets a pointer to an unsigned char (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<unsigned char> getUnsignedCharPointer(int index);

/**
 * Gets a pointer to an unsigned short (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<unsigned short> getUnsignedShortPointer(int index);

/**
 * Gets a pointer to an unsigned int (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<unsigned int> getUnsignedIntPointer(int index);

/**
 * Gets a pointer to an unsigned long (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<unsigned long> getUnsignedLongPointer(int index);

/**
 * Gets a pointer to a float (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<float> getFloatPointer(int index);

/**
 * Gets a pointer to a double (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
 * 
 * @param index The stack index.
 * 
 * @return The pointer.
 * 
 * @script{ignore}
 */
LuaArray<double> getDoublePointer(int index);

/**
 * Gets an object pointer of the given type for the given stack index.
 * 
 * @param type The type of object pointer to retrieve.
 * @param index The stack index.
 * @param nonNull Whether the pointer must be non-null (e.g. if the parameter we 
 *      are retrieving is actually a reference or by-value parameter).
 * @param success An out parameter that is set to true if the Lua parameter was successfully
 *      converted to a valid object, or false if it was unable to perform a valid conversion.
 * 
 * @return The object pointer or <code>NULL</code> if the data at the stack index
 *      is not an object or if the object is not derived from the given type.
 * 
 * @script{ignore}
 */
template <typename T>
LuaArray<T> getObjectPointer(int index, const char* type, bool nonNull, bool* success);

/**
 * Gets a string for the given stack index.
 * 
 * @param index The stack index.
 * @param isStdString Whether the string being retrieved is a std::string object or not.
 * 
 * @return The string or <code>NULL</code>.
 * 
 * @script{ignore}
 */
const char* getString(int index, bool isStdString);

/**
 * Checks that the parameter at the given stack position is a boolean and returns it.
 * 
 * @param state The Lua state.
 * @param n The stack index.
 * 
 * @return The boolean (if successful; otherwise it logs an error).
 * 
 * @script{ignore}
 */
bool luaCheckBool(lua_State* state, int n);

}

/**
 * Controls and manages all scripts.
 */
class ScriptController
{
    friend class Game;
    friend class Platform;

public:

    /**
     * Loads the given script file and executes its global code.
     *
     * The script is loaded into the global script environment, where any
     * non-local functions and variables in the script will override those
     * in the global environment.
     *
     * Scripts loaded into the global environment cannot be explicitly unloaded.
     * Once all data within the script is no longer referenced, it will be
     * garbage collected through normal means.
     * 
     * @param path The path to the script.
     * @param forceReload Whether the script should be reloaded if it has already been loaded.
     */
    bool loadScript(const char* path, bool forceReload = false);

    /**
     * Loads the given script into its own isolated environment and executes its code.
     *
     * The script is loaded into a new isolated script environment that is separate from
     * the global environment. Non-local functions and variables do not overlap with
     * those in the global table. However, the script does still have access to functions
     * and variables in the global table (unless they are re-declared in the script).
     *
     * Since this function installs the script into an isolated envrionment, the 
     * script is guaranteed to be newly loaded (even if the same script was previously
     * loaded) and any global code within it is executed.
     *
     * The script may later be unloaded by calling unloadScript(int).
     *
     * @param path The path of the script.
     * @return A unique ID for the script, which may be passed to unloadScript(int),
     *      or -1 if the script could not be loaded.
     */
    int loadScriptIsolated(const char* path);

    /**
     * Attempts to unload an isolated script with the given ID.
     *
     * Unloading a script causes the non-global data within the script to be
     * released such that a reference is no longer held to it. It will then
     * be garbage collected at some point in the future. If the script stored
     * any data in the global environment or if any other code still references
     * data from the script, that data will not be freed.
     *
     * @param id The unique ID of the isolated script, as returned from loadScriptIsolated(const char*).
     */
    void unloadScript(int id);

    /**
     * Given a URL, loads the referenced file and returns the referenced function name.
     * 
     * @param url The url to load.
     * @param scriptId Optional integer pointer that is populated with 
     * @return The function that the URL references.
     */
    std::string loadUrl(const char* url, int* scriptId = NULL);

    /**
     * Parses the given url into a separate script filename and function name.
     *
     * @param url Url to parse.
     * @param script Populated with the path of the script, or an empty string if the URL does not include a script.
     * @param function Populated with the function name.
     *
     * @script{ignore}
     */
    void parseUrl(const char* url, std::string* script, std::string* function);

    /**
     * Calls the specified no-parameter global Lua function.
     * 
     * @param func The name of the function to call.
     * 
     * @return The return value of the executed Lua function.
     *
     * @script{ignore}
     */
    template<typename T> T executeFunction(const char* func);

    /**
     * Calls the specified global Lua function using the given parameters.
     * 
     * @param func The name of the function to call.
     * @param args The argument signature of the function. Of the form 'xxx', where each 'x' is a parameter type and must be one of:
     *      - 'b' - bool
     *      - 'c' - char
     *      - 'h' - short
     *      - 'i' - int
     *      - 'l' - long
     *      - 'f' - float
     *      - 'd' - double
     *      - 'ui' - unsigned int
     *      - 'ul' - unsigned long
     *      - 'uc' - unsigned char
     *      - 'uh' - unsigned short
     *      - 's' - string
     *      - 'p' - pointer
     *      - '<object-type>' - a <b>pointer</b> to an object of the given type (where the qualified type name is enclosed by angle brackets).
     *      - '[enum-type]' - an enumerated value of the given type (where the qualified type name is enclosed by square brackets).
     * 
     * @return The return value of the executed Lua function.
     *
     * @script{ignore}
     */
    template<typename T> T executeFunction(const char* func, const char* args, ...);

    /**
     * Calls the specified global Lua function using the given parameters.
     * 
     * @param func The name of the function to call.
     * @param args The argument signature of the function. Of the form 'xxx', where each 'x' is a parameter type and must be one of:
     *      - 'b' - bool
     *      - 'c' - char
     *      - 'h' - short
     *      - 'i' - int
     *      - 'l' - long
     *      - 'f' - float
     *      - 'd' - double
     *      - 'ui' - unsigned int
     *      - 'ul' - unsigned long
     *      - 'uc' - unsigned char
     *      - 'uh' - unsigned short
     *      - 's' - string
     *      - 'p' - pointer
     *      - '<object-type>' - a <b>pointer</b> to an object of the given type (where the qualified type name is enclosed by angle brackets).
     *      - '[enum-type]' - an enumerated value of the given type (where the qualified type name is enclosed by square brackets).
     * @param list The variable argument list containing the function's parameters.
     * @param script Optional ID for the script environment of the function to execute, or zero for the default/global environment.
     *
     * @return The return value of the executed Lua function.
     *
     * @script{ignore}
     */
    template<typename T> T executeFunction(const char* func, const char* args, va_list* list, int script = 0);

    /**
     * Gets the global boolean script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a bool.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     *
     * @return The global boolean script variable.
     * 
     * @script{ignore}
     */
    bool getBool(const char* name, bool defaultValue = false, int script = 0);

    /**
     * Gets the global char script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global char script variable.
     * 
     * @script{ignore}
     */
    char getChar(const char* name, char defaultValue = 0, int script = 0);

    /**
     * Gets the global short script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global short script variable.
     * 
     * @script{ignore}
     */
    short getShort(const char* name, short defaultValue = 0, int script = 0);

    /**
     * Gets the global int script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global int script variable.
     * 
     * @script{ignore}
     */
    int getInt(const char* name, int defaultValue = 0, int script = 0);

    /**
     * Gets the global long script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global long script variable.
     * 
     * @script{ignore}
     */
    long getLong(const char* name, long defaultValue = 0, int script = 0);

    /**
     * Gets the global unsigned char script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global unsigned char script variable.
     * 
     * @script{ignore}
     */
    unsigned char getUnsignedChar(const char* name, unsigned char defaultValue = 0, int script = 0);

    /**
     * Gets the global unsigned short script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global unsigned short script variable.
     * 
     * @script{ignore}
     */
    unsigned short getUnsignedShort(const char* name, unsigned short defaultValue = 0, int script = 0);

    /**
     * Gets the global unsigned int script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global unsigned int script variable.
     * 
     * @script{ignore}
     */
    unsigned int getUnsignedInt(const char* name, unsigned int defaultValue = 0, int script = 0);

    /**
     * Gets the global unsigned long script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global unsigned long script variable.
     * 
     * @script{ignore}
     */
    unsigned long getUnsignedLong(const char* name, unsigned long defaultValue = 0, int script = 0);

    /**
     * Gets the global float script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global float script variable.
     * 
     * @script{ignore}
     */
    float getFloat(const char* name, float defaultValue = 0, int script = 0);

    /**
     * Gets the global double script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The global double script variable.
     * 
     * @script{ignore}
     */
    double getDouble(const char* name, double defaultValue = 0, int script = 0);

    /**
     * Gets the global string variable with the given name.
     *
     * The returned character string is temporary and is not gauranteed to live forever, therefore you
     * should not store a pointer to the returned value. If long-term access to the returned string is
     * required, it should be copied into another string for storage.
     *
     * @param name The name of the variable.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @return The string variable or NULL if the variable is not a string.
     * 
     * @script{ignore}
     */
    const char* getString(const char* name, int script = 0);

    /**
     * Gets the global pointer script variable of the given type with the given name.
     * 
     * @param type The type of the variable in Lua.
     * @param name The name of the variable.
     * 
     * @return The global pointer script variable.
     * 
     * @script{ignore}
     */
    template<typename T>T* getObjectPointer(const char* type, const char* name);

    /**
     * Sets the global boolean script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The boolean value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setBool(const char* name, bool v, int script = 0);

    /**
     * Sets the global char script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The char value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setChar(const char* name, char v, int script = 0);

    /**
     * Sets the global short script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The short value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setShort(const char* name, short v, int script = 0);

    /**
     * Sets the global int script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The int value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setInt(const char* name, int v, int script = 0);

    /**
     * Sets the global long script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The long value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setLong(const char* name, long v, int script = 0);

    /**
     * Gets the global unsigned char script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned char value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setUnsignedChar(const char* name, unsigned char v, int script = 0);

    /**
     * Sets the global unsigned short script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned short value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setUnsignedShort(const char* name, unsigned short v, int script = 0);

    /**
     * Sets the global unsigned int script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned int value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setUnsignedInt(const char* name, unsigned int v, int script = 0);

    /**
     * Sets the global unsigned long script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned long value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setUnsignedLong(const char* name, unsigned long v, int script = 0);

    /**
     * Sets the global float script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The float value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setFloat(const char* name, float v, int script = 0);

    /**
     * Sets the global double script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The double value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setDouble(const char* name, double v, int script = 0);

    /**
     * Sets the global string script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The string value.
     * @param script Optional ID for the script environment to use, or zero for the default/global environment.
     * 
     * @script{ignore}
     */
    void setString(const char* name, const char* v, int script = 0);

    /**
     * Sets the global pointer script variable of the given type with the given name to the given value.
     * 
     * @param type The type of the script variable.
     * @param name The name of the variable.
     * @param v The pointer value.
     * 
     * @script{ignore}
     */
    template<typename T>void setObjectPointer(const char* type, const char* name, T* v);

    /**
     * Determines if there exists a function with the specified name in the given script environment.
     *
     * @param name The name of the function to check.
     * @param script Optional ID of the script environment to inspect, or zero to inspect the global environment.
     *
     * @return True if the specified function exists, false otherwise.
     */
    bool functionExists(const char* name, int script = 0) const;

    /**
     * Prints the string to the platform's output stream or log file.
     * Used for overriding Lua's print function.
     * 
     * @param str The string to print.
     */
    static void print(const char* str);

    /**
     * Prints the strings to the platform's output stream or log file.
     * Used for overriding Lua's print function.
     * 
     * @param str1 The first string to print.
     * @param str2 The second string to print on the same line as str1.
     */
    static void print(const char* str1, const char* str2);

private:

    /**
     * Constructor.
     */
    ScriptController();

    /**
     * Copy constructor.
     */
    ScriptController(const ScriptController& copy);

    /**
     * Destructor.
     */
    ~ScriptController();

    /**
     * Called to initialize the script controller.
     */
    void initialize();

    /*
     * Called to finalize the script controller.
     */
    void finalize();

    /**
     * Calls the specified Lua function using the given parameters.
     * 
     * @param resultCount The expected number of returned values.
     * @param func The name of the function to call.
     * @param args The argument signature of the function, as a string of the form
     *      'xxx', where each 'x' is a parameter type and must be one of:
     *      - 'b' - bool
     *      - 'c' - char
     *      - 'h' - short
     *      - 'i' - int
     *      - 'l' - long
     *      - 'f' - float
     *      - 'd' - double
     *      - 'ui' - unsigned int
     *      - 'ul' - unsigned long
     *      - 'uc' - unsigned char
     *      - 'uh' - unsigned short
     *      - 's' - string
     *      - 'p' - pointer
     *      - '<object-type>' - a <b>pointer</b> to an object of the given type (where the qualified type name is enclosed by angle brackets).
     *      - '[enum-type]' - an enumerated value of the given type (where the qualified type name is enclosed by square brackets).
     * @param list The variable argument list.
     * @param script Optional ID for the script environment of the function to execute, or zero for the default/global environment.
     */
    void executeFunctionHelper(int resultCount, const char* func, const char* args, va_list* list, int script = 0);

    /**
     * Converts a Gameplay userdata value to the type with the given class name.
     * This function will change the metatable of the userdata value to the metatable that matches the given string.
     * 
     * Example:
     * <code>
     * local launchButton = form:getControl("launch")
     * convert(launchButton, "Button")
     * print("Button text: " .. launchButton:getText())
     * </code>
     * 
     * <code>
     * -- The signature of the lua function:
     * -- param: object    A userdata object that represents a Gameplay object.
     * -- param: className The name of the class to convert the object to. (Examples: "Button", "PhysicsRigidBody")
     * function convert(object, className)
     * </code>
     * 
     * @param state The Lua state.
     * 
     * @return The number of values being returned by this function.
     * 
     * @script{ignore}
     */
    static int convert(lua_State* state);

    // Friend functions (used by Lua script bindings).
    friend void ScriptUtil::registerLibrary(const char* name, const luaL_Reg* functions);
    friend void ScriptUtil::registerConstantBool(const std::string& name, bool value, const std::vector<std::string>& scopePath);
    friend void ScriptUtil::registerConstantNumber(const std::string& name, double value, const std::vector<std::string>& scopePath);
    friend void ScriptUtil::registerConstantString(const std::string& name, const std::string& value, const std::vector<std::string>& scopePath);
    friend void ScriptUtil::registerEnumValue(int enumValue, const std::string& enumValueString, const std::vector<std::string>& scopePath);
    friend void ScriptUtil::registerClass(const char* name, const luaL_Reg* members, lua_CFunction newFunction,
        lua_CFunction deleteFunction, const luaL_Reg* statics, const std::vector<std::string>& scopePath);
    friend void ScriptUtil::registerFunction(const char* luaFunction, lua_CFunction cppFunction);
    friend void ScriptUtil::setGlobalHierarchyPair(const std::string& base, const std::string& derived);
    friend ScriptUtil::LuaArray<bool> ScriptUtil::getBoolPointer(int index);
    friend ScriptUtil::LuaArray<short> ScriptUtil::getShortPointer(int index);
    friend ScriptUtil::LuaArray<int> ScriptUtil::getIntPointer(int index);
    friend ScriptUtil::LuaArray<long> ScriptUtil::getLongPointer(int index);
    friend ScriptUtil::LuaArray<unsigned char> ScriptUtil::getUnsignedCharPointer(int index);
    friend ScriptUtil::LuaArray<unsigned short> ScriptUtil::getUnsignedShortPointer(int index);
    friend ScriptUtil::LuaArray<unsigned int> ScriptUtil::getUnsignedIntPointer(int index);
    friend ScriptUtil::LuaArray<unsigned long> ScriptUtil::getUnsignedLongPointer(int index);
    friend ScriptUtil::LuaArray<float> ScriptUtil::getFloatPointer(int index);
    friend ScriptUtil::LuaArray<double> ScriptUtil::getDoublePointer(int index);
    template<typename T> friend ScriptUtil::LuaArray<T> ScriptUtil::getObjectPointer(int index, const char* type, bool nonNull, bool* success);
    friend const char* ScriptUtil::getString(int index, bool isStdString);

    lua_State* _lua;
    unsigned int _returnCount;
    std::map<std::string, std::vector<std::string> > _hierarchy;
    std::set<std::string> _loadedScripts;
    std::map<int, std::string> _environments;
    std::vector<int> _envStack;
};

/** Template specialization. */
template<> void ScriptController::executeFunction<void>(const char* func);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func);
/** Template specialization. */
template<> char ScriptController::executeFunction<char>(const char* func);
/** Template specialization. */
template<> short ScriptController::executeFunction<short>(const char* func);
/** Template specialization. */
template<> int ScriptController::executeFunction<int>(const char* func);
/** Template specialization. */
template<> long ScriptController::executeFunction<long>(const char* func);
/** Template specialization. */
template<> unsigned char ScriptController::executeFunction<unsigned char>(const char* func);
/** Template specialization. */
template<> unsigned short ScriptController::executeFunction<unsigned short>(const char* func);
/** Template specialization. */
template<> unsigned int ScriptController::executeFunction<unsigned int>(const char* func);
/** Template specialization. */
template<> unsigned long ScriptController::executeFunction<unsigned long>(const char* func);
/** Template specialization. */
template<> float ScriptController::executeFunction<float>(const char* func);
/** Template specialization. */
template<> double ScriptController::executeFunction<double>(const char* func);
/** Template specialization. */
template<> std::string ScriptController::executeFunction<std::string>(const char* func);

/** Template specialization. */
template<> void ScriptController::executeFunction<void>(const char* func, const char* args, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, ...);
/** Template specialization. */
template<> char ScriptController::executeFunction<char>(const char* func, const char* args, ...);
/** Template specialization. */
template<> short ScriptController::executeFunction<short>(const char* func, const char* args, ...);
/** Template specialization. */
template<> int ScriptController::executeFunction<int>(const char* func, const char* args, ...);
/** Template specialization. */
template<> long ScriptController::executeFunction<long>(const char* func, const char* args, ...);
/** Template specialization. */
template<> unsigned char ScriptController::executeFunction<unsigned char>(const char* func, const char* args, ...);
/** Template specialization. */
template<> unsigned short ScriptController::executeFunction<unsigned short>(const char* func, const char* args, ...);
/** Template specialization. */
template<> unsigned int ScriptController::executeFunction<unsigned int>(const char* func, const char* args, ...);
/** Template specialization. */
template<> unsigned long ScriptController::executeFunction<unsigned long>(const char* func, const char* args, ...);
/** Template specialization. */
template<> float ScriptController::executeFunction<float>(const char* func, const char* args, ...);
/** Template specialization. */
template<> double ScriptController::executeFunction<double>(const char* func, const char* args, ...);
/** Template specialization. */
template<> std::string ScriptController::executeFunction<std::string>(const char* func, const char* args, ...);

/** Template specialization. */
template<> void ScriptController::executeFunction<void>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> char ScriptController::executeFunction<char>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> short ScriptController::executeFunction<short>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> int ScriptController::executeFunction<int>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> long ScriptController::executeFunction<long>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> unsigned char ScriptController::executeFunction<unsigned char>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> unsigned short ScriptController::executeFunction<unsigned short>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> unsigned int ScriptController::executeFunction<unsigned int>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> unsigned long ScriptController::executeFunction<unsigned long>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> float ScriptController::executeFunction<float>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> double ScriptController::executeFunction<double>(const char* func, const char* args, va_list* list, int script);
/** Template specialization. */
template<> std::string ScriptController::executeFunction<std::string>(const char* func, const char* args, va_list* list, int script);

}

#include "ScriptController.inl"

#endif
