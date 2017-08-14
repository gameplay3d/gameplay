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
     * Mouse button
     */
    enum MouseButton
    {
        MOUSE_BUTTON_LEFT = 0,
        MOUSE_BUTTON_RIGHT = 1,
        MOUSE_BUTTON_MIDDLE
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

    static bool isButtonHeld(std::string buttonName);

    static bool isButtonDown(std::string buttonName);

    static bool isButtonUp(std::string buttonName);

    static bool getAxis(std::string axisName);

    static bool isMouseButtonHeld(MouseButton button);

    static bool isMouseButtonDown(MouseButton button);

    static bool isMouseButtonUp(MouseButton button);

    static float getMouseScroll();

    static bool isKeyHeld(KeyboardKey key);

    static bool isKeyDown(KeyboardKey key);

    static bool isKeyUp(KeyboardKey key);

    protected:

        static std::map<std::string, unsigned int> _inputMapping;
        static std::vector<bool> _buttonState;
        static std::vector<bool> _buttonsHeld;
};

}


