#ifndef LUA_GAMEPAD_H_
#define LUA_GAMEPAD_H_

namespace gameplay
{

// Lua bindings for Gamepad.
int lua_Gamepad_draw(lua_State* state);
int lua_Gamepad_getButtonCount(lua_State* state);
int lua_Gamepad_getButtonState(lua_State* state);
int lua_Gamepad_getForm(lua_State* state);
int lua_Gamepad_getId(lua_State* state);
int lua_Gamepad_getJoystickAxisValues(lua_State* state);
int lua_Gamepad_getJoystickAxisX(lua_State* state);
int lua_Gamepad_getJoystickAxisY(lua_State* state);
int lua_Gamepad_getJoystickCount(lua_State* state);
int lua_Gamepad_isJoystickActive(lua_State* state);
int lua_Gamepad_isVirtual(lua_State* state);
int lua_Gamepad_update(lua_State* state);

void luaRegister_Gamepad();

}

#endif
