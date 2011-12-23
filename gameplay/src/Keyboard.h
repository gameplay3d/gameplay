
#ifndef KEYBOARD_H_
#define KEYBOARD_H_

namespace gameplay
{

/**
 * Keyboard key event and key definitions.
 */
class Keyboard
{
    friend class Platform;

public:

   /**
     * The keyboard event.
     */
    enum KeyEvent
    {
        KEY_PRESS,
        KEY_RELEASE
    };

    /**
     * The key (code).
     */
    enum Key
    {
        KEY_NONE,
        KEY_PAUSE,
        KEY_SCROLL_LOCK,
        KEY_PRINT,
        KEY_SYSREQ,
        KEY_BREAK,
        KEY_ESCAPE,
        KEY_BACKSPACE,
        KEY_TAB,
        KEY_BACK_TAB,
        KEY_RETURN,
        KEY_CAPS_LOCK,
        KEY_LEFT_SHIFT,
        KEY_RIGHT_SHIFT,
        KEY_LEFT_CTRL,
        KEY_RIGHT_CTRL,
        KEY_LEFT_ALT,
        KEY_RIGHT_ALT,
        KEY_MENU,
        KEY_LEFT_HYPER,
        KEY_RIGHT_HYPER,
        KEY_INSERT,
        KEY_HOME,
        KEY_PG_UP,
        KEY_DELETE,
        KEY_END,
        KEY_PG_DOWN,
        KEY_LEFT_ARROW,
        KEY_RIGHT_ARROW,
        KEY_UP_ARROW,
        KEY_DOWN_ARROW,
        KEY_NUM_LOCK,
        KEY_KP_PLUS,
        KEY_KP_MINUS,
        KEY_KP_MULTIPLY,
        KEY_KP_DIVIDE,
        KEY_KP_ENTER,
        KEY_KP_HOME,
        KEY_KP_UP,
        KEY_KP_PG_UP,
        KEY_KP_LEFT,
        KEY_KP_FIVE,
        KEY_KP_RIGHT,
        KEY_KP_END,
        KEY_KP_DOWN,
        KEY_KP_PG_DOWN,
        KEY_KP_INSERT,
        KEY_KP_DELETE,
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
        KEY_SPACE,
        KEY_EXCLAM,
        KEY_QUOTE,
        KEY_NUMBER,
        KEY_DOLLAR,
        KEY_PERCENT,
        KEY_AMPERSAND,
        KEY_APOSTROPHE,
        KEY_LEFT_PARENTHESIS,
        KEY_RIGHT_PARENTHESIS,
        KEY_ASTERISK,
        KEY_PLUS,
        KEY_COMMA,
        KEY_MINUS,
        KEY_PERIOD,
        KEY_SLASH,
        KEY_ZERO,
        KEY_ONE,
        KEY_TWO,
        KEY_THREE,
        KEY_FOUR,
        KEY_FIVE,
        KEY_SIX,
        KEY_SEVEN,
        KEY_EIGHT,
        KEY_NINE,
        KEY_COLON,
        KEY_SEMICOLON,
        KEY_LESS_THAN,
        KEY_EQUAL,
        KEY_GREATER_THAN,
        KEY_QUESTION,
        KEY_AT,
        KEY_CAPITAL_A,
        KEY_CAPITAL_B,
        KEY_CAPITAL_C,
        KEY_CAPITAL_D,
        KEY_CAPITAL_E,
        KEY_CAPITAL_F,
        KEY_CAPITAL_G,
        KEY_CAPITAL_H,
        KEY_CAPITAL_I,
        KEY_CAPITAL_J,
        KEY_CAPITAL_K,
        KEY_CAPITAL_L,
        KEY_CAPITAL_M,
        KEY_CAPITAL_N,
        KEY_CAPITAL_O,
        KEY_CAPITAL_P,
        KEY_CAPITAL_Q,
        KEY_CAPITAL_R,
        KEY_CAPITAL_S,
        KEY_CAPITAL_T,
        KEY_CAPITAL_U,
        KEY_CAPITAL_V,
        KEY_CAPITAL_W,
        KEY_CAPITAL_X,
        KEY_CAPITAL_Y,
        KEY_CAPITAL_Z,
        KEY_LEFT_BRACKET,
        KEY_BACK_SLASH,
        KEY_RIGHT_BRACKET,
        KEY_CIRCUMFLEX,
        KEY_UNDERSCORE,
        KEY_GRAVE,
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
        KEY_LEFT_BRACE,
        KEY_BAR,
        KEY_RIGHT_BRACE,
        KEY_TILDE
    };

private:

    /**
     * Constructor. Private for static.
     */
    Keyboard() { }
};

}

#endif
