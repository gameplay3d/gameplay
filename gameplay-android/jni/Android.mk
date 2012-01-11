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
LOCAL_PATH := $(call my-dir)/../../gameplay/src

include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay
LOCAL_SRC_FILES := Animation.cpp DepthStencilTarget.cpp MeshBatch.cpp PhysicsRigidBody.cpp SceneLoader.cpp AnimationClip.cpp Effect.cpp MeshPart.cpp PhysicsSocketConstraint.cpp SpriteBatch.cpp AnimationController.cpp FileSystem.cpp MeshSkin.cpp PhysicsSpringConstraint.cpp Technique.cpp AnimationTarget.cpp Font.cpp Model.cpp Plane.cpp Texture.cpp AnimationValue.cpp FrameBuffer.cpp Node.cpp PlatformAndroid.cpp PlatformQNX.cpp AudioBuffer.cpp Frustum.cpp Package.cpp PlatformWin32.cpp Transform.cpp AudioController.cpp Game.cpp ParticleEmitter.cpp Properties.cpp Vector2.cpp AudioListener.cpp Image.cpp Pass.cpp Quaternion.cpp Vector3.cpp AudioSource.cpp Joint.cpp PhysicsConstraint.cpp Ray.cpp Vector4.cpp BoundingBox.cpp Light.cpp PhysicsController.cpp Rectangle.cpp VertexAttributeBinding.cpp BoundingSphere.cpp Material.cpp PhysicsFixedConstraint.cpp Ref.cpp VertexFormat.cpp Camera.cpp MaterialParameter.cpp PhysicsGenericConstraint.cpp RenderState.cpp Viewport.cpp Curve.cpp Matrix.cpp PhysicsHingeConstraint.cpp RenderTarget.cpp DebugNew.cpp Mesh.cpp PhysicsMotionState.cpp Scene.cpp gameplay-main-android.cpp
LOCAL_CFLAGS := -D__ANDROID__ -I"../external-deps/bullet/include" -I"../external-deps/openal/include" -I"../external-deps/libpng/include" -I"../external-deps/oggvorbis/include"
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)