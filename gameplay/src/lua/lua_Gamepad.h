#ifndef LUA_GAMEPAD_H_
#define LUA_GAMEPAD_H_

namespace gameplay
{

// Lua bindings for Gamepad.
int lua_Gamepad_draw(lua_State* state);
int lua_Gamepad_getButtonCount(lua_State* state);
int lua_Gamepad_getForm(lua_State* state);
int lua_Gamepad_getJoystickCount(lua_State* state);
int lua_Gamepad_getJoystickValues(lua_State* state);
int lua_Gamepad_getProductId(lua_State* state);
int lua_Gamepad_getProductString(lua_State* state);
int lua_Gamepad_getTriggerCount(lua_State* state);
int lua_Gamepad_getTriggerValue(lua_State* state);
int lua_Gamepad_getVendorId(lua_State* state);
int lua_Gamepad_getVendorString(lua_State* state);
int lua_Gamepad_isButtonDown(lua_State* state);
int lua_Gamepad_isVirtual(lua_State* state);
int lua_Gamepad_update(lua_State* state);

void luaRegister_Gamepad();

}

#endif
