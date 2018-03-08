#pragma once

namespace gameplay
{

/**
 * Defines user input api.
 */
class Input
{
public:

	static const uint32_t GAMEPADS_MAX = 4;

    /**
     * Defines input keys.
     */
    enum Key
    {
		KEY_NONE = 0,
		KEY_ESC,
		KEY_RETURN,
		KEY_TAB,
		KEY_SPACE,
		KEY_BACKSPACE,
		KEY_UP,
		KEY_DOWN,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_INSERT,
		KEY_DELETE,
		KEY_HOME,
		KEY_END,
		KEY_PAGE_UP,
		KEY_PAGE_DOWN,
		KEY_PRINT,
		KEY_PLUS,
		KEY_MINUS,
		KEY_LEFT_BRACKET,
		KEY_RIGHT_BRACKET,
		KEY_SEMICOLON,
		KEY_QUOTE,
		KEY_COMMA,
		KEY_PERIOD,
		KEY_SLASH,
		KEY_BACKSLASH,
		KEY_TILDE,
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
		KEY_NUMPAD0,
		KEY_NUMPAD1,
		KEY_NUMPAD2,
		KEY_NUMPAD3,
		KEY_NUMPAD4,
		KEY_NUMPAD5,
		KEY_NUMPAD6,
		KEY_NUMPAD7,
		KEY_NUMPAD8,
		KEY_NUMPAD9,
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
		KEY_GAMEPAD_A,
		KEY_GAMEPAD_B,
		KEY_GAMEPAD_X,
		KEY_GAMEPAD_Y,
		KEY_GAMEPAD_THUMB_LEFT,
		KEY_GAMEPAD_THUMB_RIGHT,
		KEY_GAMEPAD_SHOULDER_LEFT,
		KEY_GAMEPAD_SHOULDER_RIGHT,
		KEY_GAMEPAD_UP,
		KEY_GAMEPAD_DOWN,
		KEY_GAMEPAD_LEFT,
		KEY_GAMEPAD_RIGHT,
		KEY_GAMEPAD_BACK,
		KEY_GAMEPAD_START,
		KEY_GAMEPAD_GUIDE,
		KEY_COUNT
	};

	/**
	 * Defines key modifiers
	 */
	enum KeyModifier
	{
		KEY_MODIFIER_NONE = 0,
		KEY_MODIFIER_LEFT_ALT = 0x01,
		KEY_MODIFIER_RIGHT_ALT = 0x02,
		KEY_MODIFIER_LEFT_CTRL   = 0x04,
		KEY_MODIFIER_RIGHT_CTRL  = 0x08,
		KEY_MODIFIER_LEFT_SHIFT  = 0x10,
		KEY_MODIFIER_RIGHT_SHIFT = 0x20,
		KEY_MODIFIER_LEFT_META   = 0x40,
		KEY_MODIFIER_RIGHT_META  = 0x80
	};

    /**
     * Defines input mouse buttons.
     */
    enum MouseButton
    {
		MOUSE_BUTTON_NONE,
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE,
		MOUSE_BUTTON_COUNT
    };

    /**
     * Defines gamepad axis
     */
	enum GamepadAxis
	{
		GAMEPAD_AXIS_LEFT_X,
		GAMEPAD_AXIS_LEFT_Y,
		GAMEPAD_AXIS_LEFT_Z,
		GAMEPAD_AXIS_RIGHT_X,
		GAMEPAD_AXIS_RIGHT_Y,
		GAMEPAD_AXIS_RIGHT_Z,
		GAMEPAD_AXIS_COUNT
	};

    /**
     * Gets the string name for a key.
     *
     * @param key The key to get the string name for.
     * @return The string name for a key.
     */
	static std::string getKeyName(Input::Key key);

    /**
     * Gets the ascii keycode for a key.
     *
     * @param key The key to get the ascii keycode for.
     * @param keyModifiers The key modifiers applied.
     * @return The ascii keycode for a key.
     */
	static char getKeyCode(Input::Key key, uint8_t keyModifiers);

    static bool isKeyHeld(Input::Key key);

    static bool isKeyDown(Input::Key key);

    static bool isKeyUp(Input::Key key);

    static bool isButtonHeld(std::string buttonName);

    static bool isButtonDown(std::string buttonName);

    static bool isButtonUp(std::string buttonName);

    static bool getAxis(std::string axisName);

    static bool isMouseButtonHeld(MouseButton button);

    static bool isMouseButtonDown(MouseButton button);

    static bool isMouseButtonUp(MouseButton button);

	static bool isMouseLocked();

	static void setMouseLocked(bool lock);

    static float getMouseScroll();

    ////////////////////////////////////////////////////////////////////////////////////////
    // Used only by input providers (Platform/Editor)
    static Input* getInput();
    void initialize();
    void update();
    void postGamepadConnectionEvent(uint32_t controllerIndex, bool connected);
    void postGamepadAxisEvent(uint32_t controllerIndex, GamepadAxis axis, int value);
    void postMouseMotionEvent(int mx, int my, int mz);
    void postMousePressEvent(int mx, int my, int mz, MouseButton button, bool down);
    void postKeyCharEvent(char chr);
    void postKeyPressEvent(Key key, uint8_t keyModifiers, bool down);

private:

	Input();

	~Input();
};

}
