#include "Base.h"
#include "lua_KeyboardKey.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_KeyboardKey_KEY_NONE = "KEY_NONE";
static const char* luaEnumString_KeyboardKey_KEY_PAUSE = "KEY_PAUSE";
static const char* luaEnumString_KeyboardKey_KEY_SCROLL_LOCK = "KEY_SCROLL_LOCK";
static const char* luaEnumString_KeyboardKey_KEY_PRINT = "KEY_PRINT";
static const char* luaEnumString_KeyboardKey_KEY_SYSREQ = "KEY_SYSREQ";
static const char* luaEnumString_KeyboardKey_KEY_BREAK = "KEY_BREAK";
static const char* luaEnumString_KeyboardKey_KEY_ESCAPE = "KEY_ESCAPE";
static const char* luaEnumString_KeyboardKey_KEY_BACKSPACE = "KEY_BACKSPACE";
static const char* luaEnumString_KeyboardKey_KEY_TAB = "KEY_TAB";
static const char* luaEnumString_KeyboardKey_KEY_BACK_TAB = "KEY_BACK_TAB";
static const char* luaEnumString_KeyboardKey_KEY_RETURN = "KEY_RETURN";
static const char* luaEnumString_KeyboardKey_KEY_CAPS_LOCK = "KEY_CAPS_LOCK";
static const char* luaEnumString_KeyboardKey_KEY_SHIFT = "KEY_SHIFT";
static const char* luaEnumString_KeyboardKey_KEY_CTRL = "KEY_CTRL";
static const char* luaEnumString_KeyboardKey_KEY_ALT = "KEY_ALT";
static const char* luaEnumString_KeyboardKey_KEY_MENU = "KEY_MENU";
static const char* luaEnumString_KeyboardKey_KEY_HYPER = "KEY_HYPER";
static const char* luaEnumString_KeyboardKey_KEY_INSERT = "KEY_INSERT";
static const char* luaEnumString_KeyboardKey_KEY_HOME = "KEY_HOME";
static const char* luaEnumString_KeyboardKey_KEY_PG_UP = "KEY_PG_UP";
static const char* luaEnumString_KeyboardKey_KEY_DELETE = "KEY_DELETE";
static const char* luaEnumString_KeyboardKey_KEY_END = "KEY_END";
static const char* luaEnumString_KeyboardKey_KEY_PG_DOWN = "KEY_PG_DOWN";
static const char* luaEnumString_KeyboardKey_KEY_LEFT_ARROW = "KEY_LEFT_ARROW";
static const char* luaEnumString_KeyboardKey_KEY_RIGHT_ARROW = "KEY_RIGHT_ARROW";
static const char* luaEnumString_KeyboardKey_KEY_UP_ARROW = "KEY_UP_ARROW";
static const char* luaEnumString_KeyboardKey_KEY_DOWN_ARROW = "KEY_DOWN_ARROW";
static const char* luaEnumString_KeyboardKey_KEY_NUM_LOCK = "KEY_NUM_LOCK";
static const char* luaEnumString_KeyboardKey_KEY_KP_PLUS = "KEY_KP_PLUS";
static const char* luaEnumString_KeyboardKey_KEY_KP_MINUS = "KEY_KP_MINUS";
static const char* luaEnumString_KeyboardKey_KEY_KP_MULTIPLY = "KEY_KP_MULTIPLY";
static const char* luaEnumString_KeyboardKey_KEY_KP_DIVIDE = "KEY_KP_DIVIDE";
static const char* luaEnumString_KeyboardKey_KEY_KP_ENTER = "KEY_KP_ENTER";
static const char* luaEnumString_KeyboardKey_KEY_KP_HOME = "KEY_KP_HOME";
static const char* luaEnumString_KeyboardKey_KEY_KP_UP = "KEY_KP_UP";
static const char* luaEnumString_KeyboardKey_KEY_KP_PG_UP = "KEY_KP_PG_UP";
static const char* luaEnumString_KeyboardKey_KEY_KP_LEFT = "KEY_KP_LEFT";
static const char* luaEnumString_KeyboardKey_KEY_KP_FIVE = "KEY_KP_FIVE";
static const char* luaEnumString_KeyboardKey_KEY_KP_RIGHT = "KEY_KP_RIGHT";
static const char* luaEnumString_KeyboardKey_KEY_KP_END = "KEY_KP_END";
static const char* luaEnumString_KeyboardKey_KEY_KP_DOWN = "KEY_KP_DOWN";
static const char* luaEnumString_KeyboardKey_KEY_KP_PG_DOWN = "KEY_KP_PG_DOWN";
static const char* luaEnumString_KeyboardKey_KEY_KP_INSERT = "KEY_KP_INSERT";
static const char* luaEnumString_KeyboardKey_KEY_KP_DELETE = "KEY_KP_DELETE";
static const char* luaEnumString_KeyboardKey_KEY_F1 = "KEY_F1";
static const char* luaEnumString_KeyboardKey_KEY_F2 = "KEY_F2";
static const char* luaEnumString_KeyboardKey_KEY_F3 = "KEY_F3";
static const char* luaEnumString_KeyboardKey_KEY_F4 = "KEY_F4";
static const char* luaEnumString_KeyboardKey_KEY_F5 = "KEY_F5";
static const char* luaEnumString_KeyboardKey_KEY_F6 = "KEY_F6";
static const char* luaEnumString_KeyboardKey_KEY_F7 = "KEY_F7";
static const char* luaEnumString_KeyboardKey_KEY_F8 = "KEY_F8";
static const char* luaEnumString_KeyboardKey_KEY_F9 = "KEY_F9";
static const char* luaEnumString_KeyboardKey_KEY_F10 = "KEY_F10";
static const char* luaEnumString_KeyboardKey_KEY_F11 = "KEY_F11";
static const char* luaEnumString_KeyboardKey_KEY_F12 = "KEY_F12";
static const char* luaEnumString_KeyboardKey_KEY_SPACE = "KEY_SPACE";
static const char* luaEnumString_KeyboardKey_KEY_EXCLAM = "KEY_EXCLAM";
static const char* luaEnumString_KeyboardKey_KEY_QUOTE = "KEY_QUOTE";
static const char* luaEnumString_KeyboardKey_KEY_NUMBER = "KEY_NUMBER";
static const char* luaEnumString_KeyboardKey_KEY_DOLLAR = "KEY_DOLLAR";
static const char* luaEnumString_KeyboardKey_KEY_PERCENT = "KEY_PERCENT";
static const char* luaEnumString_KeyboardKey_KEY_CIRCUMFLEX = "KEY_CIRCUMFLEX";
static const char* luaEnumString_KeyboardKey_KEY_AMPERSAND = "KEY_AMPERSAND";
static const char* luaEnumString_KeyboardKey_KEY_APOSTROPHE = "KEY_APOSTROPHE";
static const char* luaEnumString_KeyboardKey_KEY_LEFT_PARENTHESIS = "KEY_LEFT_PARENTHESIS";
static const char* luaEnumString_KeyboardKey_KEY_RIGHT_PARENTHESIS = "KEY_RIGHT_PARENTHESIS";
static const char* luaEnumString_KeyboardKey_KEY_ASTERISK = "KEY_ASTERISK";
static const char* luaEnumString_KeyboardKey_KEY_PLUS = "KEY_PLUS";
static const char* luaEnumString_KeyboardKey_KEY_COMMA = "KEY_COMMA";
static const char* luaEnumString_KeyboardKey_KEY_MINUS = "KEY_MINUS";
static const char* luaEnumString_KeyboardKey_KEY_PERIOD = "KEY_PERIOD";
static const char* luaEnumString_KeyboardKey_KEY_SLASH = "KEY_SLASH";
static const char* luaEnumString_KeyboardKey_KEY_ZERO = "KEY_ZERO";
static const char* luaEnumString_KeyboardKey_KEY_ONE = "KEY_ONE";
static const char* luaEnumString_KeyboardKey_KEY_TWO = "KEY_TWO";
static const char* luaEnumString_KeyboardKey_KEY_THREE = "KEY_THREE";
static const char* luaEnumString_KeyboardKey_KEY_FOUR = "KEY_FOUR";
static const char* luaEnumString_KeyboardKey_KEY_FIVE = "KEY_FIVE";
static const char* luaEnumString_KeyboardKey_KEY_SIX = "KEY_SIX";
static const char* luaEnumString_KeyboardKey_KEY_SEVEN = "KEY_SEVEN";
static const char* luaEnumString_KeyboardKey_KEY_EIGHT = "KEY_EIGHT";
static const char* luaEnumString_KeyboardKey_KEY_NINE = "KEY_NINE";
static const char* luaEnumString_KeyboardKey_KEY_COLON = "KEY_COLON";
static const char* luaEnumString_KeyboardKey_KEY_SEMICOLON = "KEY_SEMICOLON";
static const char* luaEnumString_KeyboardKey_KEY_LESS_THAN = "KEY_LESS_THAN";
static const char* luaEnumString_KeyboardKey_KEY_EQUAL = "KEY_EQUAL";
static const char* luaEnumString_KeyboardKey_KEY_GREATER_THAN = "KEY_GREATER_THAN";
static const char* luaEnumString_KeyboardKey_KEY_QUESTION = "KEY_QUESTION";
static const char* luaEnumString_KeyboardKey_KEY_AT = "KEY_AT";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_A = "KEY_CAPITAL_A";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_B = "KEY_CAPITAL_B";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_C = "KEY_CAPITAL_C";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_D = "KEY_CAPITAL_D";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_E = "KEY_CAPITAL_E";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_F = "KEY_CAPITAL_F";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_G = "KEY_CAPITAL_G";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_H = "KEY_CAPITAL_H";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_I = "KEY_CAPITAL_I";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_J = "KEY_CAPITAL_J";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_K = "KEY_CAPITAL_K";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_L = "KEY_CAPITAL_L";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_M = "KEY_CAPITAL_M";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_N = "KEY_CAPITAL_N";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_O = "KEY_CAPITAL_O";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_P = "KEY_CAPITAL_P";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_Q = "KEY_CAPITAL_Q";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_R = "KEY_CAPITAL_R";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_S = "KEY_CAPITAL_S";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_T = "KEY_CAPITAL_T";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_U = "KEY_CAPITAL_U";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_V = "KEY_CAPITAL_V";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_W = "KEY_CAPITAL_W";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_X = "KEY_CAPITAL_X";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_Y = "KEY_CAPITAL_Y";
static const char* luaEnumString_KeyboardKey_KEY_CAPITAL_Z = "KEY_CAPITAL_Z";
static const char* luaEnumString_KeyboardKey_KEY_LEFT_BRACKET = "KEY_LEFT_BRACKET";
static const char* luaEnumString_KeyboardKey_KEY_BACK_SLASH = "KEY_BACK_SLASH";
static const char* luaEnumString_KeyboardKey_KEY_RIGHT_BRACKET = "KEY_RIGHT_BRACKET";
static const char* luaEnumString_KeyboardKey_KEY_UNDERSCORE = "KEY_UNDERSCORE";
static const char* luaEnumString_KeyboardKey_KEY_GRAVE = "KEY_GRAVE";
static const char* luaEnumString_KeyboardKey_KEY_A = "KEY_A";
static const char* luaEnumString_KeyboardKey_KEY_B = "KEY_B";
static const char* luaEnumString_KeyboardKey_KEY_C = "KEY_C";
static const char* luaEnumString_KeyboardKey_KEY_D = "KEY_D";
static const char* luaEnumString_KeyboardKey_KEY_E = "KEY_E";
static const char* luaEnumString_KeyboardKey_KEY_F = "KEY_F";
static const char* luaEnumString_KeyboardKey_KEY_G = "KEY_G";
static const char* luaEnumString_KeyboardKey_KEY_H = "KEY_H";
static const char* luaEnumString_KeyboardKey_KEY_I = "KEY_I";
static const char* luaEnumString_KeyboardKey_KEY_J = "KEY_J";
static const char* luaEnumString_KeyboardKey_KEY_K = "KEY_K";
static const char* luaEnumString_KeyboardKey_KEY_L = "KEY_L";
static const char* luaEnumString_KeyboardKey_KEY_M = "KEY_M";
static const char* luaEnumString_KeyboardKey_KEY_N = "KEY_N";
static const char* luaEnumString_KeyboardKey_KEY_O = "KEY_O";
static const char* luaEnumString_KeyboardKey_KEY_P = "KEY_P";
static const char* luaEnumString_KeyboardKey_KEY_Q = "KEY_Q";
static const char* luaEnumString_KeyboardKey_KEY_R = "KEY_R";
static const char* luaEnumString_KeyboardKey_KEY_S = "KEY_S";
static const char* luaEnumString_KeyboardKey_KEY_T = "KEY_T";
static const char* luaEnumString_KeyboardKey_KEY_U = "KEY_U";
static const char* luaEnumString_KeyboardKey_KEY_V = "KEY_V";
static const char* luaEnumString_KeyboardKey_KEY_W = "KEY_W";
static const char* luaEnumString_KeyboardKey_KEY_X = "KEY_X";
static const char* luaEnumString_KeyboardKey_KEY_Y = "KEY_Y";
static const char* luaEnumString_KeyboardKey_KEY_Z = "KEY_Z";
static const char* luaEnumString_KeyboardKey_KEY_LEFT_BRACE = "KEY_LEFT_BRACE";
static const char* luaEnumString_KeyboardKey_KEY_BAR = "KEY_BAR";
static const char* luaEnumString_KeyboardKey_KEY_RIGHT_BRACE = "KEY_RIGHT_BRACE";
static const char* luaEnumString_KeyboardKey_KEY_TILDE = "KEY_TILDE";
static const char* luaEnumString_KeyboardKey_KEY_EURO = "KEY_EURO";
static const char* luaEnumString_KeyboardKey_KEY_POUND = "KEY_POUND";
static const char* luaEnumString_KeyboardKey_KEY_YEN = "KEY_YEN";
static const char* luaEnumString_KeyboardKey_KEY_MIDDLE_DOT = "KEY_MIDDLE_DOT";
static const char* luaEnumString_KeyboardKey_KEY_SEARCH = "KEY_SEARCH";

Keyboard::Key lua_enumFromString_KeyboardKey(const char* s)
{
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_NONE) == 0)
        return Keyboard::KEY_NONE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PAUSE) == 0)
        return Keyboard::KEY_PAUSE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SCROLL_LOCK) == 0)
        return Keyboard::KEY_SCROLL_LOCK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PRINT) == 0)
        return Keyboard::KEY_PRINT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SYSREQ) == 0)
        return Keyboard::KEY_SYSREQ;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BREAK) == 0)
        return Keyboard::KEY_BREAK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ESCAPE) == 0)
        return Keyboard::KEY_ESCAPE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BACKSPACE) == 0)
        return Keyboard::KEY_BACKSPACE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_TAB) == 0)
        return Keyboard::KEY_TAB;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BACK_TAB) == 0)
        return Keyboard::KEY_BACK_TAB;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RETURN) == 0)
        return Keyboard::KEY_RETURN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPS_LOCK) == 0)
        return Keyboard::KEY_CAPS_LOCK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SHIFT) == 0)
        return Keyboard::KEY_SHIFT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CTRL) == 0)
        return Keyboard::KEY_CTRL;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ALT) == 0)
        return Keyboard::KEY_ALT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_MENU) == 0)
        return Keyboard::KEY_MENU;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_HYPER) == 0)
        return Keyboard::KEY_HYPER;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_INSERT) == 0)
        return Keyboard::KEY_INSERT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_HOME) == 0)
        return Keyboard::KEY_HOME;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PG_UP) == 0)
        return Keyboard::KEY_PG_UP;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_DELETE) == 0)
        return Keyboard::KEY_DELETE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_END) == 0)
        return Keyboard::KEY_END;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PG_DOWN) == 0)
        return Keyboard::KEY_PG_DOWN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LEFT_ARROW) == 0)
        return Keyboard::KEY_LEFT_ARROW;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RIGHT_ARROW) == 0)
        return Keyboard::KEY_RIGHT_ARROW;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_UP_ARROW) == 0)
        return Keyboard::KEY_UP_ARROW;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_DOWN_ARROW) == 0)
        return Keyboard::KEY_DOWN_ARROW;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_NUM_LOCK) == 0)
        return Keyboard::KEY_NUM_LOCK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_PLUS) == 0)
        return Keyboard::KEY_KP_PLUS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_MINUS) == 0)
        return Keyboard::KEY_KP_MINUS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_MULTIPLY) == 0)
        return Keyboard::KEY_KP_MULTIPLY;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_DIVIDE) == 0)
        return Keyboard::KEY_KP_DIVIDE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_ENTER) == 0)
        return Keyboard::KEY_KP_ENTER;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_HOME) == 0)
        return Keyboard::KEY_KP_HOME;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_UP) == 0)
        return Keyboard::KEY_KP_UP;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_PG_UP) == 0)
        return Keyboard::KEY_KP_PG_UP;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_LEFT) == 0)
        return Keyboard::KEY_KP_LEFT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_FIVE) == 0)
        return Keyboard::KEY_KP_FIVE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_RIGHT) == 0)
        return Keyboard::KEY_KP_RIGHT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_END) == 0)
        return Keyboard::KEY_KP_END;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_DOWN) == 0)
        return Keyboard::KEY_KP_DOWN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_PG_DOWN) == 0)
        return Keyboard::KEY_KP_PG_DOWN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_INSERT) == 0)
        return Keyboard::KEY_KP_INSERT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_KP_DELETE) == 0)
        return Keyboard::KEY_KP_DELETE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F1) == 0)
        return Keyboard::KEY_F1;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F2) == 0)
        return Keyboard::KEY_F2;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F3) == 0)
        return Keyboard::KEY_F3;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F4) == 0)
        return Keyboard::KEY_F4;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F5) == 0)
        return Keyboard::KEY_F5;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F6) == 0)
        return Keyboard::KEY_F6;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F7) == 0)
        return Keyboard::KEY_F7;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F8) == 0)
        return Keyboard::KEY_F8;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F9) == 0)
        return Keyboard::KEY_F9;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F10) == 0)
        return Keyboard::KEY_F10;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F11) == 0)
        return Keyboard::KEY_F11;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F12) == 0)
        return Keyboard::KEY_F12;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SPACE) == 0)
        return Keyboard::KEY_SPACE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_EXCLAM) == 0)
        return Keyboard::KEY_EXCLAM;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_QUOTE) == 0)
        return Keyboard::KEY_QUOTE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_NUMBER) == 0)
        return Keyboard::KEY_NUMBER;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_DOLLAR) == 0)
        return Keyboard::KEY_DOLLAR;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PERCENT) == 0)
        return Keyboard::KEY_PERCENT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CIRCUMFLEX) == 0)
        return Keyboard::KEY_CIRCUMFLEX;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_AMPERSAND) == 0)
        return Keyboard::KEY_AMPERSAND;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_APOSTROPHE) == 0)
        return Keyboard::KEY_APOSTROPHE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LEFT_PARENTHESIS) == 0)
        return Keyboard::KEY_LEFT_PARENTHESIS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RIGHT_PARENTHESIS) == 0)
        return Keyboard::KEY_RIGHT_PARENTHESIS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ASTERISK) == 0)
        return Keyboard::KEY_ASTERISK;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PLUS) == 0)
        return Keyboard::KEY_PLUS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_COMMA) == 0)
        return Keyboard::KEY_COMMA;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_MINUS) == 0)
        return Keyboard::KEY_MINUS;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_PERIOD) == 0)
        return Keyboard::KEY_PERIOD;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SLASH) == 0)
        return Keyboard::KEY_SLASH;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ZERO) == 0)
        return Keyboard::KEY_ZERO;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_ONE) == 0)
        return Keyboard::KEY_ONE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_TWO) == 0)
        return Keyboard::KEY_TWO;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_THREE) == 0)
        return Keyboard::KEY_THREE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_FOUR) == 0)
        return Keyboard::KEY_FOUR;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_FIVE) == 0)
        return Keyboard::KEY_FIVE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SIX) == 0)
        return Keyboard::KEY_SIX;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SEVEN) == 0)
        return Keyboard::KEY_SEVEN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_EIGHT) == 0)
        return Keyboard::KEY_EIGHT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_NINE) == 0)
        return Keyboard::KEY_NINE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_COLON) == 0)
        return Keyboard::KEY_COLON;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SEMICOLON) == 0)
        return Keyboard::KEY_SEMICOLON;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LESS_THAN) == 0)
        return Keyboard::KEY_LESS_THAN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_EQUAL) == 0)
        return Keyboard::KEY_EQUAL;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_GREATER_THAN) == 0)
        return Keyboard::KEY_GREATER_THAN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_QUESTION) == 0)
        return Keyboard::KEY_QUESTION;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_AT) == 0)
        return Keyboard::KEY_AT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_A) == 0)
        return Keyboard::KEY_CAPITAL_A;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_B) == 0)
        return Keyboard::KEY_CAPITAL_B;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_C) == 0)
        return Keyboard::KEY_CAPITAL_C;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_D) == 0)
        return Keyboard::KEY_CAPITAL_D;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_E) == 0)
        return Keyboard::KEY_CAPITAL_E;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_F) == 0)
        return Keyboard::KEY_CAPITAL_F;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_G) == 0)
        return Keyboard::KEY_CAPITAL_G;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_H) == 0)
        return Keyboard::KEY_CAPITAL_H;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_I) == 0)
        return Keyboard::KEY_CAPITAL_I;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_J) == 0)
        return Keyboard::KEY_CAPITAL_J;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_K) == 0)
        return Keyboard::KEY_CAPITAL_K;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_L) == 0)
        return Keyboard::KEY_CAPITAL_L;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_M) == 0)
        return Keyboard::KEY_CAPITAL_M;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_N) == 0)
        return Keyboard::KEY_CAPITAL_N;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_O) == 0)
        return Keyboard::KEY_CAPITAL_O;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_P) == 0)
        return Keyboard::KEY_CAPITAL_P;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_Q) == 0)
        return Keyboard::KEY_CAPITAL_Q;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_R) == 0)
        return Keyboard::KEY_CAPITAL_R;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_S) == 0)
        return Keyboard::KEY_CAPITAL_S;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_T) == 0)
        return Keyboard::KEY_CAPITAL_T;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_U) == 0)
        return Keyboard::KEY_CAPITAL_U;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_V) == 0)
        return Keyboard::KEY_CAPITAL_V;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_W) == 0)
        return Keyboard::KEY_CAPITAL_W;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_X) == 0)
        return Keyboard::KEY_CAPITAL_X;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_Y) == 0)
        return Keyboard::KEY_CAPITAL_Y;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_CAPITAL_Z) == 0)
        return Keyboard::KEY_CAPITAL_Z;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LEFT_BRACKET) == 0)
        return Keyboard::KEY_LEFT_BRACKET;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BACK_SLASH) == 0)
        return Keyboard::KEY_BACK_SLASH;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RIGHT_BRACKET) == 0)
        return Keyboard::KEY_RIGHT_BRACKET;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_UNDERSCORE) == 0)
        return Keyboard::KEY_UNDERSCORE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_GRAVE) == 0)
        return Keyboard::KEY_GRAVE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_A) == 0)
        return Keyboard::KEY_A;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_B) == 0)
        return Keyboard::KEY_B;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_C) == 0)
        return Keyboard::KEY_C;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_D) == 0)
        return Keyboard::KEY_D;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_E) == 0)
        return Keyboard::KEY_E;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_F) == 0)
        return Keyboard::KEY_F;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_G) == 0)
        return Keyboard::KEY_G;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_H) == 0)
        return Keyboard::KEY_H;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_I) == 0)
        return Keyboard::KEY_I;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_J) == 0)
        return Keyboard::KEY_J;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_K) == 0)
        return Keyboard::KEY_K;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_L) == 0)
        return Keyboard::KEY_L;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_M) == 0)
        return Keyboard::KEY_M;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_N) == 0)
        return Keyboard::KEY_N;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_O) == 0)
        return Keyboard::KEY_O;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_P) == 0)
        return Keyboard::KEY_P;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_Q) == 0)
        return Keyboard::KEY_Q;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_R) == 0)
        return Keyboard::KEY_R;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_S) == 0)
        return Keyboard::KEY_S;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_T) == 0)
        return Keyboard::KEY_T;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_U) == 0)
        return Keyboard::KEY_U;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_V) == 0)
        return Keyboard::KEY_V;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_W) == 0)
        return Keyboard::KEY_W;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_X) == 0)
        return Keyboard::KEY_X;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_Y) == 0)
        return Keyboard::KEY_Y;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_Z) == 0)
        return Keyboard::KEY_Z;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_LEFT_BRACE) == 0)
        return Keyboard::KEY_LEFT_BRACE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_BAR) == 0)
        return Keyboard::KEY_BAR;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_RIGHT_BRACE) == 0)
        return Keyboard::KEY_RIGHT_BRACE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_TILDE) == 0)
        return Keyboard::KEY_TILDE;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_EURO) == 0)
        return Keyboard::KEY_EURO;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_POUND) == 0)
        return Keyboard::KEY_POUND;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_YEN) == 0)
        return Keyboard::KEY_YEN;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_MIDDLE_DOT) == 0)
        return Keyboard::KEY_MIDDLE_DOT;
    if (strcmp(s, luaEnumString_KeyboardKey_KEY_SEARCH) == 0)
        return Keyboard::KEY_SEARCH;
    return Keyboard::KEY_NONE;
}

const char* lua_stringFromEnum_KeyboardKey(Keyboard::Key e)
{
    if (e == Keyboard::KEY_NONE)
        return luaEnumString_KeyboardKey_KEY_NONE;
    if (e == Keyboard::KEY_PAUSE)
        return luaEnumString_KeyboardKey_KEY_PAUSE;
    if (e == Keyboard::KEY_SCROLL_LOCK)
        return luaEnumString_KeyboardKey_KEY_SCROLL_LOCK;
    if (e == Keyboard::KEY_PRINT)
        return luaEnumString_KeyboardKey_KEY_PRINT;
    if (e == Keyboard::KEY_SYSREQ)
        return luaEnumString_KeyboardKey_KEY_SYSREQ;
    if (e == Keyboard::KEY_BREAK)
        return luaEnumString_KeyboardKey_KEY_BREAK;
    if (e == Keyboard::KEY_ESCAPE)
        return luaEnumString_KeyboardKey_KEY_ESCAPE;
    if (e == Keyboard::KEY_BACKSPACE)
        return luaEnumString_KeyboardKey_KEY_BACKSPACE;
    if (e == Keyboard::KEY_TAB)
        return luaEnumString_KeyboardKey_KEY_TAB;
    if (e == Keyboard::KEY_BACK_TAB)
        return luaEnumString_KeyboardKey_KEY_BACK_TAB;
    if (e == Keyboard::KEY_RETURN)
        return luaEnumString_KeyboardKey_KEY_RETURN;
    if (e == Keyboard::KEY_CAPS_LOCK)
        return luaEnumString_KeyboardKey_KEY_CAPS_LOCK;
    if (e == Keyboard::KEY_SHIFT)
        return luaEnumString_KeyboardKey_KEY_SHIFT;
    if (e == Keyboard::KEY_CTRL)
        return luaEnumString_KeyboardKey_KEY_CTRL;
    if (e == Keyboard::KEY_ALT)
        return luaEnumString_KeyboardKey_KEY_ALT;
    if (e == Keyboard::KEY_MENU)
        return luaEnumString_KeyboardKey_KEY_MENU;
    if (e == Keyboard::KEY_HYPER)
        return luaEnumString_KeyboardKey_KEY_HYPER;
    if (e == Keyboard::KEY_INSERT)
        return luaEnumString_KeyboardKey_KEY_INSERT;
    if (e == Keyboard::KEY_HOME)
        return luaEnumString_KeyboardKey_KEY_HOME;
    if (e == Keyboard::KEY_PG_UP)
        return luaEnumString_KeyboardKey_KEY_PG_UP;
    if (e == Keyboard::KEY_DELETE)
        return luaEnumString_KeyboardKey_KEY_DELETE;
    if (e == Keyboard::KEY_END)
        return luaEnumString_KeyboardKey_KEY_END;
    if (e == Keyboard::KEY_PG_DOWN)
        return luaEnumString_KeyboardKey_KEY_PG_DOWN;
    if (e == Keyboard::KEY_LEFT_ARROW)
        return luaEnumString_KeyboardKey_KEY_LEFT_ARROW;
    if (e == Keyboard::KEY_RIGHT_ARROW)
        return luaEnumString_KeyboardKey_KEY_RIGHT_ARROW;
    if (e == Keyboard::KEY_UP_ARROW)
        return luaEnumString_KeyboardKey_KEY_UP_ARROW;
    if (e == Keyboard::KEY_DOWN_ARROW)
        return luaEnumString_KeyboardKey_KEY_DOWN_ARROW;
    if (e == Keyboard::KEY_NUM_LOCK)
        return luaEnumString_KeyboardKey_KEY_NUM_LOCK;
    if (e == Keyboard::KEY_KP_PLUS)
        return luaEnumString_KeyboardKey_KEY_KP_PLUS;
    if (e == Keyboard::KEY_KP_MINUS)
        return luaEnumString_KeyboardKey_KEY_KP_MINUS;
    if (e == Keyboard::KEY_KP_MULTIPLY)
        return luaEnumString_KeyboardKey_KEY_KP_MULTIPLY;
    if (e == Keyboard::KEY_KP_DIVIDE)
        return luaEnumString_KeyboardKey_KEY_KP_DIVIDE;
    if (e == Keyboard::KEY_KP_ENTER)
        return luaEnumString_KeyboardKey_KEY_KP_ENTER;
    if (e == Keyboard::KEY_KP_HOME)
        return luaEnumString_KeyboardKey_KEY_KP_HOME;
    if (e == Keyboard::KEY_KP_UP)
        return luaEnumString_KeyboardKey_KEY_KP_UP;
    if (e == Keyboard::KEY_KP_PG_UP)
        return luaEnumString_KeyboardKey_KEY_KP_PG_UP;
    if (e == Keyboard::KEY_KP_LEFT)
        return luaEnumString_KeyboardKey_KEY_KP_LEFT;
    if (e == Keyboard::KEY_KP_FIVE)
        return luaEnumString_KeyboardKey_KEY_KP_FIVE;
    if (e == Keyboard::KEY_KP_RIGHT)
        return luaEnumString_KeyboardKey_KEY_KP_RIGHT;
    if (e == Keyboard::KEY_KP_END)
        return luaEnumString_KeyboardKey_KEY_KP_END;
    if (e == Keyboard::KEY_KP_DOWN)
        return luaEnumString_KeyboardKey_KEY_KP_DOWN;
    if (e == Keyboard::KEY_KP_PG_DOWN)
        return luaEnumString_KeyboardKey_KEY_KP_PG_DOWN;
    if (e == Keyboard::KEY_KP_INSERT)
        return luaEnumString_KeyboardKey_KEY_KP_INSERT;
    if (e == Keyboard::KEY_KP_DELETE)
        return luaEnumString_KeyboardKey_KEY_KP_DELETE;
    if (e == Keyboard::KEY_F1)
        return luaEnumString_KeyboardKey_KEY_F1;
    if (e == Keyboard::KEY_F2)
        return luaEnumString_KeyboardKey_KEY_F2;
    if (e == Keyboard::KEY_F3)
        return luaEnumString_KeyboardKey_KEY_F3;
    if (e == Keyboard::KEY_F4)
        return luaEnumString_KeyboardKey_KEY_F4;
    if (e == Keyboard::KEY_F5)
        return luaEnumString_KeyboardKey_KEY_F5;
    if (e == Keyboard::KEY_F6)
        return luaEnumString_KeyboardKey_KEY_F6;
    if (e == Keyboard::KEY_F7)
        return luaEnumString_KeyboardKey_KEY_F7;
    if (e == Keyboard::KEY_F8)
        return luaEnumString_KeyboardKey_KEY_F8;
    if (e == Keyboard::KEY_F9)
        return luaEnumString_KeyboardKey_KEY_F9;
    if (e == Keyboard::KEY_F10)
        return luaEnumString_KeyboardKey_KEY_F10;
    if (e == Keyboard::KEY_F11)
        return luaEnumString_KeyboardKey_KEY_F11;
    if (e == Keyboard::KEY_F12)
        return luaEnumString_KeyboardKey_KEY_F12;
    if (e == Keyboard::KEY_SPACE)
        return luaEnumString_KeyboardKey_KEY_SPACE;
    if (e == Keyboard::KEY_EXCLAM)
        return luaEnumString_KeyboardKey_KEY_EXCLAM;
    if (e == Keyboard::KEY_QUOTE)
        return luaEnumString_KeyboardKey_KEY_QUOTE;
    if (e == Keyboard::KEY_NUMBER)
        return luaEnumString_KeyboardKey_KEY_NUMBER;
    if (e == Keyboard::KEY_DOLLAR)
        return luaEnumString_KeyboardKey_KEY_DOLLAR;
    if (e == Keyboard::KEY_PERCENT)
        return luaEnumString_KeyboardKey_KEY_PERCENT;
    if (e == Keyboard::KEY_CIRCUMFLEX)
        return luaEnumString_KeyboardKey_KEY_CIRCUMFLEX;
    if (e == Keyboard::KEY_AMPERSAND)
        return luaEnumString_KeyboardKey_KEY_AMPERSAND;
    if (e == Keyboard::KEY_APOSTROPHE)
        return luaEnumString_KeyboardKey_KEY_APOSTROPHE;
    if (e == Keyboard::KEY_LEFT_PARENTHESIS)
        return luaEnumString_KeyboardKey_KEY_LEFT_PARENTHESIS;
    if (e == Keyboard::KEY_RIGHT_PARENTHESIS)
        return luaEnumString_KeyboardKey_KEY_RIGHT_PARENTHESIS;
    if (e == Keyboard::KEY_ASTERISK)
        return luaEnumString_KeyboardKey_KEY_ASTERISK;
    if (e == Keyboard::KEY_PLUS)
        return luaEnumString_KeyboardKey_KEY_PLUS;
    if (e == Keyboard::KEY_COMMA)
        return luaEnumString_KeyboardKey_KEY_COMMA;
    if (e == Keyboard::KEY_MINUS)
        return luaEnumString_KeyboardKey_KEY_MINUS;
    if (e == Keyboard::KEY_PERIOD)
        return luaEnumString_KeyboardKey_KEY_PERIOD;
    if (e == Keyboard::KEY_SLASH)
        return luaEnumString_KeyboardKey_KEY_SLASH;
    if (e == Keyboard::KEY_ZERO)
        return luaEnumString_KeyboardKey_KEY_ZERO;
    if (e == Keyboard::KEY_ONE)
        return luaEnumString_KeyboardKey_KEY_ONE;
    if (e == Keyboard::KEY_TWO)
        return luaEnumString_KeyboardKey_KEY_TWO;
    if (e == Keyboard::KEY_THREE)
        return luaEnumString_KeyboardKey_KEY_THREE;
    if (e == Keyboard::KEY_FOUR)
        return luaEnumString_KeyboardKey_KEY_FOUR;
    if (e == Keyboard::KEY_FIVE)
        return luaEnumString_KeyboardKey_KEY_FIVE;
    if (e == Keyboard::KEY_SIX)
        return luaEnumString_KeyboardKey_KEY_SIX;
    if (e == Keyboard::KEY_SEVEN)
        return luaEnumString_KeyboardKey_KEY_SEVEN;
    if (e == Keyboard::KEY_EIGHT)
        return luaEnumString_KeyboardKey_KEY_EIGHT;
    if (e == Keyboard::KEY_NINE)
        return luaEnumString_KeyboardKey_KEY_NINE;
    if (e == Keyboard::KEY_COLON)
        return luaEnumString_KeyboardKey_KEY_COLON;
    if (e == Keyboard::KEY_SEMICOLON)
        return luaEnumString_KeyboardKey_KEY_SEMICOLON;
    if (e == Keyboard::KEY_LESS_THAN)
        return luaEnumString_KeyboardKey_KEY_LESS_THAN;
    if (e == Keyboard::KEY_EQUAL)
        return luaEnumString_KeyboardKey_KEY_EQUAL;
    if (e == Keyboard::KEY_GREATER_THAN)
        return luaEnumString_KeyboardKey_KEY_GREATER_THAN;
    if (e == Keyboard::KEY_QUESTION)
        return luaEnumString_KeyboardKey_KEY_QUESTION;
    if (e == Keyboard::KEY_AT)
        return luaEnumString_KeyboardKey_KEY_AT;
    if (e == Keyboard::KEY_CAPITAL_A)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_A;
    if (e == Keyboard::KEY_CAPITAL_B)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_B;
    if (e == Keyboard::KEY_CAPITAL_C)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_C;
    if (e == Keyboard::KEY_CAPITAL_D)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_D;
    if (e == Keyboard::KEY_CAPITAL_E)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_E;
    if (e == Keyboard::KEY_CAPITAL_F)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_F;
    if (e == Keyboard::KEY_CAPITAL_G)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_G;
    if (e == Keyboard::KEY_CAPITAL_H)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_H;
    if (e == Keyboard::KEY_CAPITAL_I)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_I;
    if (e == Keyboard::KEY_CAPITAL_J)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_J;
    if (e == Keyboard::KEY_CAPITAL_K)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_K;
    if (e == Keyboard::KEY_CAPITAL_L)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_L;
    if (e == Keyboard::KEY_CAPITAL_M)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_M;
    if (e == Keyboard::KEY_CAPITAL_N)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_N;
    if (e == Keyboard::KEY_CAPITAL_O)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_O;
    if (e == Keyboard::KEY_CAPITAL_P)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_P;
    if (e == Keyboard::KEY_CAPITAL_Q)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_Q;
    if (e == Keyboard::KEY_CAPITAL_R)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_R;
    if (e == Keyboard::KEY_CAPITAL_S)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_S;
    if (e == Keyboard::KEY_CAPITAL_T)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_T;
    if (e == Keyboard::KEY_CAPITAL_U)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_U;
    if (e == Keyboard::KEY_CAPITAL_V)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_V;
    if (e == Keyboard::KEY_CAPITAL_W)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_W;
    if (e == Keyboard::KEY_CAPITAL_X)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_X;
    if (e == Keyboard::KEY_CAPITAL_Y)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_Y;
    if (e == Keyboard::KEY_CAPITAL_Z)
        return luaEnumString_KeyboardKey_KEY_CAPITAL_Z;
    if (e == Keyboard::KEY_LEFT_BRACKET)
        return luaEnumString_KeyboardKey_KEY_LEFT_BRACKET;
    if (e == Keyboard::KEY_BACK_SLASH)
        return luaEnumString_KeyboardKey_KEY_BACK_SLASH;
    if (e == Keyboard::KEY_RIGHT_BRACKET)
        return luaEnumString_KeyboardKey_KEY_RIGHT_BRACKET;
    if (e == Keyboard::KEY_UNDERSCORE)
        return luaEnumString_KeyboardKey_KEY_UNDERSCORE;
    if (e == Keyboard::KEY_GRAVE)
        return luaEnumString_KeyboardKey_KEY_GRAVE;
    if (e == Keyboard::KEY_A)
        return luaEnumString_KeyboardKey_KEY_A;
    if (e == Keyboard::KEY_B)
        return luaEnumString_KeyboardKey_KEY_B;
    if (e == Keyboard::KEY_C)
        return luaEnumString_KeyboardKey_KEY_C;
    if (e == Keyboard::KEY_D)
        return luaEnumString_KeyboardKey_KEY_D;
    if (e == Keyboard::KEY_E)
        return luaEnumString_KeyboardKey_KEY_E;
    if (e == Keyboard::KEY_F)
        return luaEnumString_KeyboardKey_KEY_F;
    if (e == Keyboard::KEY_G)
        return luaEnumString_KeyboardKey_KEY_G;
    if (e == Keyboard::KEY_H)
        return luaEnumString_KeyboardKey_KEY_H;
    if (e == Keyboard::KEY_I)
        return luaEnumString_KeyboardKey_KEY_I;
    if (e == Keyboard::KEY_J)
        return luaEnumString_KeyboardKey_KEY_J;
    if (e == Keyboard::KEY_K)
        return luaEnumString_KeyboardKey_KEY_K;
    if (e == Keyboard::KEY_L)
        return luaEnumString_KeyboardKey_KEY_L;
    if (e == Keyboard::KEY_M)
        return luaEnumString_KeyboardKey_KEY_M;
    if (e == Keyboard::KEY_N)
        return luaEnumString_KeyboardKey_KEY_N;
    if (e == Keyboard::KEY_O)
        return luaEnumString_KeyboardKey_KEY_O;
    if (e == Keyboard::KEY_P)
        return luaEnumString_KeyboardKey_KEY_P;
    if (e == Keyboard::KEY_Q)
        return luaEnumString_KeyboardKey_KEY_Q;
    if (e == Keyboard::KEY_R)
        return luaEnumString_KeyboardKey_KEY_R;
    if (e == Keyboard::KEY_S)
        return luaEnumString_KeyboardKey_KEY_S;
    if (e == Keyboard::KEY_T)
        return luaEnumString_KeyboardKey_KEY_T;
    if (e == Keyboard::KEY_U)
        return luaEnumString_KeyboardKey_KEY_U;
    if (e == Keyboard::KEY_V)
        return luaEnumString_KeyboardKey_KEY_V;
    if (e == Keyboard::KEY_W)
        return luaEnumString_KeyboardKey_KEY_W;
    if (e == Keyboard::KEY_X)
        return luaEnumString_KeyboardKey_KEY_X;
    if (e == Keyboard::KEY_Y)
        return luaEnumString_KeyboardKey_KEY_Y;
    if (e == Keyboard::KEY_Z)
        return luaEnumString_KeyboardKey_KEY_Z;
    if (e == Keyboard::KEY_LEFT_BRACE)
        return luaEnumString_KeyboardKey_KEY_LEFT_BRACE;
    if (e == Keyboard::KEY_BAR)
        return luaEnumString_KeyboardKey_KEY_BAR;
    if (e == Keyboard::KEY_RIGHT_BRACE)
        return luaEnumString_KeyboardKey_KEY_RIGHT_BRACE;
    if (e == Keyboard::KEY_TILDE)
        return luaEnumString_KeyboardKey_KEY_TILDE;
    if (e == Keyboard::KEY_EURO)
        return luaEnumString_KeyboardKey_KEY_EURO;
    if (e == Keyboard::KEY_POUND)
        return luaEnumString_KeyboardKey_KEY_POUND;
    if (e == Keyboard::KEY_YEN)
        return luaEnumString_KeyboardKey_KEY_YEN;
    if (e == Keyboard::KEY_MIDDLE_DOT)
        return luaEnumString_KeyboardKey_KEY_MIDDLE_DOT;
    if (e == Keyboard::KEY_SEARCH)
        return luaEnumString_KeyboardKey_KEY_SEARCH;
    return enumStringEmpty;
}

}

