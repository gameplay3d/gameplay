#ifndef SCRIPTCONTROLLER_H
#define SCRIPTCONTROLLER_H

#include "Base.h"
#include "Gamepad.h"

namespace gameplay
{

/**
 * Controls and manages all scripts.
 */
class ScriptController
{
    friend class Game;
    friend class Platform;

public:

    /**
     * Represents a C++ object from within Lua.
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
     * Calls the specifed Lua function using the given parameters.
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
     * @return The return value of the executed Lua function.
     */
    template<typename T> T executeFunction(const char* func, const char* args, ...);

    // Template specializations.
    template<> void executeFunction<void>(const char* func, const char* args, ...);
    template<> bool executeFunction<bool>(const char* func, const char* args, ...);
    template<> char executeFunction<char>(const char* func, const char* args, ...);
    template<> short executeFunction<short>(const char* func, const char* args, ...);
    template<> int executeFunction<int>(const char* func, const char* args, ...);
    template<> long executeFunction<long>(const char* func, const char* args, ...);
    template<> unsigned char executeFunction<unsigned char>(const char* func, const char* args, ...);
    template<> unsigned short executeFunction<unsigned short>(const char* func, const char* args, ...);
    template<> unsigned int executeFunction<unsigned int>(const char* func, const char* args, ...);
    template<> unsigned long executeFunction<unsigned long>(const char* func, const char* args, ...);
    template<> float executeFunction<float>(const char* func, const char* args, ...);
    template<> double executeFunction<double>(const char* func, const char* args, ...);
    template<> std::string executeFunction<std::string>(const char* func, const char* args, ...);

    /**
     * Used to specify the pointer type for executing Lua functions that return pointers.
     * @script{ignore}
     */
    struct Type
    {
        explicit Type(const char* type) : type(type) {}
        const char* type;
    };

    /**
     * Calls the specifed Lua function using the given parameters.
     * 
     * @param type The class of the return value pointer.
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
     * @return The return value of the executed Lua function.
     */
    template<typename T> T* executeFunction(const Type& type, const char* func, const char* args, ...);

    /**
     * Gets the global instance of the script controller.
     * 
     * @return The global instance of the script controller (NULL if it hasn't yet been created).
     */
    static ScriptController* getInstance();

    /**
     * Loads the given script file and executes its global code.
     * 
     * @param path The path to the script.
     */
    void loadScript(const char* path);

    /**
     * Gets a pointer to a bool (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    bool* getBoolPointer(int index);

    /**
     * Gets a pointer to a short (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    short* getShortPointer(int index);

    /**
     * Gets a pointer to an int (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    int* getIntPointer(int index);

    /**
     * Gets a pointer to a long (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    long* getLongPointer(int index);

    /**
     * Gets a pointer to an unsigned char (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    unsigned char* getUnsignedCharPointer(int index);

    /**
     * Gets a pointer to an unsigned short (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    unsigned short* getUnsignedShortPointer(int index);

    /**
     * Gets a pointer to an unsigned int (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    unsigned int* getUnsignedIntPointer(int index);

    /**
     * Gets a pointer to an unsigned long (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    unsigned long* getUnsignedLongPointer(int index);

    /**
     * Gets a pointer to a float (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    float* getFloatPointer(int index);

    /**
     * Gets a pointer to a double (as an array-use SAFE_DELETE_ARRAY to clean up) for the given stack index.
     * 
     * @param index The stack index.
     * @return The pointer.
     */
    double* getDoublePointer(int index);

    /**
     * Gets an object pointer of the given type for the given stack index.
     * 
     * @param type The type of object pointer to retrieve.
     * @param index The stack index.
     * @param nonNull Whether the pointer must be non-null (e.g. if the parameter we 
     *      are retreiving is actually a reference or by-value parameter).
     * @return The object pointer or <code>NULL</code> if the data at the stack index
     *      is not an object or if the object is not derived from the given type.
     * @script{ignore}
     */
    template<typename T> T* getObjectPointer(int index, const char* type, bool nonNull);

    /**
     * Gets a string for the given stack index.
     * 
     * @param index The stack index.
     * @return The string or <code>NULL</code>.
     * @script{ignore}
     */
    const char* getString(int index, bool isStdString);

    /**
     * Gets the global boolean variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global boolean variable.
     * @script{ignore}
     */
    bool getBool(const char* name);

    /**
     * Gets the global char variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global char variable.
     * @script{ignore}
     */
    char getChar(const char* name);

    /**
     * Gets the global short variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global short variable.
     * @script{ignore}
     */
    short getShort(const char* name);

    /**
     * Gets the global int variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global int variable.
     * @script{ignore}
     */
    int getInt(const char* name);

    /**
     * Gets the global long variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global long variable.
     * @script{ignore}
     */
    long getLong(const char* name);

    /**
     * Gets the global unsigned char variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global unsigned char variable.
     * @script{ignore}
     */
    unsigned char getUnsignedChar(const char* name);

    /**
     * Gets the global unsigned short variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global unsigned short variable.
     * @script{ignore}
     */
    unsigned short getUnsignedShort(const char* name);

    /**
     * Gets the global unsigned int variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global unsigned int variable.
     * @script{ignore}
     */
    unsigned int getUnsignedInt(const char* name);

    /**
     * Gets the global unsigned long variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global unsigned long variable.
     * @script{ignore}
     */
    unsigned long getUnsignedLong(const char* name);

    /**
     * Gets the global float variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global float variable.
     * @script{ignore}
     */
    float getFloat(const char* name);

    /**
     * Gets the global double variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global double variable.
     * @script{ignore}
     */
    double getDouble(const char* name);

    /**
     * Gets the global string variable with the given name.
     * 
     * @param name The name of the variable.
     * @return The global string variable.
     * @script{ignore}
     */
    const char* getString(const char* name);

    /**
     * Gets the global pointer variable of the given type with the given name.
     * 
     * @param type The type of the variable in Lua.
     * @param name The name of the variable.
     * @return The global pointer variable.
     * @script{ignore}
     */
    template<typename T>T* getObjectPointer(const char* type, const char* name);

    /**
     * Sets the global boolean variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The boolean variable.
     * @script{ignore}
     */
    void setBool(const char* name, bool v);

    /**
     * Sets the global char variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The char variable.
     * @script{ignore}
     */
    void setChar(const char* name, char v);

    /**
     * Sets the global short variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The short variable.
     * @script{ignore}
     */
    void setShort(const char* name, short v);

    /**
     * Sets the global int variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The int variable.
     * @script{ignore}
     */
    void setInt(const char* name, int v);

    /**
     * Sets the global long variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The long variable.
     * @script{ignore}
     */
    void setLong(const char* name, long v);

    /**
     * Gets the global unsigned char variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The unsigned char variable.
     * @script{ignore}
     */
    void setUnsignedChar(const char* name, unsigned char v);

    /**
     * Sets the global unsigned short variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The unsigned short variable.
     * @script{ignore}
     */
    void setUnsignedShort(const char* name, unsigned short v);

    /**
     * Sets the global unsigned int variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The unsigned int variable.
     * @script{ignore}
     */
    void setUnsignedInt(const char* name, unsigned int v);

    /**
     * Sets the global unsigned long variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The unsigned long variable.
     * @script{ignore}
     */
    void setUnsignedLong(const char* name, unsigned long v);

    /**
     * Sets the global float variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The float variable.
     * @script{ignore}
     */
    void setFloat(const char* name, float v);

    /**
     * Sets the global double variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The double variable.
     * @script{ignore}
     */
    void setDouble(const char* name, double v);

    /**
     * Sets the global string variable with the given name.
     * 
     * @param name The name of the variable.
     * @param v The string variable.
     * @script{ignore}
     */
    void setString(const char* name, const char* v);

    /**
     * Sets the global pointer variable of the given type with the given name.
     * 
     * @param type The type of the variable in Lua.
     * @param name The name of the variable.
     * @param v The pointer variable.
     * @script{ignore}
     */
    template<typename T>void setObjectPointer(const char* type, const char* name, T* v);

    /**
     * Registers the given library with Lua.
     * 
     * @param name The name of the library from within Lua.
     * @param function The library function mapping (Lua function names to C++ functions).
     * @script{ignore}
     */
    void registerLibrary(const char* name, const luaL_Reg* functions);

    /**
     * Registers the given boolean constant as valid for the given scope path.
     * 
     * @param name The name of the constant (what the user would use from Lua).
     * @param value The constant's value.
     * @param scopePath The list of containing classes, going inward from the most outer class.
     * @script{ignore}
     */
    void registerConstantBool(std::string name, bool value, std::vector<std::string> scopePath);

    /**
     * Registers the given number constant as valid for the given scope path.
     * 
     * @param name The name of the constant (what the user would use from Lua).
     * @param value The constant's value.
     * @param scopePath The list of containing classes, going inward from the most outer class.
     * @script{ignore}
     */
    void registerConstantNumber(std::string name, double value, std::vector<std::string> scopePath);

    /**
     * Registers the given string constant as valid for the given scope path.
     * 
     * @param name The name of the constant (what the user would use from Lua).
     * @param value The constant's value.
     * @param scopePath The list of containing classes, going inward from the most outer class.
     * @script{ignore}
     */
    void registerConstantString(std::string name, std::string value, std::vector<std::string> scopePath);

    /**
     * Registers the given class type with Lua.
     * 
     * @param name The name of the class from within Lua.
     * @param members The library function mapping for all the member functions (Lua function names to C++ functions).
     * @param newFunction The function to call that creates an instance of the class.
     * @param deleteFunction The function to call that destroys an instance of the class.
     * @param statics The library function mapping for all the static functions (Lua function names to C++ functions).
     * @param scopePath For an inner class, this is a list of its containing classes, going inward from the most outer class.
     * @script{ignore}
     */
    void registerClass(const char* name, const luaL_Reg* members, lua_CFunction newFunction, lua_CFunction deleteFunction, const luaL_Reg* statics,
        std::vector<std::string> scopePath = std::vector<std::string>());

    /**
     * Register a function with Lua.
     * 
     * @param luaFunction The name of the function from within Lua.
     * @param cppFunction The C++ function pointer.
     * @script{ignore}
     */
    void registerFunction(const char* luaFunction, lua_CFunction cppFunction);

    /**
     * Sets the global inheritance hierarchy.
     * 
     * @param hierarchy The inheritance hierarchy stored as a map of class names
     *      to a list of all derived class names.
     * @script{ignore}
     */
    void setGlobalHierarchy(std::map<std::string, std::vector<std::string> > hierarchy);

    /**
     * Checks that the parameter at the given stack position is a boolean and returns it.
     * 
     * @param state The Lua state.
     * @param n The stack index.
     * @return The boolean (if successful; otherwise it logs an error).
     * @script{ignore}
     */
    static bool luaCheckBool(lua_State* state, int n);

private:

    /**
     * Represents a Lua callback function binding.
     */
    enum ScriptCallback
    {
        INITIALIZE = 0,
        UPDATE,
        RENDER,
        FINALIZE,
        KEY_EVENT,
        MOUSE_EVENT,
        TOUCH_EVENT,
        GAMEPAD_EVENT,
        CALLBACK_COUNT,
        INVALID_CALLBACK = CALLBACK_COUNT
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
     * Callback for when the controller is initialized.
     */
    void initialize();

    /**
     * Initializes the game using the appropriate callback script (if it was specified).
     */
    void initializeGame();

    /*
     * Callback for when the controller is finalized.
     */
    void finalize();

    /**
     * Finalizes the game using the appropriate callback script (if it was specified).
     */
    void finalizeGame();
    
    /**
     * Callback for when the controller receives a frame update event.
     */
    void update(float elapsedTime);

    /**
     * Renders the game using the appropriate callback script (if it was specified).
     */
    void render(float elapsedTime);

    /**
     * Script keyboard callback on key events.
     *
     * @param evt The key event that occured.
     * @param key If evt is KEY_PRESS or KEY_RELEASE then key is the key code from Keyboard::Key.
     *            If evt is KEY_CHAR then key is the unicode value of the character.
     * 
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * Script touch callback on touch events.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @see Touch::TouchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Script mouse callback on mouse events. If the game does not consume the mouse move event or left mouse click event
     * then it is interpreted as a touch event instead.
     *
     * @param evt The mouse event that occurred.
     * @param x The x position of the mouse in pixels. Left edge is zero.
     * @param y The y position of the mouse in pixels. Top edge is zero.
     * @param wheelDelta The number of mouse wheel ticks. Positive is up (forward), negative is down (backward).
     *
     * @return True if the mouse event is consumed or false if it is not consumed.
     *
     * @see Mouse::MouseEvent
     */
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * Script gamepad callback on gamepad events.
     *
     * @param evt The gamepad event that occurred.
     * @param gamepad the gamepad the event occurred on
     */
    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

    /**
     * Calls the specifed Lua function using the given parameters.
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
     */
    void executeFunctionHelper(int resultCount, const char* func, const char* args, va_list& list);

    /**
     * Registers the given script callback.
     * 
     * @param callback The script callback to register for.
     * @param function The name of the function within the Lua script to call.
     */
    void registerCallback(ScriptCallback callback, std::string function);

    lua_State* _lua;
    unsigned int _returnCount;
    std::map<std::string, std::vector<std::string> > _hierarchy;
    static ScriptController* __instance;
    std::string* _callbacks[CALLBACK_COUNT];
};

}

#include "ScriptController.inl"

#endif
