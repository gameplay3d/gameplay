package org.gameplay3d;

import android.app.NativeActivity;
import android.content.Context;
import android.content.res.Configuration;
import android.hardware.input.InputManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.util.SparseArray;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;

/**
 * Game extension for Android platform.
 * 
 * Handles any platform features that cannot be handled natively in PlatformAndroid.cpp 
 * 
 * Developers may choose to directly modify/extend this for any addition platform features 
 * that are not offered directly the gameplay3d framework such as platform events, access to 
 * android user-interface, life-cycle events for saving game state and custom plug-ins/extensions.
 */
public class GameNativeActivity extends NativeActivity
    implements InputManager.InputDeviceListener {
    
    static {
        System.loadLibrary("gameplay");
    }
    
    private static final String TAG = "GameNativeActivity";
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        _gamepadDevices = new SparseArray<InputDevice>();
        _inputManager = (InputManager)getSystemService(Context.INPUT_SERVICE);

        View decorView = getWindow().getDecorView();
        int uiOptions = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION | View.SYSTEM_UI_FLAG_FULLSCREEN;
        if (Build.VERSION.SDK_INT >= 18) 
            uiOptions ^= 0x00000800; //View.SYSTEM_UI_FLAG_IMMERSIVE;
        decorView.setSystemUiVisibility(uiOptions);
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
        _inputManager.registerInputDeviceListener(this, null);
        int[] ids = _inputManager.getInputDeviceIds();
        for (int i = 0; i < ids.length; i++) {
            getGamepadDevice(ids[i]);
        }
    }
    
    @Override
    protected void onPause() {
        _inputManager.unregisterInputDeviceListener(this);
        super.onPause();
    }
    
    @Override
    public void onInputDeviceAdded(int deviceId) {
        Log.v(TAG, "Input Device added: " + deviceId);
        getGamepadDevice(deviceId);
    }

    @Override
    public void onInputDeviceRemoved(int deviceId) {
        Log.v(TAG, "Input Device removed: " + deviceId);
        InputDevice device = _gamepadDevices.get(deviceId);
        if (device != null) {
            _gamepadDevices.remove(deviceId);
            gamepadEventDisconnectedImpl(deviceId);
        }
    }
    
    @Override
    public void onInputDeviceChanged(int deviceId) {
        Log.i(TAG, "Input Device changed: " + deviceId);
    }
    
    private InputDevice getGamepadDevice(int deviceId) {
        InputDevice device = _gamepadDevices.get(deviceId);
        if (device == null) {
            device = _inputManager.getInputDevice(deviceId);
            if (device == null)
                return null;
            if ((device.getSources() & InputDevice.SOURCE_GAMEPAD) != 0) {
                _gamepadDevices.put(deviceId, device);
                gamepadEventConnectedImpl(deviceId, 26, 2, 2, 0, 0, "", "");
            }
            if ((device.getSources() & InputDevice.SOURCE_JOYSTICK) != 0) {
                _gamepadDevices.put(deviceId, device);
                gamepadEventConnectedImpl(deviceId, 10, 0, 0, 0, 0, "", "");
            }
        }
        return device;
    }
    
    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        final int deviceId = event.getDeviceId();
        final int keyCode = event.getKeyCode();
        switch (event.getAction()) {
            case KeyEvent.ACTION_DOWN:
                gamepadButtonPressedEventImpl(deviceId, keyCode);
                Log.i(TAG, "dispatchKeyEvent: " + deviceId + "," + keyCode);
                return true;
            case KeyEvent.ACTION_UP:
                gamepadButtonReleasedEventImpl(deviceId, keyCode);
                Log.i(TAG, "dispatchKeyEvent: " + deviceId + "," + keyCode);
                return true;
        }
        return false;
    }
    
    @Override
    public boolean dispatchGenericMotionEvent(MotionEvent event) {
        return false;
    }
    
    // JNI calls to PlatformAndroid.cpp
    private static native void gamepadEventConnectedImpl(int deviceId, int buttonCount, int joystickCount, int triggerCount, int vendorId, int productId, String vendorString, String productString);
    private static native void gamepadEventDisconnectedImpl(int deviceId);
    private static native void gamepadButtonPressedEventImpl(int deviceId, int keycode);
    private static native void gamepadButtonReleasedEventImpl(int deviceId, int keycode);
    private static native void gamepadTriggerChangedEventImpl(int deviceId, int index, float value);
    private static native void gamepadJoystickChangedEventImpl(int deviceId, int index, float x, float y);
    
    private InputManager _inputManager;
    private SparseArray<InputDevice> _gamepadDevices;
}
