#ifndef LUA_CAMERALISTENER_H_
#define LUA_CAMERALISTENER_H_

namespace gameplay
{

// Lua bindings for Camera::Listener.
int lua_CameraListener__gc(lua_State* state);
int lua_CameraListener_cameraChanged(lua_State* state);

void luaRegister_CameraListener();

}

#endif
