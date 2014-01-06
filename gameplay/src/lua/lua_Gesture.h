#ifndef LUA_GESTURE_H_
#define LUA_GESTURE_H_

namespace gameplay
{

// Lua bindings for Gesture.
int lua_Gesture__gc(lua_State* state);
int lua_Gesture_static_SWIPE_DIRECTION_DOWN(lua_State* state);
int lua_Gesture_static_SWIPE_DIRECTION_LEFT(lua_State* state);
int lua_Gesture_static_SWIPE_DIRECTION_RIGHT(lua_State* state);
int lua_Gesture_static_SWIPE_DIRECTION_UP(lua_State* state);

void luaRegister_Gesture();

}

#endif
