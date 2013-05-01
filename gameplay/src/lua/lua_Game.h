#ifndef LUA_GAME_H_
#define LUA_GAME_H_

namespace gameplay
{

// Lua bindings for Game.
int lua_Game__gc(lua_State* state);
int lua_Game_canExit(lua_State* state);
int lua_Game_clear(lua_State* state);
int lua_Game_displayKeyboard(lua_State* state);
int lua_Game_exit(lua_State* state);
int lua_Game_frame(lua_State* state);
int lua_Game_gamepadEvent(lua_State* state);
int lua_Game_gesturePinchEvent(lua_State* state);
int lua_Game_gestureSwipeEvent(lua_State* state);
int lua_Game_gestureTapEvent(lua_State* state);
int lua_Game_getAIController(lua_State* state);
int lua_Game_getAccelerometerValues(lua_State* state);
int lua_Game_getAnimationController(lua_State* state);
int lua_Game_getAspectRatio(lua_State* state);
int lua_Game_getAudioController(lua_State* state);
int lua_Game_getAudioListener(lua_State* state);
int lua_Game_getConfig(lua_State* state);
int lua_Game_getFrameRate(lua_State* state);
int lua_Game_getGamepad(lua_State* state);
int lua_Game_getGamepadCount(lua_State* state);
int lua_Game_getHeight(lua_State* state);
int lua_Game_getPhysicsController(lua_State* state);
int lua_Game_getRawSensorValues(lua_State* state);
int lua_Game_getScriptController(lua_State* state);
int lua_Game_getState(lua_State* state);
int lua_Game_getViewport(lua_State* state);
int lua_Game_getWidth(lua_State* state);
int lua_Game_hasAccelerometer(lua_State* state);
int lua_Game_hasMouse(lua_State* state);
int lua_Game_isCursorVisible(lua_State* state);
int lua_Game_isGestureRegistered(lua_State* state);
int lua_Game_isGestureSupported(lua_State* state);
int lua_Game_isInitialized(lua_State* state);
int lua_Game_isMouseCaptured(lua_State* state);
int lua_Game_isMultiSampling(lua_State* state);
int lua_Game_isMultiTouch(lua_State* state);
int lua_Game_keyEvent(lua_State* state);
int lua_Game_launchURL(lua_State* state);
int lua_Game_menuEvent(lua_State* state);
int lua_Game_mouseEvent(lua_State* state);
int lua_Game_pause(lua_State* state);
int lua_Game_registerGesture(lua_State* state);
int lua_Game_resizeEvent(lua_State* state);
int lua_Game_resume(lua_State* state);
int lua_Game_run(lua_State* state);
int lua_Game_schedule(lua_State* state);
int lua_Game_setCursorVisible(lua_State* state);
int lua_Game_setMouseCaptured(lua_State* state);
int lua_Game_setMultiSampling(lua_State* state);
int lua_Game_setMultiTouch(lua_State* state);
int lua_Game_setViewport(lua_State* state);
int lua_Game_static_getAbsoluteTime(lua_State* state);
int lua_Game_static_getGameTime(lua_State* state);
int lua_Game_static_getInstance(lua_State* state);
int lua_Game_static_isVsync(lua_State* state);
int lua_Game_static_setVsync(lua_State* state);
int lua_Game_touchEvent(lua_State* state);
int lua_Game_unregisterGesture(lua_State* state);

void luaRegister_Game();

}

#endif
