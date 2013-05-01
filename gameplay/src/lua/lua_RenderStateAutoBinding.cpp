#include "Base.h"
#include "lua_RenderStateAutoBinding.h"

namespace gameplay
{

static const char* enumStringEmpty = "";

static const char* luaEnumString_RenderStateAutoBinding_NONE = "NONE";
static const char* luaEnumString_RenderStateAutoBinding_WORLD_MATRIX = "WORLD_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_VIEW_MATRIX = "VIEW_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_PROJECTION_MATRIX = "PROJECTION_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_WORLD_VIEW_MATRIX = "WORLD_VIEW_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_VIEW_PROJECTION_MATRIX = "VIEW_PROJECTION_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_WORLD_VIEW_PROJECTION_MATRIX = "WORLD_VIEW_PROJECTION_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_MATRIX = "INVERSE_TRANSPOSE_WORLD_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX = "INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX";
static const char* luaEnumString_RenderStateAutoBinding_CAMERA_WORLD_POSITION = "CAMERA_WORLD_POSITION";
static const char* luaEnumString_RenderStateAutoBinding_CAMERA_VIEW_POSITION = "CAMERA_VIEW_POSITION";
static const char* luaEnumString_RenderStateAutoBinding_MATRIX_PALETTE = "MATRIX_PALETTE";
static const char* luaEnumString_RenderStateAutoBinding_SCENE_AMBIENT_COLOR = "SCENE_AMBIENT_COLOR";
static const char* luaEnumString_RenderStateAutoBinding_SCENE_LIGHT_COLOR = "SCENE_LIGHT_COLOR";
static const char* luaEnumString_RenderStateAutoBinding_SCENE_LIGHT_DIRECTION = "SCENE_LIGHT_DIRECTION";

RenderState::AutoBinding lua_enumFromString_RenderStateAutoBinding(const char* s)
{
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_NONE) == 0)
        return RenderState::NONE;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_WORLD_MATRIX) == 0)
        return RenderState::WORLD_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_VIEW_MATRIX) == 0)
        return RenderState::VIEW_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_PROJECTION_MATRIX) == 0)
        return RenderState::PROJECTION_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_WORLD_VIEW_MATRIX) == 0)
        return RenderState::WORLD_VIEW_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_VIEW_PROJECTION_MATRIX) == 0)
        return RenderState::VIEW_PROJECTION_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_WORLD_VIEW_PROJECTION_MATRIX) == 0)
        return RenderState::WORLD_VIEW_PROJECTION_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_MATRIX) == 0)
        return RenderState::INVERSE_TRANSPOSE_WORLD_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX) == 0)
        return RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_CAMERA_WORLD_POSITION) == 0)
        return RenderState::CAMERA_WORLD_POSITION;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_CAMERA_VIEW_POSITION) == 0)
        return RenderState::CAMERA_VIEW_POSITION;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_MATRIX_PALETTE) == 0)
        return RenderState::MATRIX_PALETTE;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_SCENE_AMBIENT_COLOR) == 0)
        return RenderState::SCENE_AMBIENT_COLOR;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_SCENE_LIGHT_COLOR) == 0)
        return RenderState::SCENE_LIGHT_COLOR;
    if (strcmp(s, luaEnumString_RenderStateAutoBinding_SCENE_LIGHT_DIRECTION) == 0)
        return RenderState::SCENE_LIGHT_DIRECTION;
    return RenderState::NONE;
}

const char* lua_stringFromEnum_RenderStateAutoBinding(RenderState::AutoBinding e)
{
    if (e == RenderState::NONE)
        return luaEnumString_RenderStateAutoBinding_NONE;
    if (e == RenderState::WORLD_MATRIX)
        return luaEnumString_RenderStateAutoBinding_WORLD_MATRIX;
    if (e == RenderState::VIEW_MATRIX)
        return luaEnumString_RenderStateAutoBinding_VIEW_MATRIX;
    if (e == RenderState::PROJECTION_MATRIX)
        return luaEnumString_RenderStateAutoBinding_PROJECTION_MATRIX;
    if (e == RenderState::WORLD_VIEW_MATRIX)
        return luaEnumString_RenderStateAutoBinding_WORLD_VIEW_MATRIX;
    if (e == RenderState::VIEW_PROJECTION_MATRIX)
        return luaEnumString_RenderStateAutoBinding_VIEW_PROJECTION_MATRIX;
    if (e == RenderState::WORLD_VIEW_PROJECTION_MATRIX)
        return luaEnumString_RenderStateAutoBinding_WORLD_VIEW_PROJECTION_MATRIX;
    if (e == RenderState::INVERSE_TRANSPOSE_WORLD_MATRIX)
        return luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_MATRIX;
    if (e == RenderState::INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX)
        return luaEnumString_RenderStateAutoBinding_INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX;
    if (e == RenderState::CAMERA_WORLD_POSITION)
        return luaEnumString_RenderStateAutoBinding_CAMERA_WORLD_POSITION;
    if (e == RenderState::CAMERA_VIEW_POSITION)
        return luaEnumString_RenderStateAutoBinding_CAMERA_VIEW_POSITION;
    if (e == RenderState::MATRIX_PALETTE)
        return luaEnumString_RenderStateAutoBinding_MATRIX_PALETTE;
    if (e == RenderState::SCENE_AMBIENT_COLOR)
        return luaEnumString_RenderStateAutoBinding_SCENE_AMBIENT_COLOR;
    if (e == RenderState::SCENE_LIGHT_COLOR)
        return luaEnumString_RenderStateAutoBinding_SCENE_LIGHT_COLOR;
    if (e == RenderState::SCENE_LIGHT_DIRECTION)
        return luaEnumString_RenderStateAutoBinding_SCENE_LIGHT_DIRECTION;
    return enumStringEmpty;
}

}

