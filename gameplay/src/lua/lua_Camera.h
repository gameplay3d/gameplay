#ifndef LUA_CAMERA_H_
#define LUA_CAMERA_H_

namespace gameplay
{

// Lua bindings for Camera.
int lua_Camera__gc(lua_State* state);
int lua_Camera_addRef(lua_State* state);
int lua_Camera_getAspectRatio(lua_State* state);
int lua_Camera_getCameraType(lua_State* state);
int lua_Camera_getFarPlane(lua_State* state);
int lua_Camera_getFieldOfView(lua_State* state);
int lua_Camera_getFrustum(lua_State* state);
int lua_Camera_getInverseViewMatrix(lua_State* state);
int lua_Camera_getInverseViewProjectionMatrix(lua_State* state);
int lua_Camera_getNearPlane(lua_State* state);
int lua_Camera_getNode(lua_State* state);
int lua_Camera_getProjectionMatrix(lua_State* state);
int lua_Camera_getRefCount(lua_State* state);
int lua_Camera_getViewMatrix(lua_State* state);
int lua_Camera_getViewProjectionMatrix(lua_State* state);
int lua_Camera_getZoomX(lua_State* state);
int lua_Camera_getZoomY(lua_State* state);
int lua_Camera_pickRay(lua_State* state);
int lua_Camera_project(lua_State* state);
int lua_Camera_release(lua_State* state);
int lua_Camera_resetProjectionMatrix(lua_State* state);
int lua_Camera_setAspectRatio(lua_State* state);
int lua_Camera_setFarPlane(lua_State* state);
int lua_Camera_setFieldOfView(lua_State* state);
int lua_Camera_setNearPlane(lua_State* state);
int lua_Camera_setProjectionMatrix(lua_State* state);
int lua_Camera_setZoomX(lua_State* state);
int lua_Camera_setZoomY(lua_State* state);
int lua_Camera_static_create(lua_State* state);
int lua_Camera_static_createOrthographic(lua_State* state);
int lua_Camera_static_createPerspective(lua_State* state);
int lua_Camera_unproject(lua_State* state);

void luaRegister_Camera();

}

#endif
