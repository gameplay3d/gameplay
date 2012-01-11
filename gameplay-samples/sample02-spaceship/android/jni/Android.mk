# Copyright (C) 2010 The Android Open Source Project
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

SAMPLE_PATH := $(call my-dir)/../../src
LIBPNG_PATH := $(call my-dir)/../../../../external-deps/libpng/lib/android/arm
ZLIB_PATH := $(call my-dir)/../../../../external-deps/zlib/lib/android/arm
OPENAL_PATH := $(call my-dir)/../../../../external-deps/openal/lib/android/arm
OGGVORBIS_PATH := $(call my-dir)/../../../../external-deps/oggvorbis/lib/android/arm
BULLET_PATH := $(call my-dir)/../../../../external-deps/bullet/lib/android/arm

# gameplay
LOCAL_PATH := $(call my-dir)/../../../../gameplay-android/obj/local/armeabi
include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay
LOCAL_SRC_FILES := libgameplay.a
include $(PREBUILT_STATIC_LIBRARY)

# libpng
LOCAL_PATH := $(LIBPNG_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libpng 
LOCAL_SRC_FILES := libpng.a
include $(PREBUILT_STATIC_LIBRARY)

# libzlib
LOCAL_PATH := $(ZLIB_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libzlib
LOCAL_SRC_FILES := libzlib.a
include $(PREBUILT_STATIC_LIBRARY)

# libopenal
LOCAL_PATH := $(OPENAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libopenal 
LOCAL_SRC_FILES := libopenal.a
include $(PREBUILT_STATIC_LIBRARY)

# libogg
LOCAL_PATH := $(OGGVORBIS_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libogg
LOCAL_SRC_FILES := libogg.a
include $(PREBUILT_STATIC_LIBRARY)

# libvorbis
LOCAL_PATH := $(OGGVORBIS_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libvorbis
LOCAL_SRC_FILES := libvorbis.a
include $(PREBUILT_STATIC_LIBRARY)

# libvorbisfile
LOCAL_PATH := $(OGGVORBIS_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libvorbisfile 
LOCAL_SRC_FILES := libvorbisfile.a
include $(PREBUILT_STATIC_LIBRARY)

# libbullet
LOCAL_PATH := $(BULLET_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libbullet
LOCAL_SRC_FILES := libbullet.a
include $(PREBUILT_STATIC_LIBRARY)

# sample02-spaceship
LOCAL_PATH := $(SAMPLE_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE    := sample02-spaceship
LOCAL_SRC_FILES := ../android/jni/main.cpp SpaceshipGame.cpp

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 
LOCAL_CFLAGS    := -D__ANDROID__ -I"../../../external-deps/bullet/include" -I"../../../external-deps/openal/include" -I"../../../external-deps/libpng/include" -I"../../../external-deps/oggvorbis/include" -I"../../../gameplay/src"

LOCAL_STATIC_LIBRARIES := android_native_app_glue libgameplay libpng libzlib libbullet libvorbisfile libvorbis libogg libopenal

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
