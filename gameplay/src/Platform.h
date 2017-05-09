#pragma once

#include "Graphics.h"

namespace gameplay
{

/**
 * Defines a platform abstraction.
 */
class Platform
{
public:   

	/**
	 * Gamepad events.
	 */
	enum GamepadEvent
	{
		CGAMEPAD_EVENT_CONNECTED,
		CGAMEPAD_EVENT_DISCONNECTED
	};

	/**
	 * Gamepad buttons.
	 */
	enum GamepadButton
	{
		GAMEPAD_BUTTON_A,
		GAMEPAD_BUTTON_B,
		GAMEPAD_BUTTON_X,
		GAMEPAD_BUTTON_Y,
		GAMEPAD_BUTTON_L1,
		GAMEPAD_BUTTON_L2,
		GAMEPAD_BUTTON_L3,
		GAMEPAD_BUTTON_R1,
		GAMEPAD_BUTTON_R2,
		GAMEPAD_BUTTON_R3,
		GAMEPAD_BUTTON_UP,
		GAMEPAD_BUTTON_DOWN,
		GAMEPAD_BUTTON_LEFT,
		GAMEPAD_BUTTON_RIGHT,
		GAMEPAD_BUTTON_MENU1,
		GAMEPAD_BUTTON_MENU2,
		GAMEPAD_BUTTON_MENU3
	};

	/**
	 * Mouse events.
	 */
	enum MouseEvent
	{
		MOUSE_EVENT_PRESS_LEFT_BUTTON,
		MOUSE_EVENT_RELEASE_LEFT_BUTTON,
		MOUSE_EVENT_PRESS_MIDDLE_BUTTON = 3,
		MOUSE_EVENT_RELEASE_MIDDLE_BUTTON = 4,
		MOUSE_EVENT_PRESS_RIGHT_BUTTON = 5,
		MOUSE_EVENT_RELEASE_RIGHT_BUTTON = 6,
		MOUSE_EVENT_MOVE = 7,
		MOUSE_EVENT_WHEEL = 8
	};

	/**
	 * Keyboard events.
	 */
	enum KeyboardEvent
	{
		KEYBOARD_EVENT_PRESS,
		KEYBOARD_EVENT_RELEASE,
		KEYBOARD_EVENT_CHAR
	};

	/**
	 * The keybaord keys(codes).
	 */
	enum KeyboardKey
	{
		KEYBOARD_KEY_NONE = 0,
		KEYBOARD_KEY_PAUSE = 0x0013,
		KEYBOARD_KEY_SCROLL_LOCK = 0x1014,
		KEYBOARD_KEY_PRINT = 0x1061,
		KEYBOARD_KEY_SYSREQ = 0x106A,
		KEYBOARD_KEY_BREAK = 0x106B,
		KEYBOARD_KEY_ESCAPE = 0x001B,
		KEYBOARD_KEY_BACKSPACE = 0x0008,
		KEYBOARD_KEY_TAB = 0x0009,
		KEYBOARD_KEY_BACK_TAB = 0x0089,
		KEYBOARD_KEY_RETURN = 0x000D,
		KEYBOARD_KEY_CAPS_LOCK = 0x00E5,
		KEYBOARD_KEY_SHIFT = 0x00E1,
		KEYBOARD_KEY_CTRL = 0x00E3,
		KEYBOARD_KEY_ALT = 0x00E9,
		KEYBOARD_KEY_MENU = 0x1067,
		KEYBOARD_KEY_HYPER = 0x10ED,
		KEYBOARD_KEY_INSERT = 0x1063,
		KEYBOARD_KEY_HOME = 0x1050,
		KEYBOARD_KEY_PG_UP = 0x1055,
		KEYBOARD_KEY_DELETE = 0x10FF,
		KEYBOARD_KEY_END = 0x1057,
		KEYBOARD_KEY_PG_DOWN = 0x1056,
		KEYBOARD_KEY_LEFT_ARROW = 0x1051,
		KEYBOARD_KEY_RIGHT_ARROW = 0x1053,
		KEYBOARD_KEY_UP_ARROW = 0x1052,
		KEYBOARD_KEY_DOWN_ARROW = 0x1054,
		KEYBOARD_KEY_NUM_LOCK = 0x107F,
		KEYBOARD_KEY_KP_PLUS = 0x10AB,
		KEYBOARD_KEY_KP_MINUS = 0x10AD,
		KEYBOARD_KEY_KP_MULTIPLY = 0x10AA,
		KEYBOARD_KEY_KP_DIVIDE = 0x10AF,
		KEYBOARD_KEY_KP_ENTER = 0x108D,
		KEYBOARD_KEY_KP_HOME = 0x10B7,
		KEYBOARD_KEY_KP_UP = 0x10B8,
		KEYBOARD_KEY_KP_PG_UP = 0x10B9,
		KEYBOARD_KEY_KP_LEFT = 0x10B4,
		KEYBOARD_KEY_KP_FIVE = 0x10B5,
		KEYBOARD_KEY_KP_RIGHT = 0x10B6,
		KEYBOARD_KEY_KP_END = 0x10B1,
		KEYBOARD_KEY_KP_DOWN = 0x10B2,
		KEYBOARD_KEY_KP_PG_DOWN = 0x10B3,
		KEYBOARD_KEY_KP_INSERT = 0x10B0,
		KEYBOARD_KEY_KP_DELETE = 0x10AE,
		KEYBOARD_KEY_F1 = 0x00BE,
		KEYBOARD_KEY_F2 = 0x00BF,
		KEYBOARD_KEY_F3 = 0x00C0,
		KEYBOARD_KEY_F4 = 0x00C1,
		KEYBOARD_KEY_F5 = 0x00C2,
		KEYBOARD_KEY_F6 = 0x00C3,
		KEYBOARD_KEY_F7 = 0x00C4,
		KEYBOARD_KEY_F8 = 0x00C5,
		KEYBOARD_KEY_F9 = 0x00C6,
		KEYBOARD_KEY_F10 = 0x00C7,
		KEYBOARD_KEY_F11 = 0x00C8,
		KEYBOARD_KEY_F12 = 0x00C9,
		KEYBOARD_KEY_SPACE = ' ',
		KEYBOARD_KEY_EXCLAM = '!',
		KEYBOARD_KEY_QUOTE = '"',
		KEYBOARD_KEY_NUMBER = '#',
		KEYBOARD_KEY_DOLLAR = '$',
		KEYBOARD_KEY_PERCENT = '%',
		KEYBOARD_KEY_CIRCUMFLEX = '^',
		KEYBOARD_KEY_AMPERSAND = '&',
		KEYBOARD_KEY_APOSTROPHE = '\'',
		KEYBOARD_KEY_LEFT_PARENTHESIS = '(',
		KEYBOARD_KEY_RIGHT_PARENTHESIS = ')',
		KEYBOARD_KEY_ASTERISK = '*',
		KEYBOARD_KEY_PLUS = '+',
		KEYBOARD_KEY_COMMA = ',',
		KEYBOARD_KEY_MINUS = '-',
		KEYBOARD_KEY_PERIOD = '.',
		KEYBOARD_KEY_SLASH = '/',
		KEYBOARD_KEY_ZERO = '0',
		KEYBOARD_KEY_ONE = '1',
		KEYBOARD_KEY_TWO = '2',
		KEYBOARD_KEY_THREE = '3',
		KEYBOARD_KEY_FOUR = '4',
		KEYBOARD_KEY_FIVE = '5',
		KEYBOARD_KEY_SIX = '6',
		KEYBOARD_KEY_SEVEN = '7',
		KEYBOARD_KEY_EIGHT = '8',
		KEYBOARD_KEY_NINE = '9',
		KEYBOARD_KEY_COLON = ':',
		KEYBOARD_KEY_SEMICOLON = ';',
		KEYBOARD_KEY_LESS_THAN = '<',
		KEYBOARD_KEY_EQUAL = '=',
		KEYBOARD_KEY_GREATER_THAN = '>',
		KEYBOARD_KEY_QUESTION = '?',
		KEYBOARD_KEY_AT = '@',
		KEYBOARD_KEY_CAPITAL_A = 'A',
		KEYBOARD_KEY_CAPITAL_B = 'B',
		KEYBOARD_KEY_CAPITAL_C = 'C',
		KEYBOARD_KEY_CAPITAL_D = 'D',
		KEYBOARD_KEY_CAPITAL_E = 'E',
		KEYBOARD_KEY_CAPITAL_F = 'F',
		KEYBOARD_KEY_CAPITAL_G = 'G',
		KEYBOARD_KEY_CAPITAL_H = 'H',
		KEYBOARD_KEY_CAPITAL_I = 'I',
		KEYBOARD_KEY_CAPITAL_J = 'J',
		KEYBOARD_KEY_CAPITAL_K = 'K',
		KEYBOARD_KEY_CAPITAL_L = 'L',
		KEYBOARD_KEY_CAPITAL_M = 'M',
		KEYBOARD_KEY_CAPITAL_N = 'N',
		KEYBOARD_KEY_CAPITAL_O = 'O',
		KEYBOARD_KEY_CAPITAL_P = 'P',
		KEYBOARD_KEY_CAPITAL_Q = 'Q',
		KEYBOARD_KEY_CAPITAL_R = 'R',
		KEYBOARD_KEY_CAPITAL_S = 'S',
		KEYBOARD_KEY_CAPITAL_T = 'T',
		KEYBOARD_KEY_CAPITAL_U = 'U',
		KEYBOARD_KEY_CAPITAL_V = 'V',
		KEYBOARD_KEY_CAPITAL_W = 'W',
		KEYBOARD_KEY_CAPITAL_X = 'X',
		KEYBOARD_KEY_CAPITAL_Y = 'Y',
		KEYBOARD_KEY_CAPITAL_Z = 'Z',
		KEYBOARD_KEY_LEFT_BRACKET = '[',
		KEYBOARD_KEY_BACK_SLASH = '\\',
		KEYBOARD_KEY_RIGHT_BRACKET = ']',
		KEYBOARD_KEY_UNDERSCORE = '_',
		KEYBOARD_KEY_GRAVE = '`',
		KEYBOARD_KEY_A = 'a',
		KEYBOARD_KEY_B = 'b',
		KEYBOARD_KEY_C = 'c',
		KEYBOARD_KEY_D = 'd',
		KEYBOARD_KEY_E = 'e',
		KEYBOARD_KEY_F = 'f',
		KEYBOARD_KEY_G = 'g',
		KEYBOARD_KEY_H = 'h',
		KEYBOARD_KEY_I = 'i',
		KEYBOARD_KEY_J = 'j',
		KEYBOARD_KEY_K = 'k',
		KEYBOARD_KEY_L = 'l',
		KEYBOARD_KEY_M = 'm',
		KEYBOARD_KEY_N = 'n',
		KEYBOARD_KEY_O = 'o',
		KEYBOARD_KEY_P = 'p',
		KEYBOARD_KEY_Q = 'q',
		KEYBOARD_KEY_R = 'r',
		KEYBOARD_KEY_S = 's',
		KEYBOARD_KEY_T = 't',
		KEYBOARD_KEY_U = 'u',
		KEYBOARD_KEY_V = 'v',
		KEYBOARD_KEY_W = 'w',
		KEYBOARD_KEY_X = 'x',
		KEYBOARD_KEY_Y = 'y',
		KEYBOARD_KEY_Z = 'z',
		KEYBOARD_KEY_LEFT_BRACE = '{',
		KEYBOARD_KEY_BAR = '|',
		KEYBOARD_KEY_RIGHT_BRACE = '}',
		KEYBOARD_KEY_TILDE = '~',
		KEYBOARD_KEY_EURO = 0x20AC,
		KEYBOARD_KEY_POUND = 0x00A3,
		KEYBOARD_KEY_YEN = 0x00A5,
		KEYBOARD_KEY_MIDDLE_DOT = 0x0095,
		KEYBOARD_KEY_SEARCH = 0xFFAA
	};

	/**
	 * Touch events.
	 */
	enum TouchEvent
	{
		TOUCH_EVENT_PRESS,
		TOUCH_EVENT_RELEASE,
		TOUCH_EVEENT_MOVE
	};

	/**
	 * Starts up the platform and creates the initial windowing, input and graphics systems.
	 *
	 * @param argc The number of startup arguments.
	 * @param argv The array of startupu string values.
	 * @return true of the platform starts up  successfully, false if fails.
	 */
    virtual bool startup(int argc, char** argv) = 0;  

	/**
	 * Shuts down and cleans up any platform resource created and then exit application.
	 */
    virtual void shutdown() = 0; 

	/**
	 * Enters the application message pump and handles on platform messages.
	 *
	 * @return The exit code when the application closes.
	 */
    virtual int enterMessagePump() = 0;

	/**
	 * Gets the windows (native window handle/pointer).
	 *
	 * @return The window (native window handle/pointer).
	 */
    virtual unsigned long getWindow() = 0;

	/**
	 * Gets the native connection (native connection/context)
	 * 
	 * @return The connection (native connection/context).
	 */
    virtual unsigned long getConnection() = 0;

	/**
	 * Determines if a gamepad button is pressed.
	 *
	 * @param button The button to test.
	 * @param gamepadIndex The gamepad index to test
	 * @return true if the button is currently down, false if up.
	 */
	virtual bool isGamepadButtonPressed(GamepadButton button, size_t gamepadIndex) = 0;

	/**
	 * Gets the current gamepad axis values.
	 *
	 * The normalized range of the axis values are [-1.0, 1.0]
	 *
	 * @param leftVertical The left vertical axis value. 
	 * @param leftHorizontal The left horizontal axis value.
	 * @param rightVertical The right horizontal axis value.
	 * @param rightHorizontal The right horizontal axis value.
	 * @param gamepadIndex The gamepad index to get the axis value from.
	 */
	virtual void getGamepadAxisValues(float* leftVertical, float* leftHorizontal,
                                      float* rightVertical, float* rightHorizontal, size_t gamepadIndex) = 0;

	/**
	 * Gets the current gamepad trigger values.
	 *
	 * The normalized range of the trigger values are [0.0, 1.0]
	 *
	 * @param leftTrigger The left trigger value.
	 * @param rightTrigger The right trigger value.
	 * @param gamepadIndex The gamepad index to get the trigger values for.
	 */
	virtual void getGamepadTriggerValues(float* leftTrigger, float* rightTrigger, size_t gamepadIndex) = 0;

    /**
     * Gets the current accelerometer values for use as an indication of device
     * orientation. Despite its name, implementations are at liberty to combine
     * accelerometer data with data from other sensors as well, such as the gyros.
     * This method is best used to obtain an indication of device orientation; it
     * does not necessarily distinguish between acceleration and rotation rate.
     *
     * @param pitch The pitch angle returned (in degrees). Zero if hasAccelerometer() returns false.
     * @param roll The roll angle returned (in degrees). Zero if hasAccelerometer() returns false.
     */
	virtual void getAccelerometerValues(float* pitch, float* roll) = 0;

	/**
	 * Gets the current sensor values (raw), if equipped, allowing a distinction between device acceleration
	 * and rotation rate.
	 *
	 * Returns zeros on platforms with no corresponding support.
	 *
	 * @param accelX The x-coordinate of the raw accelerometer data.
	 * @param accelY The y-coordinate of the raw accelerometer data.
	 * @param accelZ The z-coordinate of the raw accelerometer data.
	 * @param gyroX The x-coordinate of the raw gyroscope data.
	 * @param gyroY The y-coordinate of the raw gyroscope data.
	 * @param gyroZ The z-coordinate of the raw gyroscope data.
	 */
	virtual void getSensorValues(float* accelX, float* accelY, float* accelZ, float* gyroX, float* gyroY, float* gyroZ) = 0;
};

}


