#pragma once

#include "Platform.h"
#include "Graphics.h"

namespace gameplay
{
    
/**
 * Defines a macos platform impl.
 */
class PlatformMacOS : public Platform
{
public:
    
    /**
     * Constructor.
     */
    PlatformMacOS();
    
    /**
     * Destructor
     */
    ~PlatformMacOS();
    
    /**
     * @see Platform::startup
     */
    bool startup(int argc, char** argv);
    
    /**
     * @see Platform::shutdown
     */
    void shutdown();
    
    /**
     * @see Platform::enterMessagePump
     */
    int enterMessagePump();
    
    /**
     * @see Platform::getWindow
     */
    unsigned long getWindow();
    
    /**
     * @see Platform::getConnection
     */
    unsigned long getConnection();
    
    /**
     * @see Platform::getGraphics
     */
    Graphics* getGraphics();
    
    /**
     * @see Platform::isGamepadButtonPressed
     */
    bool isGamepadButtonPressed(GamepadButton button, size_t gamepadIndex);
    
    /**
     * @see Platform::getGamepadAxisValues
     */
    void getGamepadAxisValues(float* leftVertical, float* leftHorizontal,
                              float* rightVertical, float* rightHorizontal, size_t gamepadIndex);
    
    /**
     * @see Platform::getGamepadTriggerValues
     */
    void getGamepadTriggerValues(float* leftTrigger, float* rightTrigger, size_t gamepadIndex);
    
    /**
     * @see Platform::getAccelerometerValues
     */
    void getAccelerometerValues(float* pitch, float* roll);
    
    /**
     * @see Platform::getSensorValues
     */
    void getSensorValues(float* accelX, float* accelY, float* accelZ,
                         float* gyroX, float* gyroY, float* gyroZ);
    
private:
    
    Graphics* _graphics;
};
    
}
