#ifndef LUA_AIMESSAGEPARAMETERTYPE_H_
#define LUA_AIMESSAGEPARAMETERTYPE_H_

#include "AIMessage.h"

namespace gameplay
{

// Lua bindings for enum conversion functions for AIMessage::ParameterType.
AIMessage::ParameterType lua_enumFromString_AIMessageParameterType(const char* s);
const char* lua_stringFromEnum_AIMessageParameterType(AIMessage::ParameterType e);

}

#endif
