#ifndef LUA_GLOBAL_H_
#define LUA_GLOBAL_H_

#include "lua_AIMessageParameterType.h"
#include "lua_AnimationClipListenerEventType.h"
#include "lua_AudioSourceState.h"
#include "lua_CameraType.h"
#include "lua_ContainerScroll.h"
#include "lua_ControlAlignment.h"
#include "lua_ControlListenerEventType.h"
#include "lua_ControlState.h"
#include "lua_CurveInterpolationType.h"
#include "lua_DepthStencilTargetFormat.h"
#include "lua_FontJustify.h"
#include "lua_FontStyle.h"
#include "lua_GameClearFlags.h"
#include "lua_GameState.h"
#include "lua_GamepadButtonMapping.h"
#include "lua_GamepadGamepadEvent.h"
#include "lua_GestureGestureEvent.h"
#include "lua_ImageFormat.h"
#include "lua_KeyboardKey.h"
#include "lua_KeyboardKeyEvent.h"
#include "lua_LayoutType.h"
#include "lua_LightType.h"
#include "lua_LoggerLevel.h"
#include "lua_MeshIndexFormat.h"
#include "lua_MeshPrimitiveType.h"
#include "lua_MouseMouseEvent.h"
#include "lua_NodeType.h"
#include "lua_ParticleEmitterTextureBlending.h"
#include "lua_PhysicsCollisionObjectCollisionListenerEventType.h"
#include "lua_PhysicsCollisionObjectType.h"
#include "lua_PhysicsCollisionShapeType.h"
#include "lua_PhysicsControllerListenerEventType.h"
#include "lua_PropertiesType.h"
#include "lua_RenderStateAutoBinding.h"
#include "lua_RenderStateBlend.h"
#include "lua_RenderStateCullFaceSide.h"
#include "lua_RenderStateDepthFunction.h"
#include "lua_SceneDebugFlags.h"
#include "lua_TerrainFlags.h"
#include "lua_TextureFilter.h"
#include "lua_TextureFormat.h"
#include "lua_TextureWrap.h"
#include "lua_TouchTouchEvent.h"
#include "lua_VertexFormatUsage.h"

namespace gameplay
{

// Global enum to string conversion function (used to pass enums to Lua from C++).
const char* lua_stringFromEnumGlobal(std::string& enumname, unsigned int value);

void luaRegister_lua_Global();

}

#endif
