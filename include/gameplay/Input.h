#pragma once

#include "Defines.h"
#include "Types.h"
#include "Signal.h"

namespace gameplay
{

class Window;

enum class CursorMode : uint32_t
{
    NORMAL,
    HIDDEN,
    DISABLED
};

enum class CursorShape : uint32_t
{
    ARROW,
    IBEAM,
    CROSSHAIR,
    HAND,
    HRESIZE,
    VRESIZE
};

enum class Key : uint32_t
{
    KEY_UNKNOWN,
    KEY_SPACE,
    KEY_APOSTROPHE,
    KEY_COMMA,
    KEY_MINUS,
    KEY_PERIOD,
    KEY_SLASH,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_SEMICOLON,
    KEY_EQUAL,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_LEFT_BRACKET,
    KEY_RIGHT_BRACKET,
    KEY_BACKSLASH,
    KEY_GRAVE_ACCENT,
    KEY_ESCAPE,
    KEY_TAB,
    KEY_ENTER,
    KEY_DELETE,
    KEY_RIGHT,
    KEY_LEFT,
    KEY_DOWN,
    KEY_UP,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_CAPS_LOCK,
    KEY_NUM_LOCK,
    KEY_PRINT_SCREEN,
    KEY_PAUSE,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_NUMPAD_0,
    KEY_NUMPAD_1,
    KEY_NUMPAD_2,
    KEY_NUMPAD_3,
    KEY_NUMPAD_4,
    KEY_NUMPAD_5,
    KEY_NUMPAD_6,
    KEY_NUMPAD_7,
    KEY_NUMPAD_8,
    KEY_NUMPAD_9,
    KEY_NUMPAD_DECIMAL,
    KEY_NUMPAD_DIVIDE,
    KEY_NUMPAD_MULTIPLY,
    KEY_NUMPAD_SUBTRACT,
    KEY_NUMPAD_ADD,
    KEY_NUMPAD_ENTER,
    KEY_NUMPAD_EQUAL,
    KEY_LEFT_SHIFT,
    KEY_LEFT_CONTROL,
    KEY_LEFT_ALT,
    KEY_LEFT_SUPER,
    KEY_RIGHT_SHIFT,
    KEY_RIGHT_CONTROL,
    KEY_RIGHT_ALT,
    KEY_RIGHT_SUPER,
    KEY_MENU
};

enum class MouseButton : uint32_t
{
    LEFT,
    RIGHT,
    MIDDLE,
    X1,
    X2
};

enum class ButtonAction : uint8_t
{
    RELEASE,
    PRESS
};

enum class KeyAction : uint8_t
{
    RELEASE,
    PRESS,
    REPEAT
};

typedef uint32_t KeyModFlags;
constexpr KeyModFlags KEY_MOD_FLAGS_NONE = 0;
constexpr KeyModFlags KEY_MOD_FLAGS_SHIFT = 1 << 0;
constexpr KeyModFlags KEY_MOD_FLAGS_CONTROL = 1 << 1;
constexpr KeyModFlags KEY_MOD_FLAGS_ALT = 1 << 2;
constexpr KeyModFlags KEY_MOD_FLAGS_SUPER = 1 << 3;
constexpr KeyModFlags KEY_MOD_FLAGS_CAPS_LOCK = 1 << 4;
constexpr KeyModFlags KEY_MOD_FLAGS_NUM_LOCK = 1 << 5;

enum class GamepadButton : uint32_t
{
    BUTTON_A,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y,
    LEFT_BUMPER,
    RIGHT_BUMPER,
    BACK,
    START,
    GUIDE,
    LEFT_THUMB,
    RIGHT_THUMB,
    DPAD_UP,
    DPAD_RIGHT,
    DPAD_DOWN,
    DPAD_LEFT,
    COUNT
};

enum class GamepadAxes : uint32_t
{
    LEFT_X,
    LEFT_Y,
    RIGHT_X,
    RIGHT_Y,
    LEFT_TRIGGER,
    RIGHT_TRIGGER,
    COUNT
};

struct GamepadState
{
    ButtonAction buttons[15];
    float axes[6];
};

enum class GamepadChangeEvent : uint32_t
{
    UNKNOWN,
    CONNECTED,
    DISCONNECTED
};

enum class InputMode : uint32_t
{
    STICKY_KEYS,
    STICKY_MOUSE_BUTTONS, 
    LOCK_KEY_MODS,
    RAW_MOUSE_MOTION
};

/**
 * Defines all static utility for polling for input on the main window.
 *
 * Used for polling keyboard, mouse/cursor and joystick/gamepad actions.
 * 
 * @see Window for event based input handling.
 */
class Input
{
public:

    /**
     * Sets the position of the coordinates of the cursor.
     *
     * The position is in screen coordinates, of the cursor relative
     * to the upper-left corner of the client area of the specified window.
     * The window must have input focus. If the window does not have input
     * focus when this function is called, it fails silently.
     *
     * Do not use this function to implement things like camera controls.
     * @see set_cursor_mode with `CursorMode::DISABLED`
     * to hide the cursor, transparently re-centers it and provides
     * unconstrained cursor motion.
     *
     * @param pos The coorinates which to set the cursor position to.
     */
    static void set_cursor_pos(const Double2& pos);

    /**
     * Gets the position of the coordinates of the cursor.
     *
     * The position is in screen coordinates, of the cursor relative
     * to the upper-left corner of the client area of the specified window.
     * The window must have input focus. If the window does not have input
     * focus when this function is called, it fails silently.
     *
     * @return The cursor position coordinates
     */
    static Double2 get_cursor_pos();

    /**
     * Sets the mode for the cursor.
     *
     * CursorMode::NORMAL makes the cursor visible and behaving normally.
     * CursorMode::HIDDEN makes the cursor invisible when it is over the content area
     *  of the window but does not restrict the cursor from leaving.
     * CursorMode::DISABLED makes the cursor invisible when it is over the
     * content area of the window but does not restrict the cursor from leaving.
     *
     * @param mode The cursor mode to set to.
     */
    static void set_cursor_mode(CursorMode mode);

    /**
     * Gets the mode for the cursor.
     *
     * CursorMode::NORMAL makes the cursor visible and behaving normally.
     * CursorMode::HIDDEN makes the cursor invisible when it is over the content area
     *  of the window but does not restrict the cursor from leaving.
     * CursorMode::DISABLED makes the cursor invisible when it is over the
     * content area of the window but does not restrict the cursor from leaving.
     *
     * @return The current cursor mode.
     */
    static CursorMode get_cursor_mode();

    /**
     * Sets (enables/disables) any special input modes.
     * 
     * @param mode The input mode to be set.
     * @param enabled true to enable the input mode, false to disable it.
     */
    static void set_input_mode_enabled(InputMode mode, bool enabled);

    /**
     * Checks if an input mode is enabled.
     *
     * @param mode The input mode to be enabled.
     * @return true if the input mode is enabled, false if disabled.
     */
    static bool is_input_mode_enabled(InputMode mode);

    /**
     * Get the current mouse button action (state).
     *
     * @param button The button to get the action (state).
     * @return The current mouse button action (state)
     */
    static ButtonAction get_mouse_button_action(MouseButton button);

    /**
     * Get the current key action (state) of a key.
     *
     * @param key The key to check.
     * @return The current key action (state) of a key.
     */
    static KeyAction get_key_action(Key key);

    /**
     * Gets the name of the specified printable key, encoded as UTF-8.
     *
     * @parm key The key to get the name of.
     * @return The name of the specified printable key, encoded as UTF-8.
     */
    static const char* get_key_name(Key key);

    /**
     * Signaled when the gamepad change event has occurred.
     *
     * emits:
     * <int32_t> The gamepad index
     * <GamepadChangeEvent> The gamepad chagne event.
     */
    static Signal<int32_t, GamepadChangeEvent> on_gamepad_changed;

    /**
     * Checks if a gamepad is currently connected.
     *
     * @param gamepadIndex The index of the gamepad to check.
     */
    static bool is_gamepad_connected(int32_t gamepadIndex);

    /**
     * Checks if a gamepad has a properly configured mappings entry.
     * 
     * Based on SDL_GameControllerDB
     * https://github.com/gabomdq/SDL_GameControllerDB
     * A community sourced database of game controller mappings
     * to be used with SDL2 Game Controller functionality.
     * 
     * If it is not mapped. Then it is consider unsupported.
     * @return true if the gamepad is supported in the 
     */
    static bool has_gamepad_mappings_db(int32_t gamepadIndex);

    /**
     * Sets a gamepad mappings db.
     *
     * Based on SDL_GameControllerDB
     * https://github.com/gabomdq/SDL_GameControllerDB
     * A community sourced database of game controller mappings
     * to be used with SDL2 Game Controller functionality.
     * 
     * @param str The string containing the gamepad mappings.
     * @return true if successfully set, false if an error occurred.
     */
    static bool set_gamepad_mappings_db(const char* str);

    /**
     * Gets the name of the supported gamepad.
     *
     * This will only be valid if has_gamepad_mappings_db() is true.
     *
     * @param gamepadIndex The index of the gamepad to get the name from.
     * @return The name of the supported gamepad.
     */
    static const char* get_gamepad_name(int32_t gamepadIndex);

    /**
     * Gets the GUID of the supported gamepad.
     *
     * This will only be valid if has_gamepad_mappings_db() is true.
     *
     * @param gamepadIndex The index of the gamepad to get the GUID from.
     * @return The GUID of the supported gamepad.
     */
    static const char* get_gamepad_guid(int32_t gamepadIndex);

    /**
     * Gets the gamepad state of the supported gamepad.
     *
     * This will only be valid if has_gamepad_mappings_db() is true.
     * @param gamepadIndex The index of the gamepad to get the gamepad state from.
     * @param gamepadState The pointer to the gamepad state to be updated with state.
     */
    static void get_gamepad_state(int32_t gamepadIndex, GamepadState* gamepadState);

    /**
     * Sets user pointer associated with this gamepad .
     *
     * @param gamepadIndex The gamepad index to associate the user pointer with.
     * @param userPtr The user pointer data to associate to the gamepad.
     */
    static void set_gamepad_user_ptr(int32_t gamepadIndex, void* userPtr);

    /**
     * Gets the user pointer associated with a gamepad at the specified index.
     *
     * @param gamepadIndex The gamepad index to get the associate user pointer from.
     * @return The user pointer to associated with the gamepad.
     */
    static void* get_gamepad_user_ptr(int32_t gamepadIndex);

private:
    Input();
    ~Input();
};
}

