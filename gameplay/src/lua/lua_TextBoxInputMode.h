#ifndef LUA_TEXTBOXINPUTMODE_H_
#define LUA_TEXTBOXINPUTMODE_H_

#include "TextBox.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for TextBox::InputMode.
TextBox::InputMode lua_enumFromString_TextBoxInputMode(const char* s);
const char* lua_stringFromEnum_TextBoxInputMode(TextBox::InputMode e);

}

#endif
