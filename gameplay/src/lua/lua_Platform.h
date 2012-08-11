#ifndef LUA_PLATFORM_H_
#define LUA_PLATFORM_H_

namespace gameplay
{

// Lua bindings for Platform.
int lua_Platform__gc(lua_State* state);
int lua_Platform_enterMessagePump(lua_State* state);
int lua_Platform_static_displayKeyboard(lua_State* state);
int lua_Platform_static_getAbsoluteTime(lua_State* state);
int lua_Platform_static_getAccelerometerValues(lua_State* state);
int lua_Platform_static_getDisplayHeight(lua_State* state);
int lua_Platform_static_getDisplayWidth(lua_State* state);
int lua_Platform_static_getGamepadButtonCount(lua_State* state);
int lua_Platform_static_getGamepadButtonState(lua_State* state);
int lua_Platform_static_getGamepadId(lua_State* state);
int lua_Platform_static_getGamepadJoystickAxisValues(lua_State* state);
int lua_Platform_static_getGamepadJoystickAxisX(lua_State* state);
int lua_Platform_static_getGamepadJoystickAxisY(lua_State* state);
int lua_Platform_static_getGamepadJoystickCount(lua_State* state);
int lua_Platform_static_getGamepadTriggerCount(lua_State* state);
int lua_Platform_static_getGamepadTriggerValue(lua_State* state);
int lua_Platform_static_getGamepadsConnected(lua_State* state);
int lua_Platform_static_hasMouse(lua_State* state);
int lua_Platform_static_isCursorVisible(lua_State* state);
int lua_Platform_static_isGamepadConnected(lua_State* state);
int lua_Platform_static_isGamepadJoystickActive(lua_State* state);
int lua_Platform_static_isMouseCaptured(lua_State* state);
int lua_Platform_static_isMultiTouch(lua_State* state);
int lua_Platform_static_isVsync(lua_State* state);
int lua_Platform_static_keyEventInternal(lua_State* state);
int lua_Platform_static_mouseEventInternal(lua_State* state);
int lua_Platform_static_setAbsoluteTime(lua_State* state);
int lua_Platform_static_setCursorVisible(lua_State* state);
int lua_Platform_static_setMouseCaptured(lua_State* state);
int lua_Platform_static_setMultiTouch(lua_State* state);
int lua_Platform_static_setVsync(lua_State* state);
int lua_Platform_static_signalShutdown(lua_State* state);
int lua_Platform_static_sleep(lua_State* state);
int lua_Platform_static_swapBuffers(lua_State* state);
int lua_Platform_static_touchEventInternal(lua_State* state);

void luaRegister_Platform();

}

#endif
