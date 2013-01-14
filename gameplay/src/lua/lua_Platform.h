#ifndef LUA_PLATFORM_H_
#define LUA_PLATFORM_H_

namespace gameplay
{

// Lua bindings for Platform.
int lua_Platform__gc(lua_State* state);
int lua_Platform_enterMessagePump(lua_State* state);
int lua_Platform_static_canExit(lua_State* state);
int lua_Platform_static_displayKeyboard(lua_State* state);
int lua_Platform_static_getAbsoluteTime(lua_State* state);
int lua_Platform_static_getAccelerometerValues(lua_State* state);
int lua_Platform_static_getDisplayHeight(lua_State* state);
int lua_Platform_static_getDisplayWidth(lua_State* state);
int lua_Platform_static_hasMouse(lua_State* state);
int lua_Platform_static_isCursorVisible(lua_State* state);
int lua_Platform_static_isGestureRegistered(lua_State* state);
int lua_Platform_static_isGestureSupported(lua_State* state);
int lua_Platform_static_isMouseCaptured(lua_State* state);
int lua_Platform_static_isMultiTouch(lua_State* state);
int lua_Platform_static_isVsync(lua_State* state);
int lua_Platform_static_keyEventInternal(lua_State* state);
int lua_Platform_static_launchURL(lua_State* state);
int lua_Platform_static_mouseEventInternal(lua_State* state);
int lua_Platform_static_pollGamepadState(lua_State* state);
int lua_Platform_static_registerGesture(lua_State* state);
int lua_Platform_static_setAbsoluteTime(lua_State* state);
int lua_Platform_static_setCursorVisible(lua_State* state);
int lua_Platform_static_setMouseCaptured(lua_State* state);
int lua_Platform_static_setMultiTouch(lua_State* state);
int lua_Platform_static_setVsync(lua_State* state);
int lua_Platform_static_signalShutdown(lua_State* state);
int lua_Platform_static_sleep(lua_State* state);
int lua_Platform_static_swapBuffers(lua_State* state);
int lua_Platform_static_touchEventInternal(lua_State* state);
int lua_Platform_static_unregisterGesture(lua_State* state);

void luaRegister_Platform();

}

#endif
