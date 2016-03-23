#ifndef SCRIPTCONTROLLER_H_
#define SCRIPTCONTROLLER_H_

#include "Script.h"
#include "Game.h"

namespace gameplay
{

/**
 * Controls and manages all scripts.
 */
class ScriptController
{
    friend class Game;
    friend class Platform;
    friend class Script;
    friend class ScriptUtil;
    friend class ScriptTimeListener;

public:

    /**
     * Loads the given script file and executes its code (if it is not
     * alreay loaded).
     *
     * The script is loaded into an environment that is defined by the scope parameter.
     * If the script scope is GLOBAL and the forceReload parameter is false, a
     * previously-loaded script object may be returned. PROTECTED scope always results
     * in a new script being loaded and executed.
     * 
     * @param path The path to the script.
     * @param scope The scope for the script to be executed in.
     * @param forceReload Whether the script should be reloaded if it has already been loaded
     *      (applicable for GLOBAL scripts only).
     *
     * @return The loaded script, or NULL if the script could not be loaded.
     */
    Script* loadScript(const char* path, Script::Scope scope = Script::GLOBAL, bool forceReload = false);

    /**
     * Calls a zero-parameter global function.
     * 
     * @param func The name of the function to call.
     * @param out Pointer to populate with the return value if the function succeeds, or NULL.
     *
     * @return True if the function is successfully executed, false otherwise.
     *
     * @script{ignore}
     */
    template<typename T> bool executeFunction(const char* func, T* out);

    /**
     * Calls a zero-parameter function.
     * 
     * @param script The script to execute the function on, or NULL for the global script environment.
     * @param func The name of the function to call.
     * @param out Pointer to populate with the return value if the function succeeds, or NULL.
     *
     * @return True if the function is successfully executed, false otherwise.
     *
     * @script{ignore}
     */
    template<typename T> bool executeFunction(Script* script, const char* func, T* out);

    /**
     * Calls a global function using the given parameters.
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
     * @param out Pointer to populate with the return value if the function succeeds, or NULL.
     * @param ... Parameters to pass to the script fucntion, as specified by the args parameter.
     *
     * @return True if the function is successfully executed, false otherwise.
     *
     * @script{ignore}
     */
    template<typename T> bool executeFunction(const char* func, const char* args, T* out, ...);

    /**
     * Calls the a function from the specified script, using the given parameters.
     * 
     * @param script The script to execute the function on, or NULL for the global script environment.
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
     * @param out Pointer to populate with the return value if the function succeeds, or NULL.
     * @param ... Parameters to pass to the script fucntion, as specified by the args parameter.
     * 
     * @return True if the function is successfully executed, false otherwise.
     *
     * @script{ignore}
     */
    template<typename T> bool executeFunction(Script* script, const char* func, const char* args, T* out, ...);

    /**
     * Calls the specified global Lua function using the given parameters.
     * 
     * @param func The name of the function to call.
     * @param args The optional argument signature of the function. Of the form 'xxx', where each 'x' is a parameter type and must be one of:
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
     * @param out Pointer to populate with the return value if the function succeeds, or NULL.
     * @param list The variable argument list containing the function's parameters, or NULL for an empty parameter list.
     *
     * @return True if the function is successfully executed, false otherwise.
     *
     * @script{ignore}
     */
    template<typename T> bool executeFunction(const char* func, const char* args, T* out, va_list* list);

    /**
     * Calls the specified global Lua function using the given parameters.
     * 
     * @param script Optional script to use, or NULL for the global script environment.
     * @param func The name of the function to call.
     * @param args The optional argument signature of the function. Of the form 'xxx', where each 'x' is a parameter type and must be one of:
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
     * @param out Pointer to populate with the return value if the function succeeds, or NULL.
     * @param list The variable argument list containing the function's parameters, or NULL for an empty parameter list.
     *
     * @return True if the function is successfully executed, false otherwise.
     *
     * @script{ignore}
     */
    template<typename T> bool executeFunction(Script* script, const char* func, const char* args, T* out, va_list* list);

    /**
     * Gets the global boolean script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a bool.
     * @param script Optional script to use, or NULL for the global script environment.
     *
     * @return The global boolean script variable.
     * 
     * @script{ignore}
     */
    bool getBool(const char* name, bool defaultValue = false, Script* script = NULL);

    /**
     * Gets the global char script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global char script variable.
     * 
     * @script{ignore}
     */
    char getChar(const char* name, char defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global short script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global short script variable.
     * 
     * @script{ignore}
     */
    short getShort(const char* name, short defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global int script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global int script variable.
     * 
     * @script{ignore}
     */
    int getInt(const char* name, int defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global long script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global long script variable.
     * 
     * @script{ignore}
     */
    long getLong(const char* name, long defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global unsigned char script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global unsigned char script variable.
     * 
     * @script{ignore}
     */
    unsigned char getUnsignedChar(const char* name, unsigned char defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global unsigned short script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global unsigned short script variable.
     * 
     * @script{ignore}
     */
    unsigned short getUnsignedShort(const char* name, unsigned short defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global unsigned int script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global unsigned int script variable.
     * 
     * @script{ignore}
     */
    unsigned int getUnsignedInt(const char* name, unsigned int defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global unsigned long script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global unsigned long script variable.
     * 
     * @script{ignore}
     */
    unsigned long getUnsignedLong(const char* name, unsigned long defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global float script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global float script variable.
     * 
     * @script{ignore}
     */
    float getFloat(const char* name, float defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global double script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global double script variable.
     * 
     * @script{ignore}
     */
    double getDouble(const char* name, double defaultValue = 0, Script* script = NULL);

    /**
     * Gets the global string variable with the given name.
     *
     * The returned character string is temporary and is not gauranteed to live forever, therefore you
     * should not store a pointer to the returned value. If long-term access to the returned string is
     * required, it should be copied into another string for storage.
     *
     * @param name The name of the variable.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The string variable or NULL if the variable is not a string.
     * 
     * @script{ignore}
     */
    const char* getString(const char* name, Script* script = NULL);

    /**
     * Gets the global pointer script variable of the given type with the given name.
     * 
     * @param type The type of the variable in Lua.
     * @param name The name of the variable.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @return The global pointer script variable.
     * 
     * @script{ignore}
     */
    void* getObjectPointer(const char* type, const char* name, Script* script = NULL);

    /**
     * Sets the global boolean script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The boolean value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setBool(const char* name, bool v, Script* script = NULL);

    /**
     * Sets the global char script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The char value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setChar(const char* name, char v, Script* script = NULL);

    /**
     * Sets the global short script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The short value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setShort(const char* name, short v, Script* script = NULL);

    /**
     * Sets the global int script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The int value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setInt(const char* name, int v, Script* script = NULL);

    /**
     * Sets the global long script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The long value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setLong(const char* name, long v, Script* script = NULL);

    /**
     * Gets the global unsigned char script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned char value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setUnsignedChar(const char* name, unsigned char v, Script* script = NULL);

    /**
     * Sets the global unsigned short script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned short value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setUnsignedShort(const char* name, unsigned short v, Script* script = NULL);

    /**
     * Sets the global unsigned int script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned int value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setUnsignedInt(const char* name, unsigned int v, Script* script = NULL);

    /**
     * Sets the global unsigned long script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned long value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setUnsignedLong(const char* name, unsigned long v, Script* script = NULL);

    /**
     * Sets the global float script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The float value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setFloat(const char* name, float v, Script* script = NULL);

    /**
     * Sets the global double script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The double value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setDouble(const char* name, double v, Script* script = NULL);

    /**
     * Sets the global string script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The string value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setString(const char* name, const char* v, Script* script = NULL);

    /**
     * Sets the global pointer script variable of the given type with the given name to the given value.
     *
     * When passing an argument for the 'v' parameter, it is recommended that the object passed in
     * be converted to void* using 'dynamic_cast<void*>(o)', to ensure that the address of the 
     * most derived class possible be stored. In this case, the 'type' parameter should also be
     * set to the most derived type to ensure that the full set of methods for that object are
     * available to the script engine.
     *
     * @param type The type of the object point value being set.
     * @param name The name of the variable.
     * @param v The pointer value.
     * @param script Optional script to use, or NULL for the global script environment.
     * 
     * @script{ignore}
     */
    void setObjectPointer(const char* type, const char* name, void* v, Script* script = NULL);

    /**
     * Determines if there exists a function with the specified name in the given script environment.
     *
     * @param name The name of the function to check.
     * @param script Optional script to inspect, or NULL to inspect the global script environment.
     *
     * @return True if the specified function exists, false otherwise.
     */
    bool functionExists(const char* name, const Script* script = NULL) const;

    /**
     * Returns the currently executing script.
     *
     * Note that this function returns the currently executing Script object,
     * which is not guaranteed to be valid unless the returned script's
     * reference count is increased while the caller holds onto it.
     *
     * @return The currently executing script, or NULL if either there is no currently
     *      executing script or the global script environment is current.
     */
    Script* getCurrentScript() const;

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
     * Allows time listener interaction from Lua scripts.
     */
    struct ScriptTimeListener : public TimeListener
    {
        /**
         * Constructor.
         */
        ScriptTimeListener(Script* script, const char* function);

        /**
         * Destructor.
         */
        ~ScriptTimeListener();

        /**
         * @see TimeListener#timeEvent(long, void*)
         */
        void timeEvent(long timeDiff, void* cookie);

        /** Holds the script to execute the function within. */
        Script* script;
        /** Holds the name of the Lua script function to call back. */
        std::string function;
    };

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
     * Internal loadScript variant that supports loading into an existing Script object
     * for reloading purposes.
     *
     * @param script A Script object containing a valid path and scope to load.
     *
     * @return True if the script is successfully loaded, false otherwise.
     *
     * @see loadScript(const char*, Script::scope, bool)
     */
    bool loadScript(Script* script);

    /**
     * Attempts to unload the specified script.
     *
     * Unloading a script causes the non-global data within the script to be
     * released such that a reference is no longer held to it. It will then
     * be garbage collected at some point in the future. If the script stored
     * any data in the global environment or if any other code still references
     * data from the script, that data will not be freed.
     *
     * Global scripts cannot be explicitly unloaded and this function will do
     * nothing for such scripts.
     *
     * @param script The script to be unloaded.
     */
    void unloadScript(Script* script);

    /**
     * Calls the specified Lua function using the given parameters.
     * 
     * Note that this function may push one or more nested tables onto the stack, as neccessary
     * to execute the function. It will also push 'resultCount' results onto the stack.
     * The caller must ensure the stack is cleaned up afterwards.
     *
     * @param resultCount The expected number of returned values that will be pushed onto the stack.
     * @param func The name of the function to call.
     * @param args The optional argument signature of the function, as a string of the form
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
     * @param script Optional script to execute the function in, or NULL for to execute it in the global environment.
     * @return True if the function is executed and results were pushed, false if an error occurred (in which case nothing is pushed).
     */
    bool executeFunctionHelper(int resultCount, const char* func, const char* args, va_list* list, Script* script = NULL);

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

    /**
     * Schedules a script function to execute after the given time interval.
     *
     * This function is executed in the environment of the script that calls this function.
     *
     * @param timeOffset The number of game milliseconds in the future to schedule the event to be fired.
     * @param function The Lua script function that will receive the event.
     */
    void schedule(float timeOffset, const char* function);

    void pushScript(Script* script);

    void popScript();

    lua_State* _lua;
    unsigned int _returnCount;
    std::map<std::string, std::vector<Script*> > _scripts;
    std::vector<Script*> _envStack;
    std::list<ScriptTimeListener*> _timeListeners;
};

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(const char* func, void* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, bool* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(const char* func, char* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(const char* func, short* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(const char* func, int* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(const char* func, long* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(const char* func, unsigned char* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(const char* func, unsigned short* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(const char* func, unsigned int* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(const char* func, unsigned long* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(const char* func, float* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(const char* func, double* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(const char* func, std::string* out);

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(Script* script, const char* func, void* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(Script* script, const char* func, bool* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(Script* script, const char* func, char* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(Script* script, const char* func, short* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(Script* script, const char* func, int* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(Script* script, const char* func, long* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(Script* script, const char* func, unsigned char* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(Script* script, const char* func, unsigned short* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(Script* script, const char* func, unsigned int* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(Script* script, const char* func, unsigned long* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(Script* script, const char* func, float* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(Script* script, const char* func, double* out);
/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(Script* script, const char* func, std::string* out);

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(const char* func, const char* args, void* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, bool* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(const char* func, const char* args, char* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(const char* func, const char* args, short* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(const char* func, const char* args, int* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(const char* func, const char* args, long* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(const char* func, const char* args, unsigned char* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(const char* func, const char* args, unsigned short* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(const char* func, const char* args, unsigned int* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(const char* func, const char* args, unsigned long* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(const char* func, const char* args, float* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(const char* func, const char* args, double* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(const char* func, const char* args, std::string* out, ...);

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(Script* script, const char* func, const char* args, void* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(Script* script, const char* func, const char* args, bool* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(Script* script, const char* func, const char* args, char* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(Script* script, const char* func, const char* args, short* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(Script* script, const char* func, const char* args, int* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(Script* script, const char* func, const char* args, long* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(Script* script, const char* func, const char* args, unsigned char* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(Script* script, const char* func, const char* args, unsigned short* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(Script* script, const char* func, const char* args, unsigned int* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(Script* script, const char* func, const char* args, unsigned long* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(Script* script, const char* func, const char* args, float* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(Script* script, const char* func, const char* args, double* out, ...);
/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(Script* script, const char* func, const char* args, std::string* out, ...);

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(const char* func, const char* args, void* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, bool* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(const char* func, const char* args, char* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(const char* func, const char* args, short* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(const char* func, const char* args, int* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(const char* func, const char* args, long* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(const char* func, const char* args, unsigned char* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(const char* func, const char* args, unsigned short* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(const char* func, const char* args, unsigned int* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(const char* func, const char* args, unsigned long* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(const char* func, const char* args, float* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(const char* func, const char* args, double* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(const char* func, const char* args, std::string* out, va_list* list);

/** Template specialization. */
template<> bool ScriptController::executeFunction<void>(Script* script, const char* func, const char* args, void* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(Script* script, const char* func, const char* args, bool* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<char>(Script* script, const char* func, const char* args, char* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<short>(Script* script, const char* func, const char* args, short* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<int>(Script* script, const char* func, const char* args, int* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<long>(Script* script, const char* func, const char* args, long* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned char>(Script* script, const char* func, const char* args, unsigned char* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned short>(Script* script, const char* func, const char* args, unsigned short* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned int>(Script* script, const char* func, const char* args, unsigned int* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<unsigned long>(Script* script, const char* func, const char* args, unsigned long* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<float>(Script* script, const char* func, const char* args, float* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<double>(Script* script, const char* func, const char* args, double* out, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<std::string>(Script* script, const char* func, const char* args, std::string* out, va_list* list);

/**
 * Functions and structures used by the generated Lua script bindings.
 *
 * This class is used internally by the generated script bindings and is not intended
 * to be used directly.
 *
 * @script{ignore}
 */
class ScriptUtil
{
public:

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
     */
    static void registerLibrary(const char* name, const luaL_Reg* functions);

    /**
     * Registers the given boolean constant as valid for the given scope path.
     * 
     * @param name The name of the constant (what the user would use from Lua).
     * @param value The constant's value.
     * @param scopePath The list of containing classes, going inward from the most outer class.
     */
    static void registerConstantBool(const std::string& name, bool value, const std::vector<std::string>& scopePath);

    /**
     * Registers the given number constant as valid for the given scope path.
     * 
     * @param name The name of the constant (what the user would use from Lua).
     * @param value The constant's value.
     * @param scopePath The list of containing classes, going inward from the most outer class.
     */
    static void registerConstantNumber(const std::string& name, double value, const std::vector<std::string>& scopePath);

    /**
     * Registers the given string constant as valid for the given scope path.
     * 
     * @param name The name of the constant (what the user would use from Lua).
     * @param value The constant's value.
     * @param scopePath The list of containing classes, going inward from the most outer class.
     */
    static void registerConstantString(const std::string& name, const std::string& value, const std::vector<std::string>& scopePath);

    /**
     * Registers the given enum value as valid for the given scope path.
     *
     * @param enumValue The enumeration value, expressed as an integer.
     * @param enumValueString The string representation of the enum value (what the user would use from Lua).
     * @param scopePath The list of containing classes, going inward from the most outer class.
     */
    static void registerEnumValue(int enumValue, const std::string& enumValueString, const std::vector<std::string>& scopePath);

    /**
     * Registers the given class type with Lua.
     * 
     * @param name The name of the class from within Lua.
     * @param members The library function mapping for all the member functions (Lua function names to C++ functions).
     * @param newFunction The function to call that creates an instance of the class.
     * @param deleteFunction The function to call that destroys an instance of the class.
     * @param statics The library function mapping for all the static functions (Lua function names to C++ functions).
     * @param scopePath For an inner class, this is a list of its containing classes, going inward from the most outer class.
     */
    static void registerClass(const char* name, const luaL_Reg* members, lua_CFunction newFunction, lua_CFunction deleteFunction, const luaL_Reg* statics,
                       const std::vector<std::string>& scopePath);

    /**
     * Register a function with Lua.
     * 
     * @param luaFunction The name of the function from within Lua.
     * @param cppFunction The C++ function pointer.
     */
    static void registerFunction(const char* luaFunction, lua_CFunction cppFunction);

    /**
     * Gets a pointer to a bool (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<bool> getBoolPointer(int index);

    /**
     * Gets a pointer to a short (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<short> getShortPointer(int index);

    /**
     * Gets a pointer to an int (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<int> getIntPointer(int index);

    /**
     * Gets a pointer to a long (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<long> getLongPointer(int index);

    /**
     * Gets a pointer to an unsigned char (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<unsigned char> getUnsignedCharPointer(int index);

    /**
     * Gets a pointer to an unsigned short (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<unsigned short> getUnsignedShortPointer(int index);

    /**
     * Gets a pointer to an unsigned int (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<unsigned int> getUnsignedIntPointer(int index);

    /**
     * Gets a pointer to an unsigned long (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<unsigned long> getUnsignedLongPointer(int index);

    /**
     * Gets a pointer to a float (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<float> getFloatPointer(int index);

    /**
     * Gets a pointer to a double (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * 
     * @return The pointer.
     */
    static LuaArray<double> getDoublePointer(int index);

    /**
     * Gets an object pointer of the given type for the given stack index.
     *
     * This method supports both objects and arrays/tables of objects.
     * 
     * @param index The stack index.
     * @param type The type of object pointer to retrieve.
     * @param nonNull Whether the pointer must be non-null (e.g. if the parameter we 
     *      are retrieving is actually a reference or by-value parameter).
     * @param success An out parameter that is set to true if the Lua parameter was successfully
     *      converted to a valid object, or false if it was unable to perform a valid conversion.
     * 
     * @return The object pointer or <code>NULL</code> if the data at the stack index
     *      is not an object or if the object is not derived from the given type.
     */
    template <typename T>
    static LuaArray<T> getObjectPointer(int index, const char* type, bool nonNull, bool* success);

    /**
     * Gets a raw pointer that points to the correct address for the given type interface.
     *
     * This method requires that the type at the specified stack index is of type userdata
     * and is in the inheritence chain of the given type. If either of these are not true,
     * NULL will be returned.
     *
     * @param index The stack index.
     * @param type The type of object pointer to retrieve.
     *
     * @return The raw pointer or <code>NULL</code> if the data at the stack index
     *        is not an object of the given type.
     */
    static void* getUserDataObjectPointer(int index, const char* type);

    /**
     * Gets a string for the given stack index.
     * 
     * @param index The stack index.
     * @param isStdString Whether the string being retrieved is a std::string object or not.
     * 
     * @return The string or <code>NULL</code>.
     */
    static const char* getString(int index, bool isStdString);

    /**
     * Checks that the parameter at the given stack position is a boolean and returns it.
     * 
     * @param state The Lua state.
     * @param n The stack index.
     * 
     * @return The boolean (if successful; otherwise it logs an error).
     */
    static bool luaCheckBool(lua_State* state, int n);

};

}

#include "ScriptController.inl"

#endif
