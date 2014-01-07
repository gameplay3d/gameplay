#ifndef LUA_ANIMATIONCONTROLLER_H_
#define LUA_ANIMATIONCONTROLLER_H_

namespace gameplay
{

// Lua bindings for AnimationController.
int lua_AnimationController_stopAllAnimations(lua_State* state);

void luaRegister_AnimationController();

}

#endif
