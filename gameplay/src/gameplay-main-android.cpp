#ifdef __ANDROID__

#include "gameplay.h"
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include <jni.h>


#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))

using namespace gameplay;

extern struct android_app* __state;
AAssetManager* __assetManager;
std::string __assetsPath;
extern ASensorManager* __sensorManager;
extern const ASensor* __accelerometerSensor;
extern ASensorEventQueue* __sensorEventQueue;
extern ASensorEvent __sensorEvent;
bool __displayKeyboard = false;
bool __initialized = false;


namespace gameplay
{
extern Keyboard::Key getKey(int keycode, int metastate);
extern void displayKeyboard(android_app* mApplication, bool pShow);
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
    Platform* platform = static_cast<Platform*>(app->userData);
    
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        
        int32_t data = AMotionEvent_getAction(event);
        int contactIndex = data >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        Touch::TouchEvent touchEvent;
        switch (data & AMOTION_EVENT_ACTION_MASK)
        {
            case AMOTION_EVENT_ACTION_DOWN:
                touchEvent = Touch::TOUCH_PRESS;
                break;
            case AMOTION_EVENT_ACTION_UP:
                touchEvent = Touch::TOUCH_RELEASE;
                break;
            case AMOTION_EVENT_ACTION_MOVE:
                touchEvent = Touch::TOUCH_MOVE;
                break;
        }
    
        Game::getInstance()->touchEvent(touchEvent, AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0), contactIndex);
        return 1;
    } 
    else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY)
    {
        int32_t action = AKeyEvent_getAction(event);
        int32_t keycode = AKeyEvent_getKeyCode(event);
        int32_t metastate = AKeyEvent_getMetaState(event); 
        
        switch(action)
        {
            case AKEY_EVENT_ACTION_DOWN:
                Game::getInstance()->keyEvent(Keyboard::KEY_PRESS, getKey(keycode, metastate));
                break;
                    
            case AKEY_EVENT_ACTION_UP:
                Game::getInstance()->keyEvent(Keyboard::KEY_RELEASE, getKey(keycode, metastate));
                break;
        }
    }
    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd) 
{
    switch (cmd) 
    {
        case APP_CMD_SAVE_STATE:
            // TODO
            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (app->window != NULL)
            {
                Game* game = Game::getInstance();
                assert(game != NULL);
                Platform* platform = Platform::create(game);
                app->userData = platform;
                __initialized = true;
            }
            break;
        case APP_CMD_TERM_WINDOW:
        {
            Game::getInstance()->exit();
            Platform* platform = static_cast<Platform*>(app->userData);
            if (!platform)
            {
                return;
            }
            delete platform;
            app->userData = NULL;
            break;
        }
        case APP_CMD_GAINED_FOCUS:
            // When our app gains focus, we start monitoring the accelerometer.
            if (__accelerometerSensor != NULL) {
                ASensorEventQueue_enableSensor(__sensorEventQueue, __accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(__sensorEventQueue, __accelerometerSensor, (1000L/60)*1000);
            }
            Game::getInstance()->resume();
            break;
        case APP_CMD_LOST_FOCUS:
            // When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (__accelerometerSensor != NULL) {
                ASensorEventQueue_disableSensor(__sensorEventQueue, __accelerometerSensor);
            }
            Game::getInstance()->pause();
            ((Platform*)static_cast<Platform*>(app->userData))->swapBuffers();
            break;
    }
}

/**
 * Main entry point.
 */
void android_main(struct android_app* state)
{
	// Make sure glue isn't stripped.
    app_dummy();
    
    ANativeActivity* activity = state->activity;
    JNIEnv* env = activity->env;

    jclass clazz = env->GetObjectClass(activity->clazz);
    jmethodID methodID = env->GetMethodID(clazz, "getPackageName", "()Ljava/lang/String;");
    jobject result = env->CallObjectMethod(activity->clazz, methodID);
    
    const char* packageName;
    jboolean isCopy;
    packageName = env->GetStringUTFChars((jstring)result, &isCopy);
    
    __assetsPath = "/mnt/sdcard/android/data/";
    __assetsPath += packageName;
    __assetsPath += "/";
    
    __assetManager = state->activity->assetManager; 
    
    state->onAppCmd = engine_handle_cmd;
    state->onInputEvent = engine_handle_input;
    __state = state;
    
    // Prepare to monitor accelerometer.
    __sensorManager = ASensorManager_getInstance();
    __accelerometerSensor = ASensorManager_getDefaultSensor(__sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    __sensorEventQueue = ASensorManager_createEventQueue(__sensorManager, __state->looper, LOOPER_ID_USER, NULL, NULL);
    
    while (true)
    {
    
        // Read all pending events.
        int ident;
        int events;
        struct android_poll_source* source;
        
        bool _shouldPoll = !(__initialized && Game::getInstance()->getState() == Game::UNINITIALIZED) && (Game::getInstance()->getState() != Game::PAUSED); 
        
        while ((ident=ALooper_pollAll( _shouldPoll ? 0 : -1, NULL, &events, (void**)&source)) >= 0) 
        {
            // Process this event.
            if (source != NULL) 
                source->process(__state, source);
            
            // If a sensor has data, process it now.
            if (ident == LOOPER_ID_USER && __accelerometerSensor != NULL)
                ASensorEventQueue_getEvents(__sensorEventQueue, &__sensorEvent, 1);
            
            if (__state->destroyRequested != 0)
                break;
        }
        
        Platform* platform = static_cast<Platform*>(state->userData);
        if (platform)
            platform->enterMessagePump();
            
        
        // Check if we are exiting.
        if ((__state->destroyRequested != 0) || (__initialized && Game::getInstance()->getState() == Game::UNINITIALIZED))
            break;
        
        // Display the keyboard.
        displayKeyboard(state, __displayKeyboard);
    }
    
    // We need to exit the process to cleanup global resources.
    exit(0);
}

#endif
