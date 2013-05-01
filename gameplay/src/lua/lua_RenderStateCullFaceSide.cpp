#include "Base.h"
#include "lua_RenderStateCullFaceSide.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_RenderStateCullFaceSide_CULL_FACE_SIDE_BACK = "CULL_FACE_SIDE_BACK";
static const char* luaEnumString_RenderStateCullFaceSide_CULL_FACE_SIDE_FRONT = "CULL_FACE_SIDE_FRONT";
static const char* luaEnumString_RenderStateCullFaceSide_CULL_FACE_SIDE_FRONT_AND_BACK = "CULL_FACE_SIDE_FRONT_AND_BACK";

RenderState::CullFaceSide lua_enumFromString_RenderStateCullFaceSide(const char* s)
{
    if (strcmp(s, luaEnumString_RenderStateCullFaceSide_CULL_FACE_SIDE_BACK) == 0)
        return RenderState::CULL_FACE_SIDE_BACK;
    if (strcmp(s, luaEnumString_RenderStateCullFaceSide_CULL_FACE_SIDE_FRONT) == 0)
        return RenderState::CULL_FACE_SIDE_FRONT;
    if (strcmp(s, luaEnumString_RenderStateCullFaceSide_CULL_FACE_SIDE_FRONT_AND_BACK) == 0)
        return RenderState::CULL_FACE_SIDE_FRONT_AND_BACK;
    return RenderState::CULL_FACE_SIDE_BACK;
}

const char* lua_stringFromEnum_RenderStateCullFaceSide(RenderState::CullFaceSide e)
{
    if (e == RenderState::CULL_FACE_SIDE_BACK)
        return luaEnumString_RenderStateCullFaceSide_CULL_FACE_SIDE_BACK;
    if (e == RenderState::CULL_FACE_SIDE_FRONT)
        return luaEnumString_RenderStateCullFaceSide_CULL_FACE_SIDE_FRONT;
    if (e == RenderState::CULL_FACE_SIDE_FRONT_AND_BACK)
        return luaEnumString_RenderStateCullFaceSide_CULL_FACE_SIDE_FRONT_AND_BACK;
    return enumStringEmpty;
}

}

