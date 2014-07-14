SAMPLE_PATH := $(call my-dir)/../../src

# external-deps
PNG_PATH := ../GAMEPLAY_PATH/external-deps/png/lib/android/arm
ZLIB_PATH := ../GAMEPLAY_PATH/external-deps/zlib/lib/android/arm
LUA_PATH := ../GAMEPLAY_PATH/external-deps/lua/lib/android/arm
BULLET_PATH := ../GAMEPLAY_PATH/external-deps/bullet/lib/android/arm
OGG_PATH := ../GAMEPLAY_PATH/external-deps/ogg/lib/android/arm
VORBIS_PATH := ../GAMEPLAY_PATH/external-deps/vorbis/lib/android/arm
OPENAL_PATH := ../GAMEPLAY_PATH/external-deps/openal/lib/android/arm

# gameplay
LOCAL_PATH := ../GAMEPLAY_PATH/gameplay/android/obj/local/armeabi-v7a
include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay
LOCAL_SRC_FILES := libgameplay.a
include $(PREBUILT_STATIC_LIBRARY)

# libpng
LOCAL_PATH := $(PNG_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libpng 
LOCAL_SRC_FILES := libpng.a
include $(PREBUILT_STATIC_LIBRARY)

# libz
LOCAL_PATH := $(ZLIB_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libz
LOCAL_SRC_FILES := libz.a
include $(PREBUILT_STATIC_LIBRARY)

# liblua
LOCAL_PATH := $(LUA_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := liblua
LOCAL_SRC_FILES := liblua.a
include $(PREBUILT_STATIC_LIBRARY)

# libLinearMath
LOCAL_PATH := $(BULLET_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libLinearMath
LOCAL_SRC_FILES := libLinearMath.a
include $(PREBUILT_STATIC_LIBRARY)

# libBulletCollision
LOCAL_PATH := $(BULLET_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libBulletCollision
LOCAL_SRC_FILES := libBulletCollision.a
include $(PREBUILT_STATIC_LIBRARY)

# libBulletDynamics
LOCAL_PATH := $(BULLET_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libBulletDynamics
LOCAL_SRC_FILES := libBulletDynamics.a
include $(PREBUILT_STATIC_LIBRARY)

# libogg
LOCAL_PATH := $(OGG_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libogg
LOCAL_SRC_FILES := libogg.a
include $(PREBUILT_STATIC_LIBRARY)

# libvorbis
LOCAL_PATH := $(VORBIS_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libvorbis
LOCAL_SRC_FILES := libvorbis.a

# libOpenAL
LOCAL_PATH := $(OPENAL_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := libOpenAL
LOCAL_SRC_FILES := libOpenAL.a
include $(PREBUILT_STATIC_LIBRARY)

# TEMPLATE_PROJECT
LOCAL_PATH := $(SAMPLE_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE    := TEMPLATE_PROJECT
LOCAL_SRC_FILES := ../GAMEPLAY_PATH/gameplay/src/gameplay-main-android.cpp TemplateGame.cpp

LOCAL_CPPFLAGS += -std=c++11
LOCAL_ARM_MODE := arm
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -D__ANDROID__ -I"../GAMEPLAY_PATH/external-deps/lua/include" -I"../GAMEPLAY_PATH/external-deps/bullet/include" -I"../GAMEPLAY_PATH/external-deps/png/include" -I"../GAMEPLAY_PATH/external-deps/ogg/include" -I"../GAMEPLAY_PATH/external-deps/vorbis/include" -I"../GAMEPLAY_PATH/external-deps/openal/include" -I"../GAMEPLAY_PATH/gameplay/src"

LOCAL_STATIC_LIBRARIES := android_native_app_glue libgameplay libpng libz liblua libBulletDynamics libBulletCollision libLinearMath libvorbis libogg libOpenAL

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
