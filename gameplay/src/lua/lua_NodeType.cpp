#include "Base.h"
#include "lua_NodeType.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_NodeType_NODE = "NODE";
static const char* luaEnumString_NodeType_JOINT = "JOINT";

Node::Type lua_enumFromString_NodeType(const char* s)
{
    if (strcmp(s, luaEnumString_NodeType_NODE) == 0)
        return Node::NODE;
    if (strcmp(s, luaEnumString_NodeType_JOINT) == 0)
        return Node::JOINT;
    return Node::NODE;
}

const char* lua_stringFromEnum_NodeType(Node::Type e)
{
    if (e == Node::NODE)
        return luaEnumString_NodeType_NODE;
    if (e == Node::JOINT)
        return luaEnumString_NodeType_JOINT;
    return enumStringEmpty;
}

}

