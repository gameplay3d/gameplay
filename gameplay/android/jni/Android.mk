# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)/../../src

include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay
LOCAL_SRC_FILES := AbsoluteLayout.cpp Animation.cpp AnimationClip.cpp AnimationController.cpp AnimationTarget.cpp AnimationValue.cpp AudioBuffer.cpp AudioController.cpp AudioListener.cpp AudioSource.cpp BoundingBox.cpp BoundingSphere.cpp Bundle.cpp Button.cpp Camera.cpp CheckBox.cpp Container.cpp Control.cpp Curve.cpp DebugNew.cpp DepthStencilTarget.cpp Effect.cpp FileSystem.cpp FlowLayout.cpp Font.cpp Form.cpp FrameBuffer.cpp Frustum.cpp Game.cpp gameplay-main-android.cpp Image.cpp Joint.cpp Label.cpp Layout.cpp Light.cpp Material.cpp MaterialParameter.cpp Matrix.cpp Mesh.cpp MeshBatch.cpp MeshPart.cpp MeshSkin.cpp Model.cpp Node.cpp ParticleEmitter.cpp Pass.cpp PhysicsCharacter.cpp PhysicsCollisionObject.cpp PhysicsCollisionShape.cpp PhysicsConstraint.cpp PhysicsController.cpp PhysicsFixedConstraint.cpp PhysicsGenericConstraint.cpp PhysicsGhostObject.cpp PhysicsHingeConstraint.cpp PhysicsMotionState.cpp PhysicsRigidBody.cpp PhysicsSocketConstraint.cpp PhysicsSpringConstraint.cpp Plane.cpp PlatformAndroid.cpp Properties.cpp Quaternion.cpp RadioButton.cpp Ray.cpp Rectangle.cpp Ref.cpp RenderState.cpp RenderTarget.cpp Scene.cpp SceneLoader.cpp Slider.cpp SpriteBatch.cpp Technique.cpp TextBox.cpp Texture.cpp Theme.cpp ThemeStyle.cpp Transform.cpp Vector2.cpp Vector3.cpp Vector4.cpp VertexAttributeBinding.cpp VertexFormat.cpp VerticalLayout.cpp
LOCAL_CFLAGS := -D__ANDROID__ -I"../../external-deps/bullet/include" -I"../../external-deps/libpng/include" -I"../../external-deps/oggvorbis/include" -I"../../external-deps/openal/include"
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)