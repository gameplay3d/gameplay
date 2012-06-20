#ifndef LUA_PLATFORM_H_
#define LUA_PLATFORM_H_

namespace gameplay
{

// Lua bindings for Platform.
int lua_Platform__gc(lua_State* state);
int lua_Platform_enterMessagePump(lua_State* state);
int lua_Platform_static_create(lua_State* state);
int lua_Platform_static_displayKeyboard(lua_State* state);
int lua_Platform_static_getAbsoluteTime(lua_State* state);
int lua_Platform_static_getAccelerometerValues(lua_State* state);
int lua_Platform_static_getDisplayHeight(lua_State* state);
int lua_Platform_static_getDisplayWidth(lua_State* state);
int lua_Platform_static_isMultiTouch(lua_State* state);
int lua_Platform_static_isVsync(lua_State* state);
int lua_Platform_static_keyEventInternal(lua_State* state);
int lua_Platform_static_setAbsoluteTime(lua_State* state);
int lua_Platform_static_setMultiTouch(lua_State* state);
int lua_Platform_static_setVsync(lua_State* state);
int lua_Platform_static_signalShutdown(lua_State* state);
int lua_Platform_static_sleep(lua_State* state);
int lua_Platform_static_swapBuffers(lua_State* state);
int lua_Platform_static_touchEventInternal(lua_State* state);

void luaRegister_Platform();

}

#endif
