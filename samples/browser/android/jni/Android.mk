SAMPLE_PATH := $(call my-dir)/../../src

# external-deps
GAMEPLAY_DEPS := $(call my-dir)/../../../../external-deps/lib/android/$(TARGET_ARCH_ABI)

# libgameplay
LOCAL_PATH := $(call my-dir)/../../../../gameplay/android/libs/$(TARGET_ARCH_ABI)
include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay
LOCAL_SRC_FILES := libgameplay.so
include $(PREBUILT_SHARED_LIBRARY)

# libgameplay-deps
LOCAL_PATH := $(GAMEPLAY_DEPS)
include $(CLEAR_VARS)
LOCAL_MODULE    := libgameplay-deps 
LOCAL_SRC_FILES := libgameplay-deps.a
include $(PREBUILT_STATIC_LIBRARY)

# sample-browser
LOCAL_PATH := $(SAMPLE_PATH)
include $(CLEAR_VARS)

LOCAL_MODULE    := sample-browser
LOCAL_SRC_FILES := ../../../gameplay/src/gameplay-main-android.cpp \
    FirstPersonCamera.cpp \
    Grid.cpp \
    Sample.cpp \
    SamplesGame.cpp \
    Audio3DSample.cpp \
    AudioSample.cpp \
    BillboardSample.cpp \
    FontSample.cpp \
    FormsSample.cpp \
    GestureSample.cpp \
    GamepadSample.cpp \
    InputSample.cpp \
    LightSample.cpp \
    MeshBatchSample.cpp \
    MeshPrimitiveSample.cpp \
    ParticlesSample.cpp \
    PhysicsCollisionObjectSample.cpp \
    PostProcessSample.cpp \
    SceneCreateSample.cpp \
    SceneLoadSample.cpp \
    SpriteBatchSample.cpp \
    SpriteSample.cpp \
    TerrainSample.cpp \
    TextureSample.cpp \
    TriangleSample.cpp \
    WaterSample.cpp

LOCAL_CPPFLAGS += -std=c++11 -frtti -Wno-switch-enum -Wno-switch
LOCAL_ARM_MODE := arm
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -D__ANDROID__ -I"../../../external-deps/include" -I"../../../gameplay/src"
LOCAL_STATIC_LIBRARIES := android_native_app_glue libgameplay-deps
LOCAL_SHARED_LIBRARIES := gameplay
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
