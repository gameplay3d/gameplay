#include "Base.h"
#include "lua_GamepadButtonMapping.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_GamepadButtonMapping_BUTTON_A = "BUTTON_A";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_B = "BUTTON_B";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_C = "BUTTON_C";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_X = "BUTTON_X";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_Y = "BUTTON_Y";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_Z = "BUTTON_Z";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_MENU1 = "BUTTON_MENU1";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_MENU2 = "BUTTON_MENU2";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_MENU3 = "BUTTON_MENU3";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_MENU4 = "BUTTON_MENU4";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_L1 = "BUTTON_L1";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_L2 = "BUTTON_L2";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_L3 = "BUTTON_L3";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_R1 = "BUTTON_R1";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_R2 = "BUTTON_R2";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_R3 = "BUTTON_R3";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_UP = "BUTTON_UP";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_DOWN = "BUTTON_DOWN";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_LEFT = "BUTTON_LEFT";
static const char* luaEnumString_GamepadButtonMapping_BUTTON_RIGHT = "BUTTON_RIGHT";

Gamepad::ButtonMapping lua_enumFromString_GamepadButtonMapping(const char* s)
{
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_A) == 0)
        return Gamepad::BUTTON_A;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_B) == 0)
        return Gamepad::BUTTON_B;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_C) == 0)
        return Gamepad::BUTTON_C;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_X) == 0)
        return Gamepad::BUTTON_X;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_Y) == 0)
        return Gamepad::BUTTON_Y;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_Z) == 0)
        return Gamepad::BUTTON_Z;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_MENU1) == 0)
        return Gamepad::BUTTON_MENU1;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_MENU2) == 0)
        return Gamepad::BUTTON_MENU2;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_MENU3) == 0)
        return Gamepad::BUTTON_MENU3;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_MENU4) == 0)
        return Gamepad::BUTTON_MENU4;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_L1) == 0)
        return Gamepad::BUTTON_L1;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_L2) == 0)
        return Gamepad::BUTTON_L2;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_L3) == 0)
        return Gamepad::BUTTON_L3;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_R1) == 0)
        return Gamepad::BUTTON_R1;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_R2) == 0)
        return Gamepad::BUTTON_R2;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_R3) == 0)
        return Gamepad::BUTTON_R3;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_UP) == 0)
        return Gamepad::BUTTON_UP;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_DOWN) == 0)
        return Gamepad::BUTTON_DOWN;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_LEFT) == 0)
        return Gamepad::BUTTON_LEFT;
    if (strcmp(s, luaEnumString_GamepadButtonMapping_BUTTON_RIGHT) == 0)
        return Gamepad::BUTTON_RIGHT;
    return Gamepad::BUTTON_A;
}

const char* lua_stringFromEnum_GamepadButtonMapping(Gamepad::ButtonMapping e)
{
    if (e == Gamepad::BUTTON_A)
        return luaEnumString_GamepadButtonMapping_BUTTON_A;
    if (e == Gamepad::BUTTON_B)
        return luaEnumString_GamepadButtonMapping_BUTTON_B;
    if (e == Gamepad::BUTTON_C)
        return luaEnumString_GamepadButtonMapping_BUTTON_C;
    if (e == Gamepad::BUTTON_X)
        return luaEnumString_GamepadButtonMapping_BUTTON_X;
    if (e == Gamepad::BUTTON_Y)
        return luaEnumString_GamepadButtonMapping_BUTTON_Y;
    if (e == Gamepad::BUTTON_Z)
        return luaEnumString_GamepadButtonMapping_BUTTON_Z;
    if (e == Gamepad::BUTTON_MENU1)
        return luaEnumString_GamepadButtonMapping_BUTTON_MENU1;
    if (e == Gamepad::BUTTON_MENU2)
        return luaEnumString_GamepadButtonMapping_BUTTON_MENU2;
    if (e == Gamepad::BUTTON_MENU3)
        return luaEnumString_GamepadButtonMapping_BUTTON_MENU3;
    if (e == Gamepad::BUTTON_MENU4)
        return luaEnumString_GamepadButtonMapping_BUTTON_MENU4;
    if (e == Gamepad::BUTTON_L1)
        return luaEnumString_GamepadButtonMapping_BUTTON_L1;
    if (e == Gamepad::BUTTON_L2)
        return luaEnumString_GamepadButtonMapping_BUTTON_L2;
    if (e == Gamepad::BUTTON_L3)
        return luaEnumString_GamepadButtonMapping_BUTTON_L3;
    if (e == Gamepad::BUTTON_R1)
        return luaEnumString_GamepadButtonMapping_BUTTON_R1;
    if (e == Gamepad::BUTTON_R2)
        return luaEnumString_GamepadButtonMapping_BUTTON_R2;
    if (e == Gamepad::BUTTON_R3)
        return luaEnumString_GamepadButtonMapping_BUTTON_R3;
    if (e == Gamepad::BUTTON_UP)
        return luaEnumString_GamepadButtonMapping_BUTTON_UP;
    if (e == Gamepad::BUTTON_DOWN)
        return luaEnumString_GamepadButtonMapping_BUTTON_DOWN;
    if (e == Gamepad::BUTTON_LEFT)
        return luaEnumString_GamepadButtonMapping_BUTTON_LEFT;
    if (e == Gamepad::BUTTON_RIGHT)
        return luaEnumString_GamepadButtonMapping_BUTTON_RIGHT;
    return enumStringEmpty;
}

}

