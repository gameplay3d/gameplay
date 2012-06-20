#ifndef LUA_GLOBAL_H_
#define LUA_GLOBAL_H_

#include "Animation.h"
#include "AnimationClip.h"
#include "AnimationController.h"
#include "AnimationTarget.h"
#include "AnimationValue.h"
#include "AudioController.h"
#include "AudioListener.h"
#include "AudioSource.h"
#include "Base.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "Button.h"
#include "C:/git/gameplay/gameplay/src/Base.h"
#include "C:/git/gameplay/gameplay/src/BoundingBox.h"
#include "C:/git/gameplay/gameplay/src/BoundingSphere.h"
#include "C:/git/gameplay/gameplay/src/Image.h"
#include "C:/git/gameplay/gameplay/src/Matrix.h"
#include "C:/git/gameplay/gameplay/src/MeshBatch.h"
#include "C:/git/gameplay/gameplay/src/PhysicsConstraint.h"
#include "C:/git/gameplay/gameplay/src/PhysicsFixedConstraint.h"
#include "C:/git/gameplay/gameplay/src/PhysicsGenericConstraint.h"
#include "C:/git/gameplay/gameplay/src/PhysicsRigidBody.h"
#include "C:/git/gameplay/gameplay/src/PhysicsSpringConstraint.h"
#include "C:/git/gameplay/gameplay/src/Plane.h"
#include "C:/git/gameplay/gameplay/src/Platform.h"
#include "C:/git/gameplay/gameplay/src/Quaternion.h"
#include "C:/git/gameplay/gameplay/src/Ray.h"
#include "C:/git/gameplay/gameplay/src/ScriptController.h"
#include "C:/git/gameplay/gameplay/src/Vector2.h"
#include "C:/git/gameplay/gameplay/src/Vector3.h"
#include "C:/git/gameplay/gameplay/src/Vector4.h"
#include "Camera.h"
#include "Container.h"
#include "Control.h"
#include "Curve.h"
#include "DebugNew.h"
#include "DepthStencilTarget.h"
#include "Effect.h"
#include "Font.h"
#include "Form.h"
#include "FrameBuffer.h"
#include "Frustum.h"
#include "Game.h"
#include "Image.h"
#include "Keyboard.h"
#include "Label.h"
#include "Layout.h"
#include "Light.h"
#include "Material.h"
#include "MaterialParameter.h"
#include "Matrix.h"
#include "Mesh.h"
#include "MeshBatch.h"
#include "MeshSkin.h"
#include "Model.h"
#include "Mouse.h"
#include "Node.h"
#include "ParticleEmitter.h"
#include "Pass.h"
#include "PhysicsCollisionObject.h"
#include "PhysicsCollisionShape.h"
#include "PhysicsConstraint.h"
#include "PhysicsController.h"
#include "PhysicsFixedConstraint.h"
#include "PhysicsGenericConstraint.h"
#include "PhysicsGhostObject.h"
#include "PhysicsHingeConstraint.h"
#include "PhysicsRigidBody.h"
#include "PhysicsSocketConstraint.h"
#include "PhysicsSpringConstraint.h"
#include "Plane.h"
#include "Platform.h"
#include "Properties.h"
#include "Quaternion.h"
#include "Ray.h"
#include "Rectangle.h"
#include "Ref.h"
#include "RenderState.h"
#include "RenderTarget.h"
#include "Scene.h"
#include "ScriptController.h"
#include "SpriteBatch.h"
#include "Technique.h"
#include "Texture.h"
#include "Theme.h"
#include "ThemeStyle.h"
#include "TimeListener.h"
#include "Touch.h"
#include "Transform.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "VertexAttributeBinding.h"
#include "VertexFormat.h"

namespace gameplay
{

// Lua bindings for global functions.
int lua____al_error_code(lua_State* state);
int lua____gl_error_code(lua_State* state);
int lua__printError(lua_State* state);

// Lua bindings for enum conversion functions.
AnimationClip::Listener::EventType lua_enumFromString_AnimationClipListenerEventType(const char* s);
std::string lua_stringFromEnum_AnimationClipListenerEventType(AnimationClip::Listener::EventType e);
AudioSource::State lua_enumFromString_AudioSourceState(const char* s);
std::string lua_stringFromEnum_AudioSourceState(AudioSource::State e);
Camera::Type lua_enumFromString_CameraType(const char* s);
std::string lua_stringFromEnum_CameraType(Camera::Type e);
Container::Scroll lua_enumFromString_ContainerScroll(const char* s);
std::string lua_stringFromEnum_ContainerScroll(Container::Scroll e);
Control::Alignment lua_enumFromString_ControlAlignment(const char* s);
std::string lua_stringFromEnum_ControlAlignment(Control::Alignment e);
Control::Listener::EventType lua_enumFromString_ControlListenerEventType(const char* s);
std::string lua_stringFromEnum_ControlListenerEventType(Control::Listener::EventType e);
Control::State lua_enumFromString_ControlState(const char* s);
std::string lua_stringFromEnum_ControlState(Control::State e);
Curve::InterpolationType lua_enumFromString_CurveInterpolationType(const char* s);
std::string lua_stringFromEnum_CurveInterpolationType(Curve::InterpolationType e);
DepthStencilTarget::Format lua_enumFromString_DepthStencilTargetFormat(const char* s);
std::string lua_stringFromEnum_DepthStencilTargetFormat(DepthStencilTarget::Format e);
Font::Justify lua_enumFromString_FontJustify(const char* s);
std::string lua_stringFromEnum_FontJustify(Font::Justify e);
Font::Style lua_enumFromString_FontStyle(const char* s);
std::string lua_stringFromEnum_FontStyle(Font::Style e);
Game::ClearFlags lua_enumFromString_GameClearFlags(const char* s);
std::string lua_stringFromEnum_GameClearFlags(Game::ClearFlags e);
Game::State lua_enumFromString_GameState(const char* s);
std::string lua_stringFromEnum_GameState(Game::State e);
Image::Format lua_enumFromString_ImageFormat(const char* s);
std::string lua_stringFromEnum_ImageFormat(Image::Format e);
Keyboard::Key lua_enumFromString_KeyboardKey(const char* s);
std::string lua_stringFromEnum_KeyboardKey(Keyboard::Key e);
Keyboard::KeyEvent lua_enumFromString_KeyboardKeyEvent(const char* s);
std::string lua_stringFromEnum_KeyboardKeyEvent(Keyboard::KeyEvent e);
Layout::Type lua_enumFromString_LayoutType(const char* s);
std::string lua_stringFromEnum_LayoutType(Layout::Type e);
Light::Type lua_enumFromString_LightType(const char* s);
std::string lua_stringFromEnum_LightType(Light::Type e);
Mesh::IndexFormat lua_enumFromString_MeshIndexFormat(const char* s);
std::string lua_stringFromEnum_MeshIndexFormat(Mesh::IndexFormat e);
Mesh::PrimitiveType lua_enumFromString_MeshPrimitiveType(const char* s);
std::string lua_stringFromEnum_MeshPrimitiveType(Mesh::PrimitiveType e);
Mouse::MouseEvent lua_enumFromString_MouseMouseEvent(const char* s);
std::string lua_stringFromEnum_MouseMouseEvent(Mouse::MouseEvent e);
Node::Type lua_enumFromString_NodeType(const char* s);
std::string lua_stringFromEnum_NodeType(Node::Type e);
ParticleEmitter::TextureBlending lua_enumFromString_ParticleEmitterTextureBlending(const char* s);
std::string lua_stringFromEnum_ParticleEmitterTextureBlending(ParticleEmitter::TextureBlending e);
PhysicsCollisionObject::CollisionListener::EventType lua_enumFromString_PhysicsCollisionObjectCollisionListenerEventType(const char* s);
std::string lua_stringFromEnum_PhysicsCollisionObjectCollisionListenerEventType(PhysicsCollisionObject::CollisionListener::EventType e);
PhysicsCollisionObject::Type lua_enumFromString_PhysicsCollisionObjectType(const char* s);
std::string lua_stringFromEnum_PhysicsCollisionObjectType(PhysicsCollisionObject::Type e);
PhysicsCollisionShape::Type lua_enumFromString_PhysicsCollisionShapeType(const char* s);
std::string lua_stringFromEnum_PhysicsCollisionShapeType(PhysicsCollisionShape::Type e);
PhysicsController::Listener::EventType lua_enumFromString_PhysicsControllerListenerEventType(const char* s);
std::string lua_stringFromEnum_PhysicsControllerListenerEventType(PhysicsController::Listener::EventType e);
Properties::Type lua_enumFromString_PropertiesType(const char* s);
std::string lua_stringFromEnum_PropertiesType(Properties::Type e);
RenderState::AutoBinding lua_enumFromString_RenderStateAutoBinding(const char* s);
std::string lua_stringFromEnum_RenderStateAutoBinding(RenderState::AutoBinding e);
RenderState::Blend lua_enumFromString_RenderStateBlend(const char* s);
std::string lua_stringFromEnum_RenderStateBlend(RenderState::Blend e);
Scene::DebugFlags lua_enumFromString_SceneDebugFlags(const char* s);
std::string lua_stringFromEnum_SceneDebugFlags(Scene::DebugFlags e);
Texture::Filter lua_enumFromString_TextureFilter(const char* s);
std::string lua_stringFromEnum_TextureFilter(Texture::Filter e);
Texture::Format lua_enumFromString_TextureFormat(const char* s);
std::string lua_stringFromEnum_TextureFormat(Texture::Format e);
Texture::Wrap lua_enumFromString_TextureWrap(const char* s);
std::string lua_stringFromEnum_TextureWrap(Texture::Wrap e);
Touch::TouchEvent lua_enumFromString_TouchTouchEvent(const char* s);
std::string lua_stringFromEnum_TouchTouchEvent(Touch::TouchEvent e);
VertexFormat::Usage lua_enumFromString_VertexFormatUsage(const char* s);
std::string lua_stringFromEnum_VertexFormatUsage(VertexFormat::Usage e);

void luaRegister_lua_Global();

}

#endif
