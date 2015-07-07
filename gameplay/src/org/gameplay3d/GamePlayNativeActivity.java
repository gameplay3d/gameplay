package org.gameplay3d;

import android.app.NativeActivity;
import android.content.Context;
import android.content.res.Configuration;
import android.hardware.input.InputManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.util.SparseArray;
import android.view.Display;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.OrientationEventListener;

/**
 * GamePlay native activity extension for Android platform.
 * 
 * Handles any platform features that cannot be handled natively in PlatformAndroid.cpp 
 * 
 * Developers may choose to directly modify/extend this for any addition platform features 
 * that are not offered directly the gameplay3d framework such as platform events, access to 
 * android user-interface, life-cycle events for saving game state and custom plug-ins/extensions.
 */
public class GamePlayNativeActivity extends NativeActivity {
    
    static {
        System.loadLibrary("gameplay");
    }

    private class GamePlayInputDeviceListener
        implements InputManager.InputDeviceListener {
        @Override
        public void onInputDeviceAdded(int deviceId) {
            getGamepadDevice(deviceId);
        }

        @Override
        public void onInputDeviceRemoved(int deviceId) {
            InputDevice device = _gamepadDevices.get(deviceId);
            if (device != null) {
                _gamepadDevices.remove(deviceId);
                Log.v(TAG, "Gamepad disconnected:id=" + deviceId);
                gamepadEventDisconnectedImpl(deviceId);
            }
        }

        @Override
        public void onInputDeviceChanged(int deviceId) {
        }
    }

    private static final String TAG = "GamePlayNativeActivity";
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.i(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        _gamepadDevices = new SparseArray<InputDevice>();
        Log.v(TAG, "Build version: " + Build.VERSION.SDK_INT);
        if (Build.VERSION.SDK_INT >= 16) {
            _inputManager = (InputManager)getSystemService(Context.INPUT_SERVICE);
            _inputDeviceListener = new GamePlayInputDeviceListener();
        }

        if (Build.VERSION.SDK_INT >= 19)
        {
            View decorView = getWindow().getDecorView();
            int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                            View.SYSTEM_UI_FLAG_FULLSCREEN |
                            0x00000800; // View.SYSTEM_UI_FLAG_IMMERSIVE;
            decorView.setSystemUiVisibility(uiOptions);
        }

        orientationListener = new OrientationEventListener(this) {
            public void onOrientationChanged(int orientation) {
                if (orientation != OrientationEventListener.ORIENTATION_UNKNOWN) {
                    WindowManager mWindowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);

                    Display display = mWindowManager.getDefaultDisplay();
                    int rotation = display.getRotation();
                    screenOrientationChanged(rotation);
                }
            }
        };
    }
    
    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
    }
    
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
    } 
    
    @Override
    protected void onResume() {
        super.onResume();
        orientationListener.enable();
        if (_inputManager != null) {
            _inputManager.registerInputDeviceListener(_inputDeviceListener, null);
            int[] ids = InputDevice.getDeviceIds();
            for (int i = 0; i < ids.length; i++) {
                getGamepadDevice(ids[i]);
            }
        }
    }
    
    @Override
    protected void onPause() {
        orientationListener.disable();
        if (_inputManager != null) {
            _inputManager.unregisterInputDeviceListener(_inputDeviceListener);
        }
        super.onPause();
    }
    
    private void onGamepadConnected(int deviceId, String deviceName) {
        int buttonCount = 17;
        int joystickCount = 2;
        int triggerCount = 2;
        
        Log.v(TAG, "Gamepad connected:id=" + deviceId + ", name=" + deviceName);
        
        gamepadEventConnectedImpl(deviceId, buttonCount, joystickCount, triggerCount, deviceName);
    }
    
    private InputDevice getGamepadDevice(int deviceId) {
        InputDevice device = _gamepadDevices.get(deviceId);
        if (device == null) {
            device = InputDevice.getDevice(deviceId);
            if (device == null)
                return null;
            int sources = device.getSources();
            if (((sources & InputDevice.SOURCE_GAMEPAD) == InputDevice.SOURCE_GAMEPAD) || 
                ((sources & InputDevice.SOURCE_JOYSTICK) == InputDevice.SOURCE_JOYSTICK)) {
                _gamepadDevices.put(deviceId, device);
                
                onGamepadConnected(deviceId, device.getName());
            }
        }
        return device;
    }
    
    
    // JNI calls to PlatformAndroid.cpp
    private static native void gamepadEventConnectedImpl(int deviceId, int buttonCount, int joystickCount, int triggerCount, String deviceName);
    private static native void gamepadEventDisconnectedImpl(int deviceId);
    private static native void screenOrientationChanged(int orientation);
    
    private InputManager _inputManager = null;
    private SparseArray<InputDevice> _gamepadDevices;
    private OrientationEventListener orientationListener;
    GamePlayInputDeviceListener _inputDeviceListener = null;
}
