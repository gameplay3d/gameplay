#include "Base.h"
#include "lua_TextBoxInputMode.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_TextBoxInputMode_TEXT = "TEXT";
static const char* luaEnumString_TextBoxInputMode_PASSWORD = "PASSWORD";

TextBox::InputMode lua_enumFromString_TextBoxInputMode(const char* s)
{
    if (strcmp(s, luaEnumString_TextBoxInputMode_TEXT) == 0)
        return TextBox::TEXT;
    if (strcmp(s, luaEnumString_TextBoxInputMode_PASSWORD) == 0)
        return TextBox::PASSWORD;
    return TextBox::TEXT;
}

const char* lua_stringFromEnum_TextBoxInputMode(TextBox::InputMode e)
{
    if (e == TextBox::TEXT)
        return luaEnumString_TextBoxInputMode_TEXT;
    if (e == TextBox::PASSWORD)
        return luaEnumString_TextBoxInputMode_PASSWORD;
    return enumStringEmpty;
}

}

