#ifndef SCRIPTCONTROLLER_H_
#define SCRIPTCONTROLLER_H_

#include "Base.h"
#include "Game.h"
//#include "Gamepad.h"
#include "Control.h"

namespace gameplay
{

/** Function pointer typedef for string-from-enum conversion functions. */
typedef const char* (*luaStringEnumConversionFunction)(std::string&, unsigned int);

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
 * Adds the given function as a string-from-enumerated value conversion function.
 * 
 * @param stringFromEnum The pointer to the string-from-enum conversion function.
 * 
 * @script{ignore}
 */
void addStringFromEnumConversionFunction(luaStringEnumConversionFunction stringFromEnum);

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
     * @param path The path to the script.
     * @param forceReload Whether the script should be reloaded if it has already been loaded.
     */
    void loadScript(const char* path, bool forceReload = false);

    /**
     * Given a URL, loads the referenced file and returns the referenced function name.
     * 
     * @param url The url to load.
     * 
     * @return The function that the URL references.
     */
    std::string loadUrl(const char* url);

    /**
     * Registers the given script callback.
     *
     * The 'callback' parameter must be one of the supported global callback
     * event functions. The following strings are accepted: initialize, finalize,
     * update, render, resizeEvent, keyEvent, touchEvent, mouseEvent, gamepadEvent.
     * Signatures for the registered script function must match that of the
     * corresponding signatures of these events on the Game class.
     *
     * @param callback The script callback to register for.
     * @param function The name of the script function to register.
     */
    void registerCallback(const char* callback, const char* function);

    /**
     * Unregisters the given script callback.
     *
     * @param callback The script callback to unregister for.
     * @param function The name of the script function to unregister.
     *
     * @see registerCallback(const char*, const char*)
     */
    void unregisterCallback(const char* callback, const char* function);

    /**
     * Calls the specified no-parameter Lua function.
     * 
     * @param func The name of the function to call.
     * 
     * @return The return value of the executed Lua function.
     */
    template<typename T> T executeFunction(const char* func);

    /**
     * Calls the specified Lua function using the given parameters.
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
     */
    template<typename T> T executeFunction(const char* func, const char* args, ...);

    /**
     * Calls the specified Lua function using the given parameters.
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
     * 
     * @return The return value of the executed Lua function.
     */
    template<typename T> T executeFunction(const char* func, const char* args, va_list* list);

    /**
     * Gets the global boolean script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a bool.
     * 
     * @return The global boolean script variable.
     * 
     * @script{ignore}
     */
    bool getBool(const char* name, bool defaultValue = false);

    /**
     * Gets the global char script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global char script variable.
     * 
     * @script{ignore}
     */
    char getChar(const char* name, char defaultValue = 0);

    /**
     * Gets the global short script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global short script variable.
     * 
     * @script{ignore}
     */
    short getShort(const char* name, short defaultValue = 0);

    /**
     * Gets the global int script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global int script variable.
     * 
     * @script{ignore}
     */
    int getInt(const char* name, int defaultValue = 0);

    /**
     * Gets the global long script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global long script variable.
     * 
     * @script{ignore}
     */
    long getLong(const char* name, long defaultValue = 0);

    /**
     * Gets the global unsigned char script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global unsigned char script variable.
     * 
     * @script{ignore}
     */
    unsigned char getUnsignedChar(const char* name, unsigned char defaultValue = 0);

    /**
     * Gets the global unsigned short script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global unsigned short script variable.
     * 
     * @script{ignore}
     */
    unsigned short getUnsignedShort(const char* name, unsigned short defaultValue = 0);

    /**
     * Gets the global unsigned int script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global unsigned int script variable.
     * 
     * @script{ignore}
     */
    unsigned int getUnsignedInt(const char* name, unsigned int defaultValue = 0);

    /**
     * Gets the global unsigned long script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global unsigned long script variable.
     * 
     * @script{ignore}
     */
    unsigned long getUnsignedLong(const char* name, unsigned long defaultValue = 0);

    /**
     * Gets the global float script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global float script variable.
     * 
     * @script{ignore}
     */
    float getFloat(const char* name, float defaultValue = 0);

    /**
     * Gets the global double script variable with the given name.
     * 
     * @param name The name of the variable.
     * @param defaultValue The default value to return if the variable is not a number.
     * 
     * @return The global double script variable.
     * 
     * @script{ignore}
     */
    double getDouble(const char* name, double defaultValue = 0);

    /**
     * Gets the global string variable with the given name.
     * 
     * @param name The name of the variable.
     * 
     * @return The string variable or NULL if the variable is not a string.
     * 
     * @script{ignore}
     */
    const char* getString(const char* name);

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
     * 
     * @script{ignore}
     */
    void setBool(const char* name, bool v);

    /**
     * Sets the global char script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The char value.
     * 
     * @script{ignore}
     */
    void setChar(const char* name, char v);

    /**
     * Sets the global short script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The short value.
     * 
     * @script{ignore}
     */
    void setShort(const char* name, short v);

    /**
     * Sets the global int script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The int value.
     * 
     * @script{ignore}
     */
    void setInt(const char* name, int v);

    /**
     * Sets the global long script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The long value.
     * 
     * @script{ignore}
     */
    void setLong(const char* name, long v);

    /**
     * Gets the global unsigned char script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned char value.
     * 
     * @script{ignore}
     */
    void setUnsignedChar(const char* name, unsigned char v);

    /**
     * Sets the global unsigned short script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned short value.
     * 
     * @script{ignore}
     */
    void setUnsignedShort(const char* name, unsigned short v);

    /**
     * Sets the global unsigned int script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned int value.
     * 
     * @script{ignore}
     */
    void setUnsignedInt(const char* name, unsigned int v);

    /**
     * Sets the global unsigned long script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The unsigned long value.
     * 
     * @script{ignore}
     */
    void setUnsignedLong(const char* name, unsigned long v);

    /**
     * Sets the global float script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The float value.
     * 
     * @script{ignore}
     */
    void setFloat(const char* name, float v);

    /**
     * Sets the global double script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The double value.
     * 
     * @script{ignore}
     */
    void setDouble(const char* name, double v);

    /**
     * Sets the global string script variable with the given name to the given value.
     * 
     * @param name The name of the script variable.
     * @param v The string value.
     * 
     * @script{ignore}
     */
    void setString(const char* name, const char* v);

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
     * Represents a callback function that can be registered for.
     */
    enum ScriptCallback
    {
        INITIALIZE = 0,
        UPDATE,
        RENDER,
        FINALIZE,
        RESIZE_EVENT,
        KEY_EVENT,
        MOUSE_EVENT,
        TOUCH_EVENT,
        GESTURE_SWIPE_EVENT,
        GESTURE_PINCH_EVENT,
        GESTURE_TAP_EVENT,
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
     * Script callback for game resize events.
     *
     * @param width The new width of the game window content area.
     * @param height The new height of the game window content area.
     */
    void resizeEvent(unsigned int width, unsigned int height);

    /**
     * Script keyboard callback on key events.
     *
     * @param evt The key event that occurred.
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
     * Script callback for Gesture::SWIPE events.
     *
     * @param x The x-coordinate of the start of the swipe.
     * @param y The y-coordinate of the start of the swipe.
     * @param direction The direction of the swipe
     *
     * @see Gesture::SWIPE_DIRECTION_UP
     * @see Gesture::SWIPE_DIRECTION_DOWN
     * @see Gesture::SWIPE_DIRECTION_LEFT
     * @see Gesture::SWIPE_DIRECTION_RIGHT
     */
    void gestureSwipeEvent(int x, int y, int direction);

    /**
     * Script callback for Gesture::PINCH events.
     *
     * @param x The centroid x-coordinate of the pinch.
     * @param y The centroid y-coordinate of the pinch.
     * @param scale The scale of the pinch.
     */
    void gesturePinchEvent(int x, int y, float scale);

    /**
     * Script callback for Gesture::TAP events.
     *
     * @param x The x-coordinate of the tap.
     * @param y The y-coordinate of the tap.
     */
    void gestureTapEvent(int x, int y);

    /**
     * Script gamepad callback on gamepad events.
     *
     * @param evt The gamepad event that occurred.
     * @param gamepad the gamepad the event occurred on
     */
    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex = 0);

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
     */
    void executeFunctionHelper(int resultCount, const char* func, const char* args, va_list* list);

    /**
     * Converts the given string to a valid script callback enumeration value
     * or to ScriptController::INVALID_CALLBACK if there is no valid conversion.
     * 
     * @param name The name of the callback.
     * 
     * @return The corresponding callback enumeration value.
     */
    static ScriptController::ScriptCallback toCallback(const char* name);

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
    friend void ScriptUtil::registerClass(const char* name, const luaL_Reg* members, lua_CFunction newFunction,
        lua_CFunction deleteFunction, const luaL_Reg* statics, const std::vector<std::string>& scopePath);
    friend void ScriptUtil::registerFunction(const char* luaFunction, lua_CFunction cppFunction);
    friend void ScriptUtil::setGlobalHierarchyPair(const std::string& base, const std::string& derived);
    friend void ScriptUtil::addStringFromEnumConversionFunction(luaStringEnumConversionFunction stringFromEnum);
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
    std::vector<std::string> _callbacks[CALLBACK_COUNT];
    std::set<std::string> _loadedScripts;
    std::vector<luaStringEnumConversionFunction> _stringFromEnum;
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
template<> void ScriptController::executeFunction<void>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> bool ScriptController::executeFunction<bool>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> char ScriptController::executeFunction<char>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> short ScriptController::executeFunction<short>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> int ScriptController::executeFunction<int>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> long ScriptController::executeFunction<long>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> unsigned char ScriptController::executeFunction<unsigned char>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> unsigned short ScriptController::executeFunction<unsigned short>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> unsigned int ScriptController::executeFunction<unsigned int>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> unsigned long ScriptController::executeFunction<unsigned long>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> float ScriptController::executeFunction<float>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> double ScriptController::executeFunction<double>(const char* func, const char* args, va_list* list);
/** Template specialization. */
template<> std::string ScriptController::executeFunction<std::string>(const char* func, const char* args, va_list* list);

}

#include "ScriptController.inl"

#endif
