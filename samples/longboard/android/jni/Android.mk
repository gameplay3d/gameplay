SAMPLE_PATH := $(call my-dir)/../../src

# external-deps
LIBPNG_PATH := $(call my-dir)/../../../../external-deps/libpng/lib/android/arm
ZLIB_PATH := $(call my-dir)/../../../../external-deps/zlib/lib/android/arm
LUA_PATH := $(call my-dir)/../../../../external-deps/lua/lib/android/arm
BULLET_PATH := $(call my-dir)/../../../../external-deps/bullet/lib/android/arm
VORBIS_PATH := $(call my-dir)/../../../../external-deps/oggvorbis/lib/android/arm
OPENAL_PATH := $(call my-dir)/../../../../external-deps/openal/lib/android/arm

# gameplay
LOCAL_PATH := $(call my-dir)/../../../../gameplay/android/obj/local/armeabi
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

# liblua
LOCAL_PATH := $(LUA_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := liblua
LOCAL_SRC_FILES := liblua.a
include $(PREBUILT_STATIC_LIBRARY)

# libbullet
LOCAL_PATH := $(BULLET_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libbullet
LOCAL_SRC_FILES := libbullet.a
include $(PREBUILT_STATIC_LIBRARY)

# libvorbis
LOCAL_PATH := $(VORBIS_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libvorbis
LOCAL_SRC_FILES := libvorbis.a
include $(PREBUILT_STATIC_LIBRARY)

# libOpenAL
LOCAL_PATH := $(OPENAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libOpenAL
LOCAL_SRC_FILES := libOpenAL.a
include $(PREBUILT_STATIC_LIBRARY)

# sample-longboard
LOCAL_PATH := $(SAMPLE_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE    := sample-longboard
LOCAL_SRC_FILES := ../../../gameplay/src/gameplay-main-android.cpp LongboardGame.cpp

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -D__ANDROID__ -Wno-psabi -I"../../../external-deps/lua/include" -I"../../../external-deps/bullet/include" -I"../../../external-deps/libpng/include" -I"../../../external-deps/oggvorbis/include" -I"../../../external-deps/openal/include" -I"../../../gameplay/src"
LOCAL_STATIC_LIBRARIES := android_native_app_glue libgameplay libpng libzlib liblua libbullet libvorbis libOpenAL

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
